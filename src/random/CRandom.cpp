/** @file CRandom.cpp
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2020 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  *
  * History and change log are maintained in pwxlib.h
**/


#include <cassert>
#include <cmath>
#include <cstring>
#include <string>

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "basic/mem_utils.h"
#include "random/CRandom.h"
#include "random/CRandomTHash.h"
#include "random/CRandomTRandom.h"
#include "random/CRandomWordConstants.h"
#include "stream_helpers/StreamHelpers.h"


/// @namespace pwx
namespace pwx {


/* --------------------------------------- *
 * --- Private Methods Implementations --- *
 * --------------------------------------- */

/** @brief checkRule - check state and character against followup matrix rules from namecon
  * Note: It is assumed, that all three characters are lowercase
**/
void CRandom::checkRule ( uint32_t& state, char const first, char const second, char const third ) noexcept {
	int32_t one   = FUM_IDX( first );
	int32_t two   = FUM_IDX( second );
	int32_t three = FUM_IDX( third );

	assert ( ( one > -1 ) && ( two > -1 ) && ( three > -1 )
	         && "ERROR: checkRule() with at least one illegal character called!" );

	if ( ( ( one == two ) && ( two == three ) ) // eliminate triple threats
	                || ( 0 == ( FUM_IDX_RULE( nst, one, two ) & ( 1 << three ) ) ) ) {
		// The desired character is not allowed to follow the set two chars
		if ( state & NameConstants::genNextIsCon )
			state ^= NameConstants::genNextIsCon;
		if ( state & NameConstants::genNextIsVow )
			state ^= NameConstants::genNextIsVow;
	}
}


/** @brief generate a syllable out of various rules
**/
int32_t CRandom::genSyllable ( double& idx, double step, char* syll, uint32_t& state, char* lastChrs ) noexcept {
	int32_t  charCount = 0;
	int32_t  charIndex = 0;
	char     nextCon   = 0;     // Shortcut to the next consonant
	char     nextVow   = 0;     // Shortcut to the next vowel
	uint32_t oldState  = state; // save state to revert if generation failed
	char     oldLstChrs[2];     // Save lastChar to revert back if generation fails
	int32_t  oldWasLast = 0;    // Saves genLastIsVow(Con) to revert if the neding is illegal
	int32_t  conCount  = 0;     // Count generated consonants
	int32_t  vowCount  = 0;     // Count generated vowels
	int32_t  genTries  = 8;     // Eight tries to generate a syllable. That should be enough!
	double   endChance = 0.0;   // propability to end (early)
	double   vowChance = 0.0;   // 50% if last was consonant, 25% if last was vowel

	// Initialize syll and state
	memset ( syll, 0, 5 );
	state |= NameConstants::genRoundA;

	// save lastChrs
	assert ( lastChrs );
	oldLstChrs[0] = lastChrs[0];
	oldLstChrs[1] = lastChrs[1];

	// do - while state has no NameConstants::genSyllEnd
	do {
		/*********************************************************************
		 * 1) Determine charIndex and set shortcuts                          *
		 *********************************************************************/
		charIndex = static_cast<int32_t> ( round ( abs ( idx ) ) );
		nextCon   = CL_CHR( nst, charIndex );
		nextVow   = VL_CHR( nst, charIndex );
		assert ( ( nextCon > 0x60 ) && ( nextCon < 0x7b )
		         && ( ( ( nextVow > 0x60 ) && ( nextVow < 0x7b ) )
		              || IS_UMLAUT_A( nextVow )
		              || IS_UMLAUT_O( nextVow )
		              || IS_UMLAUT_U( nextVow ) )
		         && "[C/V]L_CHR() returned illegal chars!" );

		/*********************************************************************
		 * 2) Set chance to select a vowel next                              *
		 *********************************************************************/
		if ( state & NameConstants::genLastIsVow ) {
			state     ^= NameConstants::genLastIsVow;
			oldWasLast = NameConstants::genLastIsVow;
			vowChance  = 0.5; // 25% chance of a double vowel
		} else if ( state & NameConstants::genLastIsCon ) {
			state     ^= NameConstants::genLastIsCon;
			oldWasLast = NameConstants::genLastIsCon;
			vowChance  = 0.0; // 50% chance of a vowel to be next
		} else {
			vowChance  = 0.33; // 33% chance of a vowel to be next
			oldWasLast = 0;
		}

		/*********************************************************************
		 * 3) Decide whether to go for a consonant or a vowel                *
		 *********************************************************************/
		if ( simplex3D ( idx, step, charCount ) >= vowChance )
			// Next shall be a vowel
			state |= NameConstants::genNextIsVow;
		else
			// Next shall be consonant
			state |= NameConstants::genNextIsCon;

		/*********************************************************************
		 * 4) Check lastChars + firstChar against nameFUM for Round A        *
		 *********************************************************************/
		if ( ( state & NameConstants::genRoundA ) && ( 0 == ( state & NameConstants::genPartStart ) ) ) {
			if ( state & NameConstants::genNextIsCon )
				checkRule ( state, lastChrs[0], lastChrs[1], nextCon );
			else if ( state & NameConstants::genNextIsVow )
				checkRule ( state, lastChrs[0], lastChrs[1], nextVow );
		}

		/*********************************************************************
		 * 5) call checkRule() for Round B with lastChrs or check position   *
		 *********************************************************************/
		if ( state & NameConstants::genRoundB ) {
			// First, handle part start rules:
			if ( state & NameConstants::genPartStart ) {
				// On a part start, we need to check against the position:
				if ( ( state & NameConstants::genNextIsCon )
				                && !FUM_ALLOW_START( nst, syll[0], nextCon ) )
					// What a pity, this combination is illegal on a part start
					state ^= NameConstants::genNextIsCon;
				else if ( ( state & NameConstants::genNextIsVow )
				                && !FUM_ALLOW_START( nst, syll[0], nextVow ) )
					// Nope, this vowel isn't  creating a legal part start
					state ^= NameConstants::genNextIsVow;
			} else {
				// Somewhere else this is a normal checkrule
				if ( state & NameConstants::genNextIsCon ) {
					if ( FUM_ALLOW_MIDDLE( nst, syll[0], nextCon ) )
						checkRule ( state, lastChrs[1], syll[0], nextCon );
					else
						// What a pity, this combination is illegal in the middle of a part
						state ^= NameConstants::genNextIsCon;
				}
				if ( state & NameConstants::genNextIsVow ) {
					if ( FUM_ALLOW_MIDDLE( nst, syll[0], nextVow ) )
						checkRule ( state, lastChrs[1], syll[0], nextVow );
					else
						// What a pity, this combination is illegal in the middle of a part
						state ^= NameConstants::genNextIsCon;
				}
			}
		}

		/*********************************************************************
		 * 6) call checkRule() for Round C and D                             *
		 *********************************************************************/
		if ( state & ( NameConstants::genRoundC | NameConstants::genRoundD ) ) {
			if ( state & NameConstants::genNextIsCon )
				checkRule ( state, syll[charCount - 2], syll[charCount - 1], nextCon );
			if ( state & NameConstants::genNextIsVow )
				checkRule ( state, syll[charCount - 2], syll[charCount - 1], nextVow );
		}

		/*********************************************************************
		 * 7) Add the selected char if we can or decrease genTries otherwise *
		 *********************************************************************/
		if ( state & NameConstants::genNextIsCon ) {
			syll[charCount++] = nextCon;
			state ^= NameConstants::genNextIsCon;
			state |= NameConstants::genLastIsCon | NameConstants::genHasNextChar;
			conCount++;
		} else if ( state & NameConstants::genNextIsVow ) {
			syll[charCount++] = nextVow;
			state ^= NameConstants::genNextIsVow;
			state |= NameConstants::genLastIsVow | NameConstants::genHasNextChar;
			vowCount++;
		} else
			// We have nothing, so reduce our tries:
			genTries--;

		/*********************************************************************
		 * 8) If we added a char, advance the round                          *
		 **********************************************************************/
		if ( state & NameConstants::genHasNextChar ) {
			state ^= NameConstants::genHasNextChar;
			if ( state & NameConstants::genRoundD ) {
				// We have to End here:
				state ^= NameConstants::genRoundD;
				endChance = 2.0; // 100% chance to stop here
			} else if ( state & NameConstants::genRoundC ) {
				// Simple Advance
				state ^= NameConstants::genRoundC;
				state |= NameConstants::genRoundD;
				endChance = 0.20; // 60% chance to stop
			} else if ( state & NameConstants::genRoundB ) {
				// Simple advance
				state ^= NameConstants::genRoundB;
				state |= NameConstants::genRoundC;
				endChance = -0.50; // 25% chance to stop
			} else if ( state & NameConstants::genRoundA ) {
				// Simple advance
				state ^= NameConstants::genRoundA;
				state |= NameConstants::genRoundB;
				endChance = -2.0; // 0% chance to stop
			}

			// If this is a single syll (both part start and end), the endChance has to be reduced:
			if ( ( state & NameConstants::genPartStart ) && ( state & NameConstants::genPartEnd ) )
				endChance -= 0.3;
			// so 45% (RoundC) or 10% (RoundB) chance. D is too high to be affected.

			/*********************************************************************
			 * 9) Check against chance to end this syllable                      *
			 *********************************************************************/
			if ( simplex3D ( idx, charIndex, ( charCount * conCount ) + ( genTries * vowCount ) ) <= endChance ) {
				// We shall stop! But are we allowed to?
				if ( ( ( state & NameConstants::genPartEnd )  // This part ends, so:
				                // Is this combination allowed at a Parts end?
				                && ( FUM_ALLOW_END( nst, syll[charCount - 2], syll[charCount - 1] ) ) )
				                || ( ( 0 == ( state & NameConstants::genPartEnd ) ) // This part shall go on, so:
				                     // Is this combination allowed in the middle?
				                     && ( FUM_ALLOW_MIDDLE( nst, syll[charCount - 2], syll[charCount - 1] ) ) ) )
					// Yeeees!
					state |= NameConstants::genSyllEnd;
				else if ( ( ( 0 == ( state & ( NameConstants::genRoundC | NameConstants::genRoundD ) ) )
				                // If we do not have a fourth char, yet, a noise() chance is taken:
				                || ( ( ( state & NameConstants::genRoundC || state & NameConstants::genRoundD )
				                       && ( noise ( hash ( static_cast<int32_t> (
				                                       step * ( idx + charIndex + charCount
				                                                       + genTries + vowCount ) ) ) ) > 0 ) )
				                   ) ) && --genTries ) {
					// We simply search for a new char:
					syll[charCount--] = 0x0;
					if ( state & NameConstants::genRoundC ) {
						state ^= NameConstants::genRoundC;
						state |= NameConstants::genRoundB;
					} else if ( state & NameConstants::genRoundD ) {
						state ^= NameConstants::genRoundD;
						state |= NameConstants::genRoundC;
					} else
						state |= NameConstants::genRoundD;

					// Revert the counts:
					if ( state & NameConstants::genLastIsCon ) {
						state ^= NameConstants::genLastIsCon;
						conCount--;
					} else if ( state & NameConstants::genLastIsVow ) {
						state ^= NameConstants::genLastIsVow;
						vowCount--;
					}

					// Remember the previous char:
					state |= oldWasLast;
				} // end of reverting
				// No else, as a different state simply means we do not end here
			} // End of shall end here
		} // End if a char is found

		/*********************************************************************
		 * 10) No matter what happened, advance the index!                   *
		 *********************************************************************/

		idx += step;
	} while ( ( genTries > 0 ) && ( 0 == ( state & NameConstants::genSyllEnd ) ) );

	/*********************************************************************
	 * 11) if genTries reached zero or we have 0 vow/conCount, we fail   *
	 *     Further we check again if both part start and part end are    *
	 *     valid if they are used/set/forced.                            *
	 *********************************************************************/

	// Do some tests about part starts, ends and the count of vowels/consonatns
	if ( genTries && ( charCount > 1 ) && vowCount && conCount ) {
		// If this is not a part end, but the last chars do not allow
		// follow up characters, we have to force an ending:
		if ( ( 0 == ( state & NameConstants::genPartEnd ) )
		                && FUM_MUST_FINISH( nst, syll[charCount - 2], syll[charCount - 1] ) )
			// Yep, we have to
			state |= NameConstants::genPartEnd;

		// To continue the combination of the first two characters must be allowed
		// if this is a part start, and the last two must be allowed if this is a
		// part end.
		if ( ( ( state & NameConstants::genPartStart ) // check part start
		                && !FUM_ALLOW_START( nst, syll[0], syll[1] ) )
		                || ( ( state & NameConstants::genPartEnd )   // check part end
		                     && !FUM_ALLOW_END( nst, syll[charCount - 2], syll[charCount - 1] ) ) )
			genTries = 0;
	} else
		genTries = 0;


	// Finally carry on if we have genTries left, no tries left indicate failure.
	if ( genTries ) {
		// great!
		state ^= NameConstants::genSyllEnd;
		if ( state & NameConstants::genRoundC )
			state ^= NameConstants::genRoundC; // might have advanced from B, then ended
		else if ( state & NameConstants::genRoundD )
			state ^= NameConstants::genRoundD; // might have advanced from C, then ended
		// We don't end from A (advanced to B)

		// We need to record the last two chars:
		lastChrs[0] = syll[charCount - 2];
		lastChrs[1] = syll[charCount - 1];

		// If this is a part start, we have to change the first character to upper case
		if ( state & NameConstants::genPartStart ) {
			state ^= NameConstants::genPartStart;
			syll[0] -= NameConstants::chrOffsetDown;
		}

		// We keep genLastIsCon/Vow for the next round
	} else {
		// what a pity...
		state = oldState;
		lastChrs[0] = oldLstChrs[0];
		lastChrs[1] = oldLstChrs[1];
		memset ( syll, 0, 5 );
		charCount = 0;
	}

	return ( charCount );
}


/** @brief generate a stepping for rndName() - result <= -1.0 || 1.0 <= result
  *
  * i = index, cl = charsLeft, sl = syllsLeft, pl = partsLeft
**/
double CRandom::getStepping ( double i, double x, double y, double z, double w, int32_t cl, int32_t sl, int32_t pl ) noexcept {
	double result = ( i * noise ( cl ) * noiseD ( x,    z ) )
	                + ( i * noise ( sl ) * noiseD ( y,    w ) )
	                + ( i * noise ( pl ) * noiseD ( x, y, z, w ) );
	uint32_t ll = std::min( CL_LEN( nst ), VL_LEN( nst ) );
	uint32_t ul = std::max( CL_LEN( nst ), VL_LEN( nst ) );

	if ( ( result <  0 ) && ( result > -1.0 ) ) result = -2.0 + noiseD ( i );
	if ( ( result >= 0 ) && ( result <  1.0 ) ) result =  2.0 + noiseD ( i );

	// The result will be between the size of the vowel
	// and the consonant array.
	while ( static_cast<uint32_t>( abs ( result ) ) >= ul ) result /= 7.3673L;
	while ( static_cast<uint32_t>( abs ( result ) ) <= ll ) result *= 1.7667L;
	return ( result );
}


/** @brief get Simplex Dot for one dimension
**/
double CRandom::getSimpDot ( int32_t index, double x ) noexcept {
	assert ( ( index >= 0 ) && ( index < 4 ) );
	return ( ( constants::spxGrTab[index][0] * x ) );
}


/** @brief get Simplex Dot for second dimension
**/
double CRandom::getSimpDot ( int32_t index, double x, double y ) noexcept {
	assert ( ( index >= 0 ) && ( index < 8 ) );
	return ( ( constants::spxGrTab[index][0] * x )
	         + ( constants::spxGrTab[index][1] * y ) );
}


/** @brief get Simplex Dot for third dimension
**/
double CRandom::getSimpDot ( int32_t index, double x, double y, double z ) noexcept {
	assert ( ( index >= 0 ) && ( index < 12 ) );
	return ( ( constants::spxGrTab[index][0] * x )
	         + ( constants::spxGrTab[index][1] * y )
	         + ( constants::spxGrTab[index][2] * z ) );
}


/** @brief get Simplex Dot for fourth dimension
**/
double CRandom::getSimpDot ( int32_t index, double x, double y, double z, double w ) noexcept {
	assert ( ( index >= 0 ) && ( index < 32 ) );
	return ( ( constants::spxGrTab[index][0] * x )
	         + ( constants::spxGrTab[index][1] * y )
	         + ( constants::spxGrTab[index][2] * z )
	         + ( constants::spxGrTab[index][3] * w ) );
}


/** @brief simplex noise 1D
  *
  * Simplex noise for one dimensions. This method returns a noise value between -1.0 and 1.0
  *
  * @param[in] x X-Coordinate of the simplex point
  * @return Noise value between -1.0 and 1.0
**/
double CRandom::getSpx1D ( double x ) noexcept {
	double contrib = 0.0;

	spxNorms[0] = static_cast<int32_t> ( std::floor ( x ) ); // Normalized X-Coordinate
	spxPerms[0] = spxNorms[0] & 0x000000ff; // X-Coordinate factor for Permutation Table

	// Distances from left and right edge
	spxDist[0][0] = x   - spxNorms[0];
	spxDist[1][0] = 1.0 - spxDist[0][0];

	// Permuted numbers, normalized to a range of 0 to 3
	spxGrads[0] = spxTab[spxPerms[0]]     % 4;
	spxGrads[1] = spxTab[spxPerms[0] + 1] % 4;

	// Calculate the contribution from the two edges
	contrib = 0.75 - std::pow ( spxDist[0][0], 2 );
	spxCorn[0] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[0], noiseD ( x ) ) : 0.0;
	contrib = 0.75 - std::pow ( spxDist[1][0], 2 );
	spxCorn[1] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[1], noiseD ( x + 1 ) ) : 0.0;


	// Add contributions from each corner to get the final noise value.
	// The result is a value in the interval [-1,1].
	double result = 3.16049383304737219191338226664811 * ( spxCorn[0] + spxCorn[1] );
	// Note: This factor has been found by searching the factor needed
	//       To get 1.0 with the largest result out of 100M iterations
	if ( result >  1.0l ) result =  1.0l;
	if ( result < -1.0l ) result = -1.0l;
	return ( result );
}


