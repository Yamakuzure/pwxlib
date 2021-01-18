/** @file testColor.cpp
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

#include <PWaveColor>

#include "testColor.h"


/* ==================================
 * === Helper function prototypes ===
 * ==================================
 */
static int  addRGB  ( PWaveColor& wc, char const* const name, uint8_t r, uint8_t g, uint8_t b,
                      uint8_t tR, uint8_t tG, uint8_t tB );
static int  delRGB  ( PWaveColor& wc, char const* const name, uint8_t r, uint8_t g, uint8_t b,
                      uint8_t tR, uint8_t tG, uint8_t tB );
static void printRGB( PWaveColor& wc, char const* const name, char const* const call, bool isEnd = true );
static void printRGB( int32_t nr, PWaveColor& wc, char const* const name, char const* const call, bool isEnd = true );
static int  modFreq ( PWaveColor& wc, char const* const name, double modifier,
                      uint8_t tR, uint8_t tG, uint8_t tB );
static int  setGamma( PWaveColor& wc, char const* const name, double gamma,
                      uint8_t tR, uint8_t tG, uint8_t tB );
static int  setRGB  ( int32_t nr, PWaveColor& wc, char const* const name, uint8_t r, uint8_t g, uint8_t b );
static int  modWave ( PWaveColor& wc, char const* const name, double modifier,
                      uint8_t tR, uint8_t tG, uint8_t tB );


