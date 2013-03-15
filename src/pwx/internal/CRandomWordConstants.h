#pragma once
#ifndef PWX_LIBPWX_PWX_INTERNAL_CRANDOMWORDCONSTANTS_H_INCLUDED
#define PWX_LIBPWX_PWX_INTERNAL_CRANDOMWORDCONSTANTS_H_INCLUDED 1

/** @internal
  * @file CRandomWordConstants.h
  *
  * @brief Automatically generated header for the name generator.
  *
  * <B>Automatically generated Header! Do NOT modify! See bottom
  * of this file for more information!</B>
  *
  * (c) 2007 - 2013 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *		 yamakuzure@users.sourceforge.net
  *		 http://pwxlib.sourceforge.net
  *
  *  This program is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  *
  * History and Changelog are maintained in pwx.h
  *
  @verbatim
  *  ==== Automatic Text Analyzation Result from PrydeWorX textAnalyzer.pl ==== *
  *=============================================================================*
  * -- The following data has been extracted by analyzing english text files -- *
  * -- Number of text files that have been analyzed:          2982           -- *
  * -- Number of lines in all of these files       :       8854370           -- *
  * -- Number of words in all of these files       :      71324743           -- *
  * -- Number of unique words found                :        351243           -- *
  * -- Number of names from name list files        :         92452           -- *
  * -- Number of ignored words (illegal content)   :          3471           -- *
  * -- Resulting number of analyzed words          :        347772           -- *
  * -- ( ==>      15882 Non-ASCII-words have been filtered out)              -- *
  * -- ( ==>        742 Message header lines have been filtered out)         -- *
  * -- ( ==>       3083 E-Mail addresses have been filtered out)             -- *
  * -- ( ==>       5187 Internet addresses have been filtered out)           -- *
  * -- ( ==>         23 Triple Threats have been filtered out)               -- *
  * -- ( ==>        106 Roman numerals have been filtered out)               -- *
  * -- ( ==>     152374 Weird letter combinations have been filtered out)    -- *
  * -- ( ==>     806421 times Captain Caps stroke back)                      -- *
  * -- ( ==>      63688 introduction header lines have been filtered out)    -- *
  * --------------------------------------------------------------------------- *
  * Top Ten statistics - The most and fewest words that got accepted or denied: *
  * Ten most often found accepted words  | Ten most often found denied words    *
  * -------------------------------------+------------------------------------- *
  *   1.:                the (  4587125) |   1.:               ctrl (      258) *
  *   2.:                 of (  2528023) |   2.:               cpsr (      209) *
  *   3.:                and (  2485170) |   3.:               xxxx (      179) *
  *   4.:                 to (  1994234) |   4.:               xxxv (      156) *
  *   5.:                 in (  1291065) |   5.:               bbss (      141) *
  *   6.:               that (   978510) |   6.:         houyhnhnms (      106) *
  *   7.:                 it (   841166) |   7.:               tmrc (       91) *
  *   8.:                 he (   792319) |   8.:              xxxxx (       85) *
  *   9.:                was (   717994) |   9.:               sgml (       69) *
  *  10.:                 is (   671563) |  10.:               hpcc (       63) *
  * -------------------------------------+------------------------------------- *
  * Ten fewest found accepted words      | Ten fewest found denied words        *
  * -------------------------------------+------------------------------------- *
  *   1.:           coroneia (        1) |   1.:               cwns (        1) *
  *   2.:         keithville (        1) |   2.:         sgsthomson (        1) *
  *   3.:             clauso (        1) |   3.:           ioaother (        1) *
  *   4.: andstandardization (        1) |   4.:              smlth (        1) *
  *   5.:       acephalocyst (        1) |   5.:               tbbs (        1) *
  *   6.:             velers (        1) |   6.:         squidlblxx (        1) *
  *   7.:         mcconnaghy (        1) |   7.:               gblf (        1) *
  *   8.:          refrigera (        1) |   8.:        vrrrrrrrrrm (        1) *
  *   9.:            danthed (        1) |   9.:           shhhhink (        1) *
  *  10.:          greshamer (        1) |  10.:             aooouw (        1) *
  *=============================================================================*
  @endverbatim
**/