/** @brief simplex noise 2D
  *
  * Simplex noise for two dimensions. This method returns a noise value between -1.0 and 1.0
  *
  * @param[in] x X-Coordinate of the simplex point
  * @param[in] y Y-Coordinate of the simplex point
  * @return Noise value between -1.0 and 1.0
**/
double CRandom::getSpx2D ( double x, double y ) noexcept {
	double contrib = x + ( ( x + y ) * constants::spxSkew[0][0] );
	spxNorms[0] = static_cast<int32_t> ( std::floor ( contrib ) ); // Normalized X-Coordinate
	contrib = y + ( ( x + y ) * constants::spxSkew[0][0] );
	spxNorms[1] = static_cast<int32_t> ( std::floor ( contrib ) ); // Normalized Y-Coordinate
	spxPerms[0] = spxNorms[0] & 0x000000ff; // X-Coordinate factor for Permutation Table
	spxPerms[1] = spxNorms[1] & 0x000000ff; // Y-Coordinate factor for Permutation Table

	// Distances from corners, middle and last corner are filled when offsets are clear
	spxDist[0][0] = x - ( spxNorms[0] - ( ( spxNorms[0] + spxNorms[1] ) * constants::spxSkew[0][1] ) );
	spxDist[0][1] = y - ( spxNorms[1] - ( ( spxNorms[0] + spxNorms[1] ) * constants::spxSkew[0][1] ) );

	spxOffs[0][0] = ( spxDist[0][0] > spxDist[0][1] ) ? 1 : 0; // Upper triangle (1, 0),
	spxOffs[0][1] = ( spxDist[0][0] > spxDist[0][1] ) ? 0 : 1; // Lower triangle (0, 1).

	// Distance from middle corner
	spxDist[1][0] = spxDist[0][0] - spxOffs[0][0] + constants::spxSkew[0][1];
	spxDist[1][1] = spxDist[0][1] - spxOffs[0][1] + constants::spxSkew[0][1];

	// Distance from last corner
	spxDist[2][0] = spxDist[0][0] - 1.0 + ( 2.0 * constants::spxSkew[0][1] );
	spxDist[2][1] = spxDist[0][1] - 1.0 + ( 2.0 * constants::spxSkew[0][1] );

	// Permuted numbers, normalized to a range of 0 to 7
	spxGrads[0] = spxTab[spxPerms[0] + spxTab[spxPerms[1]]] % 8;
	spxGrads[1] = spxTab[spxPerms[0] + spxOffs[0][0] + spxTab[spxPerms[1] + spxOffs[0][1]]] % 8;
	spxGrads[2] = spxTab[spxPerms[0] + 1 + spxTab[spxPerms[1] + 1]] % 8;

	// Calculate the contribution from the three corners
	contrib = 0.5 - std::pow ( spxDist[0][0], 2 ) - std::pow ( spxDist[0][1], 2 );
	spxCorn[0] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[0], spxDist[0][0], spxDist[0][1] ) : 0.0;
	contrib = 0.5 - std::pow ( spxDist[1][0], 2 ) - std::pow ( spxDist[1][1], 2 );
	spxCorn[1] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[1], spxDist[1][0], spxDist[1][1] ) : 0.0;
	contrib = 0.5 - std::pow ( spxDist[2][0], 2 ) - std::pow ( spxDist[2][1], 2 );
	spxCorn[2] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[2], spxDist[2][0], spxDist[2][1] ) : 0.0;
	// Note: This is not looped, because the loop would produce more overhead than it is worth to just have 3 less lines.

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
	double result = 70.14805770653948968629265436902642 * ( spxCorn[0] + spxCorn[1] + spxCorn[2] );
	// Note: This factor has been found by searching the factor needed
	//       To get 1.0 with the largest result out of 100M iterations
	if ( result >  1.0l ) result =  1.0l;
	if ( result < -1.0l ) result = -1.0l;
	return ( result );
}


