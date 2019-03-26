#include "torture.h"


// --- Prototypes, the implementations are below main() ---
static int32_t setNumThreads( char const* chNum, uint32_t& numThreads );
static int32_t setTestType( char const* chType, eTestType& testType );
static int32_t startTest( uint32_t numThreads, eTestType testType );


int32_t main( int32_t argc, char** argv ) {
    int32_t result = EXIT_SUCCESS;
    uint32_t  numThreads = 32;
    eTestType testType   = E_TEST_ALL;

    // Check the arguments:
    for ( int32_t i = 1; ( EXIT_SUCCESS == result ) && ( i < argc ); ++i ) {
        if ( STREQ( "-c", argv[i] ) ) {
            if ( ( argc > ( i + 1 ) ) && ( argv[i+1][0] != '-' ) )
                result = setTestType( argv[++i], testType );
            else {
                cerr << "Argument -c needs a type option!\n";
                cerr << "  Use either -h or --help for further information." << endl;
                result = EXIT_FAILURE;
            }
        } else if ( STREQ( "--help", argv[i] ) || STREQ( "-h", argv[i] ) ) {
            cout << "================================================================\n";
            cout << " WARNING: This program does a lot of very stupid stuff you\n";
            cout << "          must NOT copy! It is a very bad idea to hammer this\n";
            cout << "          many different tasks onto any data container using\n";
            cout << "          simultaneous access. The only reason this program\n";
            cout << "          does that is to enable thread analyzers like Helgrind\n";
            cout << "          or DRD to give as many opportunities to detect locking\n";
            cout << "          order problems and/or possible race conditions as\n";
            cout << "          possible. Again: DO NOT DO THIS IN YOUR PROGRAMS!\n";
            cout << "================================================================\n";
            cout << "Usage:\n  " << basename( argv[0] ) << " [[-h|--help] | [-c type] [-t num]]\n";
            cout << "  -c type     : set the type of container to check\n";
            cout << "       all    : Test all containers (default)\n";
            cout << "       list   : Test TSingleList and TDoubleList\n";
            cout << "       list_d : Test TDoubleList\n";
            cout << "       list_s : Test TSingleList\n";
            cout << "       queue  : Test TQueue\n";
            cout << "       ring   : Test TSingleRing and TDoubleRing\n";
            cout << "       ring_d : Test TDoubleRing\n";
            cout << "       ring_s : Test TSingleRing\n";
            cout << "       set    : Test TSet\n";
            cout << "       stack  : Test TStack\n";
            cout << "  -h / --help : Show this help and exit\n";
            cout << "  -t num      : Set the number of threads (default: 32)" << endl;
            return result;
        } else if ( STREQ( "-t", argv[i] ) ) {
            if ( ( argc > ( i + 1 ) ) && ( argv[i+1][0] != '-' ) )
                result = setNumThreads( argv[++i], numThreads );
            else {
                cerr << "Argument -t needs a number option!\n";
                cerr << "  Use either -h or --help for further information." << endl;
                result = EXIT_FAILURE;
            }
        }
    } // Finished checking arguments


    if ( EXIT_SUCCESS == result ) {
        PWX_TRY( result = startTest( numThreads, testType ) )
        catch ( pwx::CException& e ) {
            cerr << "== Thread " << std::this_thread::get_id();
            cerr << " ==\n-----\npwx exception \"" << e.name() << "\" caught!" << endl;
            cerr << "What : \"" << e.what() << "\"" << endl;
            cerr << "Desc : \"" << e.desc() << "\"" << endl;
            cerr << "Where: \"" << e.where() << "\"" << endl;
            cerr << "pFunc: \"" << e.pfunc() << "\"" << endl;
            cerr << "\nTrace:\n" << e.trace() << "\n-----" << endl;
        } catch ( ... ) {
            cerr << "== Thread " << std::this_thread::get_id();
            cerr << " ==\n-----\nSomething completely unknown was caught!" << endl;
        }
    }

    return result;
}


// Small helper to get the number out of an argument and check it
static int32_t setNumThreads( char const* chNum, uint32_t& numThreads ) {
    int32_t result = EXIT_SUCCESS;

    numThreads = pwx::to_uint32( chNum );

    if ( numThreads < 4 ) {
        cerr << "\"" << chNum << "\" is invalid as a number of threads to start.\n";
        cerr << "Anything not even or greater than 4 is absolutely pointless." << endl;
        result = EXIT_FAILURE;
    }

    return result;
}