/** @brief central function to test PWaveColor
  *
  *************************************************************************
  ** A) Try 10 standard colors                                           **
  ** B) Mix up 5 colors                                                  **
  ** C) Set 5 colors and remove parts                                    **
  ** D) Set 5 colors and change gamma                                    **
  ** E) Set 5 colors and modify Frequency/Wavelength                     **
  *************************************************************************
**/
int32_t testColor ( sEnv& env ) {
	int32_t result = EXIT_SUCCESS;

	cout << "\nTest CWaveColor type\n------------------------------" << endl;

	PWaveColor WC;

	/************************************************************************
	** A) Try 10 standard colors                                           **
	************************************************************************/
	cout << adjRight ( 4, 0 ) << ++env.testCount << " Set 10 standard colors" << endl;
	int testOK = 0;

	// 1: Black
	testOK += setRGB( 1, WC, "Black", 0x0, 0x0, 0x0 );

	// 2: Red
	testOK += setRGB( 2, WC, "Red", 0xff, 0x0, 0x0 );

	// 3: Green
	testOK += setRGB( 3, WC, "Green", 0x0, 0xff, 0x0 );

	// 4: Blue
	testOK += setRGB( 4, WC, "Blue", 0x0, 0x0, 0xff );

	// 5: Yellow
	testOK += setRGB( 5, WC, "Yellow", 0xff, 0xff, 0x0 );

	// 6: Turquoise
	testOK += setRGB( 6, WC, "Turquoise", 0x0, 0xff, 0xff );

	// 7: Violet
	testOK += setRGB( 7, WC, "Violet", 0xff, 0x0, 0xff );

	// 8: Silver
	testOK += setRGB( 8, WC, "Silver", 0xc0, 0xc0, 0xc0 );

	// 9: Khaki 3
	testOK += setRGB( 9, WC, "Khaki 3", 0xcd, 0xc6, 0x73 );

	// 10: Sailor
	testOK += setRGB( 10, WC, "Sailor Blue", 0x5f, 0x9e, 0xa0 );

	if ( 10 == testOK ) {
		cout << " --- all colors correct ---\n" << endl;
		++env.testSuccess;
	} else {
		cout << " --- " << ( 10 - testOK ) << "/10 colors are wrong! ---\n" << endl;
		++env.testFail;
	}


	/*****************************************
	* === Test 2: Mix up 5 colors        === *
	*****************************************/
	cout << adjRight ( 4, 0 ) << ++env.testCount << " Mix 5 colors" << endl;
	testOK = 0;

	// 1.: Red and Green
	testOK += setRGB( 11, WC, "Mix red",   0xe0, 0x00, 0x00 );
	testOK += addRGB(    WC, "and green", 0x00, 0xe0, 0x00, 0xe0, 0xe0, 0x00 );

	// 2.: Red and Blue
	testOK += setRGB( 12, WC, "Mix red",  0xe0, 0x00, 0x00 );
	testOK += addRGB(    WC, "and blue", 0x00, 0x00, 0xe0, 0xe0, 0x00, 0xe0 );

	// 3.: Violet and yellow
	testOK += setRGB( 13, WC, "Mix violet", 0xe0, 0x00, 0xe0 );
	testOK += addRGB(    WC, "and yellow", 0xe0, 0xe0, 0x00, 0xff, 0x7f, 0x7f );

	// 4.: Khaki 3 and Blue
	testOK += setRGB( 14, WC, "Mix khaki 3", 0xcd, 0xc6, 0x73 );
	testOK += addRGB(    WC, "and blue",    0x00, 0x00, 0xe0, 0x9a, 0x95, 0xff );

	// 5.: Red and violet
	testOK += setRGB( 15, WC, "Mix red",    0xe0, 0x00, 0x00 );
	testOK += addRGB(    WC, "and violet", 0xff, 0x00, 0xff, 0xff, 0x00, 0x88 );

	if ( 10 == testOK ) {
		cout << " --- all colors correct ---\n" << endl;
		++env.testSuccess;
	} else {
		cout << " --- " << ( 10 - testOK ) << "/10 colors are wrong! ---\n" << endl;
		++env.testFail;
	}

	/************************************************
	* === Test 3: Set 5 colors and remove parts === *
	************************************************/
	cout << adjRight ( 4, 0 ) << ++env.testCount << " Set 5 colors and remove parts from them" << endl;
	testOK = 0;

	// 16.: Silver and red
	testOK += setRGB( 16, WC, "Set silver", 0xc0, 0xc0, 0xc0 );
	testOK += delRGB(    WC, "remove red", 0xff, 0x00, 0x00, 0x0, 0xc0, 0xc0 );

	// 17.: Violet and Blue
	testOK += setRGB( 17, WC, "Set violet",  0xff, 0x00, 0xff );
	testOK += delRGB(    WC, "remove blue", 0x00, 0x00, 0xff, 0xff, 0x00, 0x00 );

	// 18.: Turquoise and Green
	testOK += setRGB( 18, WC, "Set turquoise", 0x00, 0xff, 0xff );
	testOK += delRGB(    WC, "remove green",  0x00, 0xff, 0x00, 0x00, 0x00, 0xff );

	// 19.: Khaki 3 and Blue
	testOK += setRGB( 19, WC, "Set khaki 3",   0xcd, 0xc6, 0x73 );
	testOK += delRGB(    WC, "remove silver", 0xc0, 0xc0, 0xc0, 0x0d, 0x06, 0x00 );

	// 20.: Sailor Blue and violet
	testOK += setRGB( 20, WC, "Set sailor blue", 0x5f, 0x9e, 0xa0 );
	testOK += delRGB(    WC, "remove violet",   0xff, 0x00, 0xff, 0x00, 0x9e, 0x00 );

	if ( 10 == testOK ) {
		cout << " --- all colors correct ---\n" << endl;
		++env.testSuccess;
	} else {
		cout << " --- " << ( 10 - testOK ) << "/10 colors are wrong! ---\n" << endl;
		++env.testFail;
	}

	/************************************************
	* === Test 4: Set 5 colors and change gamma === *
	************************************************/
	cout << adjRight ( 4, 0 ) << ++env.testCount << " Set 5 colors and change their gamma value" << endl;
	testOK = 0;

	// 21.: Silver, raise to 1.5
	testOK += setRGB  ( 21, WC, "Set silver",  0xc0, 0xc0, 0xc0 );
	testOK += setGamma(    WC, "Raise Gamma", 1.5, 0xff, 0xff, 0xff );

	// 22.: Violet, raise to 2.5
	testOK += setRGB  ( 22, WC, "Set violet",  0xff, 0x00, 0xff );
	testOK += setGamma(    WC, "Raise Gamma", 2.5, 0xff, 0x00, 0xff );

	// 23.: Turquoise, reduce to 0.8
	testOK += setRGB  ( 23, WC, "Set turquoise", 0x00, 0xff, 0xff );
	testOK += setGamma(    WC, "Lower Gamma",   0.8, 0x00, 0xcc, 0xcc );

	// 24.: Khaki 3, reduce to 0.5
	testOK += setRGB  ( 24, WC, "Set khaki 3", 0xcd, 0xc6, 0x73 );
	testOK += setGamma(    WC, "Lower Gamma", 0.5, 0x67, 0x63, 0x3a );

	// 25.: Sailor Blue, triple gamma
	testOK += setRGB  ( 25, WC, "Set sailor blue", 0x5f, 0x9e, 0xa0 );
	testOK += setGamma(    WC, "Triple Gamma",    3.0, 0x97, 0xfc, 0xff );

	if ( 10 == testOK ) {
		cout << " --- all colors correct ---\n" << endl;
		++env.testSuccess;
	} else {
		cout << " --- " << ( 10 - testOK ) << "/10 colors are wrong! ---\n" << endl;
		++env.testFail;
	}

	/***************************************************************
	* === Test 5: Set 5 colors and modify Frequency/Wavelength === *
	***************************************************************/
	cout << adjRight ( 4, 0 ) << ++env.testCount << " Set 5 colors and modify their frequency and wavelength" << endl;
	testOK = 0;

	// 26.: Silver, wavelength * 0.9
	testOK += setRGB ( 26, WC, "Set silver",  0xc0, 0xc0, 0xc0 );
	testOK += modWave(    WC, "Lower wave", 0.9, 0x71, 0xc0, 0x6f );

	// 27.: Violet, wavelength * 1.1
	testOK += setRGB ( 27, WC, "Set violet",  0xff, 0x00, 0xff );
	testOK += modWave(    WC, "Raise wave", 1.1, 0x5d, 0x00, 0xff );

	// 28.: Turquoise, Frequency * 0.9
	testOK += setRGB ( 28, WC, "Set turquoise", 0x00, 0xff, 0xff );
	testOK += modFreq(    WC, "Lower freq",   0.8, 0xff, 0x7f, 0x00 );

	// 29.: Khaki 3, Frequency * 1.1
	testOK += setRGB ( 29, WC, "Set khaki 3", 0xcd, 0xc6, 0x72 );
	testOK += modFreq(    WC, "Raise freq", 1.1, 0x69, 0xcd, 0x4b );

	// 30.: Sailor Blue, Raise both Frequency and wavelength
	testOK += setRGB ( 30, WC, "Set sailor blue", 0x5f, 0x9e, 0xa0 );
	testOK += modFreq(    WC, "Raise freq", 1.15, 0x6b, 0x5f, 0xa0 );
	testOK += modWave(    WC, "Raise wave", 1.15, 0x5f, 0x9e, 0xa0 );

	if ( 11 == testOK ) {
		cout << " --- all colors correct ---\n" << endl;
		++env.testSuccess;
	} else {
		cout << " --- " << ( 11 - testOK ) << "/11 colors are wrong! ---\n" << endl;
		++env.testFail;
	}

	return result;
}