/** @brief simplex noise 3D
  *
  * Simplex noise for three dimensions. This method returns a noise value between -1.0 and 1.0
  *
  * @param[in] x X-Coordinate of the simplex point
  * @param[in] y Y-Coordinate of the simplex point
  * @param[in] z Z-Coordinate of the simplex point
  * @return Noise value between -1.0 and 1.0
**/
double CRandom::getSpx3D ( double x, double y, double z ) noexcept {
	double contrib = x + ( ( x + y + z ) * constants::spxSkew[1][0] );
	spxNorms[0] = static_cast<int32_t> ( std::floor ( contrib ) ); // Normalized X-Coordinate
	contrib = y + ( ( x + y + z ) * constants::spxSkew[1][0] );
	spxNorms[1] = static_cast<int32_t> ( std::floor ( contrib ) ); // Normalized Y-Coordinate
	contrib = z + ( ( x + y + z ) * constants::spxSkew[1][0] );
	spxNorms[2] = static_cast<int32_t> ( std::floor ( contrib ) ); // Normalized Z-Coordinate
	spxPerms[0] = spxNorms[0] & 0x000000ff; // X-Coordinate factor for Permutation Table
	spxPerms[1] = spxNorms[1] & 0x000000ff; // Y-Coordinate factor for Permutation Table
	spxPerms[2] = spxNorms[2] & 0x000000ff; // Z-Coordinate factor for Permutation Table

	// Distances from corners, second, third and last corner are filled when offsets are clear
	spxDist[0][0] = x - ( spxNorms[0] - ( ( spxNorms[0] + spxNorms[1] + spxNorms[2] ) * constants::spxSkew[1][1] ) );
	spxDist[0][1] = y - ( spxNorms[1] - ( ( spxNorms[0] + spxNorms[1] + spxNorms[2] ) * constants::spxSkew[1][1] ) );
	spxDist[0][2] = z - ( spxNorms[2] - ( ( spxNorms[0] + spxNorms[1] + spxNorms[2] ) * constants::spxSkew[1][1] ) );

	// For the 3D case, the simplex shape is a slightly irregular tetrahedron.
	// Determine which simplex we are in.
	if ( spxDist[0][0] >= spxDist[0][1] ) {
		if ( spxDist[0][1] >= spxDist[0][2] ) {
			// X Y Z order
			spxOffs[0][0] = 1;
			spxOffs[0][1] = 0;
			spxOffs[0][2] = 0;
			spxOffs[1][0] = 1;
			spxOffs[1][1] = 1;
			spxOffs[1][2] = 0;
		} else if ( spxDist[0][0] >= spxDist[0][2] ) {
			// X Z Y order
			spxOffs[0][0] = 1;
			spxOffs[0][1] = 0;
			spxOffs[0][2] = 0;
			spxOffs[1][0] = 1;
			spxOffs[1][1] = 0;
			spxOffs[1][2] = 1;
		} else {
			// Z X Y order
			spxOffs[0][0] = 0;
			spxOffs[0][1] = 0;
			spxOffs[0][2] = 1;
			spxOffs[1][0] = 1;
			spxOffs[1][1] = 0;
			spxOffs[1][2] = 1;
		}
	} else { // spxDist[0][0] < spxDist[0][1]
		if ( spxDist[0][1] < spxDist[0][2] ) {
			// Z Y X order
			spxOffs[0][0] = 0;
			spxOffs[0][1] = 0;
			spxOffs[0][2] = 1;
			spxOffs[1][0] = 0;
			spxOffs[1][1] = 1;
			spxOffs[1][2] = 1;
		} else if ( spxDist[0][0] < spxDist[0][2] ) {
			// Y Z X order
			spxOffs[0][0] = 0;
			spxOffs[0][1] = 1;
			spxOffs[0][2] = 0;
			spxOffs[1][0] = 0;
			spxOffs[1][1] = 1;
			spxOffs[1][2] = 1;
		} else {
			// Y X Z order
			spxOffs[0][0] = 0;
			spxOffs[0][1] = 1;
			spxOffs[0][2] = 0;
			spxOffs[1][0] = 1;
			spxOffs[1][1] = 1;
			spxOffs[1][2] = 0;
		}
	}

	// Distance from second corner
	spxDist[1][0] = spxDist[0][0] - spxOffs[0][0] + constants::spxSkew[1][1];
	spxDist[1][1] = spxDist[0][1] - spxOffs[0][1] + constants::spxSkew[1][1];
	spxDist[1][2] = spxDist[0][2] - spxOffs[0][2] + constants::spxSkew[1][1];

	// Distance from third corner
	spxDist[2][0] = spxDist[0][0] - spxOffs[1][0] + ( 2.0 * constants::spxSkew[1][1] );
	spxDist[2][1] = spxDist[0][1] - spxOffs[1][1] + ( 2.0 * constants::spxSkew[1][1] );
	spxDist[2][2] = spxDist[0][2] - spxOffs[1][2] + ( 2.0 * constants::spxSkew[1][1] );

	// Distance from last corner
	spxDist[3][0] = spxDist[0][0] - 1.0 + ( 3.0 * constants::spxSkew[1][1] );
	spxDist[3][1] = spxDist[0][1] - 1.0 + ( 3.0 * constants::spxSkew[1][1] );
	spxDist[3][2] = spxDist[0][2] - 1.0 + ( 3.0 * constants::spxSkew[1][1] );

	// Permuted numbers, normalized to a range of 0 to 11
	spxGrads[0] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2]]]] % 12;
	spxGrads[1] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxOffs[0][2]] + spxOffs[0][1]] + spxOffs[0][0]] % 12;
	spxGrads[2] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxOffs[1][2]] + spxOffs[1][1]] + spxOffs[1][0]] % 12;
	spxGrads[3] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + 1] + 1] + 1] % 12;

	// Calculate the contribution from the four corners
	contrib = 0.6 - std::pow ( spxDist[0][0], 2 ) - std::pow ( spxDist[0][1], 2 ) - std::pow ( spxDist[0][2], 2 );
	spxCorn[0] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[0], spxDist[0][0], spxDist[0][1], spxDist[0][2] ) : 0.0;
	contrib = 0.6 - std::pow ( spxDist[1][0], 2 ) - std::pow ( spxDist[1][1], 2 ) - std::pow ( spxDist[1][2], 2 );
	spxCorn[1] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[1], spxDist[1][0], spxDist[1][1], spxDist[1][2] ) : 0.0;
	contrib = 0.6 - std::pow ( spxDist[2][0], 2 ) - std::pow ( spxDist[2][1], 2 ) - std::pow ( spxDist[2][2], 2 );
	spxCorn[2] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[2], spxDist[2][0], spxDist[2][1], spxDist[2][2] ) : 0.0;
	contrib = 0.6 - std::pow ( spxDist[3][0], 2 ) - std::pow ( spxDist[3][1], 2 ) - std::pow ( spxDist[3][2], 2 );
	spxCorn[3] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[3], spxDist[3][0], spxDist[3][1], spxDist[3][2] ) : 0.0;

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
	double result = 36.11293688087369702088835765607655 * ( spxCorn[0] + spxCorn[1] + spxCorn[2] + spxCorn[3] );
	// Note: This factor has been found by searching the factor needed
	//       To get 1.0 with the largest result out of 100M iterations
	if ( result >  1.0l ) result =  1.0l;
	if ( result < -1.0l ) result = -1.0l;
	return ( result );
}