// Small helper to get the test type out of an argument and check it
static int32_t setTestType( char const* chType, eTestType& testType ) {
    int32_t result = EXIT_SUCCESS;

    if ( STREQ( "all", chType ) )
        testType = E_TEST_ALL;
    else if ( STREQ( "list", chType ) )
        testType = E_TEST_LIST;
    else if ( STREQ( "list_d", chType ) )
        testType = E_TEST_LIST_D;
    else if ( STREQ( "list_s", chType ) )
        testType = E_TEST_LIST_S;
    else if ( STREQ( "queue", chType ) )
        testType = E_TEST_QUEUE;
    else if ( STREQ( "ring", chType ) )
        testType = E_TEST_RING;
    else if ( STREQ( "ring_d", chType ) )
        testType = E_TEST_RING_D;
    else if ( STREQ( "ring_s", chType ) )
        testType = E_TEST_RING_S;
    else if ( STREQ( "set", chType ) )
        testType = E_TEST_SET;
    else if ( STREQ( "stack", chType ) )
        testType = E_TEST_STACK;
    else {
        cerr << "\"" << chType << "\" is invalid as a check type.\n";
        result = EXIT_FAILURE;
    }

    return result;
}


/// @brief main test starting function
static int32_t startTest( uint32_t numThreads, eTestType testType ) {
    int32_t result = EXIT_SUCCESS;

    if ( ( EXIT_SUCCESS == result )
            && ( ( testType == E_TEST_ALL )
                 || ( testType == E_TEST_LIST )
                 || ( testType == E_TEST_LIST_S ) ) ) {
        cout << " === Testing TSingleList === " << endl;
        PWX_TRY_PWX_FURTHER( result = do_test<pwx::TSingleList<data_t> >( numThreads ) )
    }
    if ( ( EXIT_SUCCESS == result )
            && ( ( testType == E_TEST_ALL )
                 || ( testType == E_TEST_LIST )
                 || ( testType == E_TEST_LIST_D ) ) ) {
        cout << " === Testing TDoubleList === " << endl;
        PWX_TRY_PWX_FURTHER( result = do_test<pwx::TDoubleList<data_t>>( numThreads ) )
    }
    if ( ( EXIT_SUCCESS == result )
            && ( ( testType == E_TEST_ALL )
                 || ( testType == E_TEST_QUEUE ) ) ) {
        cout << " === Testing TQueue === " << endl;
        PWX_TRY_PWX_FURTHER( result = do_test<pwx::TQueue<data_t>>( numThreads ) )
    }
    if ( ( EXIT_SUCCESS == result )
            && ( ( testType == E_TEST_ALL )
                 || ( testType == E_TEST_RING )
                 || ( testType == E_TEST_RING_S ) ) ) {
        cout << " === Testing TSingleRing === " << endl;
        PWX_TRY_PWX_FURTHER( result = do_test<pwx::TSingleRing<data_t>>( numThreads ) )
    }
    if ( ( EXIT_SUCCESS == result )
            && ( ( testType == E_TEST_ALL )
                 || ( testType == E_TEST_RING )
                 || ( testType == E_TEST_RING_D ) ) ) {
        cout << " === Testing TDoubleRing === " << endl;
        PWX_TRY_PWX_FURTHER( result = do_test<pwx::TDoubleRing<data_t>>( numThreads ) )
    }
    if ( ( EXIT_SUCCESS == result )
            && ( ( testType == E_TEST_ALL )
                 || ( testType == E_TEST_SET ) ) ) {
        cout << " === Testing TSet === " << endl;
        PWX_TRY_PWX_FURTHER( result = do_test<pwx::TSet<data_t>>( numThreads ) )
    }
    if ( ( EXIT_SUCCESS == result )
            && ( ( testType == E_TEST_ALL )
                 || ( testType == E_TEST_STACK ) ) ) {
        cout << " === Testing TStack === " << endl;
        PWX_TRY_PWX_FURTHER( result = do_test<pwx::TStack<data_t>>( numThreads ) )
    }

    return result;
}


// --- thread classes method implementations ---

/// @brief dtor waiting for the thread to finish
thrdBase::~thrdBase() noexcept {
    // No thread must be deleted while it is still running!
    if ( isRunning ) {
        isKilled = true;
        while ( isRunning )
            std::this_thread::yield();
    }
}

/// @brief simple sleeper to wait for the thread to start.
void thrdBase::waitForStart() noexcept {
    std::chrono::milliseconds sleepTime( 1 );
    while ( !isRunning && !isKilled ) {
        std::this_thread::sleep_for( sleepTime );
        if ( !isRunning && !isKilled )
            std::this_thread::yield();
    }
}