/* =======================================
 * === Helper function implementations ===
 * =======================================
 */

static int addRGB( PWaveColor& wc, char const* const name, uint8_t r, uint8_t g, uint8_t b,
                   uint8_t tR, uint8_t tG, uint8_t tB ) {
	char call[10] = "";
	uint8_t r2, g2, b2;
	int result = 0;

	snprintf( call, 9, "%02x %02x %02x", r, g, b );
	wc.addRGB( r, g, b );
	printRGB( wc, name, call );

	wc.getRGB( r2, g2, b2 );
	if ( ( r2 == tR ) && ( g2 == tG ) && ( b2 == tB ) )
		result = 1;
	printf( "%s\n", result ? "Success" : "FAILURE" );
	return result;
}

static int delRGB( PWaveColor& wc, char const* const name, uint8_t r, uint8_t g, uint8_t b,
                   uint8_t tR, uint8_t tG, uint8_t tB ) {
	char call[10] = "";
	uint8_t r2, g2, b2;
	int result = 0;

	snprintf( call, 9, "%02x %02x %02x", r, g, b );
	wc.delRGB( r, g, b );
	printRGB( wc, name, call );

	wc.getRGB( r2, g2, b2 );
	if ( ( r2 == tR ) && ( g2 == tG ) && ( b2 == tB ) )
		result = 1;
	printf( "%s\n", result ? "Success" : "FAILURE" );
	return result;
}