/** @brief simplex noise 4D
  *
  * Simplex noise for four dimensions. This method returns a noise value between -1.0 and 1.0
  *
  * @param[in] x X-Coordinate of the simplex point
  * @param[in] y Y-Coordinate of the simplex point
  * @param[in] z Z-Coordinate of the simplex point
  * @param[in] w W-Coordinate of the simplex point
  * @return Noise value between -1.0 and 1.0
**/
double CRandom::getSpx4D ( double x, double y, double z, double w ) noexcept {
	int32_t    traverse;
	double contrib = x + ( ( x + y + z + w ) * constants::spxSkew[2][0] );
	spxNorms[0] = static_cast<int32_t> ( std::floor ( contrib ) ); // Normalized X-Coordinate
	contrib = y + ( ( x + y + z + w ) * constants::spxSkew[2][0] );
	spxNorms[1] = static_cast<int32_t> ( std::floor ( contrib ) ); // Normalized X-Coordinate
	contrib = z + ( ( x + y + z + w ) * constants::spxSkew[2][0] );
	spxNorms[2] = static_cast<int32_t> ( std::floor ( contrib ) ); // Normalized X-Coordinate
	contrib = w + ( ( x + y + z + w ) * constants::spxSkew[2][0] );
	spxNorms[3] = static_cast<int32_t> ( std::floor ( contrib ) ); // Normalized X-Coordinate

	spxPerms[0] = spxNorms[0] & 0x000000ff; // X-Coordinate factor for Permutation Table
	spxPerms[1] = spxNorms[1] & 0x000000ff; // Y-Coordinate factor for Permutation Table
	spxPerms[2] = spxNorms[2] & 0x000000ff; // Z-Coordinate factor for Permutation Table
	spxPerms[3] = spxNorms[3] & 0x000000ff; // W-Coordinate factor for Permutation Table

	// Distances from corners, second, third and last corner are filled when offsets are clear
	spxDist[0][0] = x - ( spxNorms[0] - ( ( spxNorms[0] + spxNorms[1] + spxNorms[2] + spxNorms[3] ) * constants::spxSkew[2][1] ) );
	spxDist[0][1] = y - ( spxNorms[1] - ( ( spxNorms[0] + spxNorms[1] + spxNorms[2] + spxNorms[3] ) * constants::spxSkew[2][1] ) );
	spxDist[0][2] = z - ( spxNorms[2] - ( ( spxNorms[0] + spxNorms[1] + spxNorms[2] + spxNorms[3] ) * constants::spxSkew[2][1] ) );
	spxDist[0][3] = w - ( spxNorms[3] - ( ( spxNorms[0] + spxNorms[1] + spxNorms[2] + spxNorms[3] ) * constants::spxSkew[2][1] ) );

	// For the 4D case, the simplex is a 4D shape.
	// The method below is a good way of finding the ordering of x,y,z,w and
	// then find the correct traversal order for the simplex we are in.
	// First, six pair-wise comparisons are performed between each possible pair
	// of the four coordinates, and the results are used to add up binary bits
	// for an integer index.
	traverse = ( ( spxDist[0][0] > spxDist[0][1] ) ? 32 : 0 )
	           + ( ( spxDist[0][0] > spxDist[0][2] ) ? 16 : 0 )
	           + ( ( spxDist[0][1] > spxDist[0][2] ) ?  8 : 0 )
	           + ( ( spxDist[0][0] > spxDist[0][3] ) ?  4 : 0 )
	           + ( ( spxDist[0][1] > spxDist[0][3] ) ?  2 : 0 )
	           + ( ( spxDist[0][2] > spxDist[0][3] ) ?  1 : 0 );

	// Now we can use constants::spxTrTab to set the coordinates in turn from the largest magnitude.
	// The number 3 is at the position of the largest coordinate.
	spxOffs[0][0] = constants::spxTrTab[traverse][0] >= 3 ? 1 : 0;
	spxOffs[0][1] = constants::spxTrTab[traverse][1] >= 3 ? 1 : 0;
	spxOffs[0][2] = constants::spxTrTab[traverse][2] >= 3 ? 1 : 0;
	spxOffs[0][3] = constants::spxTrTab[traverse][3] >= 3 ? 1 : 0;
	// The number 2 is at the second largest coordinate.
	spxOffs[1][0] = constants::spxTrTab[traverse][0] >= 2 ? 1 : 0;
	spxOffs[1][1] = constants::spxTrTab[traverse][1] >= 2 ? 1 : 0;
	spxOffs[1][2] = constants::spxTrTab[traverse][2] >= 2 ? 1 : 0;
	spxOffs[1][3] = constants::spxTrTab[traverse][3] >= 2 ? 1 : 0;
	// The number 1 is at the second smallest coordinate.
	spxOffs[2][0] = constants::spxTrTab[traverse][0] >= 1 ? 1 : 0;
	spxOffs[2][1] = constants::spxTrTab[traverse][1] >= 1 ? 1 : 0;
	spxOffs[2][2] = constants::spxTrTab[traverse][2] >= 1 ? 1 : 0;
	spxOffs[2][3] = constants::spxTrTab[traverse][3] >= 1 ? 1 : 0;
	// The fifth corner has all coordinate offsets = 1, so no need to look that up.

	// Distance from second corner
	spxDist[1][0] = spxDist[0][0] - spxOffs[0][0] + constants::spxSkew[2][1];
	spxDist[1][1] = spxDist[0][1] - spxOffs[0][1] + constants::spxSkew[2][1];
	spxDist[1][2] = spxDist[0][2] - spxOffs[0][2] + constants::spxSkew[2][1];
	spxDist[1][3] = spxDist[0][3] - spxOffs[0][3] + constants::spxSkew[2][1];

	// Distance from third corner
	spxDist[2][0] = spxDist[0][0] - spxOffs[1][0] + ( 2.0 * constants::spxSkew[2][1] );
	spxDist[2][1] = spxDist[0][1] - spxOffs[1][1] + ( 2.0 * constants::spxSkew[2][1] );
	spxDist[2][2] = spxDist[0][2] - spxOffs[1][2] + ( 2.0 * constants::spxSkew[2][1] );
	spxDist[2][3] = spxDist[0][3] - spxOffs[1][3] + ( 2.0 * constants::spxSkew[2][1] );

	// Distance from fourth corner
	spxDist[3][0] = spxDist[0][0] - spxOffs[2][0] + ( 3.0 * constants::spxSkew[2][1] );
	spxDist[3][1] = spxDist[0][1] - spxOffs[2][1] + ( 3.0 * constants::spxSkew[2][1] );
	spxDist[3][2] = spxDist[0][2] - spxOffs[2][2] + ( 3.0 * constants::spxSkew[2][1] );
	spxDist[3][3] = spxDist[0][3] - spxOffs[2][3] + ( 3.0 * constants::spxSkew[2][1] );

	// Distance from last corner
	spxDist[4][0] = spxDist[0][0] - 1.0 + ( 4.0 * constants::spxSkew[2][1] );
	spxDist[4][1] = spxDist[0][1] - 1.0 + ( 4.0 * constants::spxSkew[2][1] );
	spxDist[4][2] = spxDist[0][2] - 1.0 + ( 4.0 * constants::spxSkew[2][1] );
	spxDist[4][3] = spxDist[0][3] - 1.0 + ( 4.0 * constants::spxSkew[2][1] );

	// Permuted numbers, normalized to a range of 0 to 32
	spxGrads[0] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxTab[spxPerms[3]]]]] % 32;
	spxGrads[1] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxTab[spxPerms[3] + spxOffs[0][3]] + spxOffs[0][2]] +
	                     spxOffs[0][1]] + spxOffs[0][0]] % 32;
	spxGrads[2] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxTab[spxPerms[3] + spxOffs[1][3]] + spxOffs[1][2]] +
	                     spxOffs[1][1]] + spxOffs[1][0]] % 32;
	spxGrads[3] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxTab[spxPerms[3] + spxOffs[2][3]] + spxOffs[2][2]] +
	                     spxOffs[2][1]] + spxOffs[2][0]] % 32;
	spxGrads[4] = spxTab[spxPerms[0] + spxTab[spxPerms[1] + spxTab[spxPerms[2] + spxTab[spxPerms[3] + 1] + 1] + 1] + 1] % 32;

	// Calculate the contribution from the four corners
	contrib = 0.6 - ::std::pow ( spxDist[0][0], 2 ) - std::pow ( spxDist[0][1], 2 ) - std::pow ( spxDist[0][2], 2 ) - std::pow ( spxDist[0][3], 2 );
	spxCorn[0] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[0], spxDist[0][0], spxDist[0][1], spxDist[0][2],
	                spxDist[0][3] ) : 0.0;
	contrib = 0.6 - std::pow ( spxDist[1][0], 2 ) - std::pow ( spxDist[1][1], 2 ) - std::pow ( spxDist[1][2], 2 ) - std::pow ( spxDist[1][3], 2 );
	spxCorn[1] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[1], spxDist[1][0], spxDist[1][1], spxDist[1][2],
	                spxDist[1][3] ) : 0.0;
	contrib = 0.6 - std::pow ( spxDist[2][0], 2 ) - std::pow ( spxDist[2][1], 2 ) - std::pow ( spxDist[2][2], 2 ) - std::pow ( spxDist[2][3], 2 );
	spxCorn[2] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[2], spxDist[2][0], spxDist[2][1], spxDist[2][2],
	                spxDist[2][3] ) : 0.0;
	contrib = 0.6 - std::pow ( spxDist[3][0], 2 ) - std::pow ( spxDist[3][1], 2 ) - std::pow ( spxDist[3][2], 2 ) - std::pow ( spxDist[3][3], 2 );
	spxCorn[3] = ( contrib > 0.0 ) ? std::pow ( contrib, 4 ) * getSimpDot ( spxGrads[3], spxDist[3][0], spxDist[3][1], spxDist[3][2],
	                spxDist[3][3] ) : 0.0;

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
	double result = 31.91239940056049206873467483092099 * ( spxCorn[0] + spxCorn[1] + spxCorn[2] + spxCorn[3] );
	// Note: This factor has been found by searching the factor needed
	//       To get 1.0 with the largest result out of 100M iterations
	if ( result >  1.0l ) result =  1.0l;
	if ( result < -1.0l ) result = -1.0l;
	return ( result );
}