namespace pwx {

/** @namespace pwx::NameConstants
  *
  * @brief constants for usage by the name generator of CRandom
  *
  * @see pwx for the namespace surrounding this
  * @see pwx::constants for general constants used by CRandom
**/
namespace NameConstants {

/** --- constants for generation states --- **/

/* --- General states--- */
const int32_t genFinished    = 0x00000000; //!< The generation has been finished

/* --- rounds in generation--- */
const int32_t genRoundA      = 0x00000010; //!< The first char has to be found
const int32_t genRoundB      = 0x00000020; //!< and the second
const int32_t genRoundC      = 0x00000040; //!< and the third
const int32_t genRoundD      = 0x00000080; //!< and finally the fourth

/* --- position definitions--- */
const int32_t genPartStart   = 0x00000100; //!< The next char starts a new part
const int32_t genPartEnd     = 0x00000200; //!< The current part is ended
const int32_t genSyllEnd     = 0x00000400; //!< The current syllable is finished

/* --- some states for char generation--- */
const int32_t genLastIsCon   = 0x00001000; //!< The next shall be a consonant
const int32_t genLastIsVow   = 0x00002000; //!< The next shall be a vowel
const int32_t genNextIsCon   = 0x00004000; //!< The last char was a consonant
const int32_t genNextIsVow   = 0x00008000; //!< The last char was a vowel
const int32_t genHasNextChar = 0x00010000; //!< The current round got a char!

/* --- constants for character position allowance --- */
const int32_t genCharMask    = 0x03ffffff; //!< A mask to filter the position out of the FUM
const int32_t genStartAllow  = 0x04000000; //!< A 2-char-combination is allowed to start a part
const int32_t genMiddleAllow = 0x08000000; //!< A 2-char-combination is allowed to be in the middle of a part
const int32_t genEndAllow    = 0x10000000; //!< A 2-char-combination is allowed to end a part

/* --- constants for character to index offsets --- */
const int32_t chrOffsetLower = static_cast<int32_t>('a');
const int32_t chrOffsetUpper = static_cast<int32_t>('A');
const int32_t chrOffsetDown  = chrOffsetLower - chrOffsetUpper;

/** @internal
  * @file CRandomWordConstants.h
  *
  @verbatim
  * Character statistics :
  * ----------------------
  * Note: For the rules, vowels and consonants are split.
  * -> The last column shows the total quote of a letter.
  * ----------------------------------------------------
  *       char found |normalized|  quote |   rate  |  all
  * A :       316757 |        6 | 23.68% | 0.23676 |  9.49%
  * B :        75410 |       12 |  3.77% | 0.03771 |  2.26%
  * C :       125772 |       20 |  6.29% | 0.06290 |  3.77%
  * D :       108970 |       17 |  5.45% | 0.05449 |  3.26%
  * E :       376354 |        7 | 28.13% | 0.28131 | 11.28%
  * F :        42704 |        7 |  2.14% | 0.02136 |  1.28%
  * G :        84388 |       13 |  4.22% | 0.04220 |  2.53%
  * H :       105416 |       17 |  5.27% | 0.05272 |  3.16%
  * I :       259838 |        5 | 19.42% | 0.19422 |  7.79%
  * J :        10771 |        2 |  0.54% | 0.00539 |  0.32%
  * K :        58059 |        9 |  2.90% | 0.02903 |  1.74%
  * L :       193396 |       31 |  9.67% | 0.09672 |  5.79%
  * M :       104177 |       17 |  5.21% | 0.05210 |  3.12%
  * N :       239048 |       38 | 11.95% | 0.11955 |  7.16%
  * O :       213543 |        4 | 15.96% | 0.15961 |  6.40%
  * P :        74852 |       12 |  3.74% | 0.03743 |  2.24%
  * Q :         6279 |        1 |  0.31% | 0.00314 |  0.19%
  * R :       248399 |       40 | 12.42% | 0.12422 |  7.44%
  * S :       228958 |       36 | 11.45% | 0.11450 |  6.86%
  * T :       193423 |       31 |  9.67% | 0.09673 |  5.80%
  * U :       116859 |        2 |  8.73% | 0.08735 |  3.50%
  * V :        36439 |        6 |  1.82% | 0.01822 |  1.09%
  * W :        34200 |        5 |  1.71% | 0.01710 |  1.02%
  * X :         6839 |        1 |  0.34% | 0.00342 |  0.20%
  * Y :        54531 |        1 |  4.08% | 0.04076 |  1.63%
  * Z :        22147 |        4 |  1.11% | 0.01108 |  0.66%
  @endverbatim
**/

/* --- resulting vowel array --- */
const int32_t vowListLen  = 25;
const char    vowList[26] =
  "aoaeuyeuaoiaieoeieaeiaieo";

/* --- resulting consonant array --- */
const int32_t conListLen   = 319;
const char    conList[320] =
  "rznzpdlpblnbfbkhctscrcncpcdcmcncscbcrsctmdrmdldmvgdrmfhfbtfngngdnlgtnrsrgnhlhthghgmshdhthljrskskrkpklbtnswldtlrlrlclrltsnmhlgtnsprlsmhnmprmnmhcsdrmkmntnsngnlnfntnrnrgnmnsntcndlnclgflnbntnrlpdpcrnkdpmqrhrgrkcrbnrnlrzjkrpsprtshvbrmrlrdrdrlrbrzsxstsrsphrscstslslsrwlslcsnsbsmsvsltbtntstlctltsntstghtdrtftnthtrtvrsvdvwswlwn";

/** @internal
  * @file CRandomWordConstants.h
  *
  @verbatim
  * Follow-up statistics :
  * ----------------------
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z | *
  * A | 12|  9|  9| 21| 21|  8| 10| 10| 20|  6|  7| 11|  9| 26| 16| 11|   | 25| 20| 10| 22|  4| 16|  6| 16|  7| *
  * B | 25|  7|  8|  5| 21|  6|  6|  4| 25|  5|  5|  5|  5|  4| 25|  5|   |  5|  7|  8|  6|  5|  6|   | 19|  6| *
  * C | 21|  6|  7|  5| 11|  4|  8| 11| 15|  7| 12|  5|  5|  5| 18|  4|   |  6| 12|   | 16|  5|  7|   | 13|  6| *
  * D | 26|  8|  7|  6| 26|  6|   |  5| 25|  5|  4|  5|  4|  3| 24| 10|   |  6| 15| 10| 25|  4|  5|   | 14|  3| *
  * E | 18|  8| 10| 10| 16|  9| 10| 10| 20|  9| 12| 10| 14| 11| 16| 16|   | 24| 18| 13| 16|  4|  7|  9|  9|  7| *
  * F | 19|  7|  6|  5| 13|  6|  9|  4| 12|  3|  5|  6|   |   |  4| 12|   |  5| 10|  7|  4|  7|  6|   |  4|  6| *
  * G | 22|  8|  6|  4|  9|  6|  6|  6| 15|  5|  9|  6|  4|  4| 16|  9|   |   |  7|   |  9|  4|  8|  4| 13|  5| *
  * H | 24|  7|   |  6| 24|  6|  7|  5| 24|   |  5|   |   |  5| 25| 10|   |   |   |  7| 19|   |   |   | 14|  6| *
  * I | 14|  8| 12|  6| 16|  9| 12|  7| 11|  5|  5| 11|  9| 24| 10| 11|   | 12| 24| 15|   |  3|   |  8|   |  5| *
  * J | 23|   |  5|  5| 20|   |   |  5| 17|   |   |   |  6|  7| 20|  8|   |  5|  9|  5| 13|   |   |   |   |   | *
  * K | 16|  9|  6|  7|  8|  5|  9|  5|  6|  4|  6|  5|  3|  5| 21|  6|  6|  6| 11|  7| 20|  3|  4|   | 13|  7| *
  * L | 26|  6|  7|  7| 25|  9|  6|  5| 26|  6|  5| 12|  4|  3| 26|  5|   |  6| 14| 12| 22|  4|  5|  5| 12|  5| *
  * M | 25|   | 10|  6| 20|  6|  6|  5| 19|  5|  5|   |  5|   | 22| 10|   |   |   |  8|  9|  5|  6|   | 11|  5| *
  * N | 26|  6|   |   | 26|  6|   |  5| 26|  5|  6|  6|  5|  6| 24|  8|   |  6| 19|  9| 18|  4|  5|  4| 20|  7| *
  * O | 13| 14|  9| 10| 17|  8| 12| 10|  8|  9|  8| 20| 16| 23| 17| 12|   | 19| 19| 13| 19|  3|  7|  9| 12| 10| *
  * P | 24|  6|   |  6| 21|  6|  8|  7| 17|  4|  4|  5|  3|  4| 24|  6|   |  4|  9|   | 11|  5|  6|   |  7|   | *
  * Q | 11|   |   |   | 14|   |   |   | 20|   |   |   |   |   |  9|   |   |   |   |   |  3|   |  6|   |   |   | *
  * R | 26|  8|  6| 12| 26|  7|  6|  6| 26|  5|  7|  5|  6| 12| 26|  9|   |  7| 11| 16| 21|  4|  5|  7| 17|  6| *
  * S | 24|   |  5|  5| 25|  5|  7|  5| 20|  4|  5|  6|  5|  4| 14|  8|   |  5|   |  7| 19|  3|  4|   | 10|  8| *
  * T | 26|  8|   |  5| 24|  6|  5| 11| 21|  4|  6|  4|  3|  3| 23| 11|   |  6| 14| 11|  8|  4|  4|   | 18|  6| *
  * U | 16| 12| 10|  7| 19|   |  7| 11| 15|  5|  7| 19| 11| 25| 11| 10|   | 25| 14| 22| 10|  5|  4| 10| 11|  6| *
  * V | 14|   | 13|  6| 11|   |  7|  6| 11|   |  3|  5|  7|  6| 14|   |   |  4|   | 13|  3|  6|  6|   |  8|  7| *
  * W | 23|  8|  8|  7| 15|  5|  7|   |  9|   |  6|  6|   |  9|  9|  8|   |  5|   |  7| 12|   |  5|   | 12|  6| *
  * X |  9|  7|  7|  7| 10|  3|  4|  5| 13|   |   |   |  4|   |  8|  7|   |  5|  8|  6|  9|   |  4|   | 10|   | *
  * Y | 12|  8|  8|  5|  7|  7| 12|  5|   |  5|  5|  6|  9| 10| 18| 10|   | 10|  7|  6| 16|  5|  6|  6|   |   | *
  * Z | 17|  6|  5| 10| 10|  5|  6|  9| 14|   |  5|  5|  3|  5| 13|  3|   |  4|  7|  8| 13|  4|  4|   |  9|   | *
  @endverbatim
**/

/** @brief resulting [F]ollow-[U]p rules [M]atrix
  *
  * This is the matrix that decides upon where a
  * two-chars-combination is allowed to be generated,
  * and which character is allowed to follow those two.
  *
  * The position is hinted in brackets with the following
  * characters: B = Begin, M = Middle, E = End of a part
**/
const int32_t nameFUM[26][26] = {
  /* Rules following letter A : */ {
    /* A : */ 0x1c0e3cce, /* (B|M|E) - Allowed are B,C,D,G,H,K,L,M,N,R,S and T */
    /* B : */ 0x1c164913, /* (B|M|E) - Allowed are A,B,E,I,L,O,R,S and U */
    /* C : */ 0x1c184595, /* (B|M|E) - Allowed are A,C,E,H,I,K,O,T and U */
    /* D : */ 0x1f7e7fbd, /* (B|M|E) - Allowed are A,C,D,E,F,H,I,J,K,L,M,N,O,R,S,T,U,V,W,Y and Z */
    /* E : */ 0x1e3ffdef, /* (B|M|E) - Allowed are A,B,C,D,F,G,H,I,K,L,M,N,O,P,Q,R,S,T,U,V and Z */
    /* F : */ 0x1c184931, /* (B|M|E) - Allowed are A,E,F,I,L,O,T and U */
    /* G : */ 0x1c1269d1, /* (B|M|E) - Allowed are A,E,G,H,I,L,N,O,R and U */
    /* H : */ 0x1c167911, /* (B|M|E) - Allowed are A,E,I,L,M,N,O,R,S and U */
    /* I : */ 0x1e3efd7f, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,I,K,L,M,N,O,P,R,S,T,U,V and Z */
    /* J : */ 0x1c104119, /* (B|M|E) - Allowed are A,D,E,I,O and U */
    /* K : */ 0x1c144191, /* (B|M|E) - Allowed are A,E,H,I,O,S and U */
    /* L : */ 0x1c0c591f, /* (B|M|E) - Allowed are A,B,C,D,E,I,L,M,O,S and T */
    /* M : */ 0x1c14d113, /* (B|M|E) - Allowed are A,B,E,I,M,O,P,S and U */
    /* N : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* O : */ 0x1c1ebdee, /* (B|M|E) - Allowed are B,C,D,F,G,H,I,K,L,M,N,P,R,S,T and U */
    /* P : */ 0x1c1ec991, /* (B|M|E) - Allowed are A,E,H,I,L,O,P,R,S,T and U */
    /* Q : */ 0x10000000, /* ( | |E) - None are allowed */
    /* R : */ 0x1f7fffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,Y and Z */
    /* S : */ 0x1f5ddddf, /* (B|M|E) - Allowed are A,B,C,D,E,G,H,I,K,L,M,O,P,Q,S,T,U,W,Y and Z */
    /* T : */ 0x1c1e4195, /* (B|M|E) - Allowed are A,C,E,H,I,O,R,S,T and U */
    /* U : */ 0x1eeefdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,V,W,X and Z */
    /* V : */ 0x1c004111, /* (B|M|E) - Allowed are A,E,I and O */
    /* W : */ 0x1d0e7dbb, /* (B|M|E) - Allowed are A,B,D,E,F,H,I,K,L,M,N,O,R,S,T and Y */
    /* X : */ 0x1c084911, /* (B|M|E) - Allowed are A,E,I,L,O and T */
    /* Y : */ 0x1c4e79bf, /* (B|M|E) - Allowed are A,B,C,D,E,F,H,I,L,M,N,O,R,S,T and W */
    /* Z : */ 0x1e104911  /* (B|M|E) - Allowed are A,E,I,L,O,U and Z */
  },
  /* Rules following letter B : */ {
    /* A : */ 0x1ffeffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,R,S,T,U,V,W,X,Y and Z */
    /* B : */ 0x19044911, /* ( |M|E) - Allowed are A,E,I,L,O,S and Y */
    /* C : */ 0x0a104899, /* ( |M| ) - Allowed are A,D,E,H,L,O,U and Z */
    /* D : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* E : */ 0x1f7e7dfd, /* (B|M|E) - Allowed are A,C,D,E,F,G,H,I,K,L,M,N,O,R,S,T,U,V,W,Y and Z */
    /* F : */ 0x08104903, /* ( |M| ) - Allowed are A,B,I,L,O and U */
    /* G : */ 0x08124811, /* ( |M| ) - Allowed are A,E,L,O,R and U */
    /* H : */ 0x1c104101, /* (B|M|E) - Allowed are A,I,O and U */
    /* I : */ 0x1effffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X and Z */
    /* J : */ 0x0c104111, /* (B|M| ) - Allowed are A,E,I,O and U */
    /* K : */ 0x08004191, /* ( |M| ) - Allowed are A,E,H,I and O */
    /* L : */ 0x1d004111, /* (B|M|E) - Allowed are A,E,I,O and Y */
    /* M : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* N : */ 0x18004111, /* ( |M|E) - Allowed are A,E,I and O */
    /* O : */ 0x1ffeffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,R,S,T,U,V,W,X,Y and Z */
    /* P : */ 0x08024811, /* ( |M| ) - Allowed are A,E,L,O and R */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x0c104111, /* (B|M| ) - Allowed are A,E,I,O and U */
    /* S : */ 0x18184514, /* ( |M|E) - Allowed are C,E,I,K,O,T and U */
    /* T : */ 0x18164911, /* ( |M|E) - Allowed are A,E,I,L,O,R,S and U */
    /* U : */ 0x1c0e2804, /* (B|M|E) - Allowed are C,L,N,R,S and T */
    /* V : */ 0x08024111, /* ( |M| ) - Allowed are A,E,I,O and R */
    /* W : */ 0x0d104111, /* (B|M| ) - Allowed are A,E,I,O,U and Y */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1e4eedff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,N,O,P,R,S,T,W and Z */
    /* Z : */ 0x08004199  /* ( |M| ) - Allowed are A,D,E,H,I and O */
  },
  /* Rules following letter C : */ {
    /* A : */ 0x1f3efdfe, /* (B|M|E) - Allowed are B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,U,V,Y and Z */
    /* B : */ 0x18124111, /* ( |M|E) - Allowed are A,E,I,O,R and U */
    /* C : */ 0x1c104991, /* (B|M|E) - Allowed are A,E,H,I,L,O and U */
    /* D : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* E : */ 0x1c0ebd09, /* (B|M|E) - Allowed are A,D,I,K,L,M,N,P,R,S and T */
    /* F : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* G : */ 0x08124991, /* ( |M| ) - Allowed are A,E,H,I,L,O,R and U */
    /* H : */ 0x1c5a7911, /* (B|M|E) - Allowed are A,E,I,L,M,N,O,R,T,U and W */
    /* I : */ 0x1c3ef83d, /* (B|M|E) - Allowed are A,C,D,E,F,L,M,N,O,P,R,S,T,U and V */
    /* J : */ 0x08144119, /* ( |M| ) - Allowed are A,D,E,I,O,S and U */
    /* K : */ 0x19447993, /* ( |M|E) - Allowed are A,B,E,H,I,L,M,N,O,S,W and Y */
    /* L : */ 0x1c104111, /* (B|M|E) - Allowed are A,E,I,O and U */
    /* M : */ 0x1c104111, /* (B|M|E) - Allowed are A,E,I,O and U */
    /* N : */ 0x1c104111, /* (B|M|E) - Allowed are A,E,I,O and U */
    /* O : */ 0x1c7ef95f, /* (B|M|E) - Allowed are A,B,C,D,E,G,I,L,M,N,O,P,R,S,T,U,V and W */
    /* P : */ 0x18000191, /* ( |M|E) - Allowed are A,E,H and I */
    /* Q : */ 0x10000000, /* ( | |E) - None are allowed */
    /* R : */ 0x0d104111, /* (B|M| ) - Allowed are A,E,I,O,U and Y */
    /* S : */ 0x1c5cd591, /* (B|M|E) - Allowed are A,E,H,I,K,M,O,P,S,T,U and W */
    /* T : */ 0x00000000, /* ( | | ) - None are allowed */
    /* U : */ 0x1e0ef93f, /* (B|M|E) - Allowed are A,B,C,D,E,F,I,L,M,N,O,P,R,S,T and Z */
    /* V : */ 0x08024111, /* ( |M| ) - Allowed are A,E,I,O and R */
    /* W : */ 0x0c025191, /* (B|M| ) - Allowed are A,E,H,I,M,O and R */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c0ef84f, /* (B|M|E) - Allowed are A,B,C,D,G,L,M,N,O,P,R,S and T */
    /* Z : */ 0x1d104411  /* (B|M|E) - Allowed are A,E,K,O,U and Y */
  },
  /* Rules following letter D : */ {
    /* A : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* B : */ 0x09124911, /* ( |M| ) - Allowed are A,E,I,L,O,R,U and Y */
    /* C : */ 0x18104991, /* ( |M|E) - Allowed are A,E,H,I,L,O and U */
    /* D : */ 0x19004911, /* ( |M|E) - Allowed are A,E,I,L,O and Y */
    /* E : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* F : */ 0x18104911, /* ( |M|E) - Allowed are A,E,I,L,O and U */
    /* G : */ 0x00000000, /* ( | | ) - None are allowed */
    /* H : */ 0x1c104111, /* (B|M|E) - Allowed are A,E,I,O and U */
    /* I : */ 0x1effffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X and Z */
    /* J : */ 0x08104111, /* ( |M| ) - Allowed are A,E,I,O and U */
    /* K : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* L : */ 0x1d004111, /* (B|M|E) - Allowed are A,E,I,O and Y */
    /* M : */ 0x18004111, /* ( |M|E) - Allowed are A,E,I and O */
    /* N : */ 0x18000111, /* ( |M|E) - Allowed are A,E and I */
    /* O : */ 0x1ffefdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,U,V,W,X,Y and Z */
    /* P : */ 0x185249b1, /* ( |M|E) - Allowed are A,E,F,H,I,L,O,R,U and W */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x1d104111, /* (B|M|E) - Allowed are A,E,I,O,U and Y */
    /* S : */ 0x1878dd97, /* ( |M|E) - Allowed are A,B,C,E,H,I,K,L,M,O,P,T,U,V and W */
    /* T : */ 0x18065d91, /* ( |M|E) - Allowed are A,E,H,I,K,L,M,O,R and S */
    /* U : */ 0x1fbfffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,X,Y and Z */
    /* V : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* W : */ 0x0d004111, /* (B|M| ) - Allowed are A,E,I,O and Y */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c2e7d55, /* (B|M|E) - Allowed are A,C,E,G,I,K,L,M,N,O,R,S,T and V */
    /* Z : */ 0x1c100110  /* (B|M|E) - Allowed are E,I and U */
  },
  /* Rules following letter E : */ {
    /* A : */ 0x1e3ebcfe, /* (B|M|E) - Allowed are B,C,D,E,F,G,H,K,L,M,N,P,R,S,T,U,V and Z */
    /* B : */ 0x1c124913, /* (B|M|E) - Allowed are A,B,E,I,L,O,R and U */
    /* C : */ 0x1c184d95, /* (B|M|E) - Allowed are A,C,E,H,I,K,L,O,T and U */
    /* D : */ 0x1c166919, /* (B|M|E) - Allowed are A,D,E,I,L,N,O,R,S and U */
    /* E : */ 0x1e2ebcee, /* (B|M|E) - Allowed are B,C,D,F,G,H,K,L,M,N,P,R,S,T,V and Z */
    /* F : */ 0x1c1c4931, /* (B|M|E) - Allowed are A,E,F,I,L,O,S,T and U */
    /* G : */ 0x1c1269d1, /* (B|M|E) - Allowed are A,E,G,H,I,L,N,O,R and U */
    /* H : */ 0x1d127911, /* (B|M|E) - Allowed are A,E,I,L,M,N,O,R,U and Y */
    /* I : */ 0x1e2efeff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,J,K,L,M,N,O,P,R,S,T,V and Z */
    /* J : */ 0x1c145515, /* (B|M|E) - Allowed are A,C,E,I,K,M,O,S and U */
    /* K : */ 0x1c1e6d91, /* (B|M|E) - Allowed are A,E,H,I,K,L,N,O,R,S,T and U */
    /* L : */ 0x1d0c5919, /* (B|M|E) - Allowed are A,D,E,I,L,M,O,S,T and Y */
    /* M : */ 0x1d16fd13, /* (B|M|E) - Allowed are A,B,E,I,K,L,M,N,O,P,R,S,U and Y */
    /* N : */ 0x1c0c615f, /* (B|M|E) - Allowed are A,B,C,D,E,G,I,N,O,S and T */
    /* O : */ 0x1c5eb96f, /* (B|M|E) - Allowed are A,B,C,D,F,G,I,L,M,N,P,R,S,T,U and W */
    /* P : */ 0x1d1eedb5, /* (B|M|E) - Allowed are A,C,E,F,H,I,K,L,N,O,P,R,S,T,U and Y */
    /* Q : */ 0x10000000, /* ( | |E) - None are allowed */
    /* R : */ 0x1f7effff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,R,S,T,U,V,W,Y and Z */
    /* S : */ 0x1c7ddd9f, /* (B|M|E) - Allowed are A,B,C,D,E,H,I,K,L,M,O,P,Q,S,T,U,V and W */
    /* T : */ 0x1f1e4995, /* (B|M|E) - Allowed are A,C,E,H,I,L,O,R,S,T,U,Y and Z */
    /* U : */ 0x1caeb97e, /* (B|M|E) - Allowed are B,C,D,E,F,G,I,L,M,N,P,R,S,T,V and X */
    /* V : */ 0x1c004111, /* (B|M|E) - Allowed are A,E,I and O */
    /* W : */ 0x1c044193, /* (B|M|E) - Allowed are A,B,E,H,I,O and S */
    /* X : */ 0x1c18c195, /* (B|M|E) - Allowed are A,C,E,H,I,O,P,T and U */
    /* Y : */ 0x1c263819, /* (B|M|E) - Allowed are A,D,E,L,M,N,R,S and V */
    /* Z : */ 0x1e106111  /* (B|M|E) - Allowed are A,E,I,N,O,U and Z */
  },
  /* Rules following letter F : */ {
    /* A : */ 0x1ffe39de, /* (B|M|E) - Allowed are B,C,D,E,G,H,I,L,M,N,R,S,T,U,V,W,X,Y and Z */
    /* B : */ 0x08124911, /* ( |M| ) - Allowed are A,E,I,L,O,R and U */
    /* C : */ 0x08104191, /* ( |M| ) - Allowed are A,E,H,I,O and U */
    /* D : */ 0x08024111, /* ( |M| ) - Allowed are A,E,I,O and R */
    /* E : */ 0x1d1e391d, /* (B|M|E) - Allowed are A,C,D,E,I,L,M,N,R,S,T,U and Y */
    /* F : */ 0x1c044911, /* (B|M|E) - Allowed are A,E,I,L,O and S */
    /* G : */ 0x08164195, /* ( |M| ) - Allowed are A,C,E,H,I,O,R,S and U */
    /* H : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* I : */ 0x1c8e685d, /* (B|M|E) - Allowed are A,C,D,E,G,L,N,O,R,S,T and X */
    /* J : */ 0x08004011, /* ( |M| ) - Allowed are A,E and O */
    /* K : */ 0x08004191, /* ( |M| ) - Allowed are A,E,H,I and O */
    /* L : */ 0x1d104111, /* (B|M|E) - Allowed are A,E,I,O,U and Y */
    /* M : */ 0x00000000, /* ( | | ) - None are allowed */
    /* N : */ 0x00000000, /* ( | | ) - None are allowed */
    /* O : */ 0x1c122800, /* (B|M|E) - Allowed are L,N,R and U */
    /* P : */ 0x0906d995, /* ( |M| ) - Allowed are A,C,E,H,I,L,M,O,P,R,S and Y */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x0c104111, /* (B|M| ) - Allowed are A,E,I,O and U */
    /* S : */ 0x1818c595, /* ( |M|E) - Allowed are A,C,E,H,I,K,O,P,T and U */
    /* T : */ 0x19044191, /* ( |M|E) - Allowed are A,E,H,I,O,S and Y */
    /* U : */ 0x1c062800, /* (B|M|E) - Allowed are L,N,R and S */
    /* V : */ 0x08024591, /* ( |M| ) - Allowed are A,E,H,I,K,O and R */
    /* W : */ 0x09004191, /* ( |M| ) - Allowed are A,E,H,I,O and Y */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c002910, /* (B|M|E) - Allowed are E,I,L and N */
    /* Z : */ 0x08104151  /* ( |M| ) - Allowed are A,E,G,I,O and U */
  },
  /* Rules following letter G : */ {
    /* A : */ 0x1f7ef9ff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,L,M,N,O,P,R,S,T,U,V,W,Y and Z */
    /* B : */ 0x09124911, /* ( |M| ) - Allowed are A,E,I,L,O,R,U and Y */
    /* C : */ 0x08104c81, /* ( |M| ) - Allowed are A,H,K,L,O and U */
    /* D : */ 0x18004111, /* ( |M|E) - Allowed are A,E,I and O */
    /* E : */ 0x1c0e7809, /* (B|M|E) - Allowed are A,D,L,M,N,O,R,S and T */
    /* F : */ 0x08104911, /* ( |M| ) - Allowed are A,E,I,L,O and U */
    /* G : */ 0x18060911, /* ( |M|E) - Allowed are A,E,I,L,R and S */
    /* H : */ 0x1c084911, /* (B|M|E) - Allowed are A,E,I,L,O and T */
    /* I : */ 0x1c3e785f, /* (B|M|E) - Allowed are A,B,C,D,E,G,L,M,N,O,R,S,T,U and V */
    /* J : */ 0x08104111, /* ( |M| ) - Allowed are A,E,I,O and U */
    /* K : */ 0x18126991, /* ( |M|E) - Allowed are A,E,H,I,L,N,O,R and U */
    /* L : */ 0x1d104111, /* (B|M|E) - Allowed are A,E,I,O,U and Y */
    /* M : */ 0x18004111, /* ( |M|E) - Allowed are A,E,I and O */
    /* N : */ 0x1c004111, /* (B|M|E) - Allowed are A,E,I and O */
    /* O : */ 0x1c7e795b, /* (B|M|E) - Allowed are A,B,D,E,G,I,L,M,N,O,R,S,T,U,V and W */
    /* P : */ 0x08164991, /* ( |M| ) - Allowed are A,E,H,I,L,O,R,S and U */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x00000000, /* ( | | ) - None are allowed */
    /* S : */ 0x18085093, /* ( |M|E) - Allowed are A,B,E,H,M,O and T */
    /* T : */ 0x10000000, /* ( | |E) - None are allowed */
    /* U : */ 0x1c0e3911, /* (B|M|E) - Allowed are A,E,I,L,M,N,R,S and T */
    /* V : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* W : */ 0x0d124191, /* (B|M| ) - Allowed are A,E,H,I,O,R,U and Y */
    /* X : */ 0x08c00101, /* ( |M| ) - Allowed are A,I,W and X */
    /* Y : */ 0x1c1ef919, /* (B|M|E) - Allowed are A,D,E,I,L,M,N,O,P,R,S,T and U */
    /* Z : */ 0x09000191  /* ( |M| ) - Allowed are A,E,H,I and Y */
  },
  /* Rules following letter H : */ {
    /* A : */ 0x1f7effff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,R,S,T,U,V,W,Y and Z */
    /* B : */ 0x19124111, /* ( |M|E) - Allowed are A,E,I,O,R,U and Y */
    /* C : */ 0x00000000, /* ( | | ) - None are allowed */
    /* D : */ 0x18124111, /* ( |M|E) - Allowed are A,E,I,O,R and U */
    /* E : */ 0x1ffefdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,U,V,W,X,Y and Z */
    /* F : */ 0x18104911, /* ( |M|E) - Allowed are A,E,I,L,O and U */
    /* G : */ 0x18124191, /* ( |M|E) - Allowed are A,E,H,I,O,R and U */
    /* H : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* I : */ 0x1f3fffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,Y and Z */
    /* J : */ 0x00000000, /* ( | | ) - None are allowed */
    /* K : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* L : */ 0x00000000, /* ( | | ) - None are allowed */
    /* M : */ 0x10000000, /* ( | |E) - None are allowed */
    /* N : */ 0x18044111, /* ( |M|E) - Allowed are A,E,I,O and S */
    /* O : */ 0x1ffeffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,R,S,T,U,V,W,X,Y and Z */
    /* P : */ 0x0812c995, /* ( |M| ) - Allowed are A,C,E,H,I,L,O,P,R and U */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x00000000, /* ( | | ) - None are allowed */
    /* S : */ 0x00000000, /* ( | | ) - None are allowed */
    /* T : */ 0x18044991, /* ( |M|E) - Allowed are A,E,H,I,L,O and S */
    /* U : */ 0x1f0ebdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,P,R,S,T,Y and Z */
    /* V : */ 0x00000000, /* ( | | ) - None are allowed */
    /* W : */ 0x00000000, /* ( | | ) - None are allowed */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c0ef85f, /* (B|M|E) - Allowed are A,B,C,D,E,G,L,M,N,O,P,R,S and T */
    /* Z : */ 0x09104111  /* ( |M| ) - Allowed are A,E,I,O,U and Y */
  },
  /* Rules following letter I : */ {
    /* A : */ 0x1c0ebcde, /* (B|M|E) - Allowed are B,C,D,E,G,H,K,L,M,N,P,R,S and T */
    /* B : */ 0x1c124913, /* (B|M|E) - Allowed are A,B,E,I,L,O,R and U */
    /* C : */ 0x1e1c4d95, /* (B|M|E) - Allowed are A,C,E,H,I,K,L,O,S,T,U and Z */
    /* D : */ 0x1c044119, /* (B|M|E) - Allowed are A,D,E,I,O and S */
    /* E : */ 0x1c7e3cee, /* (B|M|E) - Allowed are B,C,D,F,G,H,K,L,M,N,R,S,T,U,V and W */
    /* F : */ 0x1d184931, /* (B|M|E) - Allowed are A,E,F,I,L,O,T,U and Y */
    /* G : */ 0x1c1679d1, /* (B|M|E) - Allowed are A,E,G,H,I,L,M,N,O,R,S and U */
    /* H : */ 0x1c106911, /* (B|M|E) - Allowed are A,E,I,L,N,O and U */
    /* I : */ 0x1c0eb81d, /* (B|M|E) - Allowed are A,C,D,E,L,M,N,P,R,S and T */
    /* J : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* K : */ 0x1c004511, /* (B|M|E) - Allowed are A,E,I,K and O */
    /* L : */ 0x1d0c4d1b, /* (B|M|E) - Allowed are A,B,D,E,I,K,L,O,S,T and Y */
    /* M : */ 0x1c14d113, /* (B|M|E) - Allowed are A,B,E,I,M,O,P,S and U */
    /* N : */ 0x1f7f7fff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,Q,R,S,T,U,V,W,Y and Z */
    /* O : */ 0x1c1eb805, /* (B|M|E) - Allowed are A,C,L,M,N,P,R,S,T and U */
    /* P : */ 0x1c1ec991, /* (B|M|E) - Allowed are A,E,H,I,L,O,P,R,S,T and U */
    /* Q : */ 0x10000000, /* ( | |E) - None are allowed */
    /* R : */ 0x1c0e5d1d, /* (B|M|E) - Allowed are A,C,D,E,I,K,L,M,O,R,S and T */
    /* S : */ 0x1f7fdfff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,O,P,Q,R,S,T,U,V,W,Y and Z */
    /* T : */ 0x1f1e5d95, /* (B|M|E) - Allowed are A,C,E,H,I,K,L,M,O,R,S,T,U,Y and Z */
    /* U : */ 0x10000000, /* ( | |E) - None are allowed */
    /* V : */ 0x1c000111, /* (B|M|E) - Allowed are A,E and I */
    /* W : */ 0x10000000, /* ( | |E) - None are allowed */
    /* X : */ 0x1c1cc111, /* (B|M|E) - Allowed are A,E,I,O,P,S,T and U */
    /* Y : */ 0x10000000, /* ( | |E) - None are allowed */
    /* Z : */ 0x1e004111  /* (B|M|E) - Allowed are A,E,I,O and Z */
  },
  /* Rules following letter J : */ {
    /* A : */ 0x1f7fbdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,P,Q,R,S,T,U,V,W,Y and Z */
    /* B : */ 0x00000000, /* ( | | ) - None are allowed */
    /* C : */ 0x0a000191, /* ( |M| ) - Allowed are A,E,H,I and Z */
    /* D : */ 0x08144011, /* ( |M| ) - Allowed are A,E,O,S and U */
    /* E : */ 0x1e7efcbf, /* (B|M|E) - Allowed are A,B,C,D,E,F,H,K,L,M,N,O,P,R,S,T,U,V,W and Z */
    /* F : */ 0x00000000, /* ( | | ) - None are allowed */
    /* G : */ 0x00000000, /* ( | | ) - None are allowed */
    /* H : */ 0x04104111, /* (B| | ) - Allowed are A,E,I,O and U */
    /* I : */ 0x1c2efcdf, /* (B|M|E) - Allowed are A,B,C,D,E,G,H,K,L,M,N,O,P,R,S,T and V */
    /* J : */ 0x00000000, /* ( | | ) - None are allowed */
    /* K : */ 0x00000000, /* ( | | ) - None are allowed */
    /* L : */ 0x00000000, /* ( | | ) - None are allowed */
    /* M : */ 0x0810c015, /* ( |M| ) - Allowed are A,C,E,O,P and U */
    /* N : */ 0x19104119, /* ( |M|E) - Allowed are A,D,E,I,O,U and Y */
    /* O : */ 0x1f7e6ddf, /* (B|M|E) - Allowed are A,B,C,D,E,G,H,I,K,L,N,O,R,S,T,U,V,W,Y and Z */
    /* P : */ 0x0811c131, /* ( |M| ) - Allowed are A,E,F,I,O,P,Q and U */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x08006111, /* ( |M| ) - Allowed are A,E,I,N and O */
    /* S : */ 0x081c1591, /* ( |M| ) - Allowed are A,E,H,I,K,M,S,T and U */
    /* T : */ 0x08004415, /* ( |M| ) - Allowed are A,C,E,K and O */
    /* U : */ 0x1c2e395b, /* (B|M|E) - Allowed are A,B,D,E,G,I,L,M,N,R,S,T and V */
    /* V : */ 0x00000000, /* ( | | ) - None are allowed */
    /* W : */ 0x00000000, /* ( | | ) - None are allowed */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Z : */ 0x00000000  /* ( | | ) - None are allowed */
  },
  /* Rules following letter K : */ {
    /* A : */ 0x1f5eb98e, /* (B|M|E) - Allowed are B,C,D,H,I,L,M,N,P,R,S,T,U,W,Y and Z */
    /* B : */ 0x09164911, /* ( |M| ) - Allowed are A,E,I,L,O,R,S,U and Y */
    /* C : */ 0x08104891, /* ( |M| ) - Allowed are A,E,H,L,O and U */
    /* D : */ 0x09124111, /* ( |M| ) - Allowed are A,E,I,O,R,U and Y */
    /* E : */ 0x1d0e2818, /* (B|M|E) - Allowed are D,E,L,N,R,S,T and Y */
    /* F : */ 0x08104111, /* ( |M| ) - Allowed are A,E,I,O and U */
    /* G : */ 0x08164153, /* ( |M| ) - Allowed are A,B,E,G,I,O,R,S and U */
    /* H : */ 0x1c104111, /* (B|M|E) - Allowed are A,E,I,O and U */
    /* I : */ 0x1c0e2810, /* (B|M|E) - Allowed are E,L,N,R,S and T */
    /* J : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* K : */ 0x18104191, /* ( |M|E) - Allowed are A,E,H,I,O and U */
    /* L : */ 0x1c104111, /* (B|M|E) - Allowed are A,E,I,O and U */
    /* M : */ 0x08004011, /* ( |M| ) - Allowed are A,E and O */
    /* N : */ 0x0c104111, /* (B|M| ) - Allowed are A,E,I,O and U */
    /* O : */ 0x1e7efdfe, /* (B|M|E) - Allowed are B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,U,V,W and Z */
    /* P : */ 0x08044911, /* ( |M| ) - Allowed are A,E,I,L,O and S */
    /* Q : */ 0x08500116, /* ( |M| ) - Allowed are B,C,E,I,U and W */
    /* R : */ 0x1d104111, /* (B|M|E) - Allowed are A,E,I,O,U and Y */
    /* S : */ 0x1c08d997, /* (B|M|E) - Allowed are A,B,C,E,H,I,L,M,O,P and T */
    /* T : */ 0x18124191, /* ( |M|E) - Allowed are A,E,H,I,O,R and U */
    /* U : */ 0x1f0ebfff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,P,R,S,T,Y and Z */
    /* V : */ 0x0c000111, /* (B|M| ) - Allowed are A,E and I */
    /* W : */ 0x0c004111, /* (B|M| ) - Allowed are A,E,I and O */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c56f919, /* (B|M|E) - Allowed are A,D,E,I,L,M,N,O,P,R,S,U and W */
    /* Z : */ 0x08144119  /* ( |M| ) - Allowed are A,D,E,I,O,S and U */
  },
  /* Rules following letter L : */ {
    /* A : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* B : */ 0x18124111, /* ( |M|E) - Allowed are A,E,I,O,R and U */
    /* C : */ 0x1a104191, /* ( |M|E) - Allowed are A,E,H,I,O,U and Z */
    /* D : */ 0x180e4111, /* ( |M|E) - Allowed are A,E,I,O,R,S and T */
    /* E : */ 0x1ffeffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,R,S,T,U,V,W,X,Y and Z */
    /* F : */ 0x1814c931, /* ( |M|E) - Allowed are A,E,F,I,L,O,P,S and U */
    /* G : */ 0x18124111, /* ( |M|E) - Allowed are A,E,I,O,R and U */
    /* H : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* I : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* J : */ 0x08104511, /* ( |M| ) - Allowed are A,E,I,K,O and U */
    /* K : */ 0x18044111, /* ( |M|E) - Allowed are A,E,I,O and S */
    /* L : */ 0x1d547193, /* (B|M|E) - Allowed are A,B,E,H,I,M,N,O,S,U,W and Y */
    /* M : */ 0x18004111, /* ( |M|E) - Allowed are A,E,I and O */
    /* N : */ 0x18000111, /* ( |M|E) - Allowed are A,E and I */
    /* O : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* P : */ 0x18004191, /* ( |M|E) - Allowed are A,E,H,I and O */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x19104111, /* ( |M|E) - Allowed are A,E,I,O,U and Y */
    /* S : */ 0x185cd597, /* ( |M|E) - Allowed are A,B,C,E,H,I,K,M,O,P,S,T,U and W */
    /* T : */ 0x1b165193, /* ( |M|E) - Allowed are A,B,E,H,I,M,O,R,S,U,Y and Z */
    /* U : */ 0x1faefdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,V,X,Y and Z */
    /* V : */ 0x18004111, /* ( |M|E) - Allowed are A,E,I and O */
    /* W : */ 0x09004111, /* ( |M| ) - Allowed are A,E,I,O and Y */
    /* X : */ 0x1880c110, /* ( |M|E) - Allowed are E,I,O,P and X */
    /* Y : */ 0x1c0cf11f, /* (B|M|E) - Allowed are A,B,C,D,E,I,M,N,O,P,S and T */
    /* Z : */ 0x18005111  /* ( |M|E) - Allowed are A,E,I,M and O */
  },
  /* Rules following letter M : */ {
    /* A : */ 0x1ffeffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,R,S,T,U,V,W,X,Y and Z */
    /* B : */ 0x00000000, /* ( | | ) - None are allowed */
    /* C : */ 0x1c003d5d, /* (B|M|E) - Allowed are A,C,D,E,G,I,K,L,M and N */
    /* D : */ 0x18106111, /* ( |M|E) - Allowed are A,E,I,N,O and U */
    /* E : */ 0x1f5e7ddf, /* (B|M|E) - Allowed are A,B,C,D,E,G,H,I,K,L,M,N,O,R,S,T,U,W,Y and Z */
    /* F : */ 0x18104911, /* ( |M|E) - Allowed are A,E,I,L,O and U */
    /* G : */ 0x18124111, /* ( |M|E) - Allowed are A,E,I,O,R and U */
    /* H : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* I : */ 0x1f9e7cfd, /* (B|M|E) - Allowed are A,C,D,E,F,G,H,K,L,M,N,O,R,S,T,U,X,Y and Z */
    /* J : */ 0x08104181, /* ( |M| ) - Allowed are A,H,I,O and U */
    /* K : */ 0x08104111, /* ( |M| ) - Allowed are A,E,I,O and U */
    /* L : */ 0x00000000, /* ( | | ) - None are allowed */
    /* M : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* N : */ 0x00000000, /* ( | | ) - None are allowed */
    /* O : */ 0x1f7efddf, /* (B|M|E) - Allowed are A,B,C,D,E,G,H,I,K,L,M,N,O,P,R,S,T,U,V,W,Y and Z */
    /* P : */ 0x181e4991, /* ( |M|E) - Allowed are A,E,H,I,L,O,R,S,T and U */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x00000000, /* ( | | ) - None are allowed */
    /* S : */ 0x00000000, /* ( | | ) - None are allowed */
    /* T : */ 0x18164191, /* ( |M|E) - Allowed are A,E,H,I,O,R,S and U */
    /* U : */ 0x1c0e381c, /* (B|M|E) - Allowed are C,D,E,L,M,N,R,S and T */
    /* V : */ 0x08044111, /* ( |M| ) - Allowed are A,E,I,O and S */
    /* W : */ 0x08024191, /* ( |M| ) - Allowed are A,E,H,I,O and R */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c0e6c55, /* (B|M|E) - Allowed are A,C,E,G,K,L,N,O,R,S and T */
    /* Z : */ 0x08104111  /* ( |M| ) - Allowed are A,E,I,O and U */
  },
  /* Rules following letter N : */ {
    /* A : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* B : */ 0x08124811, /* ( |M| ) - Allowed are A,E,L,O,R and U */
    /* C : */ 0x00000000, /* ( | | ) - None are allowed */
    /* D : */ 0x00000000, /* ( | | ) - None are allowed */
    /* E : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* F : */ 0x18104911, /* ( |M|E) - Allowed are A,E,I,L,O and U */
    /* G : */ 0x00000000, /* ( | | ) - None are allowed */
    /* H : */ 0x1c104111, /* (B|M|E) - Allowed are A,E,I,O and U */
    /* I : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* J : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* K : */ 0x18044911, /* ( |M|E) - Allowed are A,E,I,L,O and S */
    /* L : */ 0x19104111, /* ( |M|E) - Allowed are A,E,I,O,U and Y */
    /* M : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* N : */ 0x19104111, /* ( |M|E) - Allowed are A,E,I,O,U and Y */
    /* O : */ 0x1ffefdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,U,V,W,X,Y and Z */
    /* P : */ 0x18124991, /* ( |M|E) - Allowed are A,E,H,I,L,O,R and U */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x19104111, /* ( |M|E) - Allowed are A,E,I,O,U and Y */
    /* S : */ 0x197cddbf, /* ( |M|E) - Allowed are A,B,C,D,E,F,H,I,K,L,M,O,P,S,T,U,V,W and Y */
    /* T : */ 0x18164991, /* ( |M|E) - Allowed are A,E,H,I,L,O,R,S and U */
    /* U : */ 0x1e0efd7f, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,I,K,L,M,N,O,P,R,S,T and Z */
    /* V : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* W : */ 0x08024111, /* ( |M| ) - Allowed are A,E,I,O and R */
    /* X : */ 0x18081110, /* ( |M|E) - Allowed are E,I,M and T */
    /* Y : */ 0x1cdefddf, /* (B|M|E) - Allowed are A,B,C,D,E,G,H,I,K,L,M,N,O,P,R,S,T,U,W and X */
    /* Z : */ 0x19104911  /* ( |M|E) - Allowed are A,E,I,L,O,U and Y */
  },
  /* Rules following letter O : */ {
    /* A : */ 0x1c0efccc, /* (B|M|E) - Allowed are C,D,G,H,K,L,M,N,O,P,R,S and T */
    /* B : */ 0x1d1e6b1b, /* (B|M|E) - Allowed are A,B,D,E,I,J,L,N,O,R,S,T,U and Y */
    /* C : */ 0x1c184595, /* (B|M|E) - Allowed are A,C,E,H,I,K,O,T and U */
    /* D : */ 0x1d164919, /* (B|M|E) - Allowed are A,D,E,I,L,O,R,S,U and Y */
    /* E : */ 0x1c3ebdee, /* (B|M|E) - Allowed are B,C,D,F,G,H,I,K,L,M,N,P,R,S,T,U and V */
    /* F : */ 0x1c0c4931, /* (B|M|E) - Allowed are A,E,F,I,L,O,S and T */
    /* G : */ 0x1d1669d1, /* (B|M|E) - Allowed are A,E,G,H,I,L,N,O,R,S,U and Y */
    /* H : */ 0x1d067911, /* (B|M|E) - Allowed are A,E,I,L,M,N,O,R,S and Y */
    /* I : */ 0x1c0e280d, /* (B|M|E) - Allowed are A,C,D,L,N,R,S and T */
    /* J : */ 0x1c186515, /* (B|M|E) - Allowed are A,C,E,I,K,N,O,T and U */
    /* K : */ 0x1c144d11, /* (B|M|E) - Allowed are A,E,I,K,L,O,S and U */
    /* L : */ 0x1f3cfd7f, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,I,K,L,M,N,O,P,S,T,U,V,Y and Z */
    /* M : */ 0x1d1efd33, /* (B|M|E) - Allowed are A,B,E,F,I,K,L,M,N,O,P,R,S,T,U and Y */
    /* N : */ 0x1f7e7fff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,R,S,T,U,V,W,Y and Z */
    /* O : */ 0x1e1ebd7e, /* (B|M|E) - Allowed are B,C,D,E,F,G,I,K,L,M,N,P,R,S,T,U and Z */
    /* P : */ 0x1d1ec991, /* (B|M|E) - Allowed are A,E,H,I,L,O,P,R,S,T,U and Y */
    /* Q : */ 0x10000000, /* ( | |E) - None are allowed */
    /* R : */ 0x1d1efd7f, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,I,K,L,M,N,O,P,R,S,T,U and Y */
    /* S : */ 0x1f5ddd9f, /* (B|M|E) - Allowed are A,B,C,D,E,H,I,K,L,M,O,P,Q,S,T,U,W,Y and Z */
    /* T : */ 0x1f1e4995, /* (B|M|E) - Allowed are A,C,E,H,I,L,O,R,S,T,U,Y and Z */
    /* U : */ 0x1eaebd7f, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,I,K,L,M,N,P,R,S,T,V,X and Z */
    /* V : */ 0x1c000111, /* (B|M|E) - Allowed are A,E and I */
    /* W : */ 0x1c042919, /* (B|M|E) - Allowed are A,D,E,I,L,N and S */
    /* X : */ 0x1d084993, /* (B|M|E) - Allowed are A,B,E,H,I,L,O,T and Y */
    /* Y : */ 0x1c0c7d1d, /* (B|M|E) - Allowed are A,C,D,E,I,K,L,M,N,O,S and T */
    /* Z : */ 0x1f106919  /* (B|M|E) - Allowed are A,D,E,I,L,N,O,U,Y and Z */
  },
  /* Rules following letter P : */ {
    /* A : */ 0x1ffeffdf, /* (B|M|E) - Allowed are A,B,C,D,E,G,H,I,J,K,L,M,N,O,P,R,S,T,U,V,W,X,Y and Z */
    /* B : */ 0x08124811, /* ( |M| ) - Allowed are A,E,L,O,R and U */
    /* C : */ 0x00000000, /* ( | | ) - None are allowed */
    /* D : */ 0x19024111, /* ( |M|E) - Allowed are A,E,I,O,R and Y */
    /* E : */ 0x1f5efdfd, /* (B|M|E) - Allowed are A,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,U,W,Y and Z */
    /* F : */ 0x1c104911, /* (B|M|E) - Allowed are A,E,I,L,O and U */
    /* G : */ 0x08124991, /* ( |M| ) - Allowed are A,E,H,I,L,O,R and U */
    /* H : */ 0x1d124111, /* (B|M|E) - Allowed are A,E,I,O,R,U and Y */
    /* I : */ 0x1e1efc7d, /* (B|M|E) - Allowed are A,C,D,E,F,G,K,L,M,N,O,P,R,S,T,U and Z */
    /* J : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* K : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* L : */ 0x1c104111, /* (B|M|E) - Allowed are A,E,I,O and U */
    /* M : */ 0x18004011, /* ( |M|E) - Allowed are A,E and O */
    /* N : */ 0x1c004111, /* (B|M|E) - Allowed are A,E,I and O */
    /* O : */ 0x1ffefdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,U,V,W,X,Y and Z */
    /* P : */ 0x18024911, /* ( |M|E) - Allowed are A,E,I,L,O and R */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x1c004111, /* (B|M|E) - Allowed are A,E,I and O */
    /* S : */ 0x1d184195, /* (B|M|E) - Allowed are A,C,E,H,I,O,T,U and Y */
    /* T : */ 0x00000000, /* ( | | ) - None are allowed */
    /* U : */ 0x1c0e385e, /* (B|M|E) - Allowed are B,C,D,E,G,L,M,N,R,S and T */
    /* V : */ 0x08004911, /* ( |M| ) - Allowed are A,E,I,L and O */
    /* W : */ 0x08024191, /* ( |M| ) - Allowed are A,E,H,I,O and R */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c0a2914, /* (B|M|E) - Allowed are C,E,I,L,N,R and T */
    /* Z : */ 0x00000000  /* ( | | ) - None are allowed */
  },
  /* Rules following letter Q : */ {
    /* A : */ 0x1c1e392a, /* (B|M|E) - Allowed are B,D,F,I,L,M,N,R,S,T and U */
    /* B : */ 0x00000000, /* ( | | ) - None are allowed */
    /* C : */ 0x00000000, /* ( | | ) - None are allowed */
    /* D : */ 0x00000000, /* ( | | ) - None are allowed */
    /* E : */ 0x08be613b, /* ( |M| ) - Allowed are A,B,D,E,F,I,N,O,R,S,T,U,V and X */
    /* F : */ 0x00000000, /* ( | | ) - None are allowed */
    /* G : */ 0x00000000, /* ( | | ) - None are allowed */
    /* H : */ 0x00000000, /* ( | | ) - None are allowed */
    /* I : */ 0x1b9e75ff, /* ( |M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,M,N,O,R,S,T,U,X,Y and Z */
    /* J : */ 0x00000000, /* ( | | ) - None are allowed */
    /* K : */ 0x00000000, /* ( | | ) - None are allowed */
    /* L : */ 0x00000000, /* ( | | ) - None are allowed */
    /* M : */ 0x00000000, /* ( | | ) - None are allowed */
    /* N : */ 0x00000000, /* ( | | ) - None are allowed */
    /* O : */ 0x0812a83a, /* ( |M| ) - Allowed are B,D,E,F,L,N,P,R and U */
    /* P : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x00000000, /* ( | | ) - None are allowed */
    /* S : */ 0x00000000, /* ( | | ) - None are allowed */
    /* T : */ 0x00000000, /* ( | | ) - None are allowed */
    /* U : */ 0x1c000111, /* (B|M|E) - Allowed are A,E and I */
    /* V : */ 0x00000000, /* ( | | ) - None are allowed */
    /* W : */ 0x09004191, /* ( |M| ) - Allowed are A,E,H,I,O and Y */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Z : */ 0x00000000  /* ( | | ) - None are allowed */
  },
  /* Rules following letter R : */ {
    /* A : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* B : */ 0x19124911, /* ( |M|E) - Allowed are A,E,I,L,O,R,U and Y */
    /* C : */ 0x18104191, /* ( |M|E) - Allowed are A,E,H,I,O and U */
    /* D : */ 0x195e6911, /* ( |M|E) - Allowed are A,E,I,L,N,O,R,S,T,U,W and Y */
    /* E : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* F : */ 0x18104931, /* ( |M|E) - Allowed are A,E,F,I,L,O and U */
    /* G : */ 0x18104191, /* ( |M|E) - Allowed are A,E,H,I,O and U */
    /* H : */ 0x1d104111, /* (B|M|E) - Allowed are A,E,I,O,U and Y */
    /* I : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* J : */ 0x08104111, /* ( |M| ) - Allowed are A,E,I,O and U */
    /* K : */ 0x18044991, /* ( |M|E) - Allowed are A,E,H,I,L,O and S */
    /* L : */ 0x19004111, /* ( |M|E) - Allowed are A,E,I,O and Y */
    /* M : */ 0x18144111, /* ( |M|E) - Allowed are A,E,I,O,S and U */
    /* N : */ 0x191c519b, /* ( |M|E) - Allowed are A,B,D,E,H,I,M,O,S,T,U and Y */
    /* O : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* P : */ 0x18164991, /* ( |M|E) - Allowed are A,E,H,I,L,O,R,S and U */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x19104191, /* ( |M|E) - Allowed are A,E,H,I,O,U and Y */
    /* S : */ 0x1818cd95, /* ( |M|E) - Allowed are A,C,E,H,I,K,L,O,P,T and U */
    /* T : */ 0x1b5e59b5, /* ( |M|E) - Allowed are A,C,E,F,H,I,L,M,O,R,S,T,U,W,Y and Z */
    /* U : */ 0x1f2efdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,V,Y and Z */
    /* V : */ 0x18004111, /* ( |M|E) - Allowed are A,E,I and O */
    /* W : */ 0x18004191, /* ( |M|E) - Allowed are A,E,H,I and O */
    /* X : */ 0x18441911, /* ( |M|E) - Allowed are A,E,I,L,M,S and W */
    /* Y : */ 0x1c2efd5f, /* (B|M|E) - Allowed are A,B,C,D,E,G,I,K,L,M,N,O,P,R,S,T and V */
    /* Z : */ 0x19104111  /* ( |M|E) - Allowed are A,E,I,O,U and Y */
  },
  /* Rules following letter S : */ {
    /* A : */ 0x1dfeffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,R,S,T,U,V,W,X and Y */
    /* B : */ 0x00000000, /* ( | | ) - None are allowed */
    /* C : */ 0x1c004191, /* (B|M|E) - Allowed are A,E,H,I and O */
    /* D : */ 0x18024111, /* ( |M|E) - Allowed are A,E,I,O and R */
    /* E : */ 0x1ffffdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* F : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* G : */ 0x18124911, /* ( |M|E) - Allowed are A,E,I,L,O,R and U */
    /* H : */ 0x1c104111, /* (B|M|E) - Allowed are A,E,I,O and U */
    /* I : */ 0x1ebefc7f, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,K,L,M,N,O,P,R,S,T,U,V,X and Z */
    /* J : */ 0x08104011, /* ( |M| ) - Allowed are A,E,O and U */
    /* K : */ 0x1d004111, /* (B|M|E) - Allowed are A,E,I,O and Y */
    /* L : */ 0x1d104111, /* (B|M|E) - Allowed are A,E,I,O,U and Y */
    /* M : */ 0x1c104111, /* (B|M|E) - Allowed are A,E,I,O and U */
    /* N : */ 0x0c004111, /* (B|M| ) - Allowed are A,E,I and O */
    /* O : */ 0x1c7ef80e, /* (B|M|E) - Allowed are B,C,D,L,M,N,O,P,R,S,T,U,V and W */
    /* P : */ 0x1c124991, /* (B|M|E) - Allowed are A,E,H,I,L,O,R and U */
    /* Q : */ 0x10000000, /* ( | |E) - None are allowed */
    /* R : */ 0x1c104111, /* (B|M|E) - Allowed are A,E,I,O and U */
    /* S : */ 0x00000000, /* ( | | ) - None are allowed */
    /* T : */ 0x1c164111, /* (B|M|E) - Allowed are A,E,I,O,R,S and U */
    /* U : */ 0x1e0efdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T and Z */
    /* V : */ 0x0c000111, /* (B|M| ) - Allowed are A,E and I */
    /* W : */ 0x0c004111, /* (B|M| ) - Allowed are A,E,I and O */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c06b81d, /* (B|M|E) - Allowed are A,C,D,E,L,M,N,P,R and S */
    /* Z : */ 0x1d104515  /* (B|M|E) - Allowed are A,C,E,I,K,O,U and Y */
  },
  /* Rules following letter T : */ {
    /* A : */ 0x1fffffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y and Z */
    /* B : */ 0x19124911, /* ( |M|E) - Allowed are A,E,I,L,O,R,U and Y */
    /* C : */ 0x10000000, /* ( | |E) - None are allowed */
    /* D : */ 0x18024111, /* ( |M|E) - Allowed are A,E,I,O and R */
    /* E : */ 0x1ffefdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,U,V,W,X,Y and Z */
    /* F : */ 0x18104911, /* ( |M|E) - Allowed are A,E,I,L,O and U */
    /* G : */ 0x08124011, /* ( |M| ) - Allowed are A,E,O,R and U */
    /* H : */ 0x1d565911, /* (B|M|E) - Allowed are A,E,I,L,M,O,R,S,U,W and Y */
    /* I : */ 0x1e3ffd7f, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,I,K,L,M,N,O,P,Q,R,S,T,U,V and Z */
    /* J : */ 0x08104011, /* ( |M| ) - Allowed are A,E,O and U */
    /* K : */ 0x09104111, /* ( |M| ) - Allowed are A,E,I,O,U and Y */
    /* L : */ 0x1d000111, /* (B|M|E) - Allowed are A,E,I and Y */
    /* M : */ 0x08004011, /* ( |M| ) - Allowed are A,E and O */
    /* N : */ 0x08000111, /* ( |M| ) - Allowed are A,E and I */
    /* O : */ 0x1dfefdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,O,P,R,S,T,U,V,W,X and Y */
    /* P : */ 0x081259b3, /* ( |M| ) - Allowed are A,B,E,F,H,I,L,M,O,R and U */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x1d104111, /* (B|M|E) - Allowed are A,E,I,O,U and Y */
    /* S : */ 0x1c78d597, /* (B|M|E) - Allowed are A,B,C,E,H,I,K,M,O,P,T,U,V and W */
    /* T : */ 0x19165991, /* ( |M|E) - Allowed are A,E,H,I,L,M,O,R,S,U and Y */
    /* U : */ 0x1c0e3809, /* (B|M|E) - Allowed are A,D,L,M,N,R,S and T */
    /* V : */ 0x18004111, /* ( |M|E) - Allowed are A,E,I and O */
    /* W : */ 0x0c004111, /* (B|M| ) - Allowed are A,E,I and O */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c4efd7f, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,I,K,L,M,N,O,P,R,S,T and W */
    /* Z : */ 0x1c001d11  /* (B|M|E) - Allowed are A,E,I,K,L and M */
  },
  /* Rules following letter U : */ {
    /* A : */ 0x1d2e3dde, /* (B|M|E) - Allowed are B,C,D,E,G,H,I,K,L,M,N,R,S,T,V and Y */
    /* B : */ 0x1c1e591b, /* (B|M|E) - Allowed are A,B,D,E,I,L,M,O,R,S,T and U */
    /* C : */ 0x1c184d95, /* (B|M|E) - Allowed are A,C,E,H,I,K,L,O,T and U */
    /* D : */ 0x1c024919, /* (B|M|E) - Allowed are A,D,E,I,L,O and R */
    /* E : */ 0x1f3e39ff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,L,M,N,R,S,T,U,V,Y and Z */
    /* F : */ 0x10000000, /* ( | |E) - None are allowed */
    /* G : */ 0x1c1009d1, /* (B|M|E) - Allowed are A,E,G,H,I,L and U */
    /* H : */ 0x1c1e7911, /* (B|M|E) - Allowed are A,E,I,L,M,N,O,R,S,T and U */
    /* I : */ 0x1e2ef85d, /* (B|M|E) - Allowed are A,C,D,E,G,L,M,N,O,P,R,S,T,V and Z */
    /* J : */ 0x18104111, /* ( |M|E) - Allowed are A,E,I,O and U */
    /* K : */ 0x1c104591, /* (B|M|E) - Allowed are A,E,H,I,K,O and U */
    /* L : */ 0x1d3cfd7f, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,I,K,L,M,N,O,P,S,T,U,V and Y */
    /* M : */ 0x1c14f913, /* (B|M|E) - Allowed are A,B,E,I,L,M,N,O,P,S and U */
    /* N : */ 0x1f7fffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,Y and Z */
    /* O : */ 0x1c1e390d, /* (B|M|E) - Allowed are A,C,D,I,L,M,N,R,S,T and U */
    /* P : */ 0x1c0ec991, /* (B|M|E) - Allowed are A,E,H,I,L,O,P,R,S and T */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x1f7fffff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,Y and Z */
    /* S : */ 0x1e1ccd97, /* (B|M|E) - Allowed are A,B,C,E,H,I,K,L,O,P,S,T,U and Z */
    /* T : */ 0x1f7e5fff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,J,K,L,M,O,R,S,T,U,V,W,Y and Z */
    /* U : */ 0x1c8e381c, /* (B|M|E) - Allowed are C,D,E,L,M,N,R,S,T and X */
    /* V : */ 0x18024111, /* ( |M|E) - Allowed are A,E,I,O and R */
    /* W : */ 0x18000511, /* ( |M|E) - Allowed are A,E,I and K */
    /* X : */ 0x18185993, /* ( |M|E) - Allowed are A,B,E,H,I,L,M,O,T and U */
    /* Y : */ 0x181e6c19, /* ( |M|E) - Allowed are A,D,E,K,L,N,O,R,S,T and U */
    /* Z : */ 0x1e005111  /* (B|M|E) - Allowed are A,E,I,M,O and Z */
  },
  /* Rules following letter V : */ {
    /* A : */ 0x1d1ead4e, /* (B|M|E) - Allowed are B,C,D,G,I,K,L,N,P,R,S,T,U and Y */
    /* B : */ 0x00000000, /* ( | | ) - None are allowed */
    /* C : */ 0x091c479d, /* ( |M| ) - Allowed are A,C,D,E,H,I,J,K,O,S,T,U and Y */
    /* D : */ 0x1810c111, /* ( |M|E) - Allowed are A,E,I,O,P and U */
    /* E : */ 0x1d0e291d, /* (B|M|E) - Allowed are A,C,D,E,I,L,N,R,S,T and Y */
    /* F : */ 0x00000000, /* ( | | ) - None are allowed */
    /* G : */ 0x08124851, /* ( |M| ) - Allowed are A,E,G,L,O,R and U */
    /* H : */ 0x09044111, /* ( |M| ) - Allowed are A,E,I,O,S and Y */
    /* I : */ 0x1c0e685d, /* (B|M|E) - Allowed are A,C,D,E,G,L,N,O,R,S and T */
    /* J : */ 0x00000000, /* ( | | ) - None are allowed */
    /* K : */ 0x08004101, /* ( |M| ) - Allowed are A,I and O */
    /* L : */ 0x0d004111, /* (B|M| ) - Allowed are A,E,I,O and Y */
    /* M : */ 0x181c4111, /* ( |M|E) - Allowed are A,E,I,O,S,T and U */
    /* N : */ 0x1d044111, /* (B|M|E) - Allowed are A,E,I,O,S and Y */
    /* O : */ 0x1d5e6d54, /* (B|M|E) - Allowed are C,E,G,I,K,L,N,O,R,S,T,U,W and Y */
    /* P : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x1c004111, /* (B|M|E) - Allowed are A,E,I and O */
    /* S : */ 0x10000000, /* ( | |E) - None are allowed */
    /* T : */ 0x0b1e5195, /* ( |M| ) - Allowed are A,C,E,H,I,M,O,R,S,T,U,Y and Z */
    /* U : */ 0x0c041800, /* (B|M| ) - Allowed are L,M and S */
    /* V : */ 0x09004191, /* ( |M| ) - Allowed are A,E,H,I,O and Y */
    /* W : */ 0x09140111, /* ( |M| ) - Allowed are A,E,I,S,U and Y */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c04291d, /* (B|M|E) - Allowed are A,C,D,E,I,L,N and S */
    /* Z : */ 0x08205191  /* ( |M| ) - Allowed are A,E,H,I,M,O and V */
  },
  /* Rules following letter W : */ {
    /* A : */ 0x1ffebdff, /* (B|M|E) - Allowed are A,B,C,D,E,F,G,H,I,K,L,M,N,P,R,S,T,U,V,W,X,Y and Z */
    /* B : */ 0x09124911, /* ( |M| ) - Allowed are A,E,I,L,O,R,U and Y */
    /* C : */ 0x0a104c91, /* ( |M| ) - Allowed are A,E,H,K,L,O,U and Z */
    /* D : */ 0x19024911, /* ( |M|E) - Allowed are A,E,I,L,O,R and Y */
    /* E : */ 0x1d0ea9df, /* (B|M|E) - Allowed are A,B,C,D,E,G,H,I,L,N,P,R,S,T and Y */
    /* F : */ 0x08104901, /* ( |M| ) - Allowed are A,I,L,O and U */
    /* G : */ 0x18064911, /* ( |M|E) - Allowed are A,E,I,L,O,R and S */
    /* H : */ 0x00000000, /* ( | | ) - None are allowed */
    /* I : */ 0x1c0e285c, /* (B|M|E) - Allowed are C,D,E,G,L,N,R,S and T */
    /* J : */ 0x00000000, /* ( | | ) - None are allowed */
    /* K : */ 0x18444111, /* ( |M|E) - Allowed are A,E,I,O,S and W */
    /* L : */ 0x1d044111, /* (B|M|E) - Allowed are A,E,I,O,S and Y */
    /* M : */ 0x10000000, /* ( | |E) - None are allowed */
    /* N : */ 0x190c0999, /* ( |M|E) - Allowed are A,D,E,H,I,L,S,T and Y */
    /* O : */ 0x1c127a90, /* (B|M|E) - Allowed are E,H,J,L,M,N,O,R and U */
    /* P : */ 0x08464911, /* ( |M| ) - Allowed are A,E,I,L,O,R,S and W */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x1d004111, /* (B|M|E) - Allowed are A,E,I,O and Y */
    /* S : */ 0x10000000, /* ( | |E) - None are allowed */
    /* T : */ 0x18064191, /* ( |M|E) - Allowed are A,E,H,I,O,R and S */
    /* U : */ 0x0c0ef85a, /* (B|M| ) - Allowed are B,D,E,G,L,M,N,O,P,R,S and T */
    /* V : */ 0x00000000, /* ( | | ) - None are allowed */
    /* W : */ 0x18004191, /* ( |M|E) - Allowed are A,E,H,I and O */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c0e3c5d, /* (B|M|E) - Allowed are A,C,D,E,G,K,L,M,N,R,S and T */
    /* Z : */ 0x09004911  /* ( |M| ) - Allowed are A,E,I,L,O and Y */
  },
  /* Rules following letter X : */ {
    /* A : */ 0x1d0e3844, /* (B|M|E) - Allowed are C,G,L,M,N,R,S,T and Y */
    /* B : */ 0x09124111, /* ( |M| ) - Allowed are A,E,I,O,R,U and Y */
    /* C : */ 0x08104991, /* ( |M| ) - Allowed are A,E,H,I,L,O and U */
    /* D : */ 0x09124111, /* ( |M| ) - Allowed are A,E,I,O,R,U and Y */
    /* E : */ 0x1d0e390c, /* (B|M|E) - Allowed are C,D,I,L,M,N,R,S,T and Y */
    /* F : */ 0x08004110, /* ( |M| ) - Allowed are E,I and O */
    /* G : */ 0x08104801, /* ( |M| ) - Allowed are A,L,O and U */
    /* H : */ 0x08104111, /* ( |M| ) - Allowed are A,E,I,O and U */
    /* I : */ 0x1cac785d, /* (B|M|E) - Allowed are A,C,D,E,G,L,M,N,O,S,T,V and X */
    /* J : */ 0x00000000, /* ( | | ) - None are allowed */
    /* K : */ 0x00000000, /* ( | | ) - None are allowed */
    /* L : */ 0x00000000, /* ( | | ) - None are allowed */
    /* M : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* N : */ 0x00000000, /* ( | | ) - None are allowed */
    /* O : */ 0x180eb804, /* ( |M|E) - Allowed are C,L,M,N,P,R,S and T */
    /* P : */ 0x08124911, /* ( |M| ) - Allowed are A,E,I,L,O,R and U */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x08104111, /* ( |M| ) - Allowed are A,E,I,O and U */
    /* S : */ 0x1818d111, /* ( |M|E) - Allowed are A,E,I,M,O,P,T and U */
    /* T : */ 0x18124111, /* ( |M|E) - Allowed are A,E,I,O,R and U */
    /* U : */ 0x1826181b, /* ( |M|E) - Allowed are A,B,D,E,L,M,R,S and V */
    /* V : */ 0x00000000, /* ( | | ) - None are allowed */
    /* W : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* X : */ 0x10000000, /* ( | |E) - None are allowed */
    /* Y : */ 0x1c04b857, /* (B|M|E) - Allowed are A,B,C,E,G,L,M,N,P and S */
    /* Z : */ 0x00000000  /* ( | | ) - None are allowed */
  },
  /* Rules following letter Y : */ {
    /* A : */ 0x1c0e3cce, /* (B|M|E) - Allowed are B,C,D,G,H,K,L,M,N,R,S and T */
    /* B : */ 0x0d124911, /* (B|M| ) - Allowed are A,E,I,L,O,R,U and Y */
    /* C : */ 0x1c104d91, /* (B|M|E) - Allowed are A,E,H,I,K,L,O and U */
    /* D : */ 0x18024111, /* ( |M|E) - Allowed are A,E,I,O and R */
    /* E : */ 0x1c0e2809, /* (B|M|E) - Allowed are A,D,L,N,R,S and T */
    /* F : */ 0x18104931, /* ( |M|E) - Allowed are A,E,F,I,L,O and U */
    /* G : */ 0x181279d9, /* ( |M|E) - Allowed are A,D,E,G,H,I,L,M,N,O,R and U */
    /* H : */ 0x08104111, /* ( |M| ) - Allowed are A,E,I,O and U */
    /* I : */ 0x10000000, /* ( | |E) - None are allowed */
    /* J : */ 0x08104091, /* ( |M| ) - Allowed are A,E,H,O and U */
    /* K : */ 0x18004191, /* ( |M|E) - Allowed are A,E,H,I and O */
    /* L : */ 0x1c204911, /* (B|M|E) - Allowed are A,E,I,L,O and V */
    /* M : */ 0x1c10f113, /* (B|M|E) - Allowed are A,B,E,I,M,N,O,P and U */
    /* N : */ 0x180c615d, /* ( |M|E) - Allowed are A,C,D,E,G,I,N,O,S and T */
    /* O : */ 0x1c5efddd, /* (B|M|E) - Allowed are A,C,D,E,G,H,I,K,L,M,N,O,P,R,S,T,U and W */
    /* P : */ 0x1c1e6191, /* (B|M|E) - Allowed are A,E,H,I,N,O,R,S,T and U */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x1d1e6111, /* (B|M|E) - Allowed are A,E,I,N,O,R,S,T,U and Y */
    /* S : */ 0x1c0c4191, /* (B|M|E) - Allowed are A,E,H,I,O,S and T */
    /* T : */ 0x1c084191, /* (B|M|E) - Allowed are A,E,H,I,O and T */
    /* U : */ 0x1c0ebddf, /* (B|M|E) - Allowed are A,B,C,D,E,G,H,I,K,L,M,N,P,R,S and T */
    /* V : */ 0x09004111, /* ( |M| ) - Allowed are A,E,I,O and Y */
    /* W : */ 0x08024191, /* ( |M| ) - Allowed are A,E,H,I,O and R */
    /* X : */ 0x19104111, /* ( |M|E) - Allowed are A,E,I,O,U and Y */
    /* Y : */ 0x10000000, /* ( | |E) - None are allowed */
    /* Z : */ 0x10000000  /* ( | |E) - None are allowed */
  },
  /* Rules following letter Z : */ {
    /* A : */ 0x1c6ebdee, /* (B|M|E) - Allowed are B,C,D,F,G,H,I,K,L,M,N,P,R,S,T,V and W */
    /* B : */ 0x08124111, /* ( |M| ) - Allowed are A,E,I,O,R and U */
    /* C : */ 0x0a104081, /* ( |M| ) - Allowed are A,H,O,U and Z */
    /* D : */ 0x1b326111, /* ( |M|E) - Allowed are A,E,I,N,O,R,U,V,Y and Z */
    /* E : */ 0x1c4e3d08, /* (B|M|E) - Allowed are D,I,K,L,M,N,R,S,T and W */
    /* F : */ 0x08104811, /* ( |M| ) - Allowed are A,E,L,O and U */
    /* G : */ 0x08124111, /* ( |M| ) - Allowed are A,E,I,O,R and U */
    /* H : */ 0x0c146519, /* (B|M| ) - Allowed are A,D,E,I,K,N,O,S and U */
    /* I : */ 0x1c0efc5d, /* (B|M|E) - Allowed are A,C,D,E,G,K,L,M,N,O,P,R,S and T */
    /* J : */ 0x00000000, /* ( | | ) - None are allowed */
    /* K : */ 0x09004111, /* ( |M| ) - Allowed are A,E,I,O and Y */
    /* L : */ 0x19004111, /* ( |M|E) - Allowed are A,E,I,O and Y */
    /* M : */ 0x18000111, /* ( |M|E) - Allowed are A,E and I */
    /* N : */ 0x09004111, /* ( |M| ) - Allowed are A,E,I,O and Y */
    /* O : */ 0x1c5ef941, /* (B|M|E) - Allowed are A,G,I,L,M,N,O,P,R,S,T,U and W */
    /* P : */ 0x08004011, /* ( |M| ) - Allowed are A,E and O */
    /* Q : */ 0x00000000, /* ( | | ) - None are allowed */
    /* R : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* S : */ 0x18184505, /* ( |M|E) - Allowed are A,C,I,K,O,T and U */
    /* T : */ 0x08064991, /* ( |M| ) - Allowed are A,E,H,I,L,O,R and S */
    /* U : */ 0x1c0ebc57, /* (B|M|E) - Allowed are A,B,C,E,G,K,L,M,N,P,R,S and T */
    /* V : */ 0x08004111, /* ( |M| ) - Allowed are A,E,I and O */
    /* W : */ 0x0c004111, /* (B|M| ) - Allowed are A,E,I and O */
    /* X : */ 0x00000000, /* ( | | ) - None are allowed */
    /* Y : */ 0x1c443c46, /* (B|M|E) - Allowed are B,C,G,K,L,M,N,S and W */
    /* Z : */ 0x10000000  /* ( | |E) - None are allowed */
  }
};

} // namespace NameConstants

} // namespace pwx

