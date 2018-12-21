#include "testSCT.h"
using pwx::RNG;
using pwx::SCT;

// just some global values for the results:
static double  degrees[12]     = { 0.L, 90.L, 180.L, 270.L, 0, 0, 0, 0, 0, 0, 0, 0 };
static double  diffRes[2][6][12]; // cos/sin, precision, index
static int64_t speedRes[3][7]; // cos/sin/sincos, precision + manual

// Helper functions
static void testDiff( int32_t angleIdx, int32_t precision );
static void testSpeed( int32_t precision, uint32_t testMaxElements );

/** @brief central function to test pwx::SCT
  *
  *************************************************************************
  ** Walk up from precision -1, 0, 1, 2, 3(, 4) and check for time and   **
  ** difference compared to standard manual calculation.                 **
  ** A) For the difference, 0, 90, 180, 270 and eight random values are  **
  **    tested.                                                          **
  ** B) For the speed tests, maxElements random angles will be tested.   **
  *************************************************************************
**/
int32_t testSCT ( sEnv& env ) {
    int32_t  result           = EXIT_SUCCESS;
    uint32_t testMaxElements  = env.doSpeed ? maxElements : 10;
    int32_t  testMaxPrecision = env.doSpeed ? 4 : 3;

    // Add the eight random angles we wanted:
    for ( int idx = 4; idx < 12; ) {
        double   newAngle = RNG.random( 0., 359.9999 );
        bool     isNew    = true;
        for ( int i = 0; isNew && ( i < 12 ); ++i ) {
            if ( pwx::areAlmostEqual( newAngle, degrees[i] ) )
                isNew = false;
        }
        if ( isNew )
            degrees[idx++] = newAngle;
    } // End of adding four random angles

    cout << "\nTest CSinCosTable instance pwx::SCT\n-----------------------------------" << endl;

    /************************************************************************
    ** A) Difference tests  and B) speed tests combined.                   **
    ** This is done to not having to reinitialize the tables twice!        **
    ************************************************************************/
    cout << " A) Test the difference between life calculations and table usage" << endl;
    cout << "and\n B) Test the speed of " << testMaxElements << " random sin/cos calculations" << endl;
    cout << "\n (  This is done or the tables would have to be\n";
    cout << "    re-initialized twice for each precision!     )\n" << endl;
    for ( int32_t prec = -2 ; prec < ( testMaxPrecision + 1 ); ++prec ) {
        cout << adjRight ( 4, 0 ) << ++env.testCount;

        if ( prec < -1 )
            cout << " manual calculation\n";
        else if ( prec < 0 )
            cout << " live calculation\n";
        else
            cout << " precision " << prec << "\n";

        if ( prec > -2 ) {
            cout << " --- Test result precision ---\n";
            cout << " degree |          sine |    difference |        cosine |   difference\n";
            cout << " -------+---------------+---------------+---------------+--------------" << endl;
            for ( int32_t idx = 0; idx < 12; ++idx )
                testDiff( idx, prec );
            cout << " -------+---------------+---------------+---------------+--------------\n" << endl;
        }

        cout << " --- Test calculation speed ---\n";
        cout << " type   |        minium |       maximum |     time | difference\n";
        cout << " -------+---------------+---------------+----------+-----------" << endl;
        testSpeed( prec, testMaxElements );
        cout << " -------+---------------+---------------+----------+-----------\n" << endl;
        ++env.testSuccess;
    }

    return result;
}


// Helper function implementations
static void testDiff( int32_t angleIdx, int32_t precision ) {
    // Adapt precision if we must
    if ( precision != SCT.getPrecision() )
        SCT.setPrecision( precision );

    // Get values
    double sin, cos;
    SCT.sincos( static_cast<double>( degrees[angleIdx] ), cos, sin );

    // Note results
    diffRes[0][precision + 1][angleIdx] = cos;
    diffRes[1][precision + 1][angleIdx] = sin;

    // Get difference
    double cosDiff = diffRes[0][precision + 1][angleIdx] - diffRes[0][0][angleIdx];
    double sinDiff = diffRes[1][precision + 1][angleIdx] - diffRes[1][0][angleIdx];

    cout << adjRight( 4,2 ) << degrees[angleIdx];
    cout << " | " << adjRight( 2,10 ) << sin;
    cout << " | " << adjRight( 2,10 ) << sinDiff;
    cout << " | " << adjRight( 2,10 ) << cos;
    cout << " | " << adjRight( 2,10 ) << cosDiff << endl;
}