double CRandom::noiseD ( double x ) const noexcept {
	return noise ( doubToInt ( x ) );
}

double CRandom::noiseD ( double x, double y ) const noexcept {
	return noise ( doubToInt ( x ), doubToInt ( y ) );
}

double CRandom::noiseD ( double x, double y, double z ) const noexcept {
	return noise ( doubToInt ( x ), doubToInt ( y ), doubToInt ( z ) );
}

double CRandom::noiseD ( double x, double y, double z, double w ) const noexcept {
	return noise ( doubToInt ( x ), doubToInt ( y ), doubToInt ( z ), doubToInt ( w ) );
}

int32_t CRandom::doubToInt ( double val ) const noexcept {
	// Bring val in range if it is between -1000 and +1000
	while ( ( ( val < 0. ) && ( val > -1000. ) ) // negative range
	                || ( ( val > 0. ) && ( val <  1000. ) ) ) // positive range
		val *= 1000.;

	// Bring val in range if it is smaller than min int or larger than maval int
	while ( ( val < static_cast<double> ( constants::fullMinInt ) ) // negative range
	                || ( val > static_cast<double> ( constants::fullMaxInt ) ) ) // positive range
		val /= 10.;

	return static_cast<int32_t> ( round ( val ) );
}

/* -------------------------------------- *
 * --- Public Methods Implementations --- *
 * -------------------------------------- */

/** @brief default ctor
*
* Initializes the random number generator and assigns a first random value
* to lastRndValue. The seed and Simplex data are initialized as well.
**/
CRandom::CRandom() noexcept
	: nst( NST_NAMES_EN ),
	  seed ( ( private_::private_get_random() - ( private_::randomValueRange / 2 ) ) / 100 ) {
	// Initialize Simplex values:
	for ( int32_t i = 0; i < 5; i++ ) {
		spxCorn[i]    = 0.0;
		spxGrads[i]   = 0;
		for ( int32_t j = 0; j < 4; j++ )
			spxDist[i][j] = 0.0;

		if ( i < 4 ) {
			spxNorms[i] = 0;
			spxPerms[i] = 0;
		}

		if ( i < 3 ) {
			for ( int32_t j = 0; j < 4; j++ )
				spxOffs[i][j] = 0;
		}
	}
}


/** @brief default dtor
*
* Empty default dtor, nothing to be done.
*
**/
CRandom::~CRandom() noexcept
{ }


/** @brief return current seed
  *
  * This method simply returns the current Seed used to manipulate values
  * to calculate Simplex Noise and random names.
  *
  * @return Current Seed
**/
int32_t CRandom::getSeed() const noexcept {
	return ( seed );
}


/** @brief hash an unsigned 16 bit integer to an unsigned 32 bit integer
  *
  * @param[in] key The key to hash
  * @return unsigned 32 bit integer hash
**/
uint32_t CRandom::hash ( int16_t key ) const noexcept {
	return private_::private_hash_int<int16_t>( key );
}


/** @brief hash a signed 16 bit integer to an unsigned 32 bit integer
  *
  * @param[in] key The key to hash
  * @return unsigned 32 bit integer hash
**/
uint32_t CRandom::hash ( uint16_t key ) const noexcept {
	return private_::private_hash_int<uint16_t>( key );
}


/** @brief hash a signed 32 bit integer to an unsigned 32 bit integer
  *
  * @param[in] key The key to hash
  * @return unsigned 32 bit integer hash
**/
uint32_t CRandom::hash ( int32_t key ) const noexcept {
	return private_::private_hash_int<int32_t>( key );
}


/** @brief hash an unsigned 32 bit integer to an unsigned 32 bit integer
  *
  * @param[in] key The key to hash
  * @return unsigned 32 bit integer hash
**/
uint32_t CRandom::hash ( uint32_t key ) const noexcept {
	return private_::private_hash_int<uint32_t>( key );
}


/** @brief hash a signed 64 bit integer to an unsigned 32 bit integer
  *
  * @param[in] key The key to hash
  * @return unsigned 32 bit integer hash
**/
uint32_t CRandom::hash ( int64_t key ) const noexcept {
	return private_::private_hash_int<int64_t>( key );
}


/** @brief hash an unsigned 64 bit integer to an unsigned 32 bit integer
  *
  * @param[in] key The key to hash
  * @return unsigned 32 bit integer hash
**/
uint32_t CRandom::hash ( uint64_t key ) const noexcept {
	return private_::private_hash_int<uint64_t>( key );
}


/** @brief hash float to an unsigned 32 bit integer
  *
  * @param[in] key The key to hash
  * @return unsigned 32 bit integer hash
**/
uint32_t CRandom::hash ( float key ) const noexcept {
	return private_::private_hash_flt<float>( &key );
}


/** @brief hash double to an unsigned 32 bit integer
  *
  * @param[in] key The key to hash
  * @return unsigned 32 bit integer hash
**/
uint32_t CRandom::hash ( double key ) const noexcept {
	return private_::private_hash_flt<double>( &key );
}


/** @brief hash long double to an unsigned 32 bit integer
  *
  * @param[in] key The key to hash
  * @return unsigned 32 bit integer hash
**/
uint32_t CRandom::hash ( long double key ) const noexcept {
	return private_::private_hash_flt<long double>( &key );
}


/** @brief hash C-String to an unsigned 32 bit integer
  *
  * @param[in] key The key to hash
  * @param[in] keyLen if omitted, a 0-terminated C-String is assumed
  * @return unsigned 32 bit integer hash
**/
uint32_t CRandom::hash ( char const* key, size_t keyLen ) const noexcept {
	return private_::private_hash_str( key, keyLen );
}


/** @brief hash an std::string to an unsigned 32 bit integer
  *
  * @param[in] key The key to hash
  * @return unsigned 32 bit integer hash
**/
uint32_t CRandom::hash ( std::string& key ) const noexcept {
	return private_::private_hash_str( key.c_str(), key.size() );
}


/** @brief Switches to the next [N]ame[S]ource[T]ype and returns that
  *
  * @return The next NST or the first, if the last was already set.
**/
eNameSourceType CRandom::nextNST( void ) noexcept {
	return ++nst;
}


/** @brief noise with one dimension
  *
  * This method calculates a noise value between -1.0 and 1.0 out of one integer
  *
  * @param[in] x paramter to transform
  * @return double noise value between -1.0 and +1.0
**/
double CRandom::noise ( int32_t x ) const noexcept {
	return ( 1.0 - ( static_cast<double>( hash( x ) ) / constants::noiseMod ) );
}


/** @brief noise with two dimensions
  *
  * This method calculates a noise value between -1.0 and 1.0 out of two integers
  *
  * @param[in] x paramter to transform
  * @param[in] y paramter to transform
  * @return double noise value between -1.0 and +1.0
**/
double CRandom::noise ( int32_t x, int32_t y ) const noexcept {
	return ( 1.0 - ( static_cast<double> (
	                         ( hash( x ) & constants::fullMaxInt )
	                         + ( hash( y ) & constants::fullMaxInt )
	                 ) / constants::noiseMod ) );
}


/** @brief noise with three dimensions
  *
  * This method calculates a noise value between -1.0 and 1.0 out of three integers
  *
  * @param[in] x paramter to transform
  * @param[in] y paramter to transform
  * @param[in] z paramter to transform
  * @return double noise value between -1.0 and +1.0
**/
double CRandom::noise ( int32_t x, int32_t y, int32_t z ) const noexcept {
	return ( 1.0 - ( static_cast<double> (
	                         ( hash( x ) & constants::fullMaxInt )
	                         + ( hash( y ) & constants::halfMaxInt )
	                         + ( hash( z ) & constants::halfMaxInt )
	                 ) / constants::noiseMod ) );
}


/** @brief noise with four dimensions
  *
  * This method calculates a noise value between -1.0 and 1.0 out of four integers
  *
  * @param[in] x paramter to transform
  * @param[in] y paramter to transform
  * @param[in] z paramter to transform
  * @param[in] w paramter to transform
  * @return double noise value between -1.0 and +1.0
**/
double CRandom::noise ( int32_t x, int32_t y, int32_t z, int32_t w ) const noexcept {
	return ( 1.0 - ( static_cast<double> (
	                         ( hash( x ) & constants::halfMaxInt )
	                         + ( hash( y ) & constants::halfMaxInt )
	                         + ( hash( z ) & constants::halfMaxInt )
	                         + ( hash( w ) & constants::halfMaxInt )
	                 ) / constants::noiseMod ) );
}


/** @brief Switches to the previous [N]ame[S]ource[T]ype and returns that
  *
  * @return The previous NST or the last, if the first was already set.
**/
eNameSourceType CRandom::prevNST( void ) noexcept {
	return --nst;
}