/** @internal
  * @file CRandomWordConstants.h
  *
  @verbatim
  * --- Information about this generated file --- *
  * --------------------------------------------- *
  * This file has been generated by analyzing     *
  * text files for (hopefully) valid words which  *
  * then got processed.                           *
  * First those words got split into their        *
  * characters, and these character sequences     *
  * got recorded.                                 *
  * Sequences with very little occurrences are    *
  * filtered out, as they might indicate typos.   *
  * All character occurrences are recorded as     *
  * well, and the resulting consonant and vowel   *
  * strings have then been generated by randomly  *
  * mixing those characters randomly. Due to this *
  * mixing the program will produce different     *
  * strings every time it is called, even with    *
  * the same text files!                          *
  * --------------------------------------------- *
  * Q: Why are all those constants mixed case?    *
  *    They should be all uppercase!              *
  * A: Yes, if they where defines or in any other *
  *    way global. But here they have their own   *
  *    namespace and can have readable names.     *
  * --------------------------------------------- *
  * Start: 19.11.2010 23:16:20                    *
  * Stop : 19.11.2010 23:26:07                    *
  * Duration:    0 h,  9 m, 47 s                  *
  * --------------------------------------------- *
  * textAnalyzer, (c) Sven Eden, PrydeWorX 2010-12*
  @endverbatim
**/

#endif // PWX_LIBPWX_PWX_INTERNAL_CRANDOMWORDCONSTANTS_H_INCLUDED