static void testSpeed( int32_t precision, uint32_t testMaxElements ) {
    // Values used
    double minCos = 2., maxCos = -2., curCos;
    double minSin = 2., maxSin = -2., curSin;
    double minCom = 2., maxCom = -2.;
    hrTime_t tStart;

    if ( precision > -2 )
        SCT.setPrecision( precision );

    // --- sine ---
    // ------------
    tStart = hrClock::now();
    if ( precision < -1 ) {
        for ( uint32_t i = 0; i < testMaxElements; ++i ) {
            curSin = std::sin( RNG.random( 0., 360. ) * M_PIl / 180. );
            if ( curSin < minSin ) minSin = curSin;
            if ( curSin > maxSin ) maxSin = curSin;
        }
    } else {
        for ( uint32_t i = 0; i < testMaxElements; ++i ) {
            curSin = SCT.sin( RNG.random( 0., 360. ) );
            if ( curSin < minSin ) minSin = curSin;
            if ( curSin > maxSin ) maxSin = curSin;
        }
    }
    speedRes[0][precision + 2] = duration_cast<milliseconds>( hrClock::now() - tStart ).count();

    // --- cosine ---
    // --------------
    tStart = hrClock::now();
    if ( precision < -1 ) {
        for ( uint32_t i = 0; i < testMaxElements; ++i ) {
            curCos = std::cos( RNG.random( 0., 360. ) * M_PIl / 180. );
            if ( curCos < minCos ) minCos = curCos;
            if ( curCos > maxCos ) maxCos = curCos;
        }
    } else {
        for ( uint32_t i = 0; i < testMaxElements; ++i ) {
            curCos = SCT.cos( RNG.random( 0., 360. ) );
            if ( curCos < minCos ) minCos = curCos;
            if ( curCos > maxCos ) maxCos = curCos;
        }
    }
    speedRes[1][precision + 2] = duration_cast<milliseconds>( hrClock::now() - tStart ).count();

    // --- sine / cosine combination ---
    // ---------------------------------
    if ( precision < -1 ) {
        // Here we no method to test, so add them up:
        minCom = std::min( minSin, minCos );
        maxCom = std::max( maxSin, maxCos );
        speedRes[2][precision + 2] = speedRes[0][precision + 2] + speedRes[1][precision + 2] ;
    } else {
        tStart = hrClock::now();
        for ( uint32_t i = 0; i < testMaxElements; ++i ) {
            SCT.sincos( RNG.random( 0., 360. ), curCos, curSin );
            if ( curCos < minCom ) minCom = curCos;
            if ( curCos > maxCom ) maxCom = curCos;
            if ( curSin < minCom ) minCom = curSin;
            if ( curSin > maxCom ) maxCom = curSin;
        }
        speedRes[2][precision + 2] = duration_cast<milliseconds>( hrClock::now() - tStart ).count();
    }

    cout << " sin   ";
    cout << " | " << adjRight( 2, 10 ) << minSin;
    cout << " | " << adjRight( 2, 10 ) << maxSin;
    cout << " | " << adjRight( 5, 0 ) << speedRes[0][precision + 2] << " ms";
    cout << " | " << adjRight( 5, 0 ) << ( speedRes[0][precision + 2] - speedRes[0][0] ) << " ms" << endl;

    cout << " cos   ";
    cout << " | " << adjRight( 2, 10 ) << minCos;
    cout << " | " << adjRight( 2, 10 ) << maxCos;
    cout << " | " << adjRight( 5, 0 ) << speedRes[1][precision + 2] << " ms";
    cout << " | " << adjRight( 5, 0 ) << ( speedRes[1][precision + 2] - speedRes[1][0] ) << " ms" << endl;

    cout << " sincos";
    cout << " | " << adjRight( 2, 10 ) << minCom;
    cout << " | " << adjRight( 2, 10 ) << maxCom;
    cout << " | " << adjRight( 5, 0 ) << speedRes[2][precision + 2] << " ms";
    cout << " | " << adjRight( 5, 0 ) << ( speedRes[2][precision + 2] - speedRes[2][0] ) << " ms" << endl;
}