/** @brief Generate a random value of int16_t between 0 and @a max.
  *
  * if a negative @a max is submitted, the result will be @a max <= result <= 0.
  *
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
int16_t CRandom::random ( int16_t max ) noexcept {
	return private_::private_random<int16_t>( 0, max );
}


/** @brief Generate a random value of int16_t between @a min and @a max.
  *
  * if @a max is lower than @a min, the result will be @a max <= result <= @a min.
  *
  * @param[in] min Minimum result.
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
int16_t CRandom::random ( int16_t min, int16_t max ) noexcept {
	return private_::private_random<int16_t>( min, max );
}


/** @brief Generate a random value of uint16_t between 0 and @a max.
  *
  * if a negative @a max is submitted, the result will be @a max <= result <= 0.
  *
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
uint16_t CRandom::random ( uint16_t max ) noexcept {
	return private_::private_random<uint16_t>( 0, max );
}


/** @brief Generate a random value of uint16_t between @a min and @a max.
  *
  * if @a max is lower than @a min, the result will be @a max <= result <= @a min.
  *
  * @param[in] min Minimum result.
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
uint16_t CRandom::random ( uint16_t min, uint16_t max ) noexcept {
	return private_::private_random<uint16_t>( min, max );
}


/** @brief Generate a random value of int32_t between 0 and @a max.
  *
  * if a negative @a max is submitted, the result will be @a max <= result <= 0.
  *
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
int32_t CRandom::random ( int32_t max ) noexcept {
	return private_::private_random<int32_t>( 0, max );
}


/** @brief Generate a random value of int32_t between @a min and @a max.
  *
  * if @a max is lower than @a min, the result will be @a max <= result <= @a min.
  *
  * This method has a default value of RAND_MAX for @a max.
  *
  * @param[in] min Minimum result.
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
int32_t CRandom::random ( int32_t min, int32_t max ) noexcept {
	return private_::private_random<int32_t>( min, max );
}


/** @brief Generate a random value of uint32_t between 0 and @a max.
  *
  * if a negative @a max is submitted, the result will be @a max <= result <= 0.
  *
  * This method has a default value of RAND_MAX for @a max.
  *
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
uint32_t CRandom::random ( uint32_t max ) noexcept {
	return private_::private_random<uint32_t>( 0, max );
}


/** @brief Generate a random value of uint32_t between @a min and @a max.
  *
  * if @a max is lower than @a min, the result will be @a max <= result <= @a min.
  *
  * @param[in] min Minimum result.
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
uint32_t CRandom::random ( uint32_t min, uint32_t max ) noexcept {
	return private_::private_random<uint32_t>( min, max );
}


/** @brief Generate a random value of int64_t between 0 and @a max.
  *
  * if a negative @a max is submitted, the result will be @a max <= result <= 0.
  *
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
int64_t CRandom::random ( int64_t max ) noexcept {
	return private_::private_random<int64_t>( 0, max );
}


/** @brief Generate a random value of int64_t between @a min and @a max.
  *
  * if @a max is lower than @a min, the result will be @a max <= result <= @a min.
  *
  * @param[in] min Minimum result.
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
int64_t CRandom::random ( int64_t min, int64_t max ) noexcept {
	return private_::private_random<int64_t>( min, max );
}


/** @brief Generate a random value of uint64_t between 0 and @a max.
  *
  * if a negative @a max is submitted, the result will be @a max <= result <= 0.
  *
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
uint64_t CRandom::random ( uint64_t max ) noexcept {
	return private_::private_random<uint64_t>( 0, max );
}


/** @brief Generate a random value of uint64_t between @a min and @a max.
  *
  * if @a max is lower than @a min, the result will be @a max <= result <= @a min.
  *
  * @param[in] min Minimum result.
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
uint64_t CRandom::random ( uint64_t min, uint64_t max ) noexcept {
	return private_::private_random<uint64_t>( min, max );
}


/** @brief Generate a random value of float between 0 and @a max.
  *
  * if a negative @a max is submitted, the result will be @a max <= result <= 0.
  *
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
float CRandom::random ( float max ) noexcept {
	return private_::private_random<float>( 0, max );
}


/** @brief Generate a random value of float between @a min and @a max.
  *
  * if @a max is lower than @a min, the result will be @a max <= result <= @a min.
  *
  * @param[in] min Minimum result.
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
float CRandom::random ( float min, float max ) noexcept {
	return private_::private_random<float>( min, max );
}


/** @brief Generate a random value of double between 0 and @a max.
  *
  * if a negative @a max is submitted, the result will be @a max <= result <= 0.
  *
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
double CRandom::random ( double max ) noexcept {
	return private_::private_random<double>( 0, max );
}


/** @brief Generate a random value of double between @a min and @a max.
  *
  * if @a max is lower than @a min, the result will be @a max <= result <= @a min.
  *
  * @param[in] min Minimum result.
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
double CRandom::random ( double min, double max ) noexcept {
	return private_::private_random<double>( min, max );
}


/** @brief Generate a random value of long double between 0 and @a max.
  *
  * if a negative @a max is submitted, the result will be @a max <= result <= 0.
  *
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
long double CRandom::random ( long double max ) noexcept {
	return private_::private_random<long double>( 0, max );
}


/** @brief Generate a random value of long double between @a min and @a max.
  *
  * if @a max is lower than @a min, the result will be @a max <= result <= @a min.
  *
  * @param[in] min Minimum result.
  * @param[in] max Maximum result.
  * @return A random value between 0 and @a max.
**/
long double CRandom::random ( long double min, long double max ) noexcept {
	return private_::private_random<long double>( min, max );
}


/** @brief Generates a random C-String with @a minLen to @a maxLen characters
  *
  * The characters are in the range of a-z and A-Z. No whitespace or special
  * characters are used.
  *
  * Note: @a dest is not checked. It must have a size of at least
  * @a maxLen + 1 zero-byte.
  * This means, that if both values are 0, nothing is done with @a dest,
  * and if the larger value is 1, @a dest will simply made to start with
  * a zero-byte.
  *
  * @param[out] dest the char buffer the random character sequence is written into.
  * @param[in] minLen minimum length of the random character sequence.
  * @param[out] maxLen maximum length of the random character sequence.
  * @return number of characters actually written including the final zero-byte.
**/
size_t CRandom::random ( char* dest, size_t minLen, size_t maxLen ) noexcept {
	return private_::private_random_str( dest, minLen, maxLen );
}


/** @brief get random name (1D)
  *
  * This is a convenient wrapper for getting a random name out of one dimension and switches
  * for long name and multi part.
  *
  * @a lN decides upon the maximum number of chars and syllables generated. A value of
  * false (the default) results in a maximum of 12 chars in four syllables. Setting this
  * argument to true results in a maximum of 20 chars in up to six syllables.
  *
  * @a mW decides upon the maximum number of parts. A value of false (the default) produces
  * a name out of one part. A value of true results in a name that can be generated out
  * of up to three parts.
  *
  * @see char * CRandom::rndName(double x, double y, double z, double w, int32_t chars, int32_t sylls, int32_t parts)
  * for documentation
  *
  * @param[in] x simple number to influence the result.
  * @param[in] lN longName  - sets syllables to 3-8 and max chars to 28
  * @param[in] mW multiWord - allows the name to contain spaces (0-2)
  * @return a malloc'd C-string with the name (WARNING: You have to free it after use!)
**/
char* CRandom::rndName ( double x, bool lN, bool mW ) noexcept {
	return ( rndName ( x, lN ? 20 : 12, lN ?  6 :  4, mW ?  3 :  1 ) );
}


/** @brief get random name (1D)
  *
  * This is a convenient wrapper for getting a random name out of one dimension.
  *
  * @see char * CRandom::rndName(double x, double y, double z, double w, int32_t chars, int32_t sylls, int32_t parts)
  * for documentation
  *
  * @param[in] x simple number to influence the result.
  * @param[in] chars set the maximum number of characters to be generated
  * @param[in] sylls set the maximum number of syllables to be generated
  * @param[in] parts set the maximum number of parts the resulting name consists of
  * @return a malloc'd C-string with the name (WARNING: You have to free it after use!)
**/
char* CRandom::rndName ( double x, int32_t chars, int32_t sylls, int32_t parts ) noexcept {
	double newY = x * noiseD ( x ) * ( abs ( x ) < 1.0 ? 1000.0 : abs ( x ) < 10.0 ? 100.0 : abs ( x ) < 100 ? 10.0 : 1.0 );
	return ( rndName ( x, newY, chars, sylls, parts ) );
}


/** @brief get random name (2D)
  *
  * This is a convenient wrapper for getting a random name out of two dimensions and switches
  * for long name and multi part.
  *
  * @a lN decides upon the maximum number of chars and syllables generated. A value of
  * false (the default) results in a maximum of 12 chars in four syllables. Setting this
  * argument to true results in a maximum of 20 chars in up to six syllables.
  *
  * @a mW decides upon the maximum number of parts. A value of false (the default) produces
  * a name out of one part. A value of true results in a name that can be generated out
  * of up to three parts.
  *
  * @see CRandom::rndName(double x, double y, double z, double w, bool lN, bool mW)
  * for documentation
  *
  * @param[in] x simple number to influence the result.
  * @param[in] y simple number to influence the result.
  * @param[in] lN longName  - sets syllables to 3-8 and max chars to 28
  * @param[in] mW multiWord - allows the name to contain spaces (0-2)
  * @return a malloc'd C-string with the name (WARNING: You have to free it after use!)
**/
char* CRandom::rndName ( double x, double y, bool lN, bool mW ) noexcept {
	return ( rndName ( x, y, lN ? 20 : 12, lN ?  6 :  4, mW ?  3 :  1 ) );
}


/** @brief get random name (2D)
  *
  * This is a convenient wrapper for getting a random name out of two dimensions.
  *
  * @see char * CRandom::rndName(double x, double y, double z, double w, int32_t chars, int32_t sylls, int32_t parts)
  * for documentation
  *
  * @param[in] x simple number to influence the result.
  * @param[in] y simple number to influence the result.
  * @param[in] chars set the maximum number of characters to be generated
  * @param[in] sylls set the maximum number of syllables to be generated
  * @param[in] parts set the maximum number of parts the resulting name consists of
  * @return a malloc'd C-string with the name (WARNING: You have to free it after use!)
**/
char* CRandom::rndName ( double x, double y, int32_t chars, int32_t sylls, int32_t parts ) noexcept {
	double newZ = ( x * noiseD ( y ) ) + ( y * noiseD ( x ) );
	newZ *= abs ( newZ ) < 1 ? 1000.0 : abs ( newZ ) < 10.0 ? 100.0 : abs ( newZ ) < 100 ? 10.0 : 1.0;
	return ( rndName ( x, y, newZ, chars, sylls, parts ) );
}