static int modFreq( PWaveColor& wc, char const* const name, double modifier,
                    uint8_t tR, uint8_t tG, uint8_t tB ) {
	char call[10]  = "";
	char fTxt[33] = "";
	std::string modTxt = "--> ";
	uint8_t r2, g2, b2;
	int result = 0;

	snprintf( call, 9, "%2.3f", modifier );

	for ( size_t i = 0; i < wc.size(); ++i ) {
		if ( i > 0 )
			modTxt += ", ";
		snprintf( fTxt, 32, "%g", wc.getFrequency( i ) / 1000.0 );
		modTxt += fTxt;
		modTxt += "THz -> ";
		wc.modFrequency( i, modifier );
		snprintf( fTxt, 32, "%g", wc.getFrequency( i ) / 1000.0 );
		modTxt += fTxt;
		modTxt += "THz";
	}
	printRGB( wc, name, call, false );

	wc.getRGB( r2, g2, b2 );
	if ( ( r2 == tR ) && ( g2 == tG ) && ( b2 == tB ) )
		result = 1;
	printf( "%s: %s\n", modTxt.c_str(), result ? "Success" : "FAILURE" );

	return result;
}

static int modWave( PWaveColor& wc, char const* const name, double modifier,
                    uint8_t tR, uint8_t tG, uint8_t tB ) {
	char call[10] = "";
	char fTxt[33] = "";
	std::string modTxt = "--> ";
	uint8_t r2, g2, b2;
	int result = 0;

	snprintf( call, 9, "%2.3f", modifier );

	for ( size_t i = 0; i < wc.size(); ++i ) {
		if ( i > 0 )
			modTxt += ", ";
		snprintf( fTxt, 32, "%g", wc.getWavelength( i ) );
		modTxt += fTxt;
		modTxt += "nm -> ";
		wc.modWavelength( i, modifier );
		snprintf( fTxt, 32, "%g", wc.getWavelength( i ) );
		modTxt += fTxt;
		modTxt += "nm";
	}
	printRGB( wc, name, call, false );

	wc.getRGB( r2, g2, b2 );
	if ( ( r2 == tR ) && ( g2 == tG ) && ( b2 == tB ) )
		result = 1;
	printf( "%s: %s\n", modTxt.c_str(), result ? "Success" : "FAILURE" );

	return result;
}

static void printRGB( PWaveColor& wc, char const* const name, char const* const call, bool isEnd ) {
	uint8_t r, g, b;
	wc.getRGB( r, g, b );
	printf ( "--> %-15s [Call %8s]: %2d waves -> RGB %02x %02x %02x %s",
	         name, call, static_cast<int32_t>( wc.size() ), r, g, b,
	         isEnd ? ": " : "" );
}

static void printRGB( int32_t nr, PWaveColor& wc, char const* const name, char const* const call, bool isEnd ) {
	uint8_t r, g, b;
	wc.getRGB( r, g, b );
	printf ( "%02d. %-15s [Call %8s]: %2d waves -> RGB %02x %02x %02x %s",
	         nr, name, call, static_cast<int32_t>( wc.size() ), r, g, b,
	         isEnd ? ": " : "" );
}

static int setGamma( PWaveColor& wc, char const* const name, double gamma,
                     uint8_t tR, uint8_t tG, uint8_t tB ) {
	char call[10] = "";
	uint8_t r2, g2, b2;
	int result = 0;

	snprintf( call, 9, "%2.3f", gamma );
	wc.setGamma( gamma );
	printRGB( wc, name, call );

	wc.getRGB( r2, g2, b2 );
	if ( ( r2 == tR ) && ( g2 == tG ) && ( b2 == tB ) )
		result = 1;
	printf( "%s\n", result ? "Success" : "FAILURE" );

	return result;
}

static int setRGB( int32_t nr, PWaveColor& wc, char const* const name, uint8_t r, uint8_t g, uint8_t b ) {
	char call[10] = "";
	uint8_t r2, g2, b2;
	int result = 0;

	snprintf( call, 9, "%02x %02x %02x", r, g, b );
	wc.setGamma( 1.0 );
	wc.setRGB( r, g, b );
	printRGB( nr, wc, name, call );

	wc.getRGB( r2, g2, b2 );
	if ( ( r2 == r ) && ( g2 == g ) && ( b2 == b ) )
		result = 1;
	printf( "%s\n", result ? "Success" : "FAILURE" );
	return result;
}