/** @brief get random name (3D)
  *
  * This is a convenient wrapper for getting a random name out of three dimensions and switches
  * for long name and multi part.
  *
  * @a lN decides upon the maximum number of chars and syllables generated. A value of
  * false (the default) results in a maximum of 12 chars in four syllables. Setting this
  * argument to true results in a maximum of 20 chars in up to six syllables.
  *
  * @a mW decides upon the maximum number of parts. A value of false (the default) produces
  * a name out of one part. A value of true results in a name that can be generated out
  * of up to three parts.
  *
  * @see CRandom::rndName(double x, double y, double z, double w, bool lN, bool mW)
  * for documentation
  *
  * @param[in] x simple number to influence the result.
  * @param[in] y simple number to influence the result.
  * @param[in] z simple number to influence the result.
  * @param[in] lN longName  - sets syllables to 3-8 and max chars to 28
  * @param[in] mW multiWord - allows the name to contain spaces (0-2)
  * @return a malloc'd C-string with the name (WARNING: You have to free it after use!)
**/
char* CRandom::rndName ( double x, double y, double z, bool lN, bool mW ) noexcept {
	return ( rndName ( x, y, z,  lN ? 20 : 12, lN ?  6 :  4, mW ?  3 :  1 ) );
}


/** @brief get random name (3D)
  *
  * This is a convenient wrapper for getting a random name out of three dimensions.
  *
  * @see char * CRandom::rndName(double x, double y, double z, double w, int32_t chars, int32_t sylls, int32_t parts)
  * for documentation
  *
  * @param[in] x simple number to influence the result.
  * @param[in] y simple number to influence the result.
  * @param[in] z simple number to influence the result.
  * @param[in] chars set the maximum number of characters to be generated
  * @param[in] sylls set the maximum number of syllables to be generated
  * @param[in] parts set the maximum number of parts the resulting name consists of
  * @return a malloc'd C-string with the name (WARNING: You have to free it after use!)
**/
char* CRandom::rndName ( double x, double y, double z, int32_t chars, int32_t sylls, int32_t parts ) noexcept {
	double newW = ( x * noiseD ( y + z ) ) + ( y * noiseD ( x + z ) ) + ( z * noiseD ( x + y ) );
	newW *= abs ( newW ) < 1 ? 1000.0 : abs ( newW ) < 10.0 ? 100.0 : abs ( newW ) < 100 ? 10.0 : 1.0;
	return ( rndName ( x, y, z, newW, chars, sylls, parts ) );
}


/** @brief get random name (4D)
  *
  * This is a convenient wrapper for getting a random name out of four dimensions and switches
  * for long name and multi part.
  *
  * @a lN decides upon the maximum number of chars and syllables generated. A value of
  * false (the default) results in a maximum of 12 chars in four syllables. Setting this
  * argument to true results in a maximum of 20 chars in up to six syllables.
  *
  * @a mW decides upon the maximum number of parts. A value of false (the default) produces
  * a name out of one part. A value of true results in a name that can be generated out
  * of up to three parts.
  *
  * @see CRandom::rndName(double x, double y, double z, double w, bool lN, bool mW)
  * for documentation
  *
  * @param[in] x simple number to influence the result.
  * @param[in] y simple number to influence the result.
  * @param[in] z simple number to influence the result.
  * @param[in] w simple number to influence the result.
  * @param[in] lN longName  - sets syllables to 3-8 and max chars to 28
  * @param[in] mW multiWord - allows the name to contain spaces (0-2)
  * @return a malloc'd C-string with the name (WARNING: You have to free it after use!)
**/
char* CRandom::rndName ( double x, double y, double z, double w, bool lN, bool mW ) noexcept {
	return ( rndName ( x, y, z, w, lN ? 20 : 12, lN ?  6 :  4, mW ?  3 :  1 ) );
}


/** @brief get random name
  *
  * This method produces random names by combining letters into syllables
  * and syllables into words. This strictly depends on pseudo random
  * generators, utilizing noise() and simplex() methods. Therefore
  * you get the same result if called twice with the same seed set and
  * arguments set.
  *
  * The main switch for the name generation is @a parts, which must be
  * at least one. Each parts consits of at least two syllables. Therefore,
  * if you set @a sylls to a lower value than @a parts + 1, sylls will be
  * raised automatically. Furthermore each syllable consist of two to four
  * chars. Thus if you set chars to a lower value than (@a sylls * 3) + 2,
  * it will be raised, too. In the end @a chars will be at least:
  * 3 * (1 + @a parts) + 2.
  *
  * You can use one of the convenient wrapper which let you decide whether
  * you want long or short names, consisting of one or up to three parts.
  *
  * @param[in] x simple number to influence the result.
  * @param[in] y simple number to influence the result.
  * @param[in] z simple number to influence the result.
  * @param[in] w simple number to influence the result.
  * @param[in] chars set the maximum number of characters to be generated
  * @param[in] sylls set the maximum number of syllables to be generated
  * @param[in] parts set the maximum number of parts the resulting name consists of
  * @return a malloc'd C-string with the name (WARNING: You have to free it after use!)
**/
char* CRandom::rndName ( double x, double y, double z, double w, int32_t chars, int32_t sylls, int32_t parts ) noexcept {
	std::string name = "";
	char    syll[5]    = { 0x0, 0x0, 0x0, 0x0, 0x0 };
	int32_t partsLeft  = std::max ( 1, parts );
	int32_t syllsLeft  = std::max ( 1 +      partsLeft, sylls );
	int32_t charsLeft  = std::max ( 2 + ( 3 * syllsLeft ), chars );
	uint32_t genState  = NameConstants::genPartStart;
	char    lastChrs[2] = { 0x0, 0x0 }; // This is an explicit array, no C-String, so no \0 ending
	int32_t syllsDone  = 0;
	double  index      = ( x * simplex3D ( y, z, w ) )
	                     + ( y * simplex3D ( x,    z, w ) )
	                     + ( z * simplex3D ( x, y,    w ) )
	                     + ( w * simplex3D ( x, y, z ) )
	                     + seed;
	double stepping    = getStepping ( index, x, y, z, w, charsLeft, syllsLeft, partsLeft );
	double endChance   = 0.0;

	// Do - while genState doesn't equal NameConstants::genFinished
	do {
		/// 1) Determine whether the next syllable ends a part, genSyllable() needs to know.
		endChance = static_cast<double> ( ( syllsLeft * 2 ) - ( partsLeft * 2 ) ) / 10.0;
		/* maximum : 12 - 2 = 10 => / 10 = 1.0 (after first syll, !mW &&  lN) =>  0%
		 * minimum :  8 - 6 =  2 => / 10 = 0.2 (after first syll,  mW && !lN) => 40%
		 */

		// Nevertheless we reduce the endchance if this is the first syllable and no multiword selected:
		if ( !syllsDone && ( 1 == partsLeft ) )
			endChance += static_cast<double> ( syllsLeft ) / 20.0;
		/* The initial chance is (8-2)/10 = 0.6 = 20%.
		 * After this modification it is 0.6 + 0.2 = 10%
		 * This, however, does not cover weird arguments set by the user!
		 */
		// However, we need to raise the chance if we have too few sylls left:
		if ( syllsLeft < ( partsLeft * 2 ) )
			endChance -= static_cast<double> ( syllsLeft ) / static_cast<double> ( partsLeft * 2 );
		/* So if we have three sylls left and two parts, the chance is raised by 0.75
		 * If we have 5 sylls left and 3 parts, it would be 0,83
		 */

		// If this is the very first syllable, the chance is halved:
		if ( 0 == syllsDone )
			endChance += ( endChance + 1.0 ) / 2.0;

		// Now test the chance:
		if ( simplex3D ( index, charsLeft, partsLeft ) > endChance )
			genState |= NameConstants::genPartEnd;

		/// 2) generate syllable:
		charsLeft -= genSyllable ( index, stepping, syll, genState, lastChrs );

		/// 3) if we have a syllable (genSyllable produces an empty string on error) it can be added:
		if ( strlen ( syll ) > 1 ) {
			name += syll;
			syllsDone++;
			syllsLeft--;

			// If this is a partEnd, react
			if ( genState & NameConstants::genPartEnd ) {
				genState = NameConstants::genPartStart;
				if ( ( charsLeft >= 4 ) && --partsLeft && syllsLeft )
					name += " "; // add a space, as we will start a new part
				memset ( lastChrs, 0, sizeof ( char ) ); // Needs to be resetted...
			}
		}
		// 4) If we have work to do, generate a new stepping and index
		if ( ( charsLeft >= 4 ) && partsLeft && syllsLeft ) {
			stepping = getStepping ( index, x, y, z, w, charsLeft, syllsLeft, partsLeft );
			index   += stepping;
		} else
			genState = NameConstants::genFinished;
	} while ( genState != NameConstants::genFinished );

	return ( pwx_strdup ( name.c_str() ) );
}


/** @brief set name source type to @a type
  *
  * @param[in] type the new name source type
**/
void CRandom::setNST ( eNameSourceType type ) noexcept {
	nst = type;
}

/** @brief set Simplex Seed
  *
  * Set the seed to @a newSeed which will cause the simplex table to be
  * reinitialized.
**/
void CRandom::setSeed ( int32_t newSeed ) noexcept {
	newSeed &= constants::fourthMaxInt;
	if ( newSeed != seed ) {
		seed = newSeed;
		for ( int32_t i = 0; i < 256; i++ ) {
			spxTab[i]       = hash ( seed + i ) % 256;
			spxTab[i + 256] = spxTab[i];
		}
	}
}


/** @brief calculate a one dimensional simplex noise value
  *
  * This method returns a simplex noise value of one dimension.
  *
  * - zoom and smooth have a default value of 1.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
**/
double CRandom::simplex1D ( double x, double zoom, double smooth ) noexcept {
	if ( zoom      < 0.001 ) zoom = 0.001;
	if ( smooth    < 1.0 )   smooth = 1.0;

	x += seed;

	return ( getSpx1D ( x / zoom ) / smooth );
}


/** @brief calculate a one dimensional simplex wave
  *
  * This method returns a simplex wave of one dimension. The number of waves specified will overlay
  * each others with multiplied smoothing, specified by 'reduction'. The default value of 1 wave
  * just returns the simplex noise value in an intervall of [-1, 1]. While having more than one waves
  * makes it _possible_ to stay in this intervall, the results will most probably be nearer to zero.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * - reduction has a minimum value of 1.0 to ensure the savety of the resulting intervall.
  *
  * - waves has a minimum value of 1.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
  * @param[in] reduction Multiplier for the smoothing factor in each round.
  * @param[in] waves Number of waves to overlay. The default of 1 returns the pure Simplex Noise Value.
**/
double CRandom::simplex1D ( double x, double zoom, double smooth, double reduction, int32_t waves ) noexcept {
	if ( zoom      < 0.001 ) zoom = 0.001;
	if ( smooth    < 1.0 )   smooth = 1.0;
	if ( reduction < 1.0 )   reduction = 1.0;
	if ( waves     < 1 )     waves = 1;

	x += seed;

	double result = getSpx1D ( x / zoom ) / smooth;

	if ( waves > 1 ) {
		double currWave   = 1.0;
		double currSmooth = smooth;
		double factor     = 1.0;
		double currZoom, dX;

		while ( currWave < waves ) {
			currWave   += 1.0;
			currSmooth *= reduction;
			currZoom    = zoom / std::pow ( currWave, 2 );
			dX          = x / currZoom;
			result     += getSpx1D ( dX ) / currSmooth;
			factor     += 1.0 / currSmooth;
		}
		result /= factor;
	}

	return ( result );
}


/** @brief calculate a two dimensional simplex noise value
  *
  * This method returns a simplex noise value of two dimensions.
  *
  * - zoom and smooth have a default value of 1.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
**/
double CRandom::simplex2D ( double x, double y, double zoom, double smooth ) noexcept {
	if ( zoom   < 0.001 ) zoom = 0.001;
	if ( smooth < 1.0 )   smooth = 1.0;

	x += seed;
	y += seed;

	return ( getSpx2D ( x / zoom, y / zoom ) / smooth );
}


/** @brief calculate a two dimensional simplex wave
  *
  * This method returns a simplex wave of two dimensions. The number of waves specified will overlay
  * each others with multiplied smoothing, specified by 'reduction'. The default value of 1 wave just
  * returns the simplex noise value in an intervall of [-1, 1]. While having more than one waves makes
  * it _possible_ to stay in this intervall, the results will most probably be nearer to zero.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * - reduction has a minimum value of 1.0 to ensure the savety of the resulting intervall.
  *
  * - waves has a minimum value of 1.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
  * @param[in] reduction Multiplier for the smoothing factor in each round.
  * @param[in] waves Number of waves to overlay. The default of 1 returns the pure Simplex Noise Value.
**/
double CRandom::simplex2D ( double x, double y, double zoom, double smooth, double reduction, int32_t waves ) noexcept {
	if ( zoom      < 0.001 ) zoom = 0.001;
	if ( smooth    < 1.0 )   smooth = 1.0;
	if ( reduction < 1.0 )   reduction = 1.0;
	if ( waves     < 1 )     waves = 1;

	x += seed;
	y += seed;

	double result = getSpx2D ( x / zoom, y / zoom ) / smooth;

	if ( waves > 1 ) {
		double currWave   = 1.0;
		double currSmooth = smooth;
		double factor     = 1.0;
		double currZoom, dX, dY;

		while ( currWave < waves ) {
			currWave   += 1.0;
			currSmooth *= reduction;
			currZoom    = zoom / std::pow ( currWave, 2 );
			dX          = x / currZoom;
			dY          = y / currZoom;
			result     += getSpx2D ( dX, dY ) / currSmooth;
			factor     += 1.0 / currSmooth;
		}
		result /= factor;
	}

	return ( result );
}


/** @brief calculate a three dimensional simplex noise value
  *
  * This method returns a simplex wave of three dimensions.
  *
  * - zoom and smooth have a default value of 1.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] z Z-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
**/
double CRandom::simplex3D ( double x, double y, double z, double zoom, double smooth ) noexcept {
	if ( zoom   < 0.001 ) zoom = 0.001;
	if ( smooth < 1.0 )   smooth = 1.0;

	x += seed;
	y += seed;
	z += seed;

	return ( getSpx3D ( x / zoom, y / zoom, z / zoom ) / smooth );
}


/** @brief calculate a three dimensional simplex wave
  *
  * This method returns a simplex wave of three dimensions. The number of waves specified will overlay
  * each others with reduced zoom, specified by 'reduction'. The default value of 1 wave just returns
  * the simplex noise value in an intervall of [-1, 1]. While having more than one waves makes it
  * _possible_ to stay in this intervall, the results will most probably be nearer to zero.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * - reduction has a minimum value of 1.0 to ensure the savety of the resulting intervall.
  *
  * - waves has a minimum value of 1.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] z Z-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
  * @param[in] reduction Multiplier for the smoothing factor in each round.
  * @param[in] waves Number of waves to overlay. The default of 1 returns the pure Simplex Noise Value.
**/
double CRandom::simplex3D ( double x, double y, double z, double zoom, double smooth, double reduction, int32_t waves ) noexcept {
	if ( zoom      < 0.001 ) zoom = 0.001;
	if ( smooth    < 1.0 )   smooth = 1.0;
	if ( reduction < 1.0 )   reduction = 1.0;
	if ( waves     < 1 )     waves = 1;

	x += seed;
	y += seed;
	z += seed;

	double result = getSpx3D ( x / zoom, y / zoom, z / zoom ) / smooth;

	if ( waves > 1 ) {
		double currWave   = 1.0;
		double currSmooth = smooth;
		double factor     = 1.0;
		double currZoom, dX, dY, dZ;

		while ( currWave < waves ) {
			currWave   += 1.0;
			currSmooth *= reduction;
			currZoom    = zoom / std::pow ( currWave, 2 );
			dX          = x / currZoom;
			dY          = y / currZoom;
			dZ          = z / currZoom;
			result     += getSpx3D ( dX, dY, dZ ) / currSmooth;
			factor     += 1.0 / currSmooth;
		}
		result /= factor;
	}

	return ( result );
}


/** @brief calculate a four dimensional simplex noise value
  *
  * This method returns a simplex noise value of four dimension.
  *
  * - zoom and smooth have a default value of 1.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] z Z-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] w W-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
**/
double CRandom::simplex4D ( double x, double y, double z, double w, double zoom, double smooth ) noexcept {
	if ( zoom   < 0.001 ) zoom = 0.001;
	if ( smooth < 1.0 )   smooth = 1.0;

	x += seed;
	y += seed;
	z += seed;
	w += seed;

	return ( getSpx4D ( x / zoom, y / zoom, z / zoom, w / zoom ) / smooth );
}


/** @brief calculate a four dimensional simplex wave
  *
  * This method returns a simplex wave of four dimension. The number of waves specified will overlay
  * each others with reduced zoom, specified by 'reduction'. The default value of 1 wave just returns
  * the simplex noise value in an intervall of [-1, 1]. While having more than one waves makes it
  * _possible_ to stay in this intervall, the results will most probably be nearer to zero.
  *
  * - zoom, smooth, reduction and waves all have a default value of 1.
  *
  * - zoom has a minimum of 0.001, as a thounsandth produces very high values already and zoom must not
  *   be zero.
  *
  * - smooth has a minimum value of 1.0 to make sure the [-1, 1] intervall isn't broken.
  *
  * - reduction has a minimum value of 1.0 to ensure the savety of the resulting intervall.
  *
  * - waves has a minimum value of 1.
  *
  * @param[in] x X-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] y Y-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] z Z-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] w W-Coordinate of the Simplex Point, modified by the simplex seed.
  * @param[in] zoom Zooming factor into the point. Your coordinate will divided by this factor.
  * @param[in] smooth Divisor for the result. The higher, the nearer the result will be to zero.
  * @param[in] reduction Multiplier for the smoothing factor in each round.
  * @param[in] waves Number of waves to overlay. The default of 1 returns the pure Simplex Noise Value.
**/
double CRandom::simplex4D ( double x, double y, double z, double w, double zoom, double smooth, double reduction, int32_t waves ) noexcept {
	if ( zoom      < 0.001 ) zoom = 0.001;
	if ( smooth    < 1.0 )   smooth = 1.0;
	if ( reduction < 1.0 )   reduction = 1.0;
	if ( waves     < 1 )     waves = 1;

	x += seed;
	y += seed;
	z += seed;
	w += seed;

	double result = getSpx4D ( x / zoom, y / zoom, z / zoom, w / zoom ) / smooth;

	if ( waves > 1 ) {
		double currWave   = 1.0;
		double currSmooth = smooth;
		double factor     = 1.0;
		double currZoom, dX, dY, dZ, dW;

		while ( currWave < waves ) {
			currWave   += 1.0;
			currSmooth *= reduction;
			currZoom    = zoom / std::pow ( currWave, 2 );
			dX          = x / currZoom;
			dY          = y / currZoom;
			dZ          = z / currZoom;
			dW          = w / currZoom;
			result     += getSpx4D ( dX, dY, dZ, dW ) / currSmooth;
			factor     += 1.0 / currSmooth;
		}
		result /= factor;
	}

	return ( result );
}


} // namespace pwx
