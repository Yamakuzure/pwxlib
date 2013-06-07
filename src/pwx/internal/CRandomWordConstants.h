#pragma once
#ifndef PWX_LIBPWX_PWX_INTERNAL_CRANDOMWORDCONSTANTS_H_INCLUDED
#define PWX_LIBPWX_PWX_INTERNAL_CRANDOMWORDCONSTANTS_H_INCLUDED 1

/** @file CRandomWordConstants.h
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
**/	


namespace pwx {

/** @namespace pwx::NameConstants
  *
  * @brief constants for usage by the name generator of CRandom
  *
  * @see pwx for the namespace surrounding this
  * @see pwx::constants for general constants used by pwxLib
**/
namespace NameConstants {

/** --- constants for generation states --- **/

/* --- General states--- */
const uint32_t genFinished    = 0x00000000; //!< The generation has been finished

/* --- rounds in generation--- */
const uint32_t genRoundA      = 0x00000010; //!< The first character has to be found
const uint32_t genRoundB      = 0x00000020; //!< and the second
const uint32_t genRoundC      = 0x00000040; //!< and the third
const uint32_t genRoundD      = 0x00000080; //!< and finally the fourth

/* --- position definitions--- */
const uint32_t genPartStart   = 0x00000100; //!< The next character starts a new part
const uint32_t genPartEnd     = 0x00000200; //!< The current part is ended
const uint32_t genSyllEnd     = 0x00000400; //!< The current syllable is finished

/* --- some states for character generation--- */
const uint32_t genLastIsCon   = 0x00001000; //!< The last character was a consonant
const uint32_t genLastIsVow   = 0x00002000; //!< The last character was a vowel
const uint32_t genNextIsCon   = 0x00004000; //!< The next character should be a consonant
const uint32_t genNextIsVow   = 0x00008000; //!< The next character should be a vowel
const uint32_t genHasNextChar = 0x00010000; //!< The current round got a character!

/* --- constants for character position allowance --- */
const uint32_t genCharMask    = 0x1fffffff; //!< A mask to filter the position out of the FUM
const uint32_t genStartAllow  = 0x20000000; //!< A 2-character-combination is allowed to start a part
const uint32_t genMiddleAllow = 0x40000000; //!< A 2-character-combination is allowed to be in the middle of a part
const uint32_t genEndAllow    = 0x80000000; //!< A 2-character-combination is allowed to end a part

/* --- constants for character to index offsets --- */
const uint32_t chrOffsetLowStart = static_cast<uint32_t>('a');            // 0x61
const uint32_t chrOffsetLowEnd   = static_cast<uint32_t>('z');            // 0x7a
const uint32_t chrOffsetUppStart = static_cast<uint32_t>('A');            // 0x41
const uint32_t chrOffsetUppEnd   = static_cast<uint32_t>('Z');            // 0x5a
const uint32_t chrOffsetDown     = chrOffsetLowStart - chrOffsetUppStart; // 0x20

/* --- constants for the umlaut indexes --- */
const int32_t chrIndexUmlautA = 26;
const int32_t chrIndexUmlautO = 27;
const int32_t chrIndexUmlautU = 28;

/* --- macros to check for umlauts and return the  ---
   --- appropriate index of a lower case character --- */
#define IS_UMLAUT_A(x) ( static_cast<uint8_t>(x) == 0xe4 )
#define IS_UMLAUT_O(x) ( static_cast<uint8_t>(x) == 0xf6 )
#define IS_UMLAUT_U(x) ( static_cast<uint8_t>(x) == 0xfc )

// Return the index of a lower case character
#define FUM_IDX(x) \
	( ( static_cast<uint8_t>(x) <= NameConstants::chrOffsetLowEnd) \
	? (static_cast<uint8_t>(x) - NameConstants::chrOffsetLowStart) \
	: IS_UMLAUT_A(x) ? NameConstants::chrIndexUmlautA \
	: IS_UMLAUT_O(x) ? NameConstants::chrIndexUmlautO \
	: IS_UMLAUT_U(x) ? NameConstants::chrIndexUmlautU \
	: -1 )

// Return the rule of the FUM using two lower case characters
#define FUM_CHR_RULE(type, chOne, chTwo)  \
	NameConstants::nameFUM[(int)type][FUM_IDX(chOne)][FUM_IDX(chTwo)]

// Return the rule of the FUM using two indexes
#define FUM_IDX_RULE(type, first, second) \
	NameConstants::nameFUM[(int)type][first][second]

// Return true if the combination is allowed at the start of a part
#define FUM_ALLOW_START(type, chOne, chTwo) \
	(NameConstants::genStartAllow & FUM_CHR_RULE(type, chOne, chTwo))

// Return true if the combination is allowed in the middle of a part
#define FUM_ALLOW_MIDDLE(type, chOne, chTwo) \
	(NameConstants::genMiddleAllow & FUM_CHR_RULE(type, chOne, chTwo))

// Return true if the combination is allowed at the end of a part
#define FUM_ALLOW_END(type, chOne, chTwo) \
	(NameConstants::genEndAllow & FUM_CHR_RULE(type, chOne, chTwo))

// Return true if the combination does not allow a following character, false otherwise
#define FUM_MUST_FINISH(type, chOne, chTwo) \
	(0 == (NameConstants::genCharMask & FUM_CHR_RULE(type, chOne, chTwo))) 

// Return typed length of the consonant array
#define CL_LEN(type) NameConstants::conListLen[(int)type]

// Return index modded character of the consonant array
#define CL_CHR(type, idx) NameConstants::conList[(int)type][(idx) % CL_LEN(type)]

// Return typed length of the vowel array
#define VL_LEN(type) NameConstants::vowListLen[(int)type]

// Return index modded character of the vowel array
#define VL_CHR(type, idx) NameConstants::vowList[(int)type][(idx) % VL_LEN(type)]

/** @file CRandomWordConstants.h
  @verbatim
  * ==== Automatic Text Analyzation Result from PrydeWorX textAnalyzer.pl  ==== *
  *=============================================================================*
  * -- The following data has been extracted by analyzing                    -- *
  * --      1 names files in german language.                                -- *
  * --      8 names files in english language.                               -- *
  * --    729 texts files in german language.                                -- *
  * --   2975 texts files in english language.                               -- *
  * -- Number of text files that have been analyzed:           173           -- *
  * -- Number of words in all of these files       :       5513833           -- *
  * --                  names / german             :         13224           -- *
  * --                  names / english            :          9640           -- *
  * --                  texts / german             :        158900           -- *
  * --                  texts / english            :       5326506           -- *
  * -- Number of unique words found                :        263892           -- *
  * --                  names / german             :         13223           -- *
  * --                  names / english            :          8025           -- *
  * --                  texts / german             :        140564           -- *
  * --                  texts / english            :         84353           -- *
  * -- Number of names from name list files        :         21282           -- *
  * -- Number of ignored words (illegal content)   :          2925           -- *
  * -- Resulting number of analyzed words          :        260967           -- *
  * -- ( ==>         41 E-Mail addresses have been filtered out)             -- *
  * -- ( ==>         30 Internet addresses have been filtered out)           -- *
  * -- ( ==>          9 File paths have been filtered out)                   -- *
  * -- ( ==>          9 Triple Threats have been filtered out)               -- *
  * -- ( ==>         12 Roman numerals have been filtered out)               -- *
  * -- ( ==>       2828 times Captain Caps stroke back)                      -- *
  * -- ( ==>          5 Weird letter combinations have been filtered out)    -- *
  * -- ( ==>        246 CamelCase words have been split)                     -- *
  * --------------------------------------------------------------------------- *
  * Top Hundred statistics - The most and fewest words that got accepted or denied: *
  * Hundred most often found accepted words  | Hundred most often found denied words    *
  * -------------------------------------+------------------------------------- *
  * ----                        Language de                                ---- *
  * -------------------------------------+------------------------------------- *
  *   1.:                und (      721) |   1.:                 lt (       43) *
  *   2.:                der (      566) |   2.:                 zs (       16) *
  *   3.:                die (      462) |   3.:               jaja (       11) *
  *   4.:                ich (      295) |   4.:               gehn (       10) *
  *   5.:                 er (      289) |   5.:               mußt (        7) *
  *   6.:                den (      276) |   6.:               laßt (        7) *
  *   7.:                ein (      245) |   7.:             schloß (        7) *
  *   8.:                das (      242) |   8.:              mutze (        6) *
  *   9.:                 in (      235) |   9.:               date (        5) *
  *  10.:                von (      220) |  10.:             flasch (        5) *
  *  11.:                 es (      216) |  11.:            subject (        4) *
  *  12.:                sie (      202) |  12.:                 aß (        4) *
  *  13.:                 zu (      195) |  13.:              kömmt (        4) *
  *  14.:                 da (      194) |  14.:                 hu (        4) *
  *  15.:              nicht (      173) |  15.:            cadfael (        4) *
  *  16.:                ist (      170) |  16.:                 ge (        4) *
  *  17.:                war (      170) |  17.:               sent (        4) *
  *  18.:                 so (      155) |  18.:             miezel (        4) *
  *  19.:                auf (      139) |  19.:               läßt (        4) *
  *  20.:                mit (      136) |  20.:               gass (        3) *
  *  21.:               sich (      136) |  21.:            brucken (        3) *
  *  22.:                wie (      127) |  22.:              mußte (        3) *
  *  23.:                 im (      111) |  23.:                 ho (        3) *
  *  24.:                 du (      107) |  24.:                 tu (        3) *
  *  25.:                dem (      106) |  25.:               that (        3) *
  *  26.:                ihm (       96) |  26.:              hazzi (        3) *
  *  27.:                hat (       95) |  27.:               faßt (        2) *
  *  28.:                 an (       93) |  28.:                 my (        2) *
  *  29.:               mich (       93) |  29.:             mußten (        2) *
  *  30.:                mir (       92) |  30.:              hurre (        2) *
  *  31.:                aus (       84) |  31.:            kikriki (        2) *
  *  32.:                was (       82) |  32.:           dämmrung (        2) *
  *  33.:               list (       80) |  33.:               fluß (        2) *
  *  34.:        verzeichnis (       80) |  34.:              hopßa (        2) *
  *  35.:        verfügbaren (       80) |  35.:                 ca (        2) *
  *  36.:                der (       80) |  36.:               zgen (        2) *
  *  37.:           gedichte (       80) |  37.:                 sa (        2) *
  *  38.:                des (       72) |  38.:              frißt (        2) *
  *  39.:               sein (       71) |  39.:             stundz (        2) *
  *  40.:                ihr (       70) |  40.:             gesehn (        2) *
  *  41.:               noch (       70) |  41.:                 he (        2) *
  *  42.:               nach (       69) |  42.:               zres (        2) *
  *  43.:               doch (       65) |  43.:                pst (        2) *
  *  44.:                als (       59) |  44.:                 it (        2) *
  *  45.:               auch (       58) |  45.:                 zn (        2) *
  *  46.:                 um (       57) |  46.:          erdenkloß (        2) *
  *  47.:                ihn (       55) |  47.:              tlein (        2) *
  *  48.:               eine (       53) |  48.:               bühn (        2) *
  *  49.:               ging (       53) |  49.:           geschehn (        2) *
  *  50.:               mein (       53) |  50.:              schuß (        2) *
  *  51.:               will (       53) |  51.:                 su (        1) *
  *  52.:               wenn (       52) |  52.:                oya (        1) *
  *  53.:                zum (       52) |  53.:                 le (        1) *
  *  54.:                 am (       49) |  54.:                 ok (        1) *
  *  55.:                man (       49) |  55.:                liv (        1) *
  *  56.:         eingesandt (       48) |  56.:                 jo (        1) *
  *  57.:            deutsch (       47) |  57.:                yui (        1) *
  *  58.:                 ja (       46) |  58.:                 ib (        1) *
  *  59.:               mann (       45) |  59.:                yao (        1) *
  *  60.:                nur (       43) |  60.:                 re (        1) *
  *  61.:                nun (       40) |  61.:                 ly (        1) *
  *  62.:               alte (       39) |  62.:                lrs (        1) *
  *  63.:              durch (       39) |  63.:                 ed (        1) *
  *  64.:                daß (       38) |  64.:                 uz (        1) *
  *  65.:                vor (       36) |  65.:                 el (        1) *
  *  66.:                 wo (       36) |  66.:                 lu (        1) *
  *  67.:        oberförster (       35) |  67.:                 sp (        1) *
  *  68.:             einmal (       33) |  68.:               fuss (        1) *
  *  69.:               sind (       33) |  69.:          reichthum (        1) *
  *  70.:               dich (       32) |  70.:                 iv (        1) *
  *  71.:               gott (       32) |  71.:               what (        1) *
  *  72.:                bei (       31) |  72.:             jaajaa (        1) *
  *  73.:               dein (       31) |  73.:               tuhl (        1) *
  *  74.:               denn (       31) |  74.:               asia (        1) *
  *  75.:                gar (       31) |  75.:             gewußt (        1) *
  *  76.:             sprach (       31) |  76.:      klinglingling (        1) *
  *  77.:               wird (       31) |  77.:                 af (        1) *
  *  78.:              einen (       30) |  78.:              cramm (        1) *
  *  79.:               hier (       30) |  79.:             ritsch (        1) *
  *  80.:                hin (       30) |  80.:             schooß (        1) *
  *  81.:              nacht (       30) |  81.:           gräßlich (        1) *
  *  82.:               wohl (       30) |  82.:               späß (        1) *
  *  83.:                dir (       29) |  83.:                iii (        1) *
  *  84.:               kann (       29) |  84.:             claque (        1) *
  *  85.:              schon (       29) |  85.:                 iq (        1) *
  *  86.:                wir (       29) |  86.:                 ai (        1) *
  *  87.:                für (       28) |  87.:                 uv (        1) *
  *  88.:              seele (       28) |  88.:             wishes (        1) *
  *  89.:               welt (       28) |  89.:                 ma (        1) *
  *  90.:             wollte (       28) |  90.:              wusst (        1) *
  *  91.:                bin (       27) |  91.:                 re (        1) *
  *  92.:               kind (       27) |  92.:                 me (        1) *
  *  93.:               dort (       26) |  93.:      totgeschoßner (        1) *
  *  94.:               fort (       26) |  94.:              fühlz (        1) *
  *  95.:                ins (       26) |  95.:          wirktlich (        1) *
  *  96.:             weiter (       26) |  96.:            tleiner (        1) *
  *  97.:             wieder (       26) |  97.:                div (        1) *
  *  98.:               dann (       25) |  98.:                 eß (        1) *
  *  99.:               herr (       25) |  99.:       vitzlibutzli (        1) *
  *  100.:               herz (       25) |  100.:                 ii (        1) *
  * ----                        Language en                                ---- *
  * -------------------------------------+------------------------------------- *
  *   1.:                the (   357540) |   1.:                 oh (     1675) *
  *   2.:                and (   182692) |   2.:              korak (      914) *
  *   3.:                 of (   176954) |   3.:                 ma (      721) *
  *   4.:                 to (   139173) |   4.:                 ah (      666) *
  *   5.:                 in (    96826) |   5.:                 ha (      658) *
  *   6.:                 is (    81993) |   6.:             werper (      630) *
  *   7.:               that (    80504) |   7.:             jeddak (      626) *
  *   8.:                 it (    66054) |   8.:               akut (      448) *
  *   9.:                 he (    65613) |   9.:              teeka (      434) *
  *  10.:                his (    57693) |  10.:              shang (      401) *
  *  11.:                was (    55808) |  11.:             sheeta (      390) *
  *  12.:                for (    48257) |  12.:             ptarth (      362) *
  *  13.:                 as (    47119) |  13.:              sikes (      349) *
  *  14.:               with (    44199) |  14.:                 la (      338) *
  *  15.:                not (    43106) |  15.:          murdstone (      322) *
  *  16.:                had (    41339) |  16.:          paulvitch (      318) *
  *  17.:                but (    34909) |  17.:                 eh (      286) *
  *  18.:                 be (    34888) |  18.:             mbonga (      270) *
  *  19.:                you (    34535) |  19.:                 de (      263) *
  *  20.:                 at (    32076) |  20.:                 ay (      251) *
  *  21.:                her (    30813) |  21.:             waziri (      250) *
  *  22.:              which (    28794) |  22.:              bwana (      236) *
  *  23.:               have (    27610) |  23.:              kulan (      231) *
  *  24.:               they (    26454) |  24.:             kantos (      214) *
  *  25.:               from (    25912) |  25.:                 em (      199) *
  *  26.:                him (    25578) |  26.:              etext (      195) *
  *  27.:                 by (    25426) |  27.:               dihn (      193) *
  *  28.:               this (    24835) |  28.:                 ye (      182) *
  *  29.:                are (    24509) |  29.:              coude (      172) *
  *  30.:                 on (    23502) |  30.:              sabor (      170) *
  *  31.:                 my (    23337) |  31.:             etexts (      157) *
  *  32.:                 or (    22819) |  32.:              thoat (      146) *
  *  33.:                she (    21486) |  33.:            jeddaks (      144) *
  *  34.:                all (    21229) |  34.:             thoats (      130) *
  *  35.:                one (    21136) |  35.:               ghak (      126) *
  *  36.:                 so (    20584) |  36.:              hajus (      124) *
  *  37.:               said (    20415) |  37.:              omean (      117) *
  *  38.:              there (    19425) |  38.:           serjeant (      115) *
  *  39.:               upon (    18892) |  39.:              komak (      112) *
  *  40.:               were (    18637) |  40.:          bududreen (      111) *
  *  41.:                 if (    18504) |  41.:         twinkleton (      109) *
  *  42.:                 we (    18457) |  42.:            gernois (      106) *
  *  43.:                 me (    18356) |  43.:                 ja (      106) *
  *  44.:                 no (    17273) |  44.:             feenix (      104) *
  *  45.:             mister (    17133) |  45.:             terkoz (      102) *
  *  46.:              their (    16737) |  46.:            kovudoo (      102) *
  *  47.:               when (    16674) |  47.:               boma (      100) *
  *  48.:               them (    16433) |  48.:               goro (       98) *
  *  49.:              would (    15558) |  49.:               toog (       96) *
  *  50.:                 an (    15455) |  50.:                 cd (       94) *
  *  51.:               what (    14852) |  51.:             ninaka (       94) *
  *  52.:               been (    14471) |  52.:          evremonde (       92) *
  *  53.:                who (    13831) |  53.:              geeka (       90) *
  *  54.:               will (    13114) |  54.:               kega (       88) *
  *  55.:               then (    12871) |  55.:              korus (       85) *
  *  56.:                man (    12858) |  56.:            nupkins (       79) *
  *  57.:               into (    12235) |  57.:              drood (       79) *
  *  58.:              could (    11287) |  58.:            kulonga (       78) *
  *  59.:               some (    11259) |  59.:                 wi (       77) *
  *  60.:               more (    10863) |  60.:              chulk (       76) *
  *  61.:              other (    10770) |  61.:                 ee (       74) *
  *  62.:                out (    10722) |  62.:             taglat (       72) *
  *  63.:                now (    10116) |  63.:              horta (       72) *
  *  64.:                 do (    10007) |  64.:              kosis (       70) *
  *  65.:                 up (     9818) |  65.:           ganwazam (       70) *
  *  66.:               than (     9766) |  66.:                 un (       68) *
  *  67.:             little (     9590) |  67.:             ptomel (       68) *
  *  68.:               very (     9550) |  68.:              hooja (       68) *
  *  69.:               time (     9491) |  69.:                 th (       67) *
  *  70.:                its (     9440) |  70.:             vastus (       67) *
  *  71.:                has (     9277) |  71.:               manu (       66) *
  *  72.:              these (     8873) |  72.:              komal (       64) *
  *  73.:             before (     8834) |  73.:           aaanthor (       64) *
  *  74.:              great (     8351) |  74.:               talu (       64) *
  *  75.:                any (     8051) |  75.:                 dr (       63) *
  *  76.:               such (     7947) |  76.:                 ii (       63) *
  *  77.:             should (     7770) |  77.:              saada (       62) *
  *  78.:              about (     7750) |  78.:              heerd (       60) *
  *  79.:                two (     7689) |  79.:                 en (       58) *
  *  80.:              again (     7653) |  80.:                 ad (       58) *
  *  81.:               must (     7623) |  81.:              gunto (       58) *
  *  82.:               your (     7485) |  82.:        impossibile (       55) *
  *  83.:               only (     7261) |  83.:            thquire (       53) *
  *  84.:              being (     7174) |  84.:                 yo (       50) *
  *  85.:                did (     7057) |  85.:            smangle (       50) *
  *  86.:               same (     7032) |  86.:           hekkador (       49) *
  *  87.:               down (     6821) |  87.:             buzfuz (       48) *
  *  88.:                old (     6777) |  88.:            bazzard (       48) *
  *  89.:              first (     6616) |  89.:                 ud (       45) *
  *  90.:                way (     6433) |  90.:             shador (       44) *
  *  91.:               know (     6431) |  91.:           frecoult (       44) *
  *  92.:                may (     6427) |  92.:                 mc (       44) *
  *  93.:               come (     6412) |  93.:           littimer (       39) *
  *  94.:               like (     6407) |  94.:                 ho (       38) *
  *  95.:             tarzan (     6381) |  95.:            mcimail (       37) *
  *  96.:                 am (     6325) |  96.:             foulon (       36) *
  *  97.:              where (     6312) |  97.:                 wy (       36) *
  *  98.:                can (     6214) |  98.:              kamak (       36) *
  *  99.:               good (     6130) |  99.:            fuwalda (       34) *
  *  100.:            through (     6088) |  100.:              ooman (       33) *
  * -------------------------------------+------------------------------------- *
  * Hundred fewest found accepted words      | Hundred fewest found denied words        *
  * -------------------------------------+------------------------------------- *
  * ----                        Language de                                ---- *
  * -------------------------------------+------------------------------------- *
  *   1.:              aaron (        1) |   1.:                 su (        1) *
  *   2.:               achi (        1) |   2.:                oya (        1) *
  *   3.:              achim (        1) |   3.:                 le (        1) *
  *   4.:           adalbert (        1) |   4.:                 ok (        1) *
  *   5.:          adalberta (        1) |   5.:                liv (        1) *
  *   6.:             adalia (        1) |   6.:                 jo (        1) *
  *   7.:               adam (        1) |   7.:                yui (        1) *
  *   8.:              adama (        1) |   8.:                 ib (        1) *
  *   9.:           adelbert (        1) |   9.:                yao (        1) *
  *  10.:              adele (        1) |  10.:                 re (        1) *
  *  11.:          adelfried (        1) |  11.:                 ly (        1) *
  *  12.:           adelgard (        1) |  12.:                lrs (        1) *
  *  13.:           adelgund (        1) |  13.:                 ed (        1) *
  *  14.:          adelgunde (        1) |  14.:                 uz (        1) *
  *  15.:           adelhaid (        1) |  15.:                 el (        1) *
  *  16.:           adelheid (        1) |  16.:                 lu (        1) *
  *  17.:           adelheit (        1) |  17.:                 sp (        1) *
  *  18.:             adelia (        1) |  18.:               fuss (        1) *
  *  19.:            adelina (        1) |  19.:          reichthum (        1) *
  *  20.:           adelinde (        1) |  20.:                 iv (        1) *
  *  21.:            adeline (        1) |  21.:               what (        1) *
  *  22.:            adelmar (        1) |  22.:             jaajaa (        1) *
  *  23.:          adeltraud (        1) |  23.:               tuhl (        1) *
  *  24.:                adi (        1) |  24.:               asia (        1) *
  *  25.:              adolf (        1) |  25.:             gewußt (        1) *
  *  26.:             adrian (        1) |  26.:      klinglingling (        1) *
  *  27.:            adriana (        1) |  27.:                 af (        1) *
  *  28.:            adriane (        1) |  28.:              cramm (        1) *
  *  29.:           adrianna (        1) |  29.:             ritsch (        1) *
  *  30.:           adrianne (        1) |  30.:             schooß (        1) *
  *  31.:            adriano (        1) |  31.:           gräßlich (        1) *
  *  32.:           adrianus (        1) |  32.:               späß (        1) *
  *  33.:            aganeta (        1) |  33.:                iii (        1) *
  *  34.:              agata (        1) |  34.:             claque (        1) *
  *  35.:              agate (        1) |  35.:                 iq (        1) *
  *  36.:             agatha (        1) |  36.:                 ai (        1) *
  *  37.:             agathe (        1) |  37.:                 uv (        1) *
  *  38.:              agnes (        1) |  38.:             wishes (        1) *
  *  39.:            agnetha (        1) |  39.:                 ma (        1) *
  *  40.:              agnis (        1) |  40.:              wusst (        1) *
  *  41.:            agustin (        1) |  41.:                 re (        1) *
  *  42.:           agustina (        1) |  42.:                 me (        1) *
  *  43.:             aharon (        1) |  43.:      totgeschoßner (        1) *
  *  44.:            ahlbert (        1) |  44.:              fühlz (        1) *
  *  45.:            ahlrich (        1) |  45.:          wirktlich (        1) *
  *  46.:               aida (        1) |  46.:            tleiner (        1) *
  *  47.:               aila (        1) |  47.:                div (        1) *
  *  48.:              alain (        1) |  48.:                 eß (        1) *
  *  49.:               alan (        1) |  49.:       vitzlibutzli (        1) *
  *  50.:               alba (        1) |  50.:                 ii (        1) *
  *  51.:             albert (        1) |  51.:              means (        1) *
  *  52.:            alberta (        1) |  52.:             ehrnen (        1) *
  *  53.:          albertina (        1) |  53.:               bäum (        1) *
  *  54.:          albertine (        1) |  54.:          slüzzelin (        1) *
  *  55.:           albrecht (        1) |  55.:          juchheija (        1) *
  *  56.:             aldina (        1) |  56.:                 eo (        1) *
  *  57.:             aldine (        1) |  57.:    vergißmeinnicht (        1) *
  *  58.:               alea (        1) |  58.:                 ah (        1) *
  *  59.:         aleksandar (        1) |  59.:               know (        1) *
  *  60.:         aleksander (        1) |  60.:        schusterbaß (        1) *
  *  61.:         aleksandra (        1) |  61.:              eimal (        1) *
  *  62.:              alena (        1) |  62.:             menßen (        1) *
  *  63.:             alenka (        1) |  63.:              malör (        1) *
  *  64.:         alessandra (        1) |  64.:               freu (        1) *
  *  65.:         alessandro (        1) |  65.:              elsaß (        1) *
  *  66.:               alex (        1) |  66.:             fürcht (        1) *
  *  67.:              alexa (        1) |  67.:           unbewußt (        1) *
  *  68.:          alexander (        1) |  68.:                 al (        1) *
  *  69.:          alexandra (        1) |  69.:              hoppß (        1) *
  *  70.:                alf (        1) |  70.:             trohnt (        1) *
  *  71.:             alfons (        1) |  71.:             really (        1) *
  *  72.:             alfred (        1) |  72.:            funfzig (        1) *
  *  73.:             alfrid (        1) |  73.:               bubu (        1) *
  *  74.:            alfried (        1) |  74.:                 dr (        1) *
  *  75.:              alida (        1) |  75.:              tuhle (        1) *
  *  76.:               alma (        1) |  76.:           verlaßne (        1) *
  *  77.:             almida (        1) |  77.:               gluh (        1) *
  *  78.:              almin (        1) |  78.:              gilet (        1) *
  *  79.:              alois (        1) |  79.:             sauerm (        1) *
  *  80.:           altfried (        1) |  80.:          beslozzen (        1) *
  *  81.:            amadeus (        1) |  81.:                 et (        1) *
  *  82.:             amanda (        1) |  82.:               told (        1) *
  *  83.:              amber (        1) |  83.:          vergeßner (        1) *
  *  84.:              ameli (        1) |  84.:                 un (        1) *
  *  85.:             amelia (        1) |  85.:        sterbehernd (        1) *
  *  86.:               amin (        1) |  86.:               best (        1) *
  *  87.:              amina (        1) |  87.:             tnicks (        1) *
  *  88.:               amir (        1) |  88.:              haißt (        1) *
  *  89.:                ana (        1) |  89.:            verdruß (        1) *
  *  90.:             anabel (        1) |  90.:                 ex (        1) *
  *  91.:          anastasia (        1) |  91.:            labekuß (        1) *
  *  92.:               anca (        1) |  92.:                 ar (        1) *
  *  93.:               andi (        1) |  93.:          flüchtger (        1) *
  *  94.:              andre (        1) |  94.:           hinieden (        1) *
  *  95.:             andrea (        1) |  95.:                 nu (        1) *
  *  96.:            andreas (        1) |  96.:           hopßassa (        1) *
  *  97.:           aneliese (        1) |  97.:             haßten (        1) *
  *  98.:           anellise (        1) |  98.:             thätig (        1) *
  *  99.:           anemarie (        1) |  99.:             könnts (        1) *
  *  100.:             anette (        1) |  100.:                 hm (        1) *
  * ----                        Language en                                ---- *
  * -------------------------------------+------------------------------------- *
  *   1.:              aaric (        1) |   1.:                 di (        1) *
  *   2.:            abagail (        1) |   2.:                 es (        1) *
  *   3.:              abbey (        1) |   3.:                viv (        1) *
  *   4.:           abbigail (        1) |   4.:                 os (        1) *
  *   5.:             abbott (        1) |   5.:                 la (        1) *
  *   6.:             abdiel (        1) |   6.:                 hy (        1) *
  *   7.:           abdullah (        1) |   7.:                 au (        1) *
  *   8.:           abercorn (        1) |   8.:                 vi (        1) *
  *   9.:           aberdeen (        1) |   9.:                 ra (        1) *
  *  10.:      aberdeenshire (        1) |  10.:                 co (        1) *
  *  11.:            abigale (        1) |  11.:                 bo (        1) *
  *  12.:           abigayle (        1) |  12.:                 em (        1) *
  *  13.:            abiquiu (        1) |  13.:                 ed (        1) *
  *  14.:              abner (        1) |  14.:                 el (        1) *
  *  15.:              abram (        1) |  15.:                 lu (        1) *
  *  16.:              abril (        1) |  16.:            faithfo (        1) *
  *  17.:            acequia (        1) |  17.:            poltics (        1) *
  *  18.:               acer (        1) |  18.:        indiwiduals (        1) *
  *  19.:           achilles (        1) |  19.:             hordit (        1) *
  *  20.:           ackerley (        1) |  20.:           immeshed (        1) *
  *  21.:             ackley (        1) |  21.:           dewoured (        1) *
  *  22.:                ada (        1) |  22.:         excietment (        1) *
  *  23.:         adalbeorht (        1) |  23.:            advithe (        1) *
  *  24.:        adalbrechta (        1) |  24.:               elab (        1) *
  *  25.:            adaline (        1) |  25.:            imbibin (        1) *
  *  26.:            adalson (        1) |  26.:            wownded (        1) *
  *  27.:            adamson (        1) |  27.:              wynds (        1) *
  *  28.:               adan (        1) |  28.:          langvidge (        1) *
  *  29.:               adda (        1) |  29.:             awmost (        1) *
  *  30.:           addaneye (        1) |  30.:            swarrys (        1) *
  *  31.:              addie (        1) |  31.:              upnor (        1) *
  *  32.:              addis (        1) |  32.:               fogo (        1) *
  *  33.:             addney (        1) |  33.:           ciations (        1) *
  *  34.:               addy (        1) |  34.:          compiegne (        1) *
  *  35.:            addyson (        1) |  35.:              aimin (        1) *
  *  36.:              adela (        1) |  36.:          pwettiest (        1) *
  *  37.:           adelaide (        1) |  37.:              erage (        1) *
  *  38.:              adele (        1) |  38.:          disappint (        1) *
  *  39.:             adelia (        1) |  39.:        rheumaticks (        1) *
  *  40.:            adelina (        1) |  40.:               tost (        1) *
  *  41.:               aden (        1) |  41.:            veather (        1) *
  *  42.:               adia (        1) |  42.:            velcome (        1) *
  *  43.:               adie (        1) |  43.:        aggerawated (        1) *
  *  44.:             aditya (        1) |  44.:              wimin (        1) *
  *  45.:              adken (        1) |  45.:     montefiaschone (        1) *
  *  46.:             adkins (        1) |  46.:               wush (        1) *
  *  47.:              adkyn (        1) |  47.:              ahind (        1) *
  *  48.:              adler (        1) |  48.:          solitoode (        1) *
  *  49.:              adley (        1) |  49.:         wurshippin (        1) *
  *  50.:              adney (        1) |  50.:        conwenienth (        1) *
  *  51.:               adny (        1) |  51.:            idquire (        1) *
  *  52.:             adolfo (        1) |  52.:              vevey (        1) *
  *  53.:             adonis (        1) |  53.:           farevell (        1) *
  *  54.:           adrianna (        1) |  54.:            onquiet (        1) *
  *  55.:             adriel (        1) |  55.:            couldst (        1) *
  *  56.:             adrion (        1) |  56.:             tooley (        1) *
  *  57.:              adron (        1) |  57.:             aitlon (        1) *
  *  58.:                ady (        1) |  58.:     handerkerchief (        1) *
  *  59.:         aeccestane (        1) |  59.:                div (        1) *
  *  60.:              aedan (        1) |  60.:         aggrawated (        1) *
  *  61.:         aegelmaere (        1) |  61.:         hextricate (        1) *
  *  62.:         aegelweard (        1) |  62.:               haec (        1) *
  *  63.:              aeker (        1) |  63.:               vuns (        1) *
  *  64.:           aekerley (        1) |  64.:               nver (        1) *
  *  65.:           aekerman (        1) |  65.:            fiesole (        1) *
  *  66.:             aekley (        1) |  66.:              powis (        1) *
  *  67.:             aeldra (        1) |  67.:              yahip (        1) *
  *  68.:           aelfdane (        1) |  68.:      tootleumboots (        1) *
  *  69.:           aelfdene (        1) |  69.:               lmas (        1) *
  *  70.:           aelfraed (        1) |  70.:               arsh (        1) *
  *  71.:            aelfric (        1) |  71.:           anoother (        1) *
  *  72.:           aelfwine (        1) |  72.:             thwear (        1) *
  *  73.:          aenedleah (        1) |  73.:              ither (        1) *
  *  74.:          aenescumb (        1) |  74.:               tcha (        1) *
  *  75.:            aerwyna (        1) |  75.:          subsekent (        1) *
  *  76.:             aescby (        1) |  76.:         vauntingly (        1) *
  *  77.:           aescford (        1) |  77.:             guvner (        1) *
  *  78.:           aescleah (        1) |  78.:            fellars (        1) *
  *  79.:            aesclin (        1) |  79.:             thtout (        1) *
  *  80.:            aesctun (        1) |  80.:           shouldst (        1) *
  *  81.:           aescwine (        1) |  81.:          arthmetic (        1) *
  *  82.:            aescwyn (        1) |  82.:          sprouters (        1) *
  *  83.:          aesoburne (        1) |  83.:              lotht (        1) *
  *  84.:       aethelbeorht (        1) |  84.:            rewerse (        1) *
  *  85.:        aethelbeorn (        1) |  85.:             studii (        1) *
  *  86.:        aethelberht (        1) |  86.:              andrh (        1) *
  *  87.:         aethelhard (        1) |  87.:          appearnce (        1) *
  *  88.:        aethelisdun (        1) |  88.:            bizagno (        1) *
  *  89.:         aethelmaer (        1) |  89.:                 pu (        1) *
  *  90.:        aethelmaere (        1) |  90.:             ischia (        1) *
  *  91.:         aethelreda (        1) |  91.:           komazein (        1) *
  *  92.:         aethelstun (        1) |  92.:             luding (        1) *
  *  93.:        aethelweard (        1) |  93.:              havid (        1) *
  *  94.:         aethelwine (        1) |  94.:             fortin (        1) *
  *  95.:         aethelwyne (        1) |  95.:            swubble (        1) *
  *  96.:          aetheston (        1) |  96.:              praps (        1) *
  *  97.:          aethretun (        1) |  97.:                 ag (        1) *
  *  98.:             afreda (        1) |  98.:          prentitht (        1) *
  *  99.:              aftan (        1) |  99.:      epieikestebon (        1) *
  *  100.:              afton (        1) |  100.:           himthelf (        1) *
  *
  * ====================================================
  *
  * Character statistics :
  * ----------------------
  * Note: For the rules, vowels and consonants are split.
  * -> The last column shows the total quote of a letter.
  * -----------------------------------------------------
  * Source names, Language de, Type NST_NAMES_DE
  *       char found | recalced |  quote |   rate  |  all
  * A :        10274 |      177 | 29.58% | 0.29580 | 12.71%
  * B :         1476 |       25 |  3.20% | 0.03202 |  1.83%
  * C :         1595 |       27 |  3.46% | 0.03461 |  1.97%
  * D :         3132 |       54 |  6.80% | 0.06795 |  3.88%
  * E :         8783 |      151 | 25.29% | 0.25287 | 10.87%
  * F :         1265 |       22 |  2.74% | 0.02745 |  1.57%
  * G :         1624 |       28 |  3.52% | 0.03523 |  2.01%
  * H :         2840 |       49 |  6.16% | 0.06162 |  3.51%
  * I :         8180 |      141 | 23.55% | 0.23551 | 10.12%
  * J :          985 |       16 |  2.14% | 0.02137 |  1.22%
  * K :         1910 |       32 |  4.14% | 0.04144 |  2.36%
  * L :         4568 |       79 |  9.91% | 0.09911 |  5.65%
  * M :         3178 |       54 |  6.90% | 0.06895 |  3.93%
  * N :         5969 |      103 | 12.95% | 0.12950 |  7.39%
  * O :         3862 |       66 | 11.12% | 0.11119 |  4.78%
  * P :          673 |       11 |  1.46% | 0.01460 |  0.83%
  * Q :           64 |        1 |  0.14% | 0.00139 |  0.08%
  * R :         6225 |      108 | 13.51% | 0.13506 |  7.70%
  * S :         4287 |       74 |  9.30% | 0.09301 |  5.30%
  * T :         3805 |       66 |  8.26% | 0.08255 |  4.71%
  * U :         2439 |       42 |  7.02% | 0.07022 |  3.02%
  * V :          874 |       15 |  1.90% | 0.01896 |  1.08%
  * W :          614 |       10 |  1.33% | 0.01332 |  0.76%
  * X :           51 |        1 |  0.11% | 0.00111 |  0.06%
  * Y :         1192 |       20 |  3.43% | 0.03432 |  1.47%
  * Z :          956 |       16 |  2.07% | 0.02074 |  1.18%
  * ä :            2 |        1 |  0.01% | 0.00006 |  0.00%
  * ö :            1 |        1 |  0.00% | 0.00003 |  0.00%
  * ü :            0 |        0 |  0.00% | 0.00000 |  0.00%
  * -----------------------------------------------------
  * Source names, Language en, Type NST_NAMES_EN
  *       char found | recalced |  quote |   rate  |  all
  * A :         6141 |        6 | 28.67% | 0.28668 | 12.00%
  * B :         1114 |       17 |  3.75% | 0.03746 |  2.18%
  * C :         1533 |       24 |  5.15% | 0.05155 |  3.00%
  * D :         2195 |       34 |  7.38% | 0.07381 |  4.29%
  * E :         6299 |        6 | 29.41% | 0.29406 | 12.31%
  * F :          599 |        9 |  2.01% | 0.02014 |  1.17%
  * G :          841 |       13 |  2.83% | 0.02828 |  1.64%
  * H :         1611 |       25 |  5.42% | 0.05417 |  3.15%
  * I :         3568 |        3 | 16.66% | 0.16657 |  6.97%
  * J :          491 |        8 |  1.65% | 0.01651 |  0.96%
  * K :          904 |       14 |  3.04% | 0.03040 |  1.77%
  * L :         3975 |       61 | 13.37% | 0.13366 |  7.77%
  * M :         1277 |       20 |  4.29% | 0.04294 |  2.50%
  * N :         4429 |       68 | 14.89% | 0.14893 |  8.66%
  * O :         2637 |        3 | 12.31% | 0.12310 |  5.15%
  * P :          411 |        6 |  1.38% | 0.01382 |  0.80%
  * Q :           65 |        1 |  0.22% | 0.00219 |  0.13%
  * R :         4167 |       64 | 14.01% | 0.14012 |  8.15%
  * S :         2098 |       32 |  7.05% | 0.07055 |  4.10%
  * T :         2402 |       37 |  8.08% | 0.08077 |  4.70%
  * U :         1031 |        1 |  4.81% | 0.04813 |  2.02%
  * V :          498 |        8 |  1.67% | 0.01675 |  0.97%
  * W :          855 |       13 |  2.88% | 0.02875 |  1.67%
  * X :          114 |        2 |  0.38% | 0.00383 |  0.22%
  * Y :         1745 |        2 |  8.15% | 0.08146 |  3.41%
  * Z :          160 |        2 |  0.54% | 0.00538 |  0.31%
  * -----------------------------------------------------
  * Source names, Language all, Type NST_NAMES_ALL
  *       char found | recalced |  quote |   rate  |  all
  * A :        16415 |        6 | 29.23% | 0.29234 | 12.44%
  * B :         2590 |       20 |  3.42% | 0.03416 |  1.96%
  * C :         3128 |       24 |  4.13% | 0.04125 |  2.37%
  * D :         5327 |       41 |  7.02% | 0.07025 |  4.04%
  * E :        15082 |        5 | 26.86% | 0.26860 | 11.43%
  * F :         1864 |       14 |  2.46% | 0.02458 |  1.41%
  * G :         2465 |       19 |  3.25% | 0.03251 |  1.87%
  * H :         4451 |       35 |  5.87% | 0.05870 |  3.37%
  * I :        11748 |        4 | 20.92% | 0.20922 |  8.90%
  * J :         1476 |       11 |  1.95% | 0.01946 |  1.12%
  * K :         2814 |       22 |  3.71% | 0.03711 |  2.13%
  * L :         8543 |       66 | 11.27% | 0.11266 |  6.47%
  * M :         4455 |       35 |  5.87% | 0.05875 |  3.38%
  * N :        10398 |       81 | 13.71% | 0.13712 |  7.88%
  * O :         6499 |        2 | 11.57% | 0.11574 |  4.92%
  * P :         1084 |        8 |  1.43% | 0.01430 |  0.82%
  * Q :          129 |        1 |  0.17% | 0.00170 |  0.10%
  * R :        10392 |       81 | 13.70% | 0.13704 |  7.87%
  * S :         6385 |       49 |  8.42% | 0.08420 |  4.84%
  * T :         6207 |       48 |  8.19% | 0.08185 |  4.70%
  * U :         3470 |        1 |  6.18% | 0.06180 |  2.63%
  * V :         1372 |       11 |  1.81% | 0.01809 |  1.04%
  * W :         1469 |       11 |  1.94% | 0.01937 |  1.11%
  * X :          165 |        1 |  0.22% | 0.00218 |  0.13%
  * Y :         2937 |        1 |  5.23% | 0.05231 |  2.23%
  * Z :         1116 |        9 |  1.47% | 0.01472 |  0.85%
  * -----------------------------------------------------
  * Source texts, Language de, Type NST_TEXTS_DE
  *       char found | recalced |  quote |   rate  |  all
  * A :       111458 |       21 | 17.56% | 0.17557 |  6.49%
  * B :        39175 |       28 |  3.62% | 0.03619 |  2.28%
  * C :        51435 |       38 |  4.75% | 0.04752 |  3.00%
  * D :        39607 |       29 |  3.66% | 0.03659 |  2.31%
  * E :       234087 |       44 | 36.87% | 0.36873 | 13.63%
  * F :        36239 |       26 |  3.35% | 0.03348 |  2.11%
  * G :        64061 |       47 |  5.92% | 0.05918 |  3.73%
  * H :        81418 |       60 |  7.52% | 0.07521 |  4.74%
  * I :       126127 |       24 | 19.87% | 0.19867 |  7.34%
  * J :         2978 |        2 |  0.28% | 0.00275 |  0.17%
  * K :        40929 |       30 |  3.78% | 0.03781 |  2.38%
  * L :        82051 |       60 |  7.58% | 0.07580 |  4.78%
  * M :        45086 |       33 |  4.17% | 0.04165 |  2.63%
  * N :       142731 |      105 | 13.19% | 0.13186 |  8.31%
  * O :        57481 |       11 |  9.05% | 0.09054 |  3.35%
  * P :        29605 |       22 |  2.73% | 0.02735 |  1.72%
  * Q :         1104 |        1 |  0.10% | 0.00102 |  0.06%
  * R :       135068 |       99 | 12.48% | 0.12478 |  7.87%
  * S :       114693 |       84 | 10.60% | 0.10595 |  6.68%
  * T :       115823 |       85 | 10.70% | 0.10700 |  6.74%
  * U :        68997 |       13 | 10.87% | 0.10868 |  4.02%
  * V :        13433 |        9 |  1.24% | 0.01241 |  0.78%
  * W :        17003 |       12 |  1.57% | 0.01571 |  0.99%
  * X :         2023 |        1 |  0.19% | 0.00187 |  0.12%
  * Y :         5362 |        1 |  0.84% | 0.00845 |  0.31%
  * Z :        28009 |       20 |  2.59% | 0.02588 |  1.63%
  * ä :        14957 |        3 |  2.36% | 0.02356 |  0.87%
  * ö :         5430 |        1 |  0.86% | 0.00855 |  0.32%
  * ü :        10954 |        2 |  1.73% | 0.01725 |  0.64%
  * -----------------------------------------------------
  * Source texts, Language en, Type NST_TEXTS_EN
  *       char found | recalced |  quote |   rate  |  all
  * A :        63006 |        4 | 21.55% | 0.21546 |  8.62%
  * B :        14945 |       12 |  3.41% | 0.03411 |  2.05%
  * C :        28786 |       22 |  6.57% | 0.06570 |  3.94%
  * D :        27043 |       21 |  6.17% | 0.06172 |  3.70%
  * E :        85231 |        6 | 29.15% | 0.29146 | 11.67%
  * F :         9483 |        7 |  2.16% | 0.02164 |  1.30%
  * G :        18838 |       15 |  4.30% | 0.04299 |  2.58%
  * H :        18836 |       15 |  4.30% | 0.04299 |  2.58%
  * I :        61515 |        4 | 21.04% | 0.21036 |  8.42%
  * J :         2136 |        2 |  0.49% | 0.00487 |  0.29%
  * K :         7635 |        6 |  1.74% | 0.01742 |  1.05%
  * L :        41955 |       33 |  9.58% | 0.09575 |  5.74%
  * M :        20737 |       16 |  4.73% | 0.04733 |  2.84%
  * N :        54609 |       43 | 12.46% | 0.12463 |  7.47%
  * O :        44854 |        3 | 15.34% | 0.15339 |  6.14%
  * P :        18991 |       15 |  4.33% | 0.04334 |  2.60%
  * Q :         1283 |        1 |  0.29% | 0.00293 |  0.18%
  * R :        53994 |       42 | 12.32% | 0.12323 |  7.39%
  * S :        52515 |       41 | 11.99% | 0.11985 |  7.19%
  * T :        46956 |       37 | 10.72% | 0.10716 |  6.43%
  * U :        23654 |        2 |  8.09% | 0.08089 |  3.24%
  * V :         7662 |        6 |  1.75% | 0.01749 |  1.05%
  * W :         7161 |        6 |  1.63% | 0.01634 |  0.98%
  * X :         1945 |        2 |  0.44% | 0.00444 |  0.27%
  * Y :        14163 |        1 |  4.84% | 0.04843 |  1.94%
  * Z :         2657 |        2 |  0.61% | 0.00606 |  0.36%
  * -----------------------------------------------------
  * Source texts, Language all, Type NST_TEXTS_ALL
  *       char found | recalced |  quote |   rate  |  all
  * A :       174464 |        9 | 19.47% | 0.19473 |  7.22%
  * B :        54120 |       23 |  3.56% | 0.03559 |  2.24%
  * C :        80221 |       34 |  5.28% | 0.05275 |  3.32%
  * D :        66650 |       28 |  4.38% | 0.04383 |  2.76%
  * E :       319318 |       16 | 35.64% | 0.35641 | 13.21%
  * F :        45722 |       19 |  3.01% | 0.03007 |  1.89%
  * G :        82899 |       35 |  5.45% | 0.05452 |  3.43%
  * H :       100254 |       42 |  6.59% | 0.06593 |  4.15%
  * I :       187642 |       10 | 20.94% | 0.20944 |  7.76%
  * J :         5114 |        2 |  0.34% | 0.00336 |  0.21%
  * K :        48564 |       20 |  3.19% | 0.03194 |  2.01%
  * L :       124006 |       52 |  8.15% | 0.08155 |  5.13%
  * M :        65823 |       28 |  4.33% | 0.04329 |  2.72%
  * N :       197340 |       83 | 12.98% | 0.12977 |  8.17%
  * O :       102335 |        5 | 11.42% | 0.11422 |  4.23%
  * P :        48596 |       20 |  3.20% | 0.03196 |  2.01%
  * Q :         2387 |        1 |  0.16% | 0.00157 |  0.10%
  * R :       189062 |       79 | 12.43% | 0.12433 |  7.82%
  * S :       167208 |       70 | 11.00% | 0.10996 |  6.92%
  * T :       162779 |       68 | 10.70% | 0.10705 |  6.74%
  * U :        92651 |        5 | 10.34% | 0.10341 |  3.83%
  * V :        21095 |        9 |  1.39% | 0.01387 |  0.87%
  * W :        24164 |       10 |  1.59% | 0.01589 |  1.00%
  * X :         3968 |        2 |  0.26% | 0.00261 |  0.16%
  * Y :        19525 |        1 |  2.18% | 0.02179 |  0.81%
  * Z :        30666 |       13 |  2.02% | 0.02017 |  1.27%
  * -----------------------------------------------------
  @endverbatim
**/

/* --- resulting consonant array --- */
const uint32_t conListLen[6] = { 791, 458, 587, 791, 344, 638 };

const char     conList[6][792] = {
  "mznzbmbnrjbsbrbnbnbfbrkvblbdcrhntctgfrsclzlctnrcdcncncscdsrlngdrdnbmfdgdncdn"
  "lkmtngdrnschtmrdndtdlhmdrdndlfntcdfmfmrtfrfzfbfnfrfcftlrgldgrglglndgkgswrlgh"
  "rngnghgnhghdhmhphjhgrchlhdhrhshrhrlnhchlmchfghlhvnhklhchdbjwdjnjmdjhjrjsjlkh"
  "kbdkdlfktkrklkgkhldjklklkskrmlhrnslsrzlmtplklsrdlnlmlrlsclnlsnlshlctrdltgbsr"
  "lhlktldltlsrlnrhlnldlrshlkhltlrlskwmsmrmlfmdmnmdmlrjmcmrmsmdmnmdmhmlmnmsmzmr"
  "mtlkmpmkmdnhnmntzntnlnrnsncnsndtgnbktmnsrtnbnbnrnmnldndtlnrntndndnznsrtdnrnf"
  "ncnlnhnbmgnlnkndnrhkrtlnknrnbnkndnrvsptpmlpsprvrqrmrgrtfrsrncnrmrgrhrsnbrhtn"
  "tlrxrnrmrlgrcrsrslrmrknrnsjvzrsrtrlrsdrvsrgnhrlrdmrfhrnrhnmnrgrprnbrnprcrhrb"
  "mrdtslfstsrldsnstscswrdsnlbrsdnsmsnrsltzscsdsmslnhvsdlstsgkspslslswsvsrnszsd"
  "sdstjtrltstjtntnstrtktlptmtvmtsrtsktndnhtrtkntrtntnstrstltlntnthtftntbvrvhvr"
  "vrvrsrvgwljwhwswmwjztzrztztfrzs",
  "ldwznbkbrsdbprksplsldnclclcvcrcdnrdhctcnrbldcldprhdlgrdmdfndmndnjxsndhdckdbd"
  "hdnclnfrntdftrgtlnlngnghgwshltndhmncnhsrvhdhrtslthnjdcjswlnknknktntlmlctlckt"
  "hdlflbfcsntlvlslrlvlrlnlmlnrlsrklhmkrjltlbrnrnlrlglnlhlblhmlmrmrmdrmdrlmhlml"
  "mtnrnhntzlnhnsnmnfntnbnrnknrndnrngndndnsnrbjnlcftsncnlknctrngnmnljntrdbpnrnp"
  "qrxnpsrbrndrbrvlrlrlrkclrjrlsrswmrlrlrsdnrscrhrgrtbrfrnrdrdrkrmlrdcsnsrhrsns"
  "bhsbsrslsmnhsgsgsltslhgntbtltstntcthrltgtwtltntrtltftwmwnkvhrvclvcwdwtwtwrjw"
  "rw",
  "nrhzdzknhgbndhblblrtgrbnlrblcfcmrhctlnchcdcfcscncsksdndsnrzdsdkfndnlrtdjdmrs"
  "drlrtfdkdmdjdrsrdfrnmdnfvtdtnpcgrgnlbvnrhgrlvgbgklhlhcfhlgthlhrnsltsglrhnhlh"
  "zhkhsrkhmjrhbjdjwjrjknhlnksgndwksncktgzdnflrlrshlrntlnlsnlkhnsldlslcrldlsltl"
  "tbltlwlglflmsrmlcdlnlnlhlsltlmldmtpdmcmsmkmbmtshmclmhmrtmrmpmrmjmdmntnltrnsn"
  "lcnlnrbnwknrgnvnrndndnbpnhnrnstnlhntknfnbtlgknvnrnfnknrblhnrnhzrnlnhnsnmspmp"
  "snhrqrgdrwmrnmdcrdrmrbtwrlrsgrhrnrctbrfrtrdnrsrtrdrlrljrzrbrlzrslnrhnrdtsrdr"
  "lrprnrnrmtsndscslsksvsrdlklsbslscsnksbncdsrsrsmrcstbshnstrljnrntnrtdgtctmtkn"
  "tmtntrtdnlthlntptrtrhtfmtlmtntsfvngvnvsvgvwkwmjwlwrtwxz",
  "ztdzfbtbgsgbhbrtzsmbrbwsfprblbnbznrcbctctchrhclrnfncschctsncsnrclctcdcbdtdtd"
  "gdrdrdfrsrntdptsdltzgrdtcfsftfrfsftfrfgfnfhfdpfsnsgsgngcgblmvnglgsgnfhsgngrg"
  "jrtbgrgmghngrghgrmglshdhlkrnshrhnhfhpghnhnchtmrlmnhrgtsdhrhkhghbntslhkhnhtlz"
  "hlgpbjkvkckgknchklntgsckfkgnwkrkrlnfdlhlthlkhgclrmnhvlkblhlnlmlklnstbtslnlsc"
  "lrnlnldlrlhstrnkltlsmkmnmrbgmlmrcmlmhmhmsmstnmngnzmtmznrcgnrnlnlnbnhrncntnhw"
  "nknbcnlsnrnmnfnkngnrhnstskntncnsngndngshncnhmnlpmnpnpntnsnrtdnrnrnpnrnhnpnsn"
  "wtbnrtgpnrspbpdprpmnpnpnptprnqrnrsrkbklrldrctrgrcdrsrhrnrdnrblhlfkrsrnrzmrfr"
  "txhrhmrlrhrtrnrcplzshrdrlrsgrtrwrgrtrshntsrkgrhrlrtrksmtghskcsrsrstrvltsmsbs"
  "rshnmsnlscrnsgswststgscsdsbftgdslswstrsdsnhtsnlslstsrskfsrsksthtcrnftftltnkn"
  "tslhshcstcnzlththtrtstlthtmtstdtltzdrvctzsbtptrtnstrsgtnthbtnsdptlntltvgvthv"
  "tskrwnrwnlswzwrtzcmrzvznzrmzwzt",
  "lxrsbmdcbsbrblgfpscsnlctfchctvpcmcgtncdbdndctndndrdfdlrnsrtcfpbfnfgwgcgrglgs"
  "grgbchdhlhmchrhphrghjtkdkrkpldlxglrlnlrsnltldlslrldblrsnlsnlsmnmrhmtsmsmtmtm"
  "nlnqnhnlnlncnrstnsmnrkvtlndsrmnlrlntnstrnbtnpcgpwdprnplphpsnrhprzthnrtrlprsr"
  "trnrznmdrtrcrbrdrsnrknrtrhrsplsbsrscnsdsgslstslsvsmslswsfrnsglsncskltdntscjt"
  "ctnmcthtwmtntstdtrtdtrtntntwvcvnsvgsptwb",
  "zlznzsbnbksnbsnsbgvbzbdbdbnmscncscrcncgfpsnrdscbcsrgchcscsnlcpcblbdrdrdcdntr"
  "dtdfdrdsrdcdmdlfhdfkfmfnskvcfmsknfgwlnlhglglghglgcgmgrvrstctrgngpgtgrhthrhsh"
  "lhlfhphtpzhrhznthmtknmhlhdrcsrchnhsjrtfknrnkrlrkslhkrkthklslcsdtklhrntflblgl"
  "rdldlnwclhlrglrgtmlclgrtrlhgltlrlbmhlgmwrtrmbnwrgmtmsmrtlgvhtnwnbntnbnrnmlnk"
  "ndshrtnsnkwntnrpnlnsntntnbnlnlnhzcndncrhtnrnxnmngsncnpsrnvshnsngtdnztrnptpdp"
  "npdpnprgptpcqrgrmslrmlmrkprtnrntrlsmrnrsrmfngrnlrsrsrskrkjrkrsrtrdfrgdrmlrnl"
  "rdcrftnkrnrshtlhsmstsnshsbstspshshrsfsrscsgznsbcrsrhnhbsnslfpsnrswstsnscnszh"
  "snsrshmsrsrtpthtngntrtntsgtltrtrtstltktnthtlntltltzltmtrthtgtbptstnftntftmht"
  "gsmlvcvfvrvgbsdwcwnwcnxtczbnzm",
};

/* --- resulting vowel array --- */
const uint32_t vowListLen[6] = { 599, 21, 19, 120, 20, 46 };

const char     vowList[6][600] = {
  "iyiyayuyiyäiaeieayaiaeiaieaieiaoaoaoauaiaueiaeueiyaeieiaeiaueaiaeauaieieaiea"
  "eaiaioieoaeaeiaeoauaiaiuaoauaiaeiaoioiaiaeaeaeaioaioeiaueaoaioaeioaeiaeiaeau"
  "iaoeaeiaoaieaeaueaeaiaiaeiauiaoeiaeaoeueaeaoeuieieaiaieaeayoeiaoeaiaeiuaueae"
  "aeaeaeiaeyaioeueaoieiaeiaueieieaeuaoieioaoeaeaiaeyueauaeyeaeiueieieaeaeaeuie"
  "oaeaeaoayeieoaieauaoeaeaeyeiaeieiaeoieiaiaiaieiyaiaoiaieieyiyiaiaieaoeiaoiea"
  "iaieioiaiaieaiaeiaieaeuieaueoaieieaeauieieoöieieiaoaeoaieiaoaeauaioiaeauaeoe"
  "aeioieaiaeaioieioiyeaiaieiaoiaoaieioaoaoeoaeoaoaoeaeaeiaieaueaoaioiaioieoaio"
  "euaeoueaeaiaieoueieuaeoiaueiaieaoauaueaouiuaeueaiuiaeoaieyeiayoieya",
  "ayaiyaeaueoeoeaieaeoi",
  "eouaoyaieaeieaeiaia",
  "eaeieueieuaiueiüeoeoäoeuaeaeaeiaeieoueaoeoeiuieöaueaeiaoieieoueuaeoeieiueoie"
  "aeieaeueieaieaeaeuieaiaieüeieiyuaeiäieoäeaea",
  "ouieauaeiyeieoaiaeoe",
  "eoeuaeieieiueuaeyeiaeiaieaeoioeaueoeaieiaoiuea",
};

/** @file CRandomWordConstants.h
  *
  @verbatim
  * Follow-up statistics :
  * ----------------------
  *  names / de => NST_NAMES_DE
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z | ä | ö | ü |
  * A | 30|129| 84|235| 39| 84| 81|203|150| 43| 95|406|251|999| 15| 25|  8|817|272|277|118| 68| 49|  8|120| 96|  0|  0|  0|
  * B |191| 15|  0|  1|289|  0|  0|  4|124|  2|  3| 12|  0|  2| 80|  0|  0| 64|  3|  0| 47|  0|  0|  0|  8|  0|  1|  0|  0|
  * C |182|  0|  6|  3|114|  0|  1|216|114|  2| 56| 32|  3|  0|108|  0|  1| 10|  0|  7| 19|  0|  0|  0| 10|  4|  0|  0|  0|
  * D |265|  5|  1| 35|333|  2|  8| 15|321| 20|  1| 14| 16|  5|194|  0|  0| 77|  5| 28| 57|  2| 15|  0| 24| 12|  0|  0|  0|
  * E | 47| 68| 74|269| 57| 82| 73| 85|182| 22| 95|616|213|535| 63| 31|  1|890|234|360| 37| 95| 18|  7| 89| 55|  0|  0|  0|
  * F |130|  0|  1|  0|121| 30|  3|  3|107|  1|  3| 19|  0|  1| 35|  0|  0|176|  4| 10| 13|  0|  1|  0|  0|  1|  0|  0|  0|
  * G |143|  4|  0|  9|211|  4| 10| 26|183|  0|  1| 22|  9| 19| 73|  0|  0| 68|  1|  2|104|  1|  6|  0|  6|  0|  0|  0|  0|
  * H |606|  5|  0|  9|320|  1|  0|  2|254|  3|  4| 12| 34| 19| 87|  1|  0| 72| 18| 23| 49|  1|  3|  0| 14|  2|  0|  0|  0|
  * I |304| 67|225|194|364| 47|103| 58|  5| 64|190|470|211|865|157| 46|  9|264|339|208| 44| 45|  4|  6| 74| 61|  0|  0|  0|
  * J |239|  1|  2|  9| 98|  1|  1|  1| 22|  0|  7|  5|  3|  4|107|  0|  0|  7|  1|  3| 59|  0|  0|  0|  0|  0|  0|  0|  0|
  * K |321|  3|  2|  1|178|  1|  0| 32|113|  4| 17| 25|  9|  6|140|  0|  0| 42| 13| 12| 50|  0|  7|  0|  7|  1|  0|  0|  0|
  * L |429| 29| 13|187|312| 68| 30| 29|521| 28| 57|189| 72|  3|164|  8|  0| 10| 31| 54|104| 49|  9|  0| 48|  4|  0|  0|  0|
  * M |610| 16|  4|  8|321|  4|  3|  6|344|  1|  6|  6| 51|  2|131| 12|  0|  9| 15|  6|144|  0|  0|  0| 15|  7|  0|  0|  0|
  * N |566|  6| 68|215|432| 21|101| 41|346| 26| 67|  7|  2|199|179|  1|  0| 24| 78|128| 86|  3|  2|  0| 41| 49|  0|  0|  0|
  * O | 24| 48| 23| 60| 25| 30| 27| 34| 15| 18| 26|247| 87|265| 26| 23|  1|307|225| 74| 92| 27| 15|  2| 15| 17|  0|  0|  0|
  * P | 91|  0|  0|  0| 83|  0|  0| 38| 50|  0|  1|  4|  1|  0| 31| 25|  0| 21|  3|  3|  4|  0|  0|  0|  1|  0|  0|  0|  0|
  * Q |  5|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 26|  0|  0|  0|  0|  0|  0|  0|  0|
  * R |536| 32| 37|211|417| 10|113| 39|753| 16| 64| 64| 75| 85|237|  6|  0| 40| 56|222|129| 17| 16|  0| 55| 24|  0|  0|  0|
  * S |433| 10| 56|  5|421|  5|  4| 81|294|  4| 27| 63| 38| 10|105| 16|  3| 16| 93|239| 90|  9| 13|  0| 30| 13|  0|  0|  0|
  * T |349|  3|  5|  1|262|  8|  5|217|307| 29| 10| 18| 15|  3|146|  1|  0|121| 13|199| 59|  3|  8|  0| 15| 20|  0|  0|  0|
  * U | 33| 43| 29|110| 56| 19| 29| 47| 32|  6| 32|152| 44|165|  3| 13|  1|211|245| 82|  3|  4|  3|  1|  8| 36|  0|  0|  0|
  * V |139|  0|  2|  3|116|  0|  3|  1|150|  0|  6|  9|  1|  1| 39|  0|  0|  7|  3|  1|  7|  1|  0|  0|  2|  4|  0|  0|  0|
  * W |131|  0|  0|  0| 49|  1|  1|  2|119|  0|  0|  5|  0|  1| 22|  0|  0|  0|  1|  1|  4|  0|  1|  0|  1|  1|  0|  0|  0|
  * X |  6|  0|  0|  0|  3|  0|  0|  1|  6|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  1|  0|  0|  0|  0|  0|  0|  0|  0|  0|
  * Y |122|  7|  4| 18| 88|  6|  4|  8| 37|  0| 10| 35| 14| 33| 23|  1|  0| 22| 24|  7| 18|  6|  0|  0|  3|  3|  0|  0|  0|
  * Z |127|  3|  1|  8|102|  0|  3|  6|127|  1|  3| 12|  4|  0| 22|  0|  0|  2|  2|  1| 17|  4|  2|  0|  9|  5|  0|  0|  0|
  * ä |  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  1|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|
  * ö |  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|
  * ü |  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|
  * ------------------------------------------------------------------------------------------------------------
  *  names / en => NST_NAMES_EN
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z |
  * A |  7| 98|116|230|171| 24| 58|136|167|  1| 47|550|218|999|  1| 23| 12|822|149|214|132| 98| 62| 26|225| 17|
  * B |103| 28|  0|  1|275|  0|  0|  2| 74|  0|  0| 48|  0|  0| 76|  0|  0|227|  3|  1|101|  0|  0|  0| 65|  0|
  * C |230|  2|  6|  1|196|  3|  4|233| 96|  0|150| 96|  2|  0|209|  0|  4| 45|  1|  7| 26|  0|  4|  0| 34|  0|
  * D |333| 16|  2| 66|412|  6| 31|  8|151|  0|  4| 37| 24| 13|152|  1|  0|118| 27|  6| 50|  0| 63|  0| 80|  0|
  * E |320| 41| 53|171|179| 47| 51| 16| 93|  3| 25|811| 89|582| 99| 18|  2|768|280|235| 23|112| 59| 37|246|  9|
  * F | 82|  0|  1|  1| 67| 49|  0|  0| 53|  0|  0| 17|  0|  0|101|  0|  0| 89|  0| 17| 12|  0|  1|  0|  4|  0|
  * G |128| 12|  0| 13|154|  2| 14|115| 71|  0|  0| 27|  3|  5| 45|  0|  0| 53|  9| 11| 27|  0|  6|  0|  9|  0|
  * H |401|  9|  5|  4|279|  5|  0|  1|132|  0|  1| 43|  9| 12| 94|  0|  0| 48|  3| 47| 64|  0| 18|  0| 24|  0|
  * I |322| 22|220| 94|454| 37|110|  0|  0|  3| 59|305| 56|646| 77| 25|  8|121|317|160| 12| 36|  4|  8| 22| 20|
  * J |170|  0|  0|  0|122|  0|  0|  0| 22|  0|  0|  0|  0|  0|106|  0|  0|  0|  0|  0| 30|  0|  0|  0|  0|  0|
  * K |204|  1|  1|  0|186|  3|  0| 11|113|  0| 11| 23|  1|  6| 31|  0|  0| 28| 15|  0|  4|  0|  8|  0| 33|  0|
  * L |462| 71| 15|178|711| 68|  6| 17|485|  0|  7|486| 46|  8|185| 10|  0| 11| 54| 54| 40| 51| 30|  1|291|  2|
  * M |433| 41|  8|  1|151|  2|  0|  2|162|  0|  1|  6| 38|  2|121| 16|  0|  8| 11|  1| 26|  0|  1|  0| 36|  2|
  * N |462| 18| 76|312|595| 21|104| 13|214|  6| 14| 35|  0|362| 96|  0|  1| 15| 81|140|  6|  1| 27|  0| 67| 18|
  * O | 17| 40| 65|119| 35| 13| 19| 18| 23|  1| 25|217| 58|635| 63| 25|  0|512|110| 66| 96| 24| 65| 16| 25|  4|
  * P | 65|  0|  0|  0| 85|  2|  0| 53| 22|  0|  0| 13|  1|  0| 26| 14|  0| 37| 10|  5|  5|  0|  2|  0|  3|  0|
  * Q |  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 59|  0|  0|  0|  0|  0|
  * R |461| 25| 49|265|498| 11| 37| 43|621|  7| 44|184| 51|154|276| 10|  5|204| 62|209| 60| 36| 54|  0|178|  0|
  * S |189| 22| 70|  8|176|  3|  1|192| 98|  0| 25| 44| 25|  1|138| 24|  1|  6|116|381| 49|  5| 28|  0| 37|  0|
  * T |262|  6| 23|  0|296|  5|  0|296|194|  1|  4| 48|  5| 14|263|  0|  0|112| 18|212| 77|  0| 26|  0| 76| 18|
  * U | 16| 14| 41| 61| 58| 10| 49|  8| 41|  0|  9| 86| 33|148|  0| 13|  0|192| 80| 45|  0|  2|  1|  7|  7|  7|
  * V | 86|  0|  0|  0|151|  0|  0|  0|147|  0|  0|  1|  0|  0| 32|  0|  0|  9|  0|  0|  0|  1|  0|  0| 25|  0|
  * W |169|  1|  0|  2|145|  2|  0| 21|187|  0|  0| 15|  0| 22| 80|  1|  0|  8|  5|  1|  9|  0|  0|  0| 75|  0|
  * X | 18|  0|  0|  0|  2|  4|  0|  0| 21|  0|  0|  4|  0|  2|  6|  0|  0|  0|  0| 10|  1|  0|  1|  0|  2|  1|
  * Y | 94|  8| 35| 42| 40| 14|  9|  6|  2|  1| 10|151| 23|323| 18|  3|  0| 41| 69| 32|  2|  8|  2|  0|  0|  3|
  * Z | 43|  2|  0|  0| 25|  0|  2|  0| 16|  0|  0|  1|  7|  0|  7|  0|  0|  0|  2|  0|  0|  0|  1|  0|  7|  7|
  * ------------------------------------------------------------------------------------------------------------
  *  names / all => NST_NAMES_ALL
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z |
  * A | 21|117| 97|233| 91| 60| 72|176|157| 27| 76|462|238|999| 10| 24|  9|819|224|252|124| 80| 54| 15|161| 65|
  * B |156| 20|  0|  1|283|  0|  0|  3|104|  1|  1| 26|  0|  1| 79|  0|  0|128|  3|  1| 68|  0|  1|  0| 31|  0|
  * C |201|  1|  6|  2|147|  1|  2|223|107|  1| 93| 57|  3|  0|148|  0|  2| 24|  1|  7| 22|  0|  1|  0| 19|  2|
  * D |292|  9|  1| 47|364|  4| 17| 12|254| 12|  2| 23| 19|  8|177|  1|  0| 93| 14| 19| 54|  1| 34|  0| 46|  8|
  * E |155| 57| 66|231|105| 68| 64| 58|147| 15| 67|693|164|554| 77| 26|  2|841|252|310| 31|102| 34| 19|151| 37|
  * F |111|  0|  1|  1|100| 37|  2|  2| 86|  1|  2| 18|  0|  1| 61|  0|  0|142|  2| 13| 12|  1|  1|  0|  2|  1|
  * G |137|  7|  0| 10|189|  4| 12| 61|139|  0|  1| 24|  7| 14| 62|  0|  0| 62|  4|  5| 74|  1|  6|  0|  7|  0|
  * H |525|  6|  2|  7|304|  3|  0|  2|206|  1|  3| 24| 24| 16| 90|  1|  0| 62| 12| 32| 54|  1|  9|  0| 18|  1|
  * I |312| 49|223|154|399| 43|106| 35|  3| 40|138|405|150|779|126| 38|  9|208|330|189| 31| 41|  4|  6| 53| 45|
  * J |212|  1|  1|  5|108|  1|  1|  1| 22|  0|  4|  3|  2|  2|107|  0|  0|  4|  1|  1| 48|  0|  0|  0|  1|  0|
  * K |275|  2|  2|  1|181|  2|  0| 23|113|  2| 15| 24|  6|  6| 97|  0|  0| 36| 14|  7| 32|  0|  7|  0| 17|  1|
  * L |442| 46| 14|183|470| 68| 20| 24|507| 17| 37|307| 61|  5|172|  9|  0| 10| 40| 54| 79| 50| 17|  1|144|  3|
  * M |540| 26|  6|  5|254|  3|  1|  4|272|  1|  4|  6| 46|  2|127| 13|  0|  9| 13|  4| 97|  0|  1|  0| 23|  5|
  * N |525| 10| 72|253|496| 21|102| 30|294| 18| 46| 18|  1|263|146|  1|  1| 20| 79|133| 54|  2| 12|  0| 51| 36|
  * O | 21| 45| 40| 83| 29| 23| 24| 28| 18| 11| 26|235| 75|411| 41| 24|  1|388|179| 71| 94| 26| 35|  8| 19| 12|
  * P | 81|  0|  1|  0| 84|  1|  0| 44| 39|  0|  1|  8|  1|  0| 29| 21|  0| 27|  5|  4|  4|  0|  1|  0|  2|  0|
  * Q |  3|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 39|  0|  0|  0|  0|  0|
  * R |507| 29| 42|232|449| 10| 83| 41|701| 12| 56|111| 65|113|253|  7|  2|105| 58|217|102| 24| 31|  0|104| 15|
  * S |336| 15| 61|  6|324|  4|  3|125|217|  2| 26| 56| 33|  7|118| 19|  2| 12|102|295| 74|  8| 19|  0| 33|  8|
  * T |314|  4| 12|  1|276|  7|  3|249|263| 18|  8| 30| 11|  7|192|  1|  0|117| 15|204| 67|  2| 15|  0| 39| 19|
  * U | 26| 32| 34| 90| 57| 16| 36| 31| 36|  4| 23|126| 39|158|  2| 13|  1|203|180| 67|  2|  4|  2|  3|  8| 24|
  * V |118|  0|  1|  2|130|  0|  1|  1|149|  0|  4|  6|  1|  1| 36|  0|  0|  8|  2|  1|  4|  1|  0|  0| 11|  2|
  * W |146|  1|  0|  1| 87|  2|  1|  9|146|  0|  1|  9|  0|  9| 45|  1|  0|  3|  3|  1|  6|  0|  1|  0| 30|  1|
  * X | 10|  0|  0|  0|  3|  2|  0|  1| 12|  0|  0|  1|  0|  1|  2|  0|  0|  0|  0|  5|  1|  0|  1|  0|  1|  1|
  * Y |111|  7| 16| 27| 69|  9|  6|  7| 23|  1| 10| 80| 17|147| 21|  2|  0| 30| 42| 17| 12|  7|  1|  1|  2|  3|
  * Z | 94|  3|  1|  5| 72|  0|  3|  4| 83|  1|  2|  8|  5|  0| 16|  1|  0|  1|  2|  1| 10|  2|  2|  0|  8|  6|
  * ------------------------------------------------------------------------------------------------------------
  *  texts / de => NST_TEXTS_DE
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z | ä | ö | ü |
  * A | 12|101| 94| 51|  4| 51| 71| 63| 22|  2| 37|211| 84|325|  2| 39|  1|207|114|177|235| 12|  4|  4|  6| 13|  0|  0|  0|
  * B | 77|  8|  1|  2|281|  3| 13|  6| 56|  1|  3| 40|  1|  3| 28|  1|  0| 55| 24| 10| 33|  1|  4|  0|  2| 12|  8|  3| 10|
  * C | 14|  0|  1|  1| 10|  0|  0|773|  7|  0|102|  3|  0|  0| 12|  0|  0|  2|  0|  1|  3|  0|  0|  0|  1|  1|  1|  0|  0|
  * D | 57|  5|  1|  5|239|  3|  5|  5| 99|  1|  2| 15|  3|  5| 33|  1|  0| 42| 19| 11| 51|  2|  4|  0|  2|  2|  4|  1|  5|
  * E | 20| 83| 66| 75| 26| 54|102| 80|440|  2| 55|288| 76|948| 16| 31|  2|999|214|142| 63| 14| 35| 20|  5| 23|  1|  0|  1|
  * F | 62|  4|  1|  1|131| 38| 12|  2| 50|  1|  2| 54|  2|  2| 38|  1|  0| 60| 14| 62| 20|  1|  3|  0|  0| 12| 19|  8| 20|
  * G | 50|  3|  0|  1|478|  2| 11|  4| 51|  1| 22| 40|  5| 13| 18|  1|  0| 74|101| 21| 31|  1|  2|  0|  1|  5|  5|  1|  2|
  * H |158|  8|  1|  2|326|  6| 12|  3|141|  1| 15| 85| 30| 56| 68|  2|  0| 88| 35|106| 35|  2| 34|  0| 13| 12| 29| 15| 11|
  * I | 46| 29|159| 50|332| 46|178|  8|  1|  2| 59|106| 54|401|102| 19|  1| 47|285|245| 17| 28|  1|  2|  2| 23|  1|  1|  0|
  * J | 17|  0|  0|  1|  9|  0|  0|  0|  1|  0|  1|  1|  0|  0|  8|  0|  0|  1|  0|  0| 10|  0|  0|  0|  0|  0|  2|  0|  1|
  * K |102|  3|  1|  1|154|  2|  5|  4| 30|  1|  4| 45|  2| 15| 98|  1|  0| 58| 16| 73| 46|  1|  2|  0|  1|  6|  8|  8|  7|
  * L |181| 34|  5| 42|251| 21| 17|  7|241|  1| 19|134| 16| 29| 85| 10|  1|  5| 36| 88| 57|  7|  5|  0| 11| 17| 31| 12| 13|
  * M |133| 16|  1|  4|173|  4|  8|  4|103|  0|  4|  7| 72|  3| 52| 41|  0|  2| 17| 13| 44|  1|  3|  0|  3|  7| 14|  6| 11|
  * N |160| 40| 11|226|185| 44|304| 31|126|  3| 73| 25| 12| 62| 47| 13|  1| 17|174|190| 36| 15| 20|  0|  3| 86| 10|  4| 11|
  * O |  5| 26| 35| 27|  4| 18| 51| 21|  6|  2| 17|106| 60|193|  6| 45|  1|195| 90| 56| 12|  7|  6|  6|  1| 11|  0|  1|  0|
  * P | 57|  1|  1|  1| 83| 47|  1| 42| 43|  0|  1| 31|  1|  1| 48| 34|  0| 79| 10| 16| 21|  1|  0|  0|  2|  0|  4|  1|  1|
  * Q |  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 19|  0|  0|  0|  0|  0|  0|  0|  0|
  * R |275| 72| 36| 51|363| 48| 80| 29|228|  1| 68| 40| 67| 73|124| 20|  1| 38|113|156|116| 17| 29|  1|  4| 56| 40| 13| 52|
  * S |105| 33|375|  9|243| 24| 37| 17|132|  1| 37| 23| 35|  6| 44| 93|  1| 15|191|395| 37| 14| 16|  0| 13| 27| 15|  2|  6|
  * T |146| 16|  4|  3|432| 11| 22| 59|268|  1|  8| 31|  9|  7| 81|  6|  1|152| 99| 96| 71|  5| 12|  0|  7| 73| 42|  6| 16|
  * U | 14| 36| 44| 25| 39| 85| 37| 17| 11|  1| 25| 61| 95|273|  3| 24|  1|128|211| 70|  1|  3|  9|  1|  1| 15|  1|  0|  0|
  * V | 20|  1|  0|  1|115|  1|  1|  0| 34|  0|  1|  1|  1|  0| 53|  1|  0|  1|  1|  1|  1|  1|  1|  0|  0|  1|  1|  1|  0|
  * W | 60|  0|  0|  0|115|  0|  1|  1| 62|  1|  0|  1|  0|  1| 17|  0|  0|  1|  1|  1| 13|  0|  0|  0|  1|  0| 20|  5|  8|
  * X |  2|  1|  0|  0|  4|  1|  1|  1|  8|  0|  1|  0|  1|  0|  1|  4|  0|  0|  1|  5|  1|  1|  1|  0|  1|  1|  0|  0|  0|
  * Y |  6|  2|  2|  4|  4|  1|  1|  1|  1|  0|  3|  8|  6|  6|  2|  9|  0|  5| 11|  4|  1|  1|  1|  0|  0|  1|  1|  0|  0|
  * Z | 17|  4|  1|  1|108|  2|  2|  2| 65|  1|  2|  3|  1|  1| 15|  1|  0|  1|  4| 12|190|  1| 19|  0|  4|  2|  5|  3|  4|
  * ä |  1|  3| 15|  9|  1|  8| 10| 29|  1|  0|  1| 13| 12| 46|  1|  1|  1| 31| 19| 37| 26|  1|  0|  0|  0|  1|  0|  0|  0|
  * ö |  0|  2|  5|  3|  0|  3|  6|  7|  0|  0|  1|  7|  3|  9|  1|  5|  0| 20| 14|  4|  0|  1|  1|  0|  0|  1|  0|  0|  0|
  * ü |  0| 34| 36|  5|  0|  3|  7| 22|  0|  0|  0|  8|  4| 24|  0|  2|  0| 24| 12| 14|  0|  0|  0|  0|  0|  0|  0|  0|  0|
  * ------------------------------------------------------------------------------------------------------------
  *  texts / en => NST_TEXTS_EN
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z |
  * A |  3|191|204|160| 45| 32|108| 33|133|  3| 49|486|166|616|  4|117|  4|519|208|533| 86| 67| 39| 15| 75| 17|
  * B |131| 34|  2|  3|165|  1|  1|  1|112|  3|  0|186|  2|  1|108|  1|  0|104| 25|  5| 78|  1|  1|  0| 19|  0|
  * C |276|  0| 29|  1|209|  0|  1|257|139|  0|134| 84|  1|  1|327|  0|  3|108| 14|127| 86|  0|  0|  0| 31|  1|
  * D |127|  8|  4| 42|369|  5| 25|  7|255|  3|  1| 44| 12| 20|106|  3|  0| 73| 47|  4| 56|  7| 13|  0| 29|  1|
  * E |251| 39|162|503|136| 61| 63| 19| 65|  6| 14|352|147|600| 51| 89| 14|999|715|240| 33| 68| 46| 75| 69| 10|
  * F | 67|  1|  1|  0| 79| 57|  0|  1|105|  0|  0| 60|  1|  1| 87|  0|  0| 55|  5| 20| 59|  0|  0|  0| 11|  0|
  * G |105|  3|  0|  2|177|  2| 41| 69|101|  0|  0| 63|  8| 36| 61|  1|  0|106| 39|  6| 55|  0|  4|  0| 21|  1|
  * H |230|  6|  2|  2|279|  4|  1|  1|175|  0|  1| 18| 13| 15|170|  2|  0| 43| 23| 43| 52|  0|  8|  0| 46|  0|
  * I |227| 65|353|135|226| 64|118|  2|  1|  2| 29|260|128|985|290| 73|  8|117|422|320| 20|119|  2| 10|  2| 85|
  * J | 38|  0|  0|  0| 32|  0|  0|  0|  9|  0|  0|  0|  0|  0| 35|  0|  0|  0|  0|  0| 27|  0|  0|  0|  0|  0|
  * K | 56|  4|  1|  1|138|  3|  1|  6| 84|  0|  3| 22|  3| 16| 16|  2|  0|  9| 27|  2|  6|  0|  4|  0| 16|  0|
  * L |342| 18| 15| 64|579| 20|  8|  4|455|  0| 16|299| 22| 18|225| 14|  0|  4| 51| 54| 89| 20|  7|  0|248|  1|
  * M |291| 63|  6|  1|264|  3|  0|  1|213|  0|  1|  5| 56| 10|158|107|  0|  1| 20|  1| 55|  0|  1|  0| 20|  0|
  * N |267| 15|172|255|526| 38|487| 14|251|  7| 38| 19| 11|112|152| 10|  6| 14|213|372| 41| 24| 11|  1| 29|  6|
  * O | 50| 55| 94| 93| 34| 29| 88| 11| 49|  2| 33|217|169|641|109|109|  3|426|133|131|241| 71| 89| 16| 20|  7|
  * P |156|  1|  1|  1|229|  2|  1|103|117|  0|  1| 91|  2|  2|141| 70|  0|160| 35| 45| 51|  0|  1|  0| 14|  0|
  * Q |  1|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 84|  0|  0|  0|  0|  0|
  * R |475| 40| 63|113|599| 20| 51| 20|483|  2| 38| 63| 80| 80|334| 33|  2|103|166|157| 96| 27| 16|  0|108|  1|
  * S |135| 11|117|  5|291|  7|  4|192|228|  1| 32| 56| 69| 47|120|105| 13|  4|356|492|112|  2| 28|  0| 36|  1|
  * T |288|  7| 32|  1|584| 11|  2|242|615|  0|  1| 57| 11| 14|231|  4|  0|237| 92|135|100|  1| 18|  0|104|  9|
  * U | 57| 53| 63| 53| 60| 17| 43|  2| 62|  1|  6|160| 97|238| 12| 51|  0|210|244|122|  1|  5|  1|  6|  2|  5|
  * V | 78|  0|  0|  0|254|  0|  0|  0|119|  0|  0|  1|  0|  0| 41|  0|  0|  2|  1|  0|  4|  1|  0|  0|  5|  0|
  * W |102|  4|  1|  4| 80|  2|  1| 28| 83|  0|  2| 13|  2| 23| 61|  1|  0| 14| 14|  2|  2|  0|  1|  0| 10|  1|
  * X |  9|  1|  9|  0| 13|  1|  0|  3| 23|  0|  0|  1|  0|  0|  5| 18|  0|  0|  0| 16|  3|  0|  1|  0|  2|  0|
  * Y | 32|  7| 19| 14| 32|  3|  4|  3| 17|  1|  1| 34| 23| 54| 13| 21|  0| 21| 41| 15|  3|  1|  5|  1|  0|  2|
  * Z | 33|  1|  0|  0| 73|  0|  0|  1| 21|  0|  1|  4|  1|  0| 10|  0|  0|  0|  1|  0|  1|  0|  1|  0|  4|  9|
  * ------------------------------------------------------------------------------------------------------------
  *  texts / all => NST_TEXTS_ALL
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z |
  * A | 10|120|117| 75| 13| 47| 79| 57| 45|  3| 39|270|101|387|  3| 55|  1|273|134|253|203| 24| 11|  7| 21| 14|
  * B | 89| 14|  1|  2|257|  3| 10|  5| 68|  1|  2| 71|  1|  2| 45|  1|  0| 66| 24|  9| 42|  1|  3|  0|  5|  9|
  * C | 70|  0|  7|  1| 52|  1|  1|663| 35|  0|109| 20|  1|  1| 79|  0|  1| 25|  3| 28| 21|  0|  0|  0|  8|  1|
  * D | 72|  5|  1| 12|267|  4|  9|  6|132|  1|  1| 22|  5|  9| 48|  1|  1| 49| 25|  9| 52|  3|  6|  0|  8|  2|
  * E | 69| 74| 86|166| 49| 56| 93| 67|360|  2| 46|302| 91|874| 24| 44|  5|998|320|163| 56| 26| 37| 32| 18| 20|
  * F | 63|  3|  1|  1|120| 42|  9|  2| 62|  1|  2| 55|  1|  2| 48|  1|  0| 59| 12| 53| 28|  1|  2|  0|  2|  9|
  * G | 62|  3|  1|  2|414|  2| 17| 18| 62|  1| 17| 45|  5| 18| 27|  1|  0| 81| 88| 18| 36|  1|  2|  0|  6|  4|
  * H |174|  7|  1|  2|316|  6|  9|  3|148|  1| 12| 70| 26| 47| 90|  2|  1| 78| 33| 93| 38|  1| 28|  0| 20|  9|
  * I | 85| 36|200| 68|310| 50|166|  7|  1|  2| 52|139| 69|525|142| 31|  3| 62|314|261| 18| 48|  2|  4|  2| 36|
  * J | 22|  0|  0|  1| 14|  0|  0|  0|  2|  0|  1|  1|  0|  0| 14|  0|  0|  1|  1|  0| 14|  0|  0|  0|  0|  0|
  * K | 92|  3|  1|  1|151|  2|  4|  4| 42|  1|  4| 40|  2| 15| 81|  1|  1| 47| 18| 58| 37|  1|  3|  0|  4|  5|
  * L |215| 31|  7| 46|321| 21| 15|  7|286|  1| 18|169| 17| 27|115| 10|  1|  5| 39| 81| 64| 10|  6|  0| 62| 13|
  * M |167| 26|  2|  3|192|  4|  7|  3|126|  1|  3|  7| 69|  4| 74| 55|  0|  2| 18| 10| 46|  1|  2|  0|  6|  6|
  * N |183| 35| 45|232|257| 43|343| 27|153|  4| 65| 24| 12| 73| 70| 12|  2| 16|182|229| 37| 17| 18|  1|  8| 69|
  * O | 14| 32| 47| 41| 11| 20| 59| 19| 15|  2| 20|130| 84|288| 28| 59|  1|244| 99| 72| 61| 20| 24|  8|  5| 10|
  * P | 78|  1|  1|  1|114| 37|  1| 55| 59|  0|  1| 44|  1|  1| 68| 42|  0| 97| 15| 22| 28|  1|  1|  0|  5|  1|
  * Q |  1|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 33|  0|  0|  0|  0|  0|
  * R |318| 65| 41| 64|413| 42| 73| 27|282|  2| 61| 45| 70| 74|169| 23|  1| 52|124|157|112| 19| 26|  1| 27| 44|
  * S |111| 29|320|  8|254| 20| 30| 54|152|  1| 36| 30| 42| 15| 60| 95|  3| 13|226|416| 53| 12| 19|  0| 18| 21|
  * T |177| 14| 10|  2|465| 11| 18| 98|342|  1|  7| 36| 10|  9|113|  5|  1|170| 98|105| 77|  4| 13|  0| 27| 60|
  * U | 23| 40| 48| 31| 43| 71| 39| 13| 22|  1| 21| 82| 96|266|  5| 30|  1|145|218| 82|  1|  4|  7|  2|  1| 13|
  * V | 32|  1|  0|  1|144|  1|  1|  0| 52|  0|  1|  1|  1|  0| 50|  1|  0|  1|  1|  1|  2|  1|  1|  0|  1|  1|
  * W | 69|  1|  1|  1|107|  1|  1|  6| 67|  1|  1|  3|  1|  5| 26|  1|  0|  3|  3|  1| 10|  0|  1|  0|  2|  1|
  * X |  4|  1|  2|  0|  6|  1|  1|  1| 11|  0|  1|  1|  1|  1|  2|  7|  0|  0|  1|  7|  1|  1|  1|  0|  1|  1|
  * Y | 11|  3|  6|  6| 10|  1|  2|  1|  4|  1|  2| 13| 10| 16|  5| 12|  0|  9| 17|  7|  1|  1|  1|  1|  1|  1|
  * Z | 21|  3|  1|  1|101|  1|  2|  1| 56|  1|  2|  3|  1|  1| 14|  1|  0|  1|  3| 10|150|  1| 15|  0|  4|  4|
  * ------------------------------------------------------------------------------------------------------------
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
const uint32_t nameFUM[6][29][29] = {
	/* ---------------------------------- *
	 * --- names / de => NST_NAMES_DE --- *
	 * ---------------------------------- */
	{
		{ /* Rules following letter "A" */
			/* A : */ 0xc00e3438, /* ( |M|E) - D, E, F, K, M, N, R, S, T */
			/* B : */ 0xc1164913, /* ( |M|E) - A, B, E, I, L, O, R, S, U, Y */
			/* C : */ 0xe0114d95, /* (B|M|E) - A, C, E, H, I, K, L, O, Q, U */
			/* D : */ 0xe35e5b99, /* (B|M|E) - A, D, E, H, I, J, L, M, O, R, S, T, U, W, Y, Z */
			/* E : */ 0xc00e381c, /* ( |M|E) - C, D, E, L, M, N, R, S, T */
			/* F : */ 0xc0184131, /* ( |M|E) - A, E, F, I, O, T, U */
			/* G : */ 0xe01279d9, /* (B|M|E) - A, D, E, G, H, I, L, M, N, O, R, U */
			/* H : */ 0xe04e7f9b, /* (B|M|E) - A, B, D, E, H, I, J, K, L, M, N, O, R, S, T, W */
			/* I : */ 0xe20efe7f, /* (B|M|E) - A, B, C, D, E, F, G, J, K, L, M, N, O, P, R, S, T, Z */
			/* J : */ 0xc0127d19, /* ( |M|E) - A, D, E, I, K, L, M, N, O, R, U */
			/* K : */ 0xc0144591, /* ( |M|E) - A, E, H, I, K, O, S, U */
			/* L : */ 0xe378dfbf, /* (B|M|E) - A, B, C, D, E, F, H, I, J, K, L, M, O, P, T, U, V, W, Y, Z */
			/* M : */ 0xe214d99f, /* (B|M|E) - A, B, C, D, E, H, I, L, M, O, P, S, U, Z */
			/* N : */ 0xe33eefff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, N, O, P, R, S, T, U, V, Y, Z */
			/* O : */ 0xc0141840, /* ( |M|E) - G, L, M, S, U */
			/* P : */ 0xc00a8891, /* ( |M|E) - A, E, H, L, P, R, T */
			/* Q : */ 0xc0100001, /* ( |M|E) - A, U */
			/* R : */ 0xe37effdf, /* (B|M|E) - A, B, C, D, E, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z */
			/* S : */ 0xc23ffdbd, /* ( |M|E) - A, C, D, E, F, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, Z */
			/* T : */ 0xc31e5f91, /* ( |M|E) - A, E, H, I, J, K, L, M, O, R, S, T, U, Y, Z */
			/* U : */ 0xe20e3c49, /* (B|M|E) - A, D, G, K, L, M, N, R, S, T, Z */
			/* V : */ 0xc0324d51, /* ( |M|E) - A, E, G, I, K, L, O, R, U, V */
			/* W : */ 0xc2586111, /* ( |M|E) - A, E, I, N, O, T, U, W, Z */
			/* X : */ 0xc0000110, /* ( |M|E) - E, I */
			/* Y : */ 0xc11e7d3b, /* ( |M|E) - A, B, D, E, F, I, K, L, M, N, O, R, S, T, U, Y */
			/* Z : */ 0xc2005951, /* ( |M|E) - A, E, G, I, L, M, O, Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "B" */
			/* A : */ 0xe13e2fcf, /* (B|M|E) - A, B, C, D, G, H, I, J, K, L, N, R, S, T, U, V, Y */
			/* B : */ 0x41004111, /* ( |M| ) - A, E, I, O, Y */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe11e2fcd, /* (B|M|E) - A, C, D, G, H, I, J, K, L, N, R, S, T, U, Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40000101, /* ( |M| ) - A, I */
			/* I : */ 0xe12e6ed7, /* (B|M|E) - A, B, C, E, G, H, J, K, L, N, O, R, S, T, V, Y */
			/* J : */ 0x20000000, /* (B| | ) - None are allowed */
			/* K : */ 0x40000010, /* ( |M| ) - E */
			/* L : */ 0xe0000111, /* (B|M|E) - A, E, I */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x40000110, /* ( |M| ) - E, I */
			/* O : */ 0xe31e7bcb, /* (B|M|E) - A, B, D, G, H, I, J, L, M, N, O, R, S, T, U, Y, Z */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U, Y */
			/* S : */ 0x80000000, /* ( | |E) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x600e0c0a, /* (B|M| ) - B, D, K, L, R, S, T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0000800, /* ( |M|E) - L */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x20020000, /* (B| | ) - R */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "C" */
			/* A : */ 0xe32ebfda, /* (B|M|E) - B, D, E, G, H, I, J, K, L, M, N, P, R, S, T, V, Y, Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40004081, /* ( |M| ) - A, H, O */
			/* D : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* E : */ 0xe32ebc2e, /* (B|M|E) - B, C, D, F, K, L, M, N, P, R, S, T, V, Y, Z */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe15e5519, /* (B|M|E) - A, D, E, I, K, M, O, R, S, T, U, W, Y */
			/* I : */ 0xe03efcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, O, P, R, S, T, U, V */
			/* J : */ 0x40000001, /* ( |M| ) - A */
			/* K : */ 0xc1044991, /* ( |M|E) - A, E, H, I, L, O, S, Y */
			/* L : */ 0x60104111, /* (B|M| ) - A, E, I, O, U */
			/* M : */ 0x40000110, /* ( |M| ) - E, I */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe0167902, /* (B|M|E) - B, I, L, M, N, O, R, S, U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0x61004111, /* (B|M| ) - A, E, I, O, Y */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x40004001, /* ( |M| ) - A, O */
			/* U : */ 0xe0063022, /* (B|M|E) - B, F, M, N, R, S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0022000, /* (B|M|E) - N, R */
			/* Z : */ 0x61000010, /* (B|M| ) - E, Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "D" */
			/* A : */ 0xe17e3dca, /* (B|M|E) - B, D, G, H, I, K, L, M, N, R, S, T, U, V, W, Y */
			/* B : */ 0x40100010, /* ( |M| ) - E, U */
			/* C : */ 0x40000080, /* ( |M| ) - H */
			/* D : */ 0x41004111, /* ( |M| ) - A, E, I, O, Y */
			/* E : */ 0xe23e3dfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, R, S, T, U, V, Z */
			/* F : */ 0x40020000, /* ( |M| ) - R */
			/* G : */ 0x40100011, /* ( |M| ) - A, E, U */
			/* H : */ 0xc0020111, /* ( |M|E) - A, E, I, R */
			/* I : */ 0xe33f7e5f, /* (B|M|E) - A, B, C, D, E, G, J, K, L, M, N, O, Q, R, S, T, U, V, Y, Z */
			/* J : */ 0xe0104111, /* (B|M|E) - A, E, I, O, U */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0xc0000111, /* ( |M|E) - A, E, I */
			/* M : */ 0x40100101, /* ( |M| ) - A, I, U */
			/* N : */ 0x40000011, /* ( |M| ) - A, E */
			/* O : */ 0xe0767e53, /* (B|M|E) - A, B, E, G, J, K, L, M, N, O, R, S, U, V, W */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U, Y */
			/* S : */ 0xc0000010, /* ( |M|E) - E */
			/* T : */ 0xc0020100, /* ( |M|E) - I, R */
			/* U : */ 0xe2063999, /* (B|M|E) - A, D, E, H, I, L, M, N, R, S, Z */
			/* V : */ 0x40000101, /* ( |M| ) - A, I */
			/* W : */ 0x40004101, /* ( |M| ) - A, I, O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0040001, /* ( |M|E) - A, S */
			/* Z : */ 0x60000111, /* (B|M| ) - A, E, I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "E" */
			/* A : */ 0xc01a2848, /* ( |M|E) - D, G, L, N, R, T, U */
			/* B : */ 0xe0026593, /* (B|M|E) - A, B, E, H, I, K, N, O, R */
			/* C : */ 0xe2185d99, /* (B|M|E) - A, D, E, H, I, K, L, M, O, T, U, Z */
			/* D : */ 0xe37e7fdb, /* (B|M|E) - A, B, D, E, G, H, I, J, K, L, M, N, O, R, S, T, U, V, W, Y, Z */
			/* E : */ 0xe00e3c08, /* (B|M|E) - D, K, L, M, N, R, S, T */
			/* F : */ 0xe00e4d35, /* (B|M|E) - A, C, E, F, I, K, L, O, R, S, T */
			/* G : */ 0xe05259f3, /* (B|M|E) - A, B, E, F, G, H, I, L, M, O, R, U, W */
			/* H : */ 0xe31e3913, /* (B|M|E) - A, B, E, I, L, M, N, R, S, T, U, Y, Z */
			/* I : */ 0xe20e7eff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, R, S, T, Z */
			/* J : */ 0xe0087909, /* (B|M|E) - A, D, I, L, M, N, O, T */
			/* K : */ 0xe00e5d15, /* (B|M|E) - A, C, E, I, K, L, M, O, R, S, T */
			/* L : */ 0xe37cffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, S, T, U, V, W, Y, Z */
			/* M : */ 0xe316dd9b, /* (B|M|E) - A, B, D, E, H, I, K, L, M, O, P, R, S, U, Y, Z */
			/* N : */ 0xe33e6fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, N, O, R, S, T, U, V, Y, Z */
			/* O : */ 0xc002a42c, /* ( |M|E) - C, D, F, K, N, P, R */
			/* P : */ 0xe000c191, /* (B|M|E) - A, E, H, I, O, P */
			/* Q : */ 0x00100000, /* ( | | ) - U */
			/* R : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z */
			/* S : */ 0xe31efd9f, /* (B|M|E) - A, B, C, D, E, H, I, K, L, M, N, O, P, R, S, T, U, Y, Z */
			/* T : */ 0xe25e5bf7, /* (B|M|E) - A, B, C, E, F, G, H, I, J, L, M, O, R, S, T, U, W, Z */
			/* U : */ 0xe20e2ce8, /* (B|M|E) - D, F, G, H, K, L, N, R, S, T, Z */
			/* V : */ 0xe30e4ddd, /* (B|M|E) - A, C, D, E, G, H, I, K, L, O, R, S, T, Y, Z */
			/* W : */ 0xe0000151, /* (B|M|E) - A, E, G, I */
			/* X : */ 0xc0000081, /* ( |M|E) - A, H */
			/* Y : */ 0xe32c7dbd, /* (B|M|E) - A, C, D, E, F, H, I, K, L, M, N, O, S, T, V, Y, Z */
			/* Z : */ 0xe002011d, /* (B|M|E) - A, C, D, E, I, R */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "F" */
			/* A : */ 0xe37e3d9f, /* (B|M|E) - A, B, C, D, E, H, I, K, L, M, N, R, S, T, U, V, W, Y, Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe12e389c, /* (B|M|E) - C, D, E, H, L, M, N, R, S, T, V, Y */
			/* F : */ 0xc0024111, /* ( |M|E) - A, E, I, O, R */
			/* G : */ 0x40000001, /* ( |M| ) - A */
			/* H : */ 0x40000001, /* ( |M| ) - A */
			/* I : */ 0xe3077e5d, /* (B|M|E) - A, C, D, E, G, J, K, L, M, N, O, Q, R, S, Y, Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000001, /* ( |M| ) - A */
			/* L : */ 0x60004101, /* (B|M| ) - A, I, O */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x40000100, /* ( |M| ) - I */
			/* O : */ 0xe01e6c0c, /* (B|M|E) - C, D, K, L, N, O, R, S, T, U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xe1004111, /* (B|M|E) - A, E, I, O, Y */
			/* S : */ 0x40080001, /* ( |M| ) - A, T */
			/* T : */ 0xc0000191, /* ( |M|E) - A, E, H, I */
			/* U : */ 0x60022c11, /* (B|M| ) - A, E, K, L, N, R */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "G" */
			/* A : */ 0xe33e3b3a, /* (B|M|E) - B, D, E, F, I, J, L, M, N, R, S, T, U, V, Y, Z */
			/* B : */ 0x40000010, /* ( |M| ) - E */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* E : */ 0xe32e799a, /* (B|M|E) - B, D, E, H, I, L, M, N, O, R, S, T, V, Y, Z */
			/* F : */ 0x40020000, /* ( |M| ) - R */
			/* G : */ 0x41000111, /* ( |M| ) - A, E, I, Y */
			/* H : */ 0xe0004111, /* (B|M|E) - A, E, I, O */
			/* I : */ 0xe33ef81f, /* (B|M|E) - A, B, C, D, E, L, M, N, O, P, R, S, T, U, V, Y, Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U, Y */
			/* M : */ 0x40104001, /* ( |M| ) - A, O, U */
			/* N : */ 0x41004111, /* ( |M| ) - A, E, I, O, Y */
			/* O : */ 0xe00e380a, /* (B|M|E) - B, D, L, M, N, R, S, T */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60104151, /* (B|M| ) - A, E, G, I, O, U */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x40020000, /* ( |M| ) - R */
			/* U : */ 0xe22e3918, /* (B|M|E) - D, E, I, L, M, N, R, S, T, V, Z */
			/* V : */ 0x40000001, /* ( |M| ) - A */
			/* W : */ 0x60000111, /* (B|M| ) - A, E, I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xa0000800, /* (B| |E) - L */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "H" */
			/* A : */ 0xe37ebfff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, P, R, S, T, U, V, W, Y, Z */
			/* B : */ 0x40104001, /* ( |M| ) - A, O, U */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* E : */ 0xe37e7d9f, /* (B|M|E) - A, B, C, D, E, H, I, K, L, M, N, O, R, S, T, U, V, W, Y, Z */
			/* F : */ 0x40000010, /* ( |M| ) - E */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40004000, /* ( |M| ) - O */
			/* I : */ 0xe30efcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, O, P, R, S, T, Y, Z */
			/* J : */ 0x40004001, /* ( |M| ) - A, O */
			/* K : */ 0x40000001, /* ( |M| ) - A */
			/* L : */ 0xc0000191, /* ( |M|E) - A, E, H, I */
			/* M : */ 0x40105111, /* ( |M| ) - A, E, I, M, O, U */
			/* N : */ 0xc1040111, /* ( |M|E) - A, E, I, S, Y */
			/* O : */ 0xe016fc1a, /* (B|M|E) - B, D, E, K, L, M, N, O, P, R, S, U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xc1004199, /* ( |M|E) - A, D, E, H, I, O, Y */
			/* S : */ 0xc0000191, /* ( |M|E) - A, E, H, I */
			/* T : */ 0xc0000191, /* ( |M|E) - A, E, H, I */
			/* U : */ 0xe006385b, /* (B|M|E) - A, B, D, E, G, L, M, N, R, S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000001, /* ( |M| ) - A */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0040011, /* (B|M|E) - A, E, S */
			/* Z : */ 0x40000001, /* ( |M| ) - A */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "I" */
			/* A : */ 0xe20e3d4c, /* (B|M|E) - C, D, G, I, K, L, M, N, R, S, T, Z */
			/* B : */ 0xe1124113, /* (B|M|E) - A, B, E, I, O, R, U, Y */
			/* C : */ 0xe21a4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, R, T, U, Z */
			/* D : */ 0xe15a6b99, /* (B|M|E) - A, D, E, H, I, J, L, N, O, R, T, U, W, Y */
			/* E : */ 0xc07e3ccf, /* ( |M|E) - A, B, C, D, G, H, K, L, M, N, R, S, T, U, V, W */
			/* F : */ 0xe20a4131, /* (B|M|E) - A, E, F, I, O, R, T, Z */
			/* G : */ 0xe01a79fb, /* (B|M|E) - A, B, D, E, F, G, H, I, L, M, N, O, R, T, U */
			/* H : */ 0xe0066111, /* (B|M|E) - A, E, I, N, O, R, S */
			/* I : */ 0xc0002000, /* ( |M|E) - N */
			/* J : */ 0xe0004411, /* (B|M|E) - A, E, K, O */
			/* K : */ 0xe01e5d91, /* (B|M|E) - A, E, H, I, K, L, M, O, R, S, T, U */
			/* L : */ 0xe17edffb, /* (B|M|E) - A, B, D, E, F, G, H, I, J, K, L, M, O, P, R, S, T, U, V, W, Y */
			/* M : */ 0xe012d5bb, /* (B|M|E) - A, B, D, E, F, H, I, K, M, O, P, R, U */
			/* N : */ 0xe31e77ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, M, N, O, R, S, T, U, Y, Z */
			/* O : */ 0xe03ea84f, /* (B|M|E) - A, B, C, D, G, L, N, P, R, S, T, U, V */
			/* P : */ 0xe002c111, /* (B|M|E) - A, E, I, O, P, R */
			/* Q : */ 0xc0100000, /* ( |M|E) - U */
			/* R : */ 0xe37e77dd, /* (B|M|E) - A, C, D, E, G, H, I, J, K, M, N, O, R, S, T, U, V, W, Y, Z */
			/* S : */ 0xe01eddb7, /* (B|M|E) - A, B, C, E, F, H, I, K, L, M, O, P, R, S, T, U */
			/* T : */ 0xe31e47d1, /* (B|M|E) - A, E, G, H, I, J, K, O, R, S, T, U, Y, Z */
			/* U : */ 0xc0040802, /* ( |M|E) - B, L, S */
			/* V : */ 0xe0004511, /* (B|M|E) - A, E, I, K, O */
			/* W : */ 0x60004001, /* (B|M| ) - A, O */
			/* X : */ 0xc0080010, /* ( |M|E) - E, T */
			/* Y : */ 0xe0005111, /* (B|M|E) - A, E, I, M, O */
			/* Z : */ 0xe2205191, /* (B|M|E) - A, E, H, I, M, O, V, Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "J" */
			/* A : */ 0xe36f3def, /* (B|M|E) - A, B, C, D, F, G, H, I, K, L, M, N, Q, R, S, T, V, W, Y, Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40000100, /* ( |M| ) - I */
			/* D : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* E : */ 0xe20ebcb9, /* (B|M|E) - A, D, E, F, H, K, L, M, N, P, R, S, T, Z */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x40000001, /* ( |M| ) - A */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe00e380e, /* (B|M|E) - B, C, D, L, M, N, R, S, T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40004001, /* ( |M| ) - A, O */
			/* L : */ 0x40000101, /* ( |M| ) - A, I */
			/* M : */ 0x40000101, /* ( |M| ) - A, I */
			/* N : */ 0x40000001, /* ( |M| ) - A */
			/* O : */ 0xe3366cbf, /* (B|M|E) - A, B, C, D, E, F, H, K, L, N, O, R, S, U, V, Y, Z */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60001111, /* (B|M| ) - A, E, I, M */
			/* S : */ 0x80000000, /* ( | |E) - None are allowed */
			/* T : */ 0x40100000, /* ( |M| ) - U */
			/* U : */ 0xe00e2c9b, /* (B|M|E) - A, B, D, E, H, K, L, N, R, S, T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "K" */
			/* A : */ 0xe34ebbfb, /* (B|M|E) - A, B, D, E, F, G, H, I, J, L, M, N, P, R, S, T, W, Y, Z */
			/* B : */ 0x40000011, /* ( |M| ) - A, E */
			/* C : */ 0x40000001, /* ( |M| ) - A */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe32e3d83, /* (B|M|E) - A, B, H, I, K, L, M, N, R, S, T, V, Y, Z */
			/* F : */ 0x40020000, /* ( |M| ) - R */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe00c4111, /* (B|M|E) - A, E, I, O, S, T */
			/* I : */ 0xe32ebe3f, /* (B|M|E) - A, B, C, D, E, F, J, K, L, M, N, P, R, S, T, V, Y, Z */
			/* J : */ 0x60000010, /* (B|M| ) - E */
			/* K : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* L : */ 0x60004111, /* (B|M| ) - A, E, I, O */
			/* M : */ 0x40000011, /* ( |M| ) - A, E */
			/* N : */ 0x60104001, /* (B|M| ) - A, O, U */
			/* O : */ 0xe176e83e, /* (B|M|E) - B, C, D, E, F, L, N, O, P, R, S, U, V, W, Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x63100111, /* (B|M| ) - A, E, I, U, Y, Z */
			/* S : */ 0xe0004111, /* (B|M|E) - A, E, I, O */
			/* T : */ 0xc0004011, /* ( |M|E) - A, E, O */
			/* U : */ 0xe00ebc0a, /* (B|M|E) - B, D, K, L, M, N, P, R, S, T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x60000101, /* (B|M| ) - A, I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xa0020000, /* (B| |E) - R */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "L" */
			/* A : */ 0xe37e7fef, /* (B|M|E) - A, B, C, D, F, G, H, I, J, K, L, M, N, O, R, S, T, U, V, W, Y, Z */
			/* B : */ 0x40124111, /* ( |M| ) - A, E, I, O, R, U */
			/* C : */ 0x40104191, /* ( |M| ) - A, E, H, I, O, U */
			/* D : */ 0xc01a4113, /* ( |M|E) - A, B, E, I, O, R, T, U */
			/* E : */ 0xe1fe7fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, R, S, T, U, V, W, X, Y */
			/* F : */ 0xc00241d0, /* ( |M|E) - E, G, H, I, O, R */
			/* G : */ 0x40124111, /* ( |M| ) - A, E, I, O, R, U */
			/* H : */ 0xc0000111, /* ( |M|E) - A, E, I */
			/* I : */ 0xe3befeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, R, S, T, U, V, X, Y, Z */
			/* J : */ 0x60104511, /* (B|M| ) - A, E, I, K, O, U */
			/* K : */ 0xc04051b1, /* ( |M|E) - A, E, F, H, I, M, O, W */
			/* L : */ 0xc11251b1, /* ( |M|E) - A, E, F, H, I, M, O, R, U, Y */
			/* M : */ 0xc0184111, /* ( |M|E) - A, E, I, O, T, U */
			/* N : */ 0x40000101, /* ( |M| ) - A, I */
			/* O : */ 0xe17ef96a, /* (B|M|E) - B, D, F, G, I, L, M, N, O, P, R, S, T, U, V, W, Y */
			/* P : */ 0x40100090, /* ( |M| ) - E, H, U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40100101, /* ( |M| ) - A, I, U */
			/* S : */ 0xc0184517, /* ( |M|E) - A, B, C, E, I, K, O, T, U */
			/* T : */ 0xc0024391, /* ( |M|E) - A, E, H, I, J, O, R */
			/* U : */ 0xe20ebd3e, /* (B|M|E) - B, C, D, E, F, I, K, L, M, N, P, R, S, T, Z */
			/* V : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* W : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0003509, /* (B|M|E) - A, D, I, K, M, N */
			/* Z : */ 0xc0000001, /* ( |M|E) - A */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "M" */
			/* A : */ 0xe3be3fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, R, S, T, U, V, X, Y, Z */
			/* B : */ 0x40124011, /* ( |M| ) - A, E, O, R, U */
			/* C : */ 0x60000080, /* (B|M| ) - H */
			/* D : */ 0x40104101, /* ( |M| ) - A, I, O, U */
			/* E : */ 0xe32e7dfd, /* (B|M|E) - A, C, D, E, F, G, H, I, K, L, M, N, O, R, S, T, V, Y, Z */
			/* F : */ 0x40020000, /* ( |M| ) - R */
			/* G : */ 0x40000001, /* ( |M| ) - A */
			/* H : */ 0x40000101, /* ( |M| ) - A, I */
			/* I : */ 0xe30e7edd, /* (B|M|E) - A, C, D, E, G, H, J, K, L, M, N, O, R, S, T, Y, Z */
			/* J : */ 0x40000001, /* ( |M| ) - A */
			/* K : */ 0x40000010, /* ( |M| ) - E */
			/* L : */ 0x40000101, /* ( |M| ) - A, I */
			/* M : */ 0xc1104111, /* ( |M|E) - A, E, I, O, U, Y */
			/* N : */ 0x40000100, /* ( |M| ) - I */
			/* O : */ 0xe01e7fd8, /* (B|M|E) - D, E, G, H, I, J, K, L, M, N, O, R, S, T, U */
			/* P : */ 0x40064111, /* ( |M| ) - A, E, I, O, R, S */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40100111, /* ( |M| ) - A, E, I, U */
			/* S : */ 0xc0104191, /* ( |M|E) - A, E, H, I, O, U */
			/* T : */ 0x40020001, /* ( |M| ) - A, R */
			/* U : */ 0xe20e3edb, /* (B|M|E) - A, B, D, E, G, H, J, K, L, M, N, R, S, T, Z */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xa0020000, /* (B| |E) - R */
			/* Z : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "N" */
			/* A : */ 0xe37fbfff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, P, Q, R, S, T, U, V, W, Y, Z */
			/* B : */ 0x40000011, /* ( |M| ) - A, E */
			/* C : */ 0xc1104591, /* ( |M|E) - A, E, H, I, K, O, U, Y */
			/* D : */ 0xc11a4911, /* ( |M|E) - A, E, I, L, O, R, T, U, Y */
			/* E : */ 0xe33effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, Y, Z */
			/* F : */ 0x40020110, /* ( |M| ) - E, I, R */
			/* G : */ 0xc0765993, /* ( |M|E) - A, B, E, H, I, L, M, O, R, S, U, V, W */
			/* H : */ 0x40004101, /* ( |M| ) - A, I, O */
			/* I : */ 0xe31f7eff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, Q, R, S, T, U, Y, Z */
			/* J : */ 0x40004011, /* ( |M| ) - A, E, O */
			/* K : */ 0xc0425011, /* ( |M|E) - A, E, M, O, R, W */
			/* L : */ 0x40100101, /* ( |M| ) - A, I, U */
			/* M : */ 0x40000011, /* ( |M| ) - A, E */
			/* N : */ 0xc11041d1, /* ( |M|E) - A, E, G, H, I, O, U, Y */
			/* O : */ 0xe13e7833, /* (B|M|E) - A, B, E, F, L, M, N, O, R, S, T, U, V, Y */
			/* P : */ 0x40000010, /* ( |M| ) - E */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x41000111, /* ( |M| ) - A, E, I, Y */
			/* S : */ 0xc05cc7f1, /* ( |M|E) - A, E, F, G, H, I, J, K, O, P, S, T, U, W */
			/* T : */ 0xc2164791, /* ( |M|E) - A, E, H, I, J, K, O, R, S, U, Z */
			/* U : */ 0xe00e2891, /* (B|M|E) - A, E, H, L, N, R, S, T */
			/* V : */ 0x40000010, /* ( |M| ) - E */
			/* W : */ 0x40000001, /* ( |M| ) - A */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0040011, /* ( |M|E) - A, E, S */
			/* Z : */ 0xc0004b11, /* ( |M|E) - A, E, I, J, L, O */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "O" */
			/* A : */ 0xc0032884, /* ( |M|E) - C, H, L, N, Q, R */
			/* B : */ 0xe1164993, /* (B|M|E) - A, B, E, H, I, L, O, R, S, U, Y */
			/* C : */ 0x60080595, /* (B|M| ) - A, C, E, H, I, K, T */
			/* D : */ 0xe1424331, /* (B|M|E) - A, E, F, I, J, O, R, W, Y */
			/* E : */ 0xe2023808, /* (B|M|E) - D, L, M, N, R, Z */
			/* F : */ 0xc0024921, /* ( |M|E) - A, F, I, L, O, R */
			/* G : */ 0x60124911, /* (B|M| ) - A, E, I, L, O, R, U */
			/* H : */ 0x40063019, /* ( |M| ) - A, D, E, M, N, R, S */
			/* I : */ 0xc0040404, /* ( |M|E) - C, K, S */
			/* J : */ 0x40020d15, /* ( |M| ) - A, C, E, I, K, L, R */
			/* K : */ 0xe01a5d95, /* (B|M|E) - A, C, E, H, I, K, L, M, O, R, T, U */
			/* L : */ 0xe17edf7f, /* (B|M|E) - A, B, C, D, E, F, G, I, J, K, L, M, O, P, R, S, T, U, V, W, Y */
			/* M : */ 0xe112f513, /* (B|M|E) - A, B, E, I, K, M, N, O, P, R, U, Y */
			/* N : */ 0xe31e67dd, /* (B|M|E) - A, C, D, E, G, H, I, J, K, N, O, R, S, T, U, Y, Z */
			/* O : */ 0x4206ac0a, /* ( |M| ) - B, D, K, L, N, P, R, S, Z */
			/* P : */ 0xe0004191, /* (B|M|E) - A, E, H, I, O */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xe15efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W, Y */
			/* S : */ 0xe15edd97, /* (B|M|E) - A, B, C, E, H, I, K, L, M, O, P, R, S, T, U, W, Y */
			/* T : */ 0xe04e43f1, /* (B|M|E) - A, E, F, G, H, I, J, O, R, S, T, W */
			/* U : */ 0xe20e3def, /* (B|M|E) - A, B, C, D, F, G, H, I, K, L, M, N, R, S, T, Z */
			/* V : */ 0xe0000111, /* (B|M|E) - A, E, I */
			/* W : */ 0xc0000111, /* ( |M|E) - A, E, I */
			/* X : */ 0x40000001, /* ( |M| ) - A */
			/* Y : */ 0xc0000015, /* ( |M|E) - A, C, E */
			/* Z : */ 0xe0044511, /* (B|M|E) - A, E, I, K, O, S */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "P" */
			/* A : */ 0xe13efc84, /* (B|M|E) - C, H, K, L, M, N, O, P, R, S, T, U, V, Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe10aac44, /* (B|M|E) - C, G, K, L, N, P, R, T, Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe0022111, /* (B|M|E) - A, E, I, N, R */
			/* I : */ 0xe00ae81d, /* (B|M|E) - A, C, D, E, L, N, O, P, R, T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000010, /* ( |M| ) - E */
			/* L : */ 0x60000101, /* (B|M| ) - A, I */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe0069800, /* (B|M|E) - L, M, P, R, S */
			/* P : */ 0xc0004111, /* ( |M|E) - A, E, I, O */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60004111, /* (B|M| ) - A, E, I, O */
			/* S : */ 0x40004000, /* ( |M| ) - O */
			/* T : */ 0x40000101, /* ( |M| ) - A, I */
			/* U : */ 0x60020000, /* (B|M| ) - R */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x40020000, /* ( |M| ) - R */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Q" */
			/* A : */ 0x60000008, /* (B|M| ) - D */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60000111, /* (B|M| ) - A, E, I */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "R" */
			/* A : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z */
			/* B : */ 0xc0124911, /* ( |M|E) - A, E, I, L, O, R, U */
			/* C : */ 0xc1104591, /* ( |M|E) - A, E, H, I, K, O, U, Y */
			/* D : */ 0xc11e5311, /* ( |M|E) - A, E, I, J, M, O, R, S, T, U, Y */
			/* E : */ 0xe3fe7dff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, R, S, T, U, V, W, X, Y, Z */
			/* F : */ 0xc0020001, /* ( |M|E) - A, R */
			/* G : */ 0xc1124991, /* ( |M|E) - A, E, H, I, L, O, R, U, Y */
			/* H : */ 0x60104111, /* (B|M| ) - A, E, I, O, U */
			/* I : */ 0xe39dfeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, Q, S, T, U, X, Y, Z */
			/* J : */ 0x40004011, /* ( |M| ) - A, E, O */
			/* K : */ 0xc0524591, /* ( |M|E) - A, E, H, I, K, O, R, U, W */
			/* L : */ 0xc11441b1, /* ( |M|E) - A, E, F, H, I, O, S, U, Y */
			/* M : */ 0xc01c09f1, /* ( |M|E) - A, E, F, G, H, I, L, S, T, U */
			/* N : */ 0xc10c41b9, /* ( |M|E) - A, D, E, F, H, I, O, S, T, Y */
			/* O : */ 0xe3fe7ddf, /* (B|M|E) - A, B, C, D, E, G, H, I, K, L, M, N, O, R, S, T, U, V, W, X, Y, Z */
			/* P : */ 0x40004081, /* ( |M| ) - A, H, O */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x41104111, /* ( |M| ) - A, E, I, O, U, Y */
			/* S : */ 0xc0184195, /* ( |M|E) - A, C, E, H, I, O, T, U */
			/* T : */ 0xc3525b91, /* ( |M|E) - A, E, H, I, J, L, M, O, R, U, W, Y, Z */
			/* U : */ 0xe34ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, W, Y, Z */
			/* V : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* W : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0042c1d, /* (B|M|E) - A, C, D, E, K, L, N, S */
			/* Z : */ 0xc1004111, /* ( |M|E) - A, E, I, O, Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "S" */
			/* A : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z */
			/* B : */ 0x40000010, /* ( |M| ) - E */
			/* C : */ 0x60104181, /* (B|M| ) - A, H, I, O, U */
			/* D : */ 0x40100101, /* ( |M| ) - A, I, U */
			/* E : */ 0xe33ebfff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, P, R, S, T, U, V, Y, Z */
			/* F : */ 0x40020111, /* ( |M| ) - A, E, I, R */
			/* G : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* H : */ 0xe018c111, /* (B|M|E) - A, E, I, O, P, T, U */
			/* I : */ 0xe1beffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, X, Y */
			/* J : */ 0x40004000, /* ( |M| ) - O */
			/* K : */ 0xc0104111, /* ( |M|E) - A, E, I, O, U */
			/* L : */ 0xe0104111, /* (B|M|E) - A, E, I, O, U */
			/* M : */ 0x60104111, /* (B|M| ) - A, E, I, O, U */
			/* N : */ 0x60000111, /* (B|M| ) - A, E, I */
			/* O : */ 0xe11efdf2, /* (B|M|E) - B, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, Y */
			/* P : */ 0x61004111, /* (B|M| ) - A, E, I, O, Y */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0x40104111, /* ( |M| ) - A, E, I, O, U */
			/* S : */ 0xc1004911, /* ( |M|E) - A, E, I, L, O, Y */
			/* T : */ 0xe1125bb1, /* (B|M|E) - A, E, F, H, I, J, L, M, O, R, U, Y */
			/* U : */ 0xe22ebdbf, /* (B|M|E) - A, B, C, D, E, F, H, I, K, L, M, N, P, R, S, T, V, Z */
			/* V : */ 0x60000011, /* (B|M| ) - A, E */
			/* W : */ 0x60000111, /* (B|M| ) - A, E, I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0040902, /* (B|M|E) - B, I, L, S */
			/* Z : */ 0xe0080011, /* (B|M|E) - A, E, T */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "T" */
			/* A : */ 0xe37ebfee, /* (B|M|E) - B, C, D, F, G, H, I, J, K, L, M, N, P, R, S, T, U, V, W, Y, Z */
			/* B : */ 0x40100010, /* ( |M| ) - E, U */
			/* C : */ 0x40000080, /* ( |M| ) - H */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z */
			/* F : */ 0x40020100, /* ( |M| ) - I, R */
			/* G : */ 0x40000011, /* ( |M| ) - A, E */
			/* H : */ 0xe1125911, /* (B|M|E) - A, E, I, L, M, O, R, U, Y */
			/* I : */ 0xe31efe7f, /* (B|M|E) - A, B, C, D, E, F, G, J, K, L, M, N, O, P, R, S, T, U, Y, Z */
			/* J : */ 0x60004011, /* (B|M| ) - A, E, O */
			/* K : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* L : */ 0xc0104111, /* ( |M|E) - A, E, I, O, U */
			/* M : */ 0x40100011, /* ( |M| ) - A, E, U */
			/* N : */ 0x40000001, /* ( |M| ) - A */
			/* O : */ 0xe056be2a, /* (B|M|E) - B, D, F, J, K, L, M, N, P, R, S, U, W */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xe1104111, /* (B|M|E) - A, E, I, O, U, Y */
			/* S : */ 0xe0004115, /* (B|M|E) - A, C, E, I, O */
			/* T : */ 0xc10059b1, /* ( |M|E) - A, E, F, H, I, L, M, O, Y */
			/* U : */ 0xe0062869, /* (B|M|E) - A, D, F, G, L, N, R, S */
			/* V : */ 0x40000101, /* ( |M| ) - A, I */
			/* W : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0002800, /* ( |M|E) - L, N */
			/* Z : */ 0xc0000111, /* ( |M|E) - A, E, I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "U" */
			/* A : */ 0xc00a3818, /* ( |M|E) - D, E, L, M, N, R, T */
			/* B : */ 0xe0004993, /* (B|M|E) - A, B, E, H, I, L, O */
			/* C : */ 0xe1020791, /* (B|M|E) - A, E, H, I, J, K, R, Y */
			/* D : */ 0xe15a59d9, /* (B|M|E) - A, D, E, G, H, I, L, M, O, R, T, U, W, Y */
			/* E : */ 0xe20e2c00, /* (B|M|E) - K, L, N, R, S, T, Z */
			/* F : */ 0xc0100131, /* ( |M|E) - A, E, F, I, U */
			/* G : */ 0xe0124911, /* (B|M|E) - A, E, I, L, O, R, U */
			/* H : */ 0xc00c6109, /* ( |M|E) - A, D, I, N, O, S, T */
			/* I : */ 0xc00e2848, /* ( |M|E) - D, G, L, N, R, S, T */
			/* J : */ 0x40000101, /* ( |M| ) - A, I */
			/* K : */ 0xe0024191, /* (B|M|E) - A, E, H, I, O, R */
			/* L : */ 0xe13e4fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, O, R, S, T, U, V, Y */
			/* M : */ 0xe01a5113, /* (B|M|E) - A, B, E, I, M, O, R, T, U */
			/* N : */ 0xe21863dd, /* (B|M|E) - A, C, D, E, G, H, I, J, N, O, T, U, Z */
			/* O : */ 0xc0000800, /* ( |M|E) - L */
			/* P : */ 0xc002c110, /* ( |M|E) - E, I, O, P, R */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0xe31e7ddf, /* (B|M|E) - A, B, C, D, E, G, H, I, K, L, M, N, O, R, S, T, U, Y, Z */
			/* S : */ 0xe21ef995, /* (B|M|E) - A, C, E, H, I, L, M, N, O, P, R, S, T, U, Z */
			/* T : */ 0xe26c09f5, /* (B|M|E) - A, C, E, F, G, H, I, L, S, T, V, W, Z */
			/* U : */ 0x60040800, /* (B|M| ) - L, S */
			/* V : */ 0x40000011, /* ( |M| ) - A, E */
			/* W : */ 0x60000011, /* (B|M| ) - A, E */
			/* X : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Y : */ 0xe0000041, /* (B|M|E) - A, G */
			/* Z : */ 0xe0100191, /* (B|M|E) - A, E, H, I, U */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "V" */
			/* A : */ 0xe00e39bc, /* (B|M|E) - C, D, E, F, H, I, L, M, N, R, S, T */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40000101, /* ( |M| ) - A, I */
			/* D : */ 0x40000101, /* ( |M| ) - A, I */
			/* E : */ 0xe30e2dbd, /* (B|M|E) - A, C, D, E, F, H, I, K, L, N, R, S, T, Y, Z */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x40000100, /* ( |M| ) - I */
			/* H : */ 0x40000010, /* ( |M| ) - E */
			/* I : */ 0xe33e7e5d, /* (B|M|E) - A, C, D, E, G, J, K, L, M, N, O, R, S, T, U, V, Y, Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* L : */ 0x60100101, /* (B|M| ) - A, I, U */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe0023a00, /* (B|M|E) - J, L, M, N, R */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40104101, /* ( |M| ) - A, I, O, U */
			/* S : */ 0x40000100, /* ( |M| ) - I */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60060000, /* (B|M| ) - R, S */
			/* V : */ 0x40000001, /* ( |M| ) - A */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x40000800, /* ( |M| ) - L */
			/* Z : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "W" */
			/* A : */ 0xe30e29bc, /* (B|M|E) - C, D, E, F, H, I, L, N, R, S, T, Y, Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe10e2948, /* (B|M|E) - D, G, I, L, N, R, S, T, Y */
			/* F : */ 0x40000100, /* ( |M| ) - I */
			/* G : */ 0x40000010, /* ( |M| ) - E */
			/* H : */ 0x60000001, /* (B|M| ) - A */
			/* I : */ 0xe00e2cdf, /* (B|M|E) - A, B, C, D, E, G, H, K, L, N, R, S, T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x20000001, /* (B| | ) - A */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x80000000, /* ( | |E) - None are allowed */
			/* O : */ 0xe0003a22, /* (B|M|E) - B, F, J, L, M, N */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x80000000, /* ( | |E) - None are allowed */
			/* U : */ 0x60000800, /* (B|M| ) - L */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000001, /* ( |M| ) - A */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x40000100, /* ( |M| ) - I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "X" */
			/* A : */ 0x60202000, /* (B|M| ) - N, V */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x40002000, /* ( |M| ) - N */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xc0001000, /* ( |M|E) - M */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Y" */
			/* A : */ 0xe37ebcdc, /* (B|M|E) - C, D, E, G, H, K, L, M, N, P, R, S, T, U, V, W, Y, Z */
			/* B : */ 0x40020101, /* ( |M| ) - A, I, R */
			/* C : */ 0x40000011, /* ( |M| ) - A, E */
			/* D : */ 0xc0004111, /* ( |M|E) - A, E, I, O */
			/* E : */ 0xe02e38aa, /* (B|M|E) - B, D, F, H, L, M, N, R, S, T, V */
			/* F : */ 0x40100110, /* ( |M| ) - E, I, U */
			/* G : */ 0x40100100, /* ( |M| ) - I, U */
			/* H : */ 0x40001001, /* ( |M| ) - A, M */
			/* I : */ 0xe00a3c48, /* (B|M|E) - D, G, K, L, M, N, R, T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc0000011, /* ( |M|E) - A, E */
			/* L : */ 0xe1200d11, /* (B|M|E) - A, E, I, K, L, V, Y */
			/* M : */ 0x40104011, /* ( |M| ) - A, E, O, U */
			/* N : */ 0xc0006119, /* ( |M|E) - A, D, E, I, N, O */
			/* O : */ 0xe21e2000, /* (B|M|E) - N, R, S, T, U, Z */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40184111, /* ( |M| ) - A, E, I, O, T, U */
			/* S : */ 0xe2080911, /* (B|M|E) - A, E, I, L, T, Z */
			/* T : */ 0x40184000, /* ( |M| ) - O, T, U */
			/* U : */ 0x60062012, /* (B|M| ) - B, E, N, R, S */
			/* V : */ 0x60004011, /* (B|M| ) - A, E, O */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x40000001, /* ( |M| ) - A */
			/* Z : */ 0x40100000, /* ( |M| ) - U */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Z" */
			/* A : */ 0xe10a3fee, /* (B|M|E) - B, C, D, F, G, H, I, J, K, L, M, N, R, T, Y */
			/* B : */ 0x60000110, /* (B|M| ) - E, I */
			/* C : */ 0x40000001, /* ( |M| ) - A */
			/* D : */ 0x62020111, /* (B|M| ) - A, E, I, R, Z */
			/* E : */ 0xe12e3faa, /* (B|M|E) - B, D, F, H, I, J, K, L, M, N, R, S, T, V, Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x40000110, /* ( |M| ) - E, I */
			/* H : */ 0x60000001, /* (B|M| ) - A */
			/* I : */ 0xe12efeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, R, S, T, V, Y */
			/* J : */ 0x40004000, /* ( |M| ) - O */
			/* K : */ 0x40100001, /* ( |M| ) - A, U */
			/* L : */ 0xe0100111, /* (B|M|E) - A, E, I, U */
			/* M : */ 0x40000110, /* ( |M| ) - E, I */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe0122180, /* (B|M|E) - H, I, N, R, U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40000001, /* ( |M| ) - A */
			/* S : */ 0x40000011, /* ( |M| ) - A, E */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60022882, /* (B|M| ) - B, H, L, N, R */
			/* V : */ 0x60004101, /* (B|M| ) - A, I, O */
			/* W : */ 0x20000001, /* (B| | ) - A */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0042840, /* (B|M|E) - G, L, N, S */
			/* Z : */ 0x40000011, /* ( |M| ) - A, E */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ä" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40000002, /* ( |M| ) - B */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ö" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ü" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		}
	}, /* End of Enum 0 / 5 */
	/* ---------------------------------- *
	 * --- names / en => NST_NAMES_EN --- *
	 * ---------------------------------- */
	{
		{ /* Rules following letter "A" */
			/* A : */ 0x60020000, /* (B|M| ) - R,  */
			/* B : */ 0xe116491b, /* (B|M|E) - A, B, D, E, I, L, O, R, S, U and Y */
			/* C : */ 0xe1014d91, /* (B|M|E) - A, E, H, I, K, L, O, Q and Y */
			/* D : */ 0xe1567d7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, R, S, U, W and Y */
			/* E : */ 0xe04e2cec, /* (B|M|E) - C, D, F, G, H, K, L, N, R, S, T and W */
			/* F : */ 0x60084031, /* (B|M| ) - A, E, F, O and T */
			/* G : */ 0x601069d9, /* (B|M| ) - A, D, E, G, H, I, L, N, O and U */
			/* H : */ 0xe0105111, /* (B|M|E) - A, E, I, M, O and U */
			/* I : */ 0xe10e3c5b, /* (B|M|E) - A, B, D, E, G, K, L, M, N, R, S, T and Y */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xe0004d11, /* (B|M|E) - A, E, I, K, L and O */
			/* L : */ 0xe17cddff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, S, T, U, V, W and Y */
			/* M : */ 0xe316d913, /* (B|M|E) - A, B, E, I, L, M, O, P, R, S, U, Y and Z */
			/* N : */ 0xe15e6fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, N, O, R, S, T, U, W and Y */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x60029881, /* (B|M| ) - A, H, L, M, P and R */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe17fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W and Y */
			/* S : */ 0xe00cdd95, /* (B|M|E) - A, C, E, H, I, K, L, M, O, P, S and T */
			/* T : */ 0xe15e4db5, /* (B|M|E) - A, C, E, F, H, I, K, L, O, R, S, T, U, W and Y */
			/* U : */ 0xe08e284e, /* (B|M|E) - B, C, D, G, L, N, R, S, T and X */
			/* V : */ 0x61024111, /* (B|M| ) - A, E, I, O, R and Y */
			/* W : */ 0xc10c6931, /* ( |M|E) - A, E, F, I, L, N, O, S, T and Y */
			/* X : */ 0xe0084100, /* (B|M|E) - I, O and T */
			/* Y : */ 0xe06e78bf, /* (B|M|E) - A, B, C, D, E, F, H, L, M, N, O, R, S, T, V and W */
			/* Z : */ 0x42001810, /* ( |M| ) - E, L, M and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "B" */
			/* A : */ 0xe1063d42, /* (B|M|E) - B, G, I, K, L, M, N, R, S and Y */
			/* B : */ 0x41004110, /* ( |M| ) - E, I, O and Y */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe1ae7c1d, /* (B|M|E) - A, C, D, E, K, L, M, N, O, R, S, T, V, X and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe00e685d, /* (B|M|E) - A, C, D, E, G, L, N, O, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe05e690e, /* (B|M|E) - B, C, D, I, L, N, O, R, S, T, U and W */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0x80000000, /* ( | |E) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x600e201c, /* (B|M| ) - C, D, E, N, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0022800, /* (B|M|E) - L, N and R */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "C" */
			/* A : */ 0xe11e39ba, /* (B|M|E) - B, D, E, F, H, I, L, M, N, R, S, T, U and Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40000001, /* ( |M| ) - A,  */
			/* D : */ 0x40004000, /* ( |M| ) - O,  */
			/* E : */ 0xe10e281d, /* (B|M|E) - A, C, D, E, L, N, R, S, T and Y */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x80000000, /* ( | |E) - None are allowed */
			/* H : */ 0xe11e5911, /* (B|M|E) - A, E, I, L, M, O, R, S, T, U and Y */
			/* I : */ 0xe11e7815, /* (B|M|E) - A, C, E, L, M, N, O, R, S, T, U and Y */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc14479b1, /* ( |M|E) - A, E, F, H, I, L, M, N, O, S, W and Y */
			/* L : */ 0x61000111, /* (B|M| ) - A, E, I and Y */
			/* M : */ 0x40000001, /* ( |M| ) - A,  */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe17efc5a, /* (B|M|E) - B, D, E, G, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0x40004000, /* ( |M| ) - O,  */
			/* T : */ 0x40004001, /* ( |M| ) - A and O */
			/* U : */ 0x610e1808, /* (B|M| ) - D, L, M, R, S, T and Y */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000110, /* ( |M| ) - E and I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xa0023008, /* (B| |E) - D, M, N and R */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "D" */
			/* A : */ 0xe1e6b9d4, /* (B|M|E) - C, E, G, H, I, L, M, N, P, R, S, V, W, X and Y */
			/* B : */ 0x41124010, /* ( |M| ) - E, O, R, U and Y */
			/* C : */ 0x40000800, /* ( |M| ) - L,  */
			/* D : */ 0xc1124111, /* ( |M|E) - A, E, I, O, R, U and Y */
			/* E : */ 0xe12e7c57, /* (B|M|E) - A, B, C, E, G, K, L, M, N, O, R, S, T, V and Y */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x40000011, /* ( |M| ) - A and E */
			/* H : */ 0x40000101, /* ( |M| ) - A and I */
			/* I : */ 0xe09e7c55, /* (B|M|E) - A, C, E, G, K, L, M, N, O, R, S, T, U and X */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000100, /* ( |M| ) - I,  */
			/* L : */ 0x41000011, /* ( |M| ) - A, E and Y */
			/* M : */ 0x40104001, /* ( |M| ) - A, O and U */
			/* N : */ 0x40000110, /* ( |M| ) - E and I */
			/* O : */ 0xe05a380d, /* (B|M|E) - A, C, D, L, M, N, R, T, U and W */
			/* P : */ 0x40000001, /* ( |M| ) - A,  */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc0404091, /* ( |M|E) - A, E, H, O and W */
			/* T : */ 0x40100000, /* ( |M| ) - U,  */
			/* U : */ 0x60063807, /* (B|M| ) - A, B, C, L, M, N, R and S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00c2804, /* (B|M|E) - C, L, N, S and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "E" */
			/* A : */ 0xe07ebcce, /* (B|M|E) - B, C, D, G, H, K, L, M, N, P, R, S, T, U, V and W */
			/* B : */ 0xe0124813, /* (B|M|E) - A, B, E, L, O, R and U */
			/* C : */ 0xc00a0dd5, /* ( |M|E) - A, C, E, G, H, I, K, L, R and T */
			/* D : */ 0xe156597b, /* (B|M|E) - A, B, D, E, F, G, I, L, M, O, R, S, U, W and Y */
			/* E : */ 0xc02e382d, /* ( |M|E) - A, C, D, F, L, M, N, R, S, T and V */
			/* F : */ 0xe0024131, /* (B|M|E) - A, E, F, I, O and R */
			/* G : */ 0xe00849d3, /* (B|M|E) - A, B, E, G, H, I, L, O and T */
			/* H : */ 0xc0004011, /* ( |M|E) - A, E and O */
			/* I : */ 0x600e386c, /* (B|M| ) - C, D, F, G, L, M, N, R, S and T */
			/* J : */ 0x40000001, /* ( |M| ) - A,  */
			/* K : */ 0xc0000911, /* ( |M|E) - A, E, I and L */
			/* L : */ 0xe17e59bf, /* (B|M|E) - A, B, C, D, E, F, H, I, L, M, O, R, S, T, U, V, W and Y */
			/* M : */ 0xe100d113, /* (B|M|E) - A, B, E, I, M, O, P and Y */
			/* N : */ 0xe34e6bff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, L, N, O, R, S, T, W, Y and Z */
			/* O : */ 0xe066386a, /* (B|M|E) - B, D, F, G, L, M, N, R, S, V and W */
			/* P : */ 0x40000080, /* ( |M| ) - H,  */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe16e7dff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, R, S, T, V, W and Y */
			/* S : */ 0xe04cd99d, /* (B|M|E) - A, C, D, E, H, I, L, M, O, P, S, T and W */
			/* T : */ 0xe05c4195, /* (B|M|E) - A, C, E, H, I, O, S, T, U and W */
			/* U : */ 0xe086a840, /* (B|M|E) - G, L, N, P, R, S and X */
			/* V : */ 0xe1024111, /* (B|M|E) - A, E, I, O, R and Y */
			/* W : */ 0xe1044191, /* (B|M|E) - A, E, H, I, O, S and Y */
			/* X : */ 0xc1180901, /* ( |M|E) - A, I, L, T, U and Y */
			/* Y : */ 0xc00c3811, /* ( |M|E) - A, E, L, M, N, S and T */
			/* Z : */ 0xe0000010, /* (B|M|E) - E,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "F" */
			/* A : */ 0xe1722912, /* (B|M|E) - B, E, I, L, N, R, U, V, W and Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40004000, /* ( |M| ) - O,  */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe00a2801, /* (B|M|E) - A, L, N, R and T */
			/* F : */ 0xc10a4111, /* ( |M|E) - A, E, I, O, R, T and Y */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x600c2810, /* (B|M| ) - E, L, N, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe0422800, /* (B|M|E) - L, N, R and W */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0xc0104000, /* ( |M|E) - O and U */
			/* U : */ 0xe0000844, /* (B|M|E) - C, G and L */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "G" */
			/* A : */ 0xe17e2902, /* (B|M|E) - B, I, L, N, R, S, T, U, V, W and Y */
			/* B : */ 0x41000010, /* ( |M| ) - E and Y */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* E : */ 0xe04e6993, /* (B|M|E) - A, B, E, H, I, L, N, O, R, S, T and W */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x41000110, /* ( |M| ) - E, I and Y */
			/* H : */ 0xc0084913, /* ( |M|E) - A, B, E, I, L, O and T */
			/* I : */ 0xe00c683b, /* (B|M|E) - A, B, D, E, F, L, N, O, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61004011, /* (B|M| ) - A, E, O and Y */
			/* M : */ 0x40100001, /* ( |M| ) - A and U */
			/* N : */ 0x40000011, /* ( |M| ) - A and E */
			/* O : */ 0xe01a6808, /* (B|M|E) - D, L, N, O, R, T and U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* S : */ 0xc0080000, /* ( |M|E) - T,  */
			/* T : */ 0x40104000, /* ( |M| ) - O and U */
			/* U : */ 0x61042111, /* (B|M| ) - A, E, I, N, S and Y */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x60000110, /* (B|M| ) - E and I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xa0000800, /* (B| |E) - L,  */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "H" */
			/* A : */ 0xe37eb97c, /* (B|M|E) - C, D, E, F, G, I, L, M, N, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x41104010, /* ( |M| ) - E, O, U and Y */
			/* C : */ 0x40000800, /* ( |M| ) - L,  */
			/* D : */ 0x40100010, /* ( |M| ) - E and U */
			/* E : */ 0xe15ef91f, /* (B|M|E) - A, B, C, D, E, I, L, M, N, O, P, R, S, T, U, W and Y */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40000001, /* ( |M| ) - A,  */
			/* I : */ 0xe00ef85f, /* (B|M|E) - A, B, C, D, E, G, L, M, N, O, P, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* M : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* N : */ 0xc0002011, /* ( |M|E) - A, E and N */
			/* O : */ 0xe052b818, /* (B|M|E) - D, E, L, M, N, P, R, U and W */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* S : */ 0x80000000, /* ( | |E) - None are allowed */
			/* T : */ 0xc1104191, /* ( |M|E) - A, E, H, I, O, U and Y */
			/* U : */ 0x608e3859, /* (B|M| ) - A, D, E, G, L, M, N, R, S, T and X */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x61000111, /* (B|M| ) - A, E, I and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0000801, /* (B|M|E) - A and L */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "I" */
			/* A : */ 0xe01e38a0, /* (B|M|E) - F, H, L, M, N, R, S, T and U */
			/* B : */ 0x41024013, /* ( |M| ) - A, B, E, O, R and Y */
			/* C : */ 0xc0184591, /* ( |M|E) - A, E, H, I, K, O, T and U */
			/* D : */ 0xe1446959, /* (B|M|E) - A, D, E, G, I, L, N, O, S, W and Y */
			/* E : */ 0xc03e3c2c, /* ( |M|E) - C, D, F, K, L, M, N, R, S, T, U and V */
			/* F : */ 0xc01a4130, /* ( |M|E) - E, F, I, O, R, T and U */
			/* G : */ 0xe01071d3, /* (B|M|E) - A, B, E, G, H, I, M, N, O and U */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x40000001, /* ( |M| ) - A,  */
			/* K : */ 0xc0004591, /* ( |M|E) - A, E, H, I, K and O */
			/* L : */ 0xe16c3d3f, /* (B|M|E) - A, B, C, D, E, F, I, K, L, M, N, S, T, V, W and Y */
			/* M : */ 0xe0005113, /* (B|M|E) - A, B, E, I, M and O */
			/* N : */ 0xe24c6d7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, N, O, S, T, W and Z */
			/* O : */ 0xe02e2844, /* (B|M|E) - C, G, L, N, R, S, T and V */
			/* P : */ 0xc00c8010, /* ( |M|E) - E, P, S and T */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe0684d5d, /* (B|M|E) - A, C, D, E, G, I, K, L, O, T, V and W */
			/* S : */ 0xe14c5db7, /* (B|M|E) - A, B, C, E, F, H, I, K, L, M, O, S, T, W and Y */
			/* T : */ 0xe30879b7, /* (B|M|E) - A, B, C, E, F, H, I, L, M, N, O, T, Y and Z */
			/* U : */ 0xc0040000, /* ( |M|E) - S,  */
			/* V : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* W : */ 0x40000010, /* ( |M| ) - E,  */
			/* X : */ 0xc0004100, /* ( |M|E) - I and O */
			/* Y : */ 0x60000021, /* (B|M| ) - A and F */
			/* Z : */ 0xe2000013, /* (B|M|E) - A, B, E and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "J" */
			/* A : */ 0xe3a73ddc, /* (B|M|E) - C, D, E, G, H, I, K, L, M, N, Q, R, S, V, X, Y and Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x600e7021, /* (B|M| ) - A, F, M, N, O, R, S and T */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xa0003800, /* (B| |E) - L, M and N */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe126289d, /* (B|M|E) - A, C, D, E, H, L, N, R, S, V and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x20042808, /* (B| | ) - D, L, N and S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "K" */
			/* A : */ 0xe10eb99c, /* (B|M|E) - C, D, E, H, I, L, M, N, P, R, S, T and Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe12e7919, /* (B|M|E) - A, D, E, I, L, M, N, O, R, S, T, V and Y */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x60000111, /* (B|M| ) - A, E and I */
			/* I : */ 0xe00eb811, /* (B|M|E) - A, E, L, M, N, P, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* L : */ 0x61000011, /* (B|M| ) - A, E and Y */
			/* M : */ 0x40000001, /* ( |M| ) - A,  */
			/* N : */ 0x60004010, /* (B|M| ) - E and O */
			/* O : */ 0xe0160802, /* (B|M|E) - B, L, R, S and U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x21000100, /* (B| | ) - I and Y */
			/* S : */ 0xc0084080, /* ( |M|E) - H, O and T */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60020000, /* (B|M| ) - R,  */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0022810, /* (B|M|E) - E, L, N and R */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "L" */
			/* A : */ 0xe17ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W and Y */
			/* B : */ 0x41124011, /* ( |M| ) - A, E, O, R, U and Y */
			/* C : */ 0x40004090, /* ( |M| ) - E, H and O */
			/* D : */ 0xc15e4111, /* ( |M|E) - A, E, I, O, R, S, T, U, W and Y */
			/* E : */ 0xe1fe7b5f, /* (B|M|E) - A, B, C, D, E, G, I, J, L, M, N, O, R, S, T, U, V, W, X and Y */
			/* F : */ 0xc042411d, /* ( |M|E) - A, C, D, E, I, O, R and W */
			/* G : */ 0x40000090, /* ( |M| ) - E and H */
			/* H : */ 0xc00a0099, /* ( |M|E) - A, D, E, H, R and T */
			/* I : */ 0xe3bcea7f, /* (B|M|E) - A, B, C, D, E, F, G, J, L, N, O, P, S, T, U, V, X, Y and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc0000010, /* ( |M|E) - E,  */
			/* L : */ 0xc1565139, /* ( |M|E) - A, D, E, F, I, M, O, R, S, U, W and Y */
			/* M : */ 0xc0004111, /* ( |M|E) - A, E, I and O */
			/* N : */ 0x40000010, /* ( |M| ) - E,  */
			/* O : */ 0xe17e7d5c, /* (B|M|E) - C, D, E, G, I, K, L, M, N, O, R, S, T, U, V, W and Y */
			/* P : */ 0x40000080, /* ( |M| ) - H,  */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40004110, /* ( |M| ) - E, I and O */
			/* S : */ 0xc1684193, /* ( |M|E) - A, B, E, H, I, O, T, V, W and Y */
			/* T : */ 0xc0124190, /* ( |M|E) - E, H, I, O, R and U */
			/* U : */ 0xe02a9d0c, /* (B|M|E) - C, D, I, K, L, M, P, R, T and V */
			/* V : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* W : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* X : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Y : */ 0xe22e383f, /* (B|M|E) - A, B, C, D, E, F, L, M, N, R, S, T, V and Z */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "M" */
			/* A : */ 0xe1be3dde, /* (B|M|E) - B, C, D, E, G, H, I, K, L, M, N, R, S, T, U, V, X and Y */
			/* B : */ 0xc0020911, /* ( |M|E) - A, E, I, L and R */
			/* C : */ 0x20000c04, /* (B| | ) - C, K and L */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe08e7c5d, /* (B|M|E) - A, C, D, E, G, K, L, M, N, O, R, S, T and X */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe00e3c15, /* (B|M|E) - A, C, E, K, L, M, N, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000100, /* ( |M| ) - I,  */
			/* L : */ 0x41000110, /* ( |M| ) - E, I and Y */
			/* M : */ 0xc1000111, /* ( |M|E) - A, E, I and Y */
			/* N : */ 0x40000010, /* ( |M| ) - E,  */
			/* O : */ 0xe01e69c0, /* (B|M|E) - G, H, I, L, N, O, R, S, T and U */
			/* P : */ 0xc00c0010, /* ( |M|E) - E, S and T */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x41004110, /* ( |M| ) - E, I, O and Y */
			/* S : */ 0x40084088, /* ( |M| ) - D, H, O and T */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60062000, /* (B|M| ) - N, R and S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00e2805, /* (B|M|E) - A, C, L, N, R, S and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "N" */
			/* A : */ 0xe12e399e, /* (B|M|E) - B, C, D, E, H, I, L, M, N, R, S, T, V and Y */
			/* B : */ 0x41124010, /* ( |M| ) - E, O, R, U and Y */
			/* C : */ 0xc1004991, /* ( |M|E) - A, E, H, I, L, O and Y */
			/* D : */ 0xc1064999, /* ( |M|E) - A, D, E, H, I, L, O, R, S and Y */
			/* E : */ 0xe17e29ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, N, R, S, T, U, V, W and Y */
			/* F : */ 0x40024110, /* ( |M| ) - E, I, O and R */
			/* G : */ 0xc01e49b9, /* ( |M|E) - A, D, E, F, H, I, L, O, R, S, T and U */
			/* H : */ 0x40004001, /* ( |M| ) - A and O */
			/* I : */ 0xe18d6c75, /* (B|M|E) - A, C, E, F, G, K, L, N, O, Q, S, T, X and Y */
			/* J : */ 0x40000011, /* ( |M| ) - A and E */
			/* K : */ 0xc0040900, /* ( |M|E) - I, L and S */
			/* L : */ 0x41000011, /* ( |M| ) - A, E and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0xc1004911, /* ( |M|E) - A, E, I, L, O and Y */
			/* O : */ 0xe07a2814, /* (B|M|E) - C, E, L, N, R, T, U, V and W */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x41004101, /* ( |M| ) - A, I, O and Y */
			/* S : */ 0xc1484895, /* ( |M|E) - A, C, E, H, L, O, T, W and Y */
			/* T : */ 0xc1564991, /* ( |M|E) - A, E, H, I, L, O, R, S, U, W and Y */
			/* U : */ 0x40000010, /* ( |M| ) - E,  */
			/* V : */ 0x40000010, /* ( |M| ) - E,  */
			/* W : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0044809, /* (B|M|E) - A, D, L, O and S */
			/* Z : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "O" */
			/* A : */ 0xe0012400, /* (B|M|E) - K, N and Q */
			/* B : */ 0xe1100193, /* (B|M|E) - A, B, E, H, I, U and Y */
			/* C : */ 0xe0080c90, /* (B|M|E) - E, H, K, L and T */
			/* D : */ 0xe1425119, /* (B|M|E) - A, D, E, I, M, O, R, W and Y */
			/* E : */ 0xc1402841, /* ( |M|E) - A, G, L, N, W and Y */
			/* F : */ 0x40080020, /* ( |M| ) - F and T */
			/* G : */ 0x60000098, /* (B|M| ) - D, E and H */
			/* H : */ 0x40002001, /* ( |M| ) - A and N */
			/* I : */ 0xc0062000, /* ( |M|E) - N, R and S */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xe0040811, /* (B|M|E) - A, E, L and S */
			/* L : */ 0xe13cfd3f, /* (B|M|E) - A, B, C, D, E, F, I, K, L, M, N, O, P, S, T, U, V and Y */
			/* M : */ 0xe0009d13, /* (B|M|E) - A, B, E, I, K, L, M and P */
			/* N : */ 0xe30e615b, /* (B|M|E) - A, B, D, E, G, I, N, O, R, S, T, Y and Z */
			/* O : */ 0x400eac08, /* ( |M| ) - D, K, L, N, P, R, S and T */
			/* P : */ 0xc0040090, /* ( |M|E) - E, H and S */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xe16effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, V, W and Y */
			/* S : */ 0xe17e1997, /* (B|M|E) - A, B, C, E, H, I, L, M, R, S, T, U, V, W and Y */
			/* T : */ 0xc0080191, /* ( |M|E) - A, E, H, I and T */
			/* U : */ 0xe00e2d7c, /* (B|M|E) - C, D, E, F, G, I, K, L, N, R, S and T */
			/* V : */ 0x40000011, /* ( |M| ) - A and E */
			/* W : */ 0xe100a91b, /* (B|M|E) - A, B, D, E, I, L, N, P and Y */
			/* X : */ 0xe0082021, /* (B|M|E) - A, F, N and T */
			/* Y : */ 0xc000401d, /* ( |M|E) - A, C, D, E and O */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "P" */
			/* A : */ 0x619e1910, /* (B|M| ) - E, I, L, M, R, S, T, U, X and Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe00a3959, /* (B|M|E) - A, D, E, G, I, L, M, N, R and T */
			/* F : */ 0x20000010, /* (B| | ) - E,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe1024111, /* (B|M|E) - A, E, I, O, R and Y */
			/* I : */ 0x6008a014, /* (B|M| ) - C, E, N, P and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x60000011, /* (B|M| ) - A and E */
			/* M : */ 0x40000001, /* ( |M| ) - A,  */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x60168900, /* (B|M| ) - I, L, P, R, S and U */
			/* P : */ 0xc0000110, /* ( |M|E) - E and I */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc1000080, /* ( |M|E) - H and Y */
			/* T : */ 0x40004000, /* ( |M| ) - O,  */
			/* U : */ 0x20000000, /* (B| | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40004000, /* ( |M| ) - O,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x20000000, /* (B| | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Q" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60000191, /* (B|M| ) - A, E, H and I */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "R" */
			/* A : */ 0xe17eb9fe, /* (B|M|E) - B, C, D, E, F, G, H, I, L, M, N, P, R, S, T, U, V, W and Y */
			/* B : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* C : */ 0x41104990, /* ( |M| ) - E, H, I, L, O, U and Y */
			/* D : */ 0xc1445991, /* ( |M|E) - A, E, H, I, L, M, O, S, W and Y */
			/* E : */ 0xe1ee7d7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, R, S, T, V, W, X and Y */
			/* F : */ 0x40004100, /* ( |M| ) - I and O */
			/* G : */ 0x40124191, /* ( |M| ) - A, E, H, I, O, R and U */
			/* H : */ 0xe0084911, /* (B|M|E) - A, E, I, L, O and T */
			/* I : */ 0xe29dfc5f, /* (B|M|E) - A, B, C, D, E, G, K, L, M, N, O, P, Q, S, T, U, X and Z */
			/* J : */ 0x40004100, /* ( |M| ) - I and O */
			/* K : */ 0xc1440994, /* ( |M|E) - C, E, H, I, L, S, W and Y */
			/* L : */ 0xc1044113, /* ( |M|E) - A, B, E, I, O, S and Y */
			/* M : */ 0xc1144111, /* ( |M|E) - A, E, I, O, S, U and Y */
			/* N : */ 0xc10c4999, /* ( |M|E) - A, D, E, H, I, L, O, S, T and Y */
			/* O : */ 0xe1fefc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, V, W, X and Y */
			/* P : */ 0xc0004010, /* ( |M|E) - E and O */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xc1044111, /* ( |M|E) - A, E, I, O, S and Y */
			/* S : */ 0xc0084195, /* ( |M|E) - A, C, E, H, I, O and T */
			/* T : */ 0xc1566991, /* ( |M|E) - A, E, H, I, L, N, O, R, S, U, W and Y */
			/* U : */ 0x600cb1fe, /* (B|M| ) - B, C, D, E, F, G, H, I, M, N, P, S and T */
			/* V : */ 0xc1004111, /* ( |M|E) - A, E, I, O and Y */
			/* W : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00cec5d, /* (B|M|E) - A, C, D, E, G, K, L, N, O, P, S and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "S" */
			/* A : */ 0xe1f6397f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, R, S, U, V, W, X and Y */
			/* B : */ 0x41104011, /* ( |M| ) - A, E, O, U and Y */
			/* C : */ 0xe04049b3, /* (B|M|E) - A, B, E, F, H, I, L, O and W */
			/* D : */ 0x40000011, /* ( |M| ) - A and E */
			/* E : */ 0xe1eeb87f, /* (B|M|E) - A, B, C, D, E, F, G, L, M, N, P, R, S, T, V, W, X and Y */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe14a4933, /* (B|M|E) - A, B, E, F, I, L, O, R, T, W and Y */
			/* I : */ 0xe00e385b, /* (B|M|E) - A, B, D, E, G, L, M, N, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x61000111, /* (B|M| ) - A, E, I and Y */
			/* L : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* M : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe012b802, /* (B|M|E) - B, L, M, N, P, R and U */
			/* P : */ 0x60024111, /* (B|M| ) - A, E, I, O and R */
			/* Q : */ 0x20100000, /* (B| | ) - U,  */
			/* R : */ 0x40000101, /* ( |M| ) - A and I */
			/* S : */ 0xc1000911, /* ( |M|E) - A, E, I, L and Y */
			/* T : */ 0xe1524917, /* (B|M|E) - A, B, C, E, I, L, O, R, U, W and Y */
			/* U : */ 0x620e3930, /* (B|M| ) - E, F, I, L, M, N, R, S, T and Z */
			/* V : */ 0x40000100, /* ( |M| ) - I,  */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe000380a, /* (B|M|E) - B, D, L, M and N */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "T" */
			/* A : */ 0xe17e39b6, /* (B|M|E) - B, C, E, F, H, I, L, M, N, R, S, T, U, V, W and Y */
			/* B : */ 0x41020000, /* ( |M| ) - R and Y */
			/* C : */ 0x40004080, /* ( |M| ) - H and O */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe166fd39, /* (B|M|E) - A, D, E, F, I, K, L, M, N, O, P, R, S, V, W and Y */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe15e5937, /* (B|M|E) - A, B, C, E, F, I, L, M, O, R, S, T, U, W and Y */
			/* I : */ 0xe00e7875, /* (B|M|E) - A, C, E, F, G, L, M, N, O, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000100, /* ( |M| ) - I,  */
			/* L : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* M : */ 0x40004000, /* ( |M| ) - O,  */
			/* N : */ 0x40000110, /* ( |M| ) - E and I */
			/* O : */ 0xe156bd3e, /* (B|M|E) - B, C, D, E, F, I, K, L, M, N, P, R, S, U, W and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc1004080, /* ( |M|E) - H, O and Y */
			/* T : */ 0xc1146591, /* ( |M|E) - A, E, H, I, K, N, O, S, U and Y */
			/* U : */ 0x6006b805, /* (B|M| ) - A, C, L, M, N, P, R and S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0062851, /* (B|M|E) - A, E, G, L, N, R and S */
			/* Z : */ 0xc0440051, /* ( |M|E) - A, E, G, S and W */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "U" */
			/* A : */ 0xc0022008, /* ( |M|E) - D, N and R */
			/* B : */ 0x40120110, /* ( |M| ) - E, I, R and U */
			/* C : */ 0xc0004591, /* ( |M|E) - A, E, H, I, K and O */
			/* D : */ 0xe1024959, /* (B|M|E) - A, D, E, G, I, L, O, R and Y */
			/* E : */ 0xc20e2812, /* ( |M|E) - B, E, L, N, R, S, T and Z */
			/* F : */ 0x40004020, /* ( |M| ) - F and O */
			/* G : */ 0x40100190, /* ( |M| ) - E, H, I and U */
			/* H : */ 0x40000801, /* ( |M| ) - A and L */
			/* I : */ 0x400e3818, /* ( |M| ) - D, E, L, M, N, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc0000110, /* ( |M|E) - E and I */
			/* L : */ 0xe1ae1973, /* (B|M|E) - A, B, E, F, G, I, L, M, R, S, T, V, X and Y */
			/* M : */ 0xc0003033, /* ( |M|E) - A, B, E, F, M and N */
			/* N : */ 0xe04c295f, /* (B|M|E) - A, B, C, D, E, G, I, L, N, S, T and W */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0xe0408090, /* (B|M|E) - E, H, P and W */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xe14e69dd, /* (B|M|E) - A, C, D, E, G, H, I, L, N, O, R, S, T, W and Y */
			/* S : */ 0xc00c0111, /* ( |M|E) - A, E, I, S and T */
			/* T : */ 0xc0180894, /* ( |M|E) - C, E, H, L, T and U */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0xc0000800, /* ( |M|E) - L,  */
			/* Y : */ 0x40000800, /* ( |M| ) - L,  */
			/* Z : */ 0xc0000001, /* ( |M|E) - A,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "V" */
			/* A : */ 0xe0122808, /* (B|M|E) - D, L, N, R and U */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe10e6810, /* (B|M|E) - E, L, N, O, R, S, T and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe0266c1d, /* (B|M|E) - A, C, D, E, K, L, N, O, R, S and V */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe0022000, /* (B|M|E) - N and R */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x41000110, /* ( |M| ) - E, I and Y */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0002000, /* ( |M|E) - N,  */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "W" */
			/* A : */ 0xe12e2d1c, /* (B|M|E) - C, D, E, I, K, L, N, R, S, T, V and Y */
			/* B : */ 0x40020000, /* ( |M| ) - R,  */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe10e7943, /* (B|M|E) - A, B, G, I, L, M, N, O, R, S, T and Y */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x60000111, /* (B|M| ) - A, E and I */
			/* I : */ 0x600e2c54, /* (B|M| ) - C, E, G, K, L, N, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x41000011, /* ( |M| ) - A, E and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0xc0040111, /* ( |M|E) - A, E, I and S */
			/* O : */ 0x60024808, /* (B|M| ) - D, L, O and R */
			/* P : */ 0x40000010, /* ( |M| ) - E,  */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60000110, /* (B|M| ) - E and I */
			/* S : */ 0x40084000, /* ( |M| ) - O and T */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60000800, /* (B|M| ) - L,  */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x60083d14, /* (B|M| ) - C, E, I, K, L, M, N and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "X" */
			/* A : */ 0xe0002000, /* (B|M|E) - N,  */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe0043011, /* (B|M|E) - A, E, M, N and S */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x40000010, /* ( |M| ) - E,  */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x40000001, /* ( |M| ) - A,  */
			/* O : */ 0x40002000, /* ( |M| ) - N,  */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x40004010, /* ( |M| ) - E and O */
			/* U : */ 0x40040000, /* ( |M| ) - S,  */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000010, /* ( |M| ) - E,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Z : */ 0x00000001, /* ( | | ) - A,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Y" */
			/* A : */ 0xe00e3c88, /* (B|M|E) - D, H, K, L, M, N, R, S and T */
			/* B : */ 0x40104010, /* ( |M| ) - E, O and U */
			/* C : */ 0x40020190, /* ( |M| ) - E, H, I and R */
			/* D : */ 0xc0006158, /* ( |M|E) - D, E, G, I, N and O */
			/* E : */ 0xe00e2809, /* (B|M|E) - A, D, L, N, R, S and T */
			/* F : */ 0xc0004100, /* ( |M|E) - I and O */
			/* G : */ 0xc0000010, /* ( |M|E) - E,  */
			/* H : */ 0x40100010, /* ( |M| ) - E and U */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc0000010, /* ( |M|E) - E,  */
			/* L : */ 0xc1204919, /* ( |M|E) - A, D, E, I, L, O, V and Y */
			/* M : */ 0xc0004011, /* ( |M|E) - A, E and O */
			/* N : */ 0xc24e6139, /* ( |M|E) - A, D, E, F, I, N, O, R, S, T, W and Z */
			/* O : */ 0x60123800, /* (B|M| ) - L, M, N, R and U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xc0586519, /* ( |M|E) - A, D, E, I, K, N, O, T, U and W */
			/* S : */ 0xc00c4195, /* ( |M|E) - A, C, E, H, I, O, S and T */
			/* T : */ 0xc0004891, /* ( |M|E) - A, E, H, L and O */
			/* U : */ 0x20000800, /* (B| | ) - L,  */
			/* V : */ 0x60000110, /* (B|M| ) - E and I */
			/* W : */ 0x40004000, /* ( |M| ) - O,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x40000001, /* ( |M| ) - A,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Z" */
			/* A : */ 0xe1222d0e, /* (B|M|E) - B, C, D, I, K, L, N, R, V and Y */
			/* B : */ 0x40000010, /* ( |M| ) - E,  */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe00a0813, /* (B|M|E) - A, B, E, L, R and T */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x40000100, /* ( |M| ) - I,  */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xc0000010, /* ( |M|E) - E,  */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x41000101, /* ( |M| ) - A, I and Y */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe0000010, /* (B|M|E) - E,  */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x40000100, /* ( |M| ) - I,  */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000001, /* ( |M| ) - A,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Z : */ 0x41000100, /* ( |M| ) - I and Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ä" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ö" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ü" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		}
	}, /* End of Enum 1 / 5 */
	/* ------------------------------------ *
	 * --- names / all => NST_NAMES_ALL --- *
	 * ------------------------------------ */
	{
		{ /* Rules following letter "A" */
			/* A : */ 0xe00e3c3c, /* (B|M|E) - C, D, E, F, K, L, M, N, R, S and T */
			/* B : */ 0xe116491b, /* (B|M|E) - A, B, D, E, I, L, O, R, S, U and Y */
			/* C : */ 0xe1134d95, /* (B|M|E) - A, C, E, H, I, K, L, O, Q, R, U and Y */
			/* D : */ 0xe35e7fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, R, S, T, U, W, Y and Z */
			/* E : */ 0xe04e3cfc, /* (B|M|E) - C, D, E, F, G, H, K, L, M, N, R, S, T and W */
			/* F : */ 0xe01a4131, /* (B|M|E) - A, E, F, I, O, R, T and U */
			/* G : */ 0xe05679d9, /* (B|M|E) - A, D, E, G, H, I, L, M, N, O, R, S, U and W */
			/* H : */ 0xe15e7f9b, /* (B|M|E) - A, B, D, E, H, I, J, K, L, M, N, O, R, S, T, U, W and Y */
			/* I : */ 0xe31efe7f, /* (B|M|E) - A, B, C, D, E, F, G, J, K, L, M, N, O, P, R, S, T, U, Y and Z */
			/* J : */ 0xc0127d19, /* ( |M|E) - A, D, E, I, K, L, M, N, O, R and U */
			/* K : */ 0xe0144d99, /* (B|M|E) - A, D, E, H, I, K, L, O, S and U */
			/* L : */ 0xe37cffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, S, T, U, V, W, Y and Z */
			/* M : */ 0xe31ed99f, /* (B|M|E) - A, B, C, D, E, H, I, L, M, O, P, R, S, T, U, Y and Z */
			/* N : */ 0xe37eefff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, N, O, P, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xc0141840, /* ( |M|E) - G, L, M, S and U */
			/* P : */ 0xe00a9991, /* (B|M|E) - A, E, H, I, L, M, P, R and T */
			/* Q : */ 0xc0100001, /* ( |M|E) - A and U */
			/* R : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* S : */ 0xe23ffdbd, /* (B|M|E) - A, C, D, E, F, H, I, K, L, M, N, O, P, Q, R, S, T, U, V and Z */
			/* T : */ 0xe35e5fb5, /* (B|M|E) - A, C, E, F, H, I, J, K, L, M, O, R, S, T, U, W, Y and Z */
			/* U : */ 0xe28e3c7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, R, S, T, X and Z */
			/* V : */ 0xe1324d51, /* (B|M|E) - A, E, G, I, K, L, O, R, U, V and Y */
			/* W : */ 0xc35e6931, /* ( |M|E) - A, E, F, I, L, N, O, R, S, T, U, W, Y and Z */
			/* X : */ 0xe0484110, /* (B|M|E) - E, I, O, T and W */
			/* Y : */ 0xe17e7dbf, /* (B|M|E) - A, B, C, D, E, F, H, I, K, L, M, N, O, R, S, T, U, V, W and Y */
			/* Z : */ 0xc2105951, /* ( |M|E) - A, E, G, I, L, M, O, U and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "B" */
			/* A : */ 0xe13e3fdf, /* (B|M|E) - A, B, C, D, E, G, H, I, J, K, L, M, N, R, S, T, U, V and Y */
			/* B : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x40100100, /* ( |M| ) - I and U */
			/* E : */ 0xe1be7fdd, /* (B|M|E) - A, C, D, E, G, H, I, J, K, L, M, N, O, R, S, T, U, V, X and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40000101, /* ( |M| ) - A and I */
			/* I : */ 0xe12e6edf, /* (B|M|E) - A, B, C, D, E, G, H, J, K, L, N, O, R, S, T, V and Y */
			/* J : */ 0x20000000, /* (B| | ) - None are allowed */
			/* K : */ 0x40000010, /* ( |M| ) - E,  */
			/* L : */ 0xe1004111, /* (B|M|E) - A, E, I, O and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x40000110, /* ( |M| ) - E and I */
			/* O : */ 0xe35e7bcf, /* (B|M|E) - A, B, C, D, G, H, I, J, L, M, N, O, R, S, T, U, W, Y and Z */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc0080000, /* ( |M|E) - T,  */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x600e2c9e, /* (B|M| ) - B, C, D, E, H, K, L, N, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000100, /* ( |M| ) - I,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0022800, /* (B|M|E) - L, N and R */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "C" */
			/* A : */ 0xe33ebffa, /* (B|M|E) - B, D, E, F, G, H, I, J, K, L, M, N, P, R, S, T, U, V, Y and Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40004081, /* ( |M| ) - A, H and O */
			/* D : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* E : */ 0xe32ebc3f, /* (B|M|E) - A, B, C, D, E, F, K, L, M, N, P, R, S, T, V, Y and Z */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x80000000, /* ( | |E) - None are allowed */
			/* H : */ 0xe15e5d19, /* (B|M|E) - A, D, E, I, K, L, M, O, R, S, T, U, W and Y */
			/* I : */ 0xe13efcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, O, P, R, S, T, U, V and Y */
			/* J : */ 0x40000001, /* ( |M| ) - A,  */
			/* K : */ 0xc14479b3, /* ( |M|E) - A, B, E, F, H, I, L, M, N, O, S, W and Y */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe17efd5a, /* (B|M|E) - B, D, E, G, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0x40004000, /* ( |M| ) - O,  */
			/* T : */ 0xc0104101, /* ( |M|E) - A, I, O and U */
			/* U : */ 0xe10e382a, /* (B|M|E) - B, D, F, L, M, N, R, S, T and Y */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000110, /* ( |M| ) - E and I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0023008, /* (B|M|E) - D, M, N and R */
			/* Z : */ 0x61000010, /* (B|M| ) - E and Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "D" */
			/* A : */ 0xe1febdde, /* (B|M|E) - B, C, D, E, G, H, I, K, L, M, N, P, R, S, T, U, V, W, X and Y */
			/* B : */ 0x41124010, /* ( |M| ) - E, O, R, U and Y */
			/* C : */ 0x40000880, /* ( |M| ) - H and L */
			/* D : */ 0xc1124111, /* ( |M|E) - A, E, I, O, R, U and Y */
			/* E : */ 0xe3be7fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, R, S, T, U, V, X, Y and Z */
			/* F : */ 0x40024000, /* ( |M| ) - O and R */
			/* G : */ 0x40100011, /* ( |M| ) - A, E and U */
			/* H : */ 0xc0020111, /* ( |M|E) - A, E, I and R */
			/* I : */ 0xe3bf7e5f, /* (B|M|E) - A, B, C, D, E, G, J, K, L, M, N, O, Q, R, S, T, U, V, X, Y and Z */
			/* J : */ 0xe0104111, /* (B|M|E) - A, E, I, O and U */
			/* K : */ 0x40000100, /* ( |M| ) - I,  */
			/* L : */ 0xc1000111, /* ( |M|E) - A, E, I and Y */
			/* M : */ 0x40104101, /* ( |M| ) - A, I, O and U */
			/* N : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* O : */ 0xe0fe7e5f, /* (B|M|E) - A, B, C, D, E, G, J, K, L, M, N, O, R, S, T, U, V, W and X */
			/* P : */ 0x40000001, /* ( |M| ) - A,  */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc0404091, /* ( |M|E) - A, E, H, O and W */
			/* T : */ 0xc0120100, /* ( |M|E) - I, R and U */
			/* U : */ 0xe20e3d9f, /* (B|M|E) - A, B, C, D, E, H, I, K, L, M, N, R, S, T and Z */
			/* V : */ 0x40000101, /* ( |M| ) - A and I */
			/* W : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00c2c05, /* (B|M|E) - A, C, K, L, N, S and T */
			/* Z : */ 0x60000111, /* (B|M| ) - A, E and I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "E" */
			/* A : */ 0xe17ebcce, /* (B|M|E) - B, C, D, G, H, K, L, M, N, P, R, S, T, U, V, W and Y */
			/* B : */ 0xe0126d93, /* (B|M|E) - A, B, E, H, I, K, L, N, O, R and U */
			/* C : */ 0xe21a5ddd, /* (B|M|E) - A, C, D, E, G, H, I, K, L, M, O, R, T, U and Z */
			/* D : */ 0xe37e7fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, R, S, T, U, V, W, Y and Z */
			/* E : */ 0xe02e3c2d, /* (B|M|E) - A, C, D, F, K, L, M, N, R, S, T and V */
			/* F : */ 0xe00e4d35, /* (B|M|E) - A, C, E, F, I, K, L, O, R, S and T */
			/* G : */ 0xe05a59f3, /* (B|M|E) - A, B, E, F, G, H, I, L, M, O, R, T, U and W */
			/* H : */ 0xe31e7913, /* (B|M|E) - A, B, E, I, L, M, N, O, R, S, T, U, Y and Z */
			/* I : */ 0xe20efeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, R, S, T and Z */
			/* J : */ 0xe0087909, /* (B|M|E) - A, D, I, L, M, N, O and T */
			/* K : */ 0xe00e5d95, /* (B|M|E) - A, C, E, H, I, K, L, M, O, R, S and T */
			/* L : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* M : */ 0xe31edd9b, /* (B|M|E) - A, B, D, E, H, I, K, L, M, O, P, R, S, T, U, Y and Z */
			/* N : */ 0xe37e6fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, N, O, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe066bc6e, /* (B|M|E) - B, C, D, F, G, K, L, M, N, P, R, S, V and W */
			/* P : */ 0xe004c191, /* (B|M|E) - A, E, H, I, O, P and S */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* S : */ 0xe35efdbf, /* (B|M|E) - A, B, C, D, E, F, H, I, K, L, M, N, O, P, R, S, T, U, W, Y and Z */
			/* T : */ 0xe25e5bf7, /* (B|M|E) - A, B, C, E, F, G, H, I, J, L, M, O, R, S, T, U, W and Z */
			/* U : */ 0xe28eacee, /* (B|M|E) - B, C, D, F, G, H, K, L, N, P, R, S, T, X and Z */
			/* V : */ 0xe30e4ddd, /* (B|M|E) - A, C, D, E, G, H, I, K, L, O, R, S, T, Y and Z */
			/* W : */ 0xe10445f1, /* (B|M|E) - A, E, F, G, H, I, K, O, S and Y */
			/* X : */ 0xc11809a1, /* ( |M|E) - A, F, H, I, L, T, U and Y */
			/* Y : */ 0xe32c7dbd, /* (B|M|E) - A, C, D, E, F, H, I, K, L, M, N, O, S, T, V, Y and Z */
			/* Z : */ 0xe202011d, /* (B|M|E) - A, C, D, E, I, R and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "F" */
			/* A : */ 0xe37e3d9f, /* (B|M|E) - A, B, C, D, E, H, I, K, L, M, N, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40004000, /* ( |M| ) - O,  */
			/* D : */ 0x40000001, /* ( |M| ) - A,  */
			/* E : */ 0xe12e389d, /* (B|M|E) - A, C, D, E, H, L, M, N, R, S, T, V and Y */
			/* F : */ 0xc10a4111, /* ( |M|E) - A, E, I, O, R, T and Y */
			/* G : */ 0x40000001, /* ( |M| ) - A,  */
			/* H : */ 0x40000101, /* ( |M| ) - A and I */
			/* I : */ 0xe30f7e5d, /* (B|M|E) - A, C, D, E, G, J, K, L, M, N, O, Q, R, S, T, Y and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000011, /* ( |M| ) - A and E */
			/* L : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x40000100, /* ( |M| ) - I,  */
			/* O : */ 0xe05e6c1c, /* (B|M|E) - C, D, E, K, L, N, O, R, S, T, U and W */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xe1004111, /* (B|M|E) - A, E, I, O and Y */
			/* S : */ 0x40080001, /* ( |M| ) - A and T */
			/* T : */ 0xc0104191, /* ( |M|E) - A, E, H, I, O and U */
			/* U : */ 0xe0062cd5, /* (B|M|E) - A, C, E, G, H, K, L, N, R and S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000100, /* ( |M| ) - I,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xa0000000, /* (B| |E) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "G" */
			/* A : */ 0xe37e3b7b, /* (B|M|E) - A, B, D, E, F, G, I, J, L, M, N, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x41104011, /* ( |M| ) - A, E, O, U and Y */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* E : */ 0xe36e799f, /* (B|M|E) - A, B, C, D, E, H, I, L, M, N, O, R, S, T, V, W, Y and Z */
			/* F : */ 0x40024000, /* ( |M| ) - O and R */
			/* G : */ 0xc1000111, /* ( |M|E) - A, E, I and Y */
			/* H : */ 0xe01a4913, /* (B|M|E) - A, B, E, I, L, O, R, T and U */
			/* I : */ 0xe33ef87f, /* (B|M|E) - A, B, C, D, E, F, G, L, M, N, O, P, R, S, T, U, V, Y and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40104001, /* ( |M| ) - A, O and U */
			/* N : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* O : */ 0xe05e780a, /* (B|M|E) - B, D, L, M, N, O, R, S, T, U and W */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60104151, /* (B|M| ) - A, E, G, I, O and U */
			/* S : */ 0xc0084900, /* ( |M|E) - I, L, O and T */
			/* T : */ 0x40124000, /* ( |M| ) - O, R and U */
			/* U : */ 0xe32e3919, /* (B|M|E) - A, D, E, I, L, M, N, R, S, T, V, Y and Z */
			/* V : */ 0x40000001, /* ( |M| ) - A,  */
			/* W : */ 0x60000111, /* (B|M| ) - A, E and I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0000800, /* (B|M|E) - L,  */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "H" */
			/* A : */ 0xe3febfff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, P, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0x41104011, /* ( |M| ) - A, E, O, U and Y */
			/* C : */ 0x40000800, /* ( |M| ) - L,  */
			/* D : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* E : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* F : */ 0x40004010, /* ( |M| ) - E and O */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40004001, /* ( |M| ) - A and O */
			/* I : */ 0xe30efcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, O, P, R, S, T, Y and Z */
			/* J : */ 0x40004001, /* ( |M| ) - A and O */
			/* K : */ 0x40000101, /* ( |M| ) - A and I */
			/* L : */ 0xe1004191, /* (B|M|E) - A, E, H, I, O and Y */
			/* M : */ 0xc0105111, /* ( |M|E) - A, E, I, M, O and U */
			/* N : */ 0xc1042111, /* ( |M|E) - A, E, I, N, S and Y */
			/* O : */ 0xe056fc5e, /* (B|M|E) - B, C, D, E, G, K, L, M, N, O, P, R, S, U and W */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xc1004199, /* ( |M|E) - A, D, E, H, I, O and Y */
			/* S : */ 0xc0000191, /* ( |M|E) - A, E, H and I */
			/* T : */ 0xc1184191, /* ( |M|E) - A, E, H, I, O, T, U and Y */
			/* U : */ 0xe18e385b, /* (B|M|E) - A, B, D, E, G, L, M, N, R, S, T, X and Y */
			/* V : */ 0x40000010, /* ( |M| ) - E,  */
			/* W : */ 0x61000111, /* (B|M| ) - A, E, I and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0041811, /* (B|M|E) - A, E, L, M and S */
			/* Z : */ 0x40000001, /* ( |M| ) - A,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "I" */
			/* A : */ 0xe23e3dec, /* (B|M|E) - C, D, F, G, H, I, K, L, M, N, R, S, T, U, V and Z */
			/* B : */ 0xe1124113, /* (B|M|E) - A, B, E, I, O, R, U and Y */
			/* C : */ 0xe21a5d95, /* (B|M|E) - A, C, E, H, I, K, L, M, O, R, T, U and Z */
			/* D : */ 0xe15e6bd9, /* (B|M|E) - A, D, E, G, H, I, J, L, N, O, R, S, T, U, W and Y */
			/* E : */ 0xc27e3cef, /* ( |M|E) - A, B, C, D, F, G, H, K, L, M, N, R, S, T, U, V, W and Z */
			/* F : */ 0xe21a4131, /* (B|M|E) - A, E, F, I, O, R, T, U and Z */
			/* G : */ 0xe05a79fb, /* (B|M|E) - A, B, D, E, F, G, H, I, L, M, N, O, R, T, U and W */
			/* H : */ 0xe0066111, /* (B|M|E) - A, E, I, N, O, R and S */
			/* I : */ 0xc0002000, /* ( |M|E) - N,  */
			/* J : */ 0xe0044411, /* (B|M|E) - A, E, K, O and S */
			/* K : */ 0xe01e5d91, /* (B|M|E) - A, E, H, I, K, L, M, O, R, S, T and U */
			/* L : */ 0xe17effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* M : */ 0xe012d5bb, /* (B|M|E) - A, B, D, E, F, H, I, K, M, O, P, R and U */
			/* N : */ 0xe35e7fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, R, S, T, U, W, Y and Z */
			/* O : */ 0xe03ea84f, /* (B|M|E) - A, B, C, D, G, L, N, P, R, S, T, U and V */
			/* P : */ 0xe00ec111, /* (B|M|E) - A, E, I, O, P, R, S and T */
			/* Q : */ 0xc0100000, /* ( |M|E) - U,  */
			/* R : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* S : */ 0xe17eddb7, /* (B|M|E) - A, B, C, E, F, H, I, K, L, M, O, P, R, S, T, U, V, W and Y */
			/* T : */ 0xe31e7ff7, /* (B|M|E) - A, B, C, E, F, G, H, I, J, K, L, M, N, O, R, S, T, U, Y and Z */
			/* U : */ 0xc0040802, /* ( |M|E) - B, L and S */
			/* V : */ 0xe1004511, /* (B|M|E) - A, E, I, K, O and Y */
			/* W : */ 0x60004011, /* (B|M| ) - A, E and O */
			/* X : */ 0xc0084110, /* ( |M|E) - E, I, O and T */
			/* Y : */ 0xe0007131, /* (B|M|E) - A, E, F, I, M, N and O */
			/* Z : */ 0xe2205193, /* (B|M|E) - A, B, E, H, I, M, O, V and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "J" */
			/* A : */ 0xe3ef3dff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, Q, R, S, T, V, W, X, Y and Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40000100, /* ( |M| ) - I,  */
			/* D : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* E : */ 0xe26efcb9, /* (B|M|E) - A, D, E, F, H, K, L, M, N, O, P, R, S, T, V, W and Z */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x40000001, /* ( |M| ) - A,  */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe00e380f, /* (B|M|E) - A, B, C, D, L, M, N, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40004001, /* ( |M| ) - A and O */
			/* L : */ 0x40000101, /* ( |M| ) - A and I */
			/* M : */ 0x40000101, /* ( |M| ) - A and I */
			/* N : */ 0x40000001, /* ( |M| ) - A,  */
			/* O : */ 0xe3366cbf, /* (B|M|E) - A, B, C, D, E, F, H, K, L, N, O, R, S, U, V, Y and Z */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60001111, /* (B|M| ) - A, E, I and M */
			/* S : */ 0x80000000, /* ( | |E) - None are allowed */
			/* T : */ 0x40100000, /* ( |M| ) - U,  */
			/* U : */ 0xe00e2c9b, /* (B|M|E) - A, B, D, E, H, K, L, N, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x20000000, /* (B| | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "K" */
			/* A : */ 0xe34ebbff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, L, M, N, P, R, S, T, W, Y and Z */
			/* B : */ 0x40100011, /* ( |M| ) - A, E and U */
			/* C : */ 0x40100001, /* ( |M| ) - A and U */
			/* D : */ 0x40000010, /* ( |M| ) - E,  */
			/* E : */ 0xe32e7dbb, /* (B|M|E) - A, B, D, E, F, H, I, K, L, M, N, O, R, S, T, V, Y and Z */
			/* F : */ 0x40024000, /* ( |M| ) - O and R */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe00e4111, /* (B|M|E) - A, E, I, O, R, S and T */
			/* I : */ 0xe32ebe3f, /* (B|M|E) - A, B, C, D, E, F, J, K, L, M, N, P, R, S, T, V, Y and Z */
			/* J : */ 0x60000010, /* (B|M| ) - E,  */
			/* K : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* L : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0x60104011, /* (B|M| ) - A, E, O and U */
			/* O : */ 0xe176e83e, /* (B|M|E) - B, C, D, E, F, L, N, O, P, R, S, U, V, W and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x63104111, /* (B|M| ) - A, E, I, O, U, Y and Z */
			/* S : */ 0xe0084191, /* (B|M|E) - A, E, H, I, O and T */
			/* T : */ 0xc0004011, /* ( |M|E) - A, E and O */
			/* U : */ 0xe00ebc0a, /* (B|M|E) - B, D, K, L, M, N, P, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0022811, /* (B|M|E) - A, E, L, N and R */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "L" */
			/* A : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x41124111, /* ( |M| ) - A, E, I, O, R, U and Y */
			/* C : */ 0x40104191, /* ( |M| ) - A, E, H, I, O and U */
			/* D : */ 0xc15e51b3, /* ( |M|E) - A, B, E, F, H, I, M, O, R, S, T, U, W and Y */
			/* E : */ 0xe1fe7fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, R, S, T, U, V, W, X and Y */
			/* F : */ 0xc04641fd, /* ( |M|E) - A, C, D, E, F, G, H, I, O, R, S and W */
			/* G : */ 0x40124191, /* ( |M| ) - A, E, H, I, O, R and U */
			/* H : */ 0xc00a0199, /* ( |M|E) - A, D, E, H, I, R and T */
			/* I : */ 0xe3fffeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* J : */ 0x60104511, /* (B|M| ) - A, E, I, K, O and U */
			/* K : */ 0xc04071b1, /* ( |M|E) - A, E, F, H, I, M, N, O and W */
			/* L : */ 0xc15e51b9, /* ( |M|E) - A, D, E, F, H, I, M, O, R, S, T, U, W and Y */
			/* M : */ 0xc1184111, /* ( |M|E) - A, E, I, O, T, U and Y */
			/* N : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* O : */ 0xe17efdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* P : */ 0x40100190, /* ( |M| ) - E, H, I and U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* S : */ 0xc178459f, /* ( |M|E) - A, B, C, D, E, H, I, K, O, T, U, V, W and Y */
			/* T : */ 0xc0124391, /* ( |M|E) - A, E, H, I, J, O, R and U */
			/* U : */ 0xe22ebd3e, /* (B|M|E) - B, C, D, E, F, I, K, L, M, N, P, R, S, T, V and Z */
			/* V : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* W : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* X : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Y : */ 0xe2ae3d3f, /* (B|M|E) - A, B, C, D, E, F, I, K, L, M, N, R, S, T, V, X and Z */
			/* Z : */ 0xc0000111, /* ( |M|E) - A, E and I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "M" */
			/* A : */ 0xe3be3fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, R, S, T, U, V, X, Y and Z */
			/* B : */ 0xc0124911, /* ( |M|E) - A, E, I, L, O, R and U */
			/* C : */ 0x60000cc4, /* (B|M| ) - C, G, H, K and L */
			/* D : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* E : */ 0xe3ae7dfd, /* (B|M|E) - A, C, D, E, F, G, H, I, K, L, M, N, O, R, S, T, V, X, Y and Z */
			/* F : */ 0x40024000, /* ( |M| ) - O and R */
			/* G : */ 0x40000001, /* ( |M| ) - A,  */
			/* H : */ 0x40000101, /* ( |M| ) - A and I */
			/* I : */ 0xe30e7edd, /* (B|M|E) - A, C, D, E, G, H, J, K, L, M, N, O, R, S, T, Y and Z */
			/* J : */ 0x40000001, /* ( |M| ) - A,  */
			/* K : */ 0x40000110, /* ( |M| ) - E and I */
			/* L : */ 0x41000111, /* ( |M| ) - A, E, I and Y */
			/* M : */ 0xc1104111, /* ( |M|E) - A, E, I, O, U and Y */
			/* N : */ 0x40000110, /* ( |M| ) - E and I */
			/* O : */ 0xe01e7fd8, /* (B|M|E) - D, E, G, H, I, J, K, L, M, N, O, R, S, T and U */
			/* P : */ 0xc00e4191, /* ( |M|E) - A, E, H, I, O, R, S and T */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc0184199, /* ( |M|E) - A, D, E, H, I, O, T and U */
			/* T : */ 0x40020001, /* ( |M| ) - A and R */
			/* U : */ 0xe20e3edb, /* (B|M|E) - A, B, D, E, G, H, J, K, L, M, N, R, S, T and Z */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00e2805, /* (B|M|E) - A, C, L, N, R, S and T */
			/* Z : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "N" */
			/* A : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x41124111, /* ( |M| ) - A, E, I, O, R, U and Y */
			/* C : */ 0xc1124d91, /* ( |M|E) - A, E, H, I, K, L, O, R, U and Y */
			/* D : */ 0xc11e5999, /* ( |M|E) - A, D, E, H, I, L, M, O, R, S, T, U and Y */
			/* E : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* F : */ 0x40024110, /* ( |M| ) - E, I, O and R */
			/* G : */ 0xc17e59bb, /* ( |M|E) - A, B, D, E, F, H, I, L, M, O, R, S, T, U, V, W and Y */
			/* H : */ 0x41004101, /* ( |M| ) - A, I, O and Y */
			/* I : */ 0xe3bf7eff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, Q, R, S, T, U, V, X, Y and Z */
			/* J : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* K : */ 0xc0465911, /* ( |M|E) - A, E, I, L, M, O, R, S and W */
			/* L : */ 0x41100111, /* ( |M| ) - A, E, I, U and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0xc1144bd9, /* ( |M|E) - A, D, E, G, H, I, J, L, O, S, U and Y */
			/* O : */ 0xe17e7937, /* (B|M|E) - A, B, C, E, F, I, L, M, N, O, R, S, T, U, V, W and Y */
			/* P : */ 0x40000010, /* ( |M| ) - E,  */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* S : */ 0xc15ccff5, /* ( |M|E) - A, C, E, F, G, H, I, J, K, L, O, P, S, T, U, W and Y */
			/* T : */ 0xc3564fd1, /* ( |M|E) - A, E, G, H, I, J, K, L, O, R, S, U, W, Y and Z */
			/* U : */ 0xe00e3891, /* (B|M|E) - A, E, H, L, M, N, R, S and T */
			/* V : */ 0x40000010, /* ( |M| ) - E,  */
			/* W : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0044c19, /* (B|M|E) - A, D, E, K, L, O and S */
			/* Z : */ 0xc0004b11, /* ( |M|E) - A, E, I, J, L and O */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "O" */
			/* A : */ 0xe0032c84, /* (B|M|E) - C, H, K, L, N, Q and R */
			/* B : */ 0xe1164993, /* (B|M|E) - A, B, E, H, I, L, O, R, S, U and Y */
			/* C : */ 0xe0084d95, /* (B|M|E) - A, C, E, H, I, K, L, O and T */
			/* D : */ 0xe1427379, /* (B|M|E) - A, D, E, F, G, I, J, M, N, O, R, W and Y */
			/* E : */ 0xe34a386b, /* (B|M|E) - A, B, D, F, G, L, M, N, R, T, W, Y and Z */
			/* F : */ 0xc00a4931, /* ( |M|E) - A, E, F, I, L, O, R and T */
			/* G : */ 0xe0124999, /* (B|M|E) - A, D, E, H, I, L, O, R and U */
			/* H : */ 0x60063119, /* (B|M| ) - A, D, E, I, M, N, R and S */
			/* I : */ 0xc0062415, /* ( |M|E) - A, C, E, K, N, R and S */
			/* J : */ 0x60024d15, /* (B|M| ) - A, C, E, I, K, L, O and R */
			/* K : */ 0xe01e5d95, /* (B|M|E) - A, C, E, H, I, K, L, M, O, R, S, T and U */
			/* L : */ 0xe17eff7f, /* (B|M|E) - A, B, C, D, E, F, G, I, J, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* M : */ 0xe112fd13, /* (B|M|E) - A, B, E, I, K, L, M, N, O, P, R, U and Y */
			/* N : */ 0xe31e67df, /* (B|M|E) - A, B, C, D, E, G, H, I, J, K, N, O, R, S, T, U, Y and Z */
			/* O : */ 0xc20ebc4a, /* ( |M|E) - B, D, G, K, L, M, N, P, R, S, T and Z */
			/* P : */ 0xe004c191, /* (B|M|E) - A, E, H, I, O, P and S */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe17effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* S : */ 0xe17eddd7, /* (B|M|E) - A, B, C, E, G, H, I, K, L, M, O, P, R, S, T, U, V, W and Y */
			/* T : */ 0xe04e43f1, /* (B|M|E) - A, E, F, G, H, I, J, O, R, S, T and W */
			/* U : */ 0xe20e3dff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, R, S, T and Z */
			/* V : */ 0xe0000111, /* (B|M|E) - A, E and I */
			/* W : */ 0xe100a91b, /* (B|M|E) - A, B, D, E, I, L, N, P and Y */
			/* X : */ 0xe0082021, /* (B|M|E) - A, F, N and T */
			/* Y : */ 0xc000481d, /* ( |M|E) - A, C, D, E, L and O */
			/* Z : */ 0xe0044511, /* (B|M|E) - A, E, I, K, O and S */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "P" */
			/* A : */ 0xe1fefdd6, /* (B|M|E) - B, C, E, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X and Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe10ebd5d, /* (B|M|E) - A, C, D, E, G, I, K, L, M, N, P, R, S, T and Y */
			/* F : */ 0x20000010, /* (B| | ) - E,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe1026111, /* (B|M|E) - A, E, I, N, O, R and Y */
			/* I : */ 0xe00ee81d, /* (B|M|E) - A, C, D, E, L, N, O, P, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000010, /* ( |M| ) - E,  */
			/* L : */ 0x60000111, /* (B|M| ) - A, E and I */
			/* M : */ 0x40000001, /* ( |M| ) - A,  */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe016d900, /* (B|M|E) - I, L, M, O, P, R, S and U */
			/* P : */ 0xc0004911, /* ( |M|E) - A, E, I, L and O */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc1004180, /* ( |M|E) - H, I, O and Y */
			/* T : */ 0x40004101, /* ( |M| ) - A, I and O */
			/* U : */ 0x60022000, /* (B|M| ) - N and R */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40004000, /* ( |M| ) - O,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x60020000, /* (B|M| ) - R,  */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Q" */
			/* A : */ 0x60000008, /* (B|M| ) - D,  */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60000191, /* (B|M| ) - A, E, H and I */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "R" */
			/* A : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0xc1124911, /* ( |M|E) - A, E, I, L, O, R, U and Y */
			/* C : */ 0xc1104d91, /* ( |M|E) - A, E, H, I, K, L, O, U and Y */
			/* D : */ 0xc15e5b91, /* ( |M|E) - A, E, H, I, J, L, M, O, R, S, T, U, W and Y */
			/* E : */ 0xe3fe7dff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0xc0124101, /* ( |M|E) - A, I, O, R and U */
			/* G : */ 0xc1124991, /* ( |M|E) - A, E, H, I, L, O, R, U and Y */
			/* H : */ 0xe0184911, /* (B|M|E) - A, E, I, L, O, T and U */
			/* I : */ 0xe3bdfeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, Q, S, T, U, V, X, Y and Z */
			/* J : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* K : */ 0xc1564d95, /* ( |M|E) - A, C, E, H, I, K, L, O, R, S, U, W and Y */
			/* L : */ 0xc11c41b3, /* ( |M|E) - A, B, E, F, H, I, O, S, T, U and Y */
			/* M : */ 0xc11c49f1, /* ( |M|E) - A, E, F, G, H, I, L, O, S, T, U and Y */
			/* N : */ 0xc15c49b9, /* ( |M|E) - A, D, E, F, H, I, L, O, S, T, U, W and Y */
			/* O : */ 0xe3fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0xc0004091, /* ( |M|E) - A, E, H and O */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xc1145111, /* ( |M|E) - A, E, I, M, O, S, U and Y */
			/* S : */ 0xc0184195, /* ( |M|E) - A, C, E, H, I, O, T and U */
			/* T : */ 0xc35e7bb3, /* ( |M|E) - A, B, E, F, H, I, J, L, M, N, O, R, S, T, U, W, Y and Z */
			/* U : */ 0xe34ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, W, Y and Z */
			/* V : */ 0xc1004111, /* ( |M|E) - A, E, I, O and Y */
			/* W : */ 0x41004191, /* ( |M| ) - A, E, H, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00cfc5d, /* (B|M|E) - A, C, D, E, G, K, L, M, N, O, P, S and T */
			/* Z : */ 0xc1004111, /* ( |M|E) - A, E, I, O and Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "S" */
			/* A : */ 0xe3feffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* C : */ 0xe05049b3, /* (B|M|E) - A, B, E, F, H, I, L, O, U and W */
			/* D : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* E : */ 0xe3febfff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, P, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0x40024111, /* ( |M| ) - A, E, I, O and R */
			/* G : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* H : */ 0xe15ac933, /* (B|M|E) - A, B, E, F, I, L, O, P, R, T, U, W and Y */
			/* I : */ 0xe3beffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, X, Y and Z */
			/* J : */ 0x40004000, /* ( |M| ) - O,  */
			/* K : */ 0xe1104111, /* (B|M|E) - A, E, I, O, U and Y */
			/* L : */ 0xe1104111, /* (B|M|E) - A, E, I, O, U and Y */
			/* M : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* N : */ 0x60000111, /* (B|M| ) - A, E and I */
			/* O : */ 0xe11efdf2, /* (B|M|E) - B, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U and Y */
			/* P : */ 0x61024111, /* (B|M| ) - A, E, I, O, R and Y */
			/* Q : */ 0x60100000, /* (B|M| ) - U,  */
			/* R : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* S : */ 0xc1004911, /* ( |M|E) - A, E, I, L, O and Y */
			/* T : */ 0xe1527bb7, /* (B|M|E) - A, B, C, E, F, H, I, J, L, M, N, O, R, U, W and Y */
			/* U : */ 0xe22ebdbf, /* (B|M|E) - A, B, C, D, E, F, H, I, K, L, M, N, P, R, S, T, V and Z */
			/* V : */ 0x60000111, /* (B|M| ) - A, E and I */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe006391a, /* (B|M|E) - B, D, E, I, L, M, N, R and S */
			/* Z : */ 0xe0080011, /* (B|M|E) - A, E and T */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "T" */
			/* A : */ 0xe37fbffe, /* (B|M|E) - B, C, D, E, F, G, H, I, J, K, L, M, N, P, Q, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x41120010, /* ( |M| ) - E, R, U and Y */
			/* C : */ 0x40004180, /* ( |M| ) - H, I and O */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe3feffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0x40024100, /* ( |M| ) - I, O and R */
			/* G : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* H : */ 0xe15e5937, /* (B|M|E) - A, B, C, E, F, I, L, M, O, R, S, T, U, W and Y */
			/* I : */ 0xe33efe7f, /* (B|M|E) - A, B, C, D, E, F, G, J, K, L, M, N, O, P, R, S, T, U, V, Y and Z */
			/* J : */ 0x60004011, /* (B|M| ) - A, E and O */
			/* K : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* L : */ 0xc1104111, /* ( |M|E) - A, E, I, O, U and Y */
			/* M : */ 0x40104011, /* ( |M| ) - A, E, O and U */
			/* N : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* O : */ 0xe156bf3e, /* (B|M|E) - B, C, D, E, F, I, J, K, L, M, N, P, R, S, U, W and Y */
			/* P : */ 0x40000010, /* ( |M| ) - E,  */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xe1104111, /* (B|M|E) - A, E, I, O, U and Y */
			/* S : */ 0xe1004195, /* (B|M|E) - A, C, E, H, I, O and Y */
			/* T : */ 0xc1547db1, /* ( |M|E) - A, E, F, H, I, K, L, M, N, O, S, U, W and Y */
			/* U : */ 0xe006b86d, /* (B|M|E) - A, C, D, F, G, L, M, N, P, R and S */
			/* V : */ 0x40000101, /* ( |M| ) - A and I */
			/* W : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0062851, /* (B|M|E) - A, E, G, L, N, R and S */
			/* Z : */ 0xc0440151, /* ( |M|E) - A, E, G, I, S and W */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "U" */
			/* A : */ 0xc10a3818, /* ( |M|E) - D, E, L, M, N, R, T and Y */
			/* B : */ 0xe1124993, /* (B|M|E) - A, B, E, H, I, L, O, R, U and Y */
			/* C : */ 0xe1024f95, /* (B|M|E) - A, C, E, H, I, J, K, L, O, R and Y */
			/* D : */ 0xe15a59db, /* (B|M|E) - A, B, D, E, G, H, I, L, M, O, R, T, U, W and Y */
			/* E : */ 0xe20e2c12, /* (B|M|E) - B, E, K, L, N, R, S, T and Z */
			/* F : */ 0xc0104131, /* ( |M|E) - A, E, F, I, O and U */
			/* G : */ 0xe0124993, /* (B|M|E) - A, B, E, H, I, L, O, R and U */
			/* H : */ 0xc00c6909, /* ( |M|E) - A, D, I, L, N, O, S and T */
			/* I : */ 0xc00e385c, /* ( |M|E) - C, D, E, G, L, M, N, R, S and T */
			/* J : */ 0x40000101, /* ( |M| ) - A and I */
			/* K : */ 0xe0024191, /* (B|M|E) - A, E, H, I, O and R */
			/* L : */ 0xe3fe5fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, O, R, S, T, U, V, W, X, Y and Z */
			/* M : */ 0xe01af1b3, /* (B|M|E) - A, B, E, F, H, I, M, N, O, P, R, T and U */
			/* N : */ 0xe35c6bdf, /* (B|M|E) - A, B, C, D, E, G, H, I, J, L, N, O, S, T, U, W, Y and Z */
			/* O : */ 0xc0000800, /* ( |M|E) - L,  */
			/* P : */ 0xe042c190, /* (B|M|E) - E, H, I, O, P, R and W */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe35f7dff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, Q, R, S, T, U, W, Y and Z */
			/* S : */ 0xe31efd95, /* (B|M|E) - A, C, E, H, I, K, L, M, N, O, P, R, S, T, U, Y and Z */
			/* T : */ 0xe27c09f5, /* (B|M|E) - A, C, E, F, G, H, I, L, S, T, U, V, W and Z */
			/* U : */ 0x60040800, /* (B|M| ) - L and S */
			/* V : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* W : */ 0x60000011, /* (B|M| ) - A and E */
			/* X : */ 0xc0000800, /* ( |M|E) - L,  */
			/* Y : */ 0xe0000841, /* (B|M|E) - A, G and L */
			/* Z : */ 0xe1100191, /* (B|M|E) - A, E, H, I, U and Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "V" */
			/* A : */ 0xe01e39bc, /* (B|M|E) - C, D, E, F, H, I, L, M, N, R, S, T and U */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40000101, /* ( |M| ) - A and I */
			/* D : */ 0x40000101, /* ( |M| ) - A and I */
			/* E : */ 0xe30e7dbd, /* (B|M|E) - A, C, D, E, F, H, I, K, L, M, N, O, R, S, T, Y and Z */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x40000100, /* ( |M| ) - I,  */
			/* H : */ 0x40000010, /* ( |M| ) - E,  */
			/* I : */ 0xe33e7e5d, /* (B|M|E) - A, C, D, E, G, J, K, L, M, N, O, R, S, T, U, V, Y and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* L : */ 0x60100101, /* (B|M| ) - A, I and U */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe0023a00, /* (B|M|E) - J, L, M, N and R */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0x40000100, /* ( |M| ) - I,  */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60060000, /* (B|M| ) - R and S */
			/* V : */ 0x40000001, /* ( |M| ) - A,  */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0002800, /* ( |M|E) - L and N */
			/* Z : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "W" */
			/* A : */ 0xe32e2dfc, /* (B|M|E) - C, D, E, F, G, H, I, K, L, N, R, S, T, V, Y and Z */
			/* B : */ 0x40020000, /* ( |M| ) - R,  */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe10e794b, /* (B|M|E) - A, B, D, G, I, L, M, N, O, R, S, T and Y */
			/* F : */ 0x40004100, /* ( |M| ) - I and O */
			/* G : */ 0x40000010, /* ( |M| ) - E,  */
			/* H : */ 0x60000111, /* (B|M| ) - A, E and I */
			/* I : */ 0xe00e2cff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, N, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61000011, /* (B|M| ) - A, E and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0xc0040111, /* ( |M|E) - A, E, I and S */
			/* O : */ 0xe0127a2a, /* (B|M|E) - B, D, F, J, L, M, N, O, R and U */
			/* P : */ 0x40000010, /* ( |M| ) - E,  */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60000110, /* (B|M| ) - E and I */
			/* S : */ 0xc0084000, /* ( |M|E) - O and T */
			/* T : */ 0x80000000, /* ( | |E) - None are allowed */
			/* U : */ 0x60000800, /* (B|M| ) - L,  */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000001, /* ( |M| ) - A,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x60083d16, /* (B|M| ) - B, C, E, I, K, L, M, N and T */
			/* Z : */ 0x40000100, /* ( |M| ) - I,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "X" */
			/* A : */ 0xe0202000, /* (B|M|E) - N and V */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x40002800, /* ( |M| ) - L and N */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe0043011, /* (B|M|E) - A, E, M, N and S */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x40000010, /* ( |M| ) - E,  */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x40000001, /* ( |M| ) - A,  */
			/* O : */ 0x40002000, /* ( |M| ) - N,  */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x40104010, /* ( |M| ) - E, O and U */
			/* U : */ 0x40040000, /* ( |M| ) - S,  */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000010, /* ( |M| ) - E,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Z : */ 0x00000001, /* ( | | ) - A,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Y" */
			/* A : */ 0xe37ebcdc, /* (B|M|E) - C, D, E, G, H, K, L, M, N, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x40124111, /* ( |M| ) - A, E, I, O, R and U */
			/* C : */ 0x40020191, /* ( |M| ) - A, E, H, I and R */
			/* D : */ 0xc0006159, /* ( |M|E) - A, D, E, G, I, N and O */
			/* E : */ 0xe02e38ab, /* (B|M|E) - A, B, D, F, H, L, M, N, R, S, T and V */
			/* F : */ 0xc0104110, /* ( |M|E) - E, I, O and U */
			/* G : */ 0xc0100110, /* ( |M|E) - E, I and U */
			/* H : */ 0x40105011, /* ( |M| ) - A, E, M, O and U */
			/* I : */ 0xe10a3c68, /* (B|M|E) - D, F, G, K, L, M, N, R, T and Y */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc0000411, /* ( |M|E) - A, E and K */
			/* L : */ 0xe1705d19, /* (B|M|E) - A, D, E, I, K, L, M, O, U, V, W and Y */
			/* M : */ 0xc010c011, /* ( |M|E) - A, E, O, P and U */
			/* N : */ 0xc24e6139, /* ( |M|E) - A, D, E, F, I, N, O, R, S, T, W and Z */
			/* O : */ 0xe21e3800, /* (B|M|E) - L, M, N, R, S, T, U and Z */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xc0586519, /* ( |M|E) - A, D, E, I, K, N, O, T, U and W */
			/* S : */ 0xe20c4995, /* (B|M|E) - A, C, E, H, I, L, O, S, T and Z */
			/* T : */ 0xc0184891, /* ( |M|E) - A, E, H, L, O, T and U */
			/* U : */ 0x60062812, /* (B|M| ) - B, E, L, N, R and S */
			/* V : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* W : */ 0x40004000, /* ( |M| ) - O,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x40000001, /* ( |M| ) - A,  */
			/* Z : */ 0x40100001, /* ( |M| ) - A and U */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Z" */
			/* A : */ 0xe12a3fee, /* (B|M|E) - B, C, D, F, G, H, I, J, K, L, M, N, R, T, V and Y */
			/* B : */ 0x60000110, /* (B|M| ) - E and I */
			/* C : */ 0x40000001, /* ( |M| ) - A,  */
			/* D : */ 0x62020111, /* (B|M| ) - A, E, I, R and Z */
			/* E : */ 0xe12f3fbf, /* (B|M|E) - A, B, C, D, E, F, H, I, J, K, L, M, N, Q, R, S, T, V and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x40000110, /* ( |M| ) - E and I */
			/* H : */ 0x60000001, /* (B|M| ) - A,  */
			/* I : */ 0xe12efeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, R, S, T, V and Y */
			/* J : */ 0x40004001, /* ( |M| ) - A and O */
			/* K : */ 0x40100001, /* ( |M| ) - A and U */
			/* L : */ 0xe0100111, /* (B|M|E) - A, E, I and U */
			/* M : */ 0x41000111, /* ( |M| ) - A, E, I and Y */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe0122190, /* (B|M|E) - E, H, I, N, R and U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40000001, /* ( |M| ) - A,  */
			/* S : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60022882, /* (B|M| ) - B, H, L, N and R */
			/* V : */ 0x60004101, /* (B|M| ) - A, I and O */
			/* W : */ 0x60000001, /* (B|M| ) - A,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0042840, /* (B|M|E) - G, L, N and S */
			/* Z : */ 0x41000111, /* ( |M| ) - A, E, I and Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ä" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ö" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ü" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		}
	}, /* End of Enum 2 / 5 */
	/* ---------------------------------- *
	 * --- texts / de => NST_TEXTS_DE --- *
	 * ---------------------------------- */
	{
		{ /* Rules following letter "A" */
			/* A : */ 0x600e2840, /* (B|M| ) - G, L, N, R, S, T */
			/* B : */ 0xe75efdfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W, Y, Z, ä */
			/* C : */ 0xe01a4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, R, T, U */
			/* D : */ 0xe57e7bbb, /* (B|M|E) - A, B, D, E, F, H, I, J, L, M, N, O, R, S, T, U, V, W, Y, ä */
			/* E : */ 0x600e0808, /* (B|M| ) - D, L, R, S, T */
			/* F : */ 0xf00e4971, /* (B|M|E) - A, E, F, G, I, L, O, R, S, T, ü */
			/* G : */ 0xe25e79fb, /* (B|M|E) - A, B, D, E, F, G, H, I, L, M, N, O, R, S, T, U, W, Z */
			/* H : */ 0xe02e791b, /* (B|M|E) - A, B, D, E, I, L, M, N, O, R, S, T, V */
			/* I : */ 0xe00e3c7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, R, S, T */
			/* J : */ 0x40004019, /* ( |M| ) - A, D, E, O */
			/* K : */ 0xe21e4d91, /* (B|M|E) - A, E, H, I, K, L, O, R, S, T, U, Z */
			/* L : */ 0xe77effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä */
			/* M : */ 0xf11cfdfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, P, S, T, U, Y, ü */
			/* N : */ 0xef7effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ö */
			/* O : */ 0xc0068c00, /* ( |M|E) - K, L, P, R, S */
			/* P : */ 0xe11ec9b1, /* (B|M|E) - A, E, F, H, I, L, O, P, R, S, T, U, Y */
			/* Q : */ 0x60100000, /* (B|M| ) - U */
			/* R : */ 0xf3feffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X, Y, Z, ü */
			/* S : */ 0xe33efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, Y, Z */
			/* T : */ 0xf77effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ü */
			/* U : */ 0xe2febdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, X, Z */
			/* V : */ 0xe0104111, /* (B|M|E) - A, E, I, O, U */
			/* W : */ 0xc0004111, /* ( |M|E) - A, E, I, O */
			/* X : */ 0xe0004111, /* (B|M|E) - A, E, I, O */
			/* Y : */ 0xc006491b, /* ( |M|E) - A, B, D, E, I, L, O, R, S */
			/* Z : */ 0xe2584111, /* (B|M|E) - A, E, I, O, T, U, W, Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "B" */
			/* A : */ 0xe31ebdce, /* (B|M|E) - B, C, D, G, H, I, K, L, M, N, P, R, S, T, U, Y, Z */
			/* B : */ 0x41124911, /* ( |M| ) - A, E, I, L, O, R, U, Y */
			/* C : */ 0x40000080, /* ( |M| ) - H */
			/* D : */ 0x40124111, /* ( |M| ) - A, E, I, O, R, U */
			/* E : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z */
			/* F : */ 0x54024911, /* ( |M| ) - A, E, I, L, O, R, ä, ü */
			/* G : */ 0x40020811, /* ( |M| ) - A, E, L, R */
			/* H : */ 0x44104111, /* ( |M| ) - A, E, I, O, U, ä */
			/* I : */ 0xe25efc5f, /* (B|M|E) - A, B, C, D, E, G, K, L, M, N, O, P, R, S, T, U, W, Z */
			/* J : */ 0x40100010, /* ( |M| ) - E, U */
			/* K : */ 0x58126801, /* ( |M| ) - A, L, N, O, R, U, ö, ü */
			/* L : */ 0x7c104111, /* (B|M| ) - A, E, I, O, U, ä, ö, ü */
			/* M : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* N : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* O : */ 0xe3de7bdf, /* (B|M|E) - A, B, C, D, E, G, H, I, J, L, M, N, O, R, S, T, U, W, X, Y, Z */
			/* P : */ 0x40020001, /* ( |M| ) - A, R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x7d104111, /* (B|M| ) - A, E, I, O, U, Y, ä, ö, ü */
			/* S : */ 0xc61ccd77, /* ( |M|E) - A, B, C, E, F, G, I, K, L, O, P, S, T, U, Z, ä */
			/* T : */ 0xc0060191, /* ( |M|E) - A, E, H, I, R, S */
			/* U : */ 0x600e3cee, /* (B|M| ) - B, C, D, F, G, H, K, L, M, N, R, S, T */
			/* V : */ 0x40000010, /* ( |M| ) - E */
			/* W : */ 0x54000111, /* ( |M| ) - A, E, I, ä, ü */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe2080800, /* (B|M|E) - L, T, Z */
			/* Z : */ 0x40100110, /* ( |M| ) - E, I, U */
			/* ä : */ 0x6012280c, /* (B|M| ) - C, D, L, N, R, U */
			/* ö : */ 0x600e00cc, /* (B|M| ) - C, D, G, H, R, S, T */
			/* ü : */ 0x600e28e4  /* (B|M| ) - C, F, G, H, L, N, R, S, T */
		},
		{ /* Rules following letter "C" */
			/* A : */ 0xe01eb81a, /* (B|M|E) - B, D, E, L, M, N, P, R, S, T, U */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40004181, /* ( |M| ) - A, H, I, O */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe02ebc22, /* (B|M|E) - B, F, K, L, M, N, P, R, S, T, V */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xff7efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ö, ü */
			/* I : */ 0xe01e6899, /* (B|M|E) - A, D, E, H, L, N, O, R, S, T, U */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc37efdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z */
			/* L : */ 0x60104111, /* (B|M| ) - A, E, I, O, U */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe03eb84f, /* (B|M|E) - A, B, C, D, G, L, M, N, P, R, S, T, U, V */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61004111, /* (B|M| ) - A, E, I, O, Y */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x40004101, /* ( |M| ) - A, I, O */
			/* U : */ 0x60069800, /* (B|M| ) - L, M, P, R, S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x60080005, /* (B|M| ) - A, C, T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "D" */
			/* A : */ 0xe37ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, Y, Z */
			/* B : */ 0x40120811, /* ( |M| ) - A, E, L, R, U */
			/* C : */ 0x40000080, /* ( |M| ) - H */
			/* D : */ 0x41124111, /* ( |M| ) - A, E, I, O, R, U, Y */
			/* E : */ 0xe2fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Z */
			/* F : */ 0x50020911, /* ( |M| ) - A, E, I, L, R, ü */
			/* G : */ 0x40020811, /* ( |M| ) - A, E, L, R */
			/* H : */ 0x4c104111, /* ( |M| ) - A, E, I, O, U, ä, ö */
			/* I : */ 0xe73efe7d, /* (B|M|E) - A, C, D, E, F, G, J, K, L, M, N, O, P, R, S, T, U, V, Y, Z, ä */
			/* J : */ 0x60100011, /* (B|M| ) - A, E, U */
			/* K : */ 0x40024001, /* ( |M| ) - A, O, R */
			/* L : */ 0xc4104111, /* ( |M|E) - A, E, I, O, U, ä */
			/* M : */ 0x54104111, /* ( |M| ) - A, E, I, O, U, ä, ü */
			/* N : */ 0x40100111, /* ( |M| ) - A, E, I, U */
			/* O : */ 0xe2febccf, /* (B|M|E) - A, B, C, D, G, H, K, L, M, N, P, R, S, T, U, V, W, X, Z */
			/* P : */ 0x40024021, /* ( |M| ) - A, F, O, R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x7c104111, /* (B|M| ) - A, E, I, O, U, ä, ö, ü */
			/* S : */ 0xc47cddf7, /* ( |M|E) - A, B, C, E, F, G, H, I, K, L, M, O, P, S, T, U, V, W, ä */
			/* T : */ 0xc0060191, /* ( |M|E) - A, E, H, I, R, S */
			/* U : */ 0xe20efd3f, /* (B|M|E) - A, B, C, D, E, F, I, K, L, M, N, O, P, R, S, T, Z */
			/* V : */ 0x40004110, /* ( |M| ) - E, I, O */
			/* W : */ 0x44000111, /* ( |M| ) - A, E, I, ä */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0042800, /* (B|M|E) - L, N, S */
			/* Z : */ 0x40500111, /* ( |M| ) - A, E, I, U, W */
			/* ä : */ 0x60033004, /* (B|M| ) - C, M, N, Q, R */
			/* ö : */ 0x60060000, /* (B|M| ) - R, S */
			/* ü : */ 0x60062000  /* (B|M| ) - N, R, S */
		},
		{ /* Rules following letter "E" */
			/* A : */ 0xc01e3c4e, /* ( |M|E) - B, C, D, G, K, L, M, N, R, S, T, U */
			/* B : */ 0xf61e6df3, /* (B|M|E) - A, B, E, F, G, H, I, K, L, N, O, R, S, T, U, Z, ä, ü */
			/* C : */ 0xe00a4d91, /* (B|M|E) - A, E, H, I, K, L, O, R, T */
			/* D : */ 0xf75e79db, /* (B|M|E) - A, B, D, E, G, H, I, L, M, N, O, R, S, T, U, W, Y, Z, ä, ü */
			/* E : */ 0xc04ebdea, /* ( |M|E) - B, D, F, G, H, I, K, L, M, N, P, R, S, T, W */
			/* F : */ 0xfe3e5d73, /* (B|M|E) - A, B, E, F, G, I, K, L, M, O, R, S, T, U, V, Z, ä, ö, ü */
			/* G : */ 0xf65e7dfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, R, S, T, U, W, Z, ä, ü */
			/* H : */ 0xff5efd7b, /* (B|M|E) - A, B, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, W, Y, Z, ä, ö, ü */
			/* I : */ 0xe27effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Z */
			/* J : */ 0x40104001, /* ( |M| ) - A, O, U */
			/* K : */ 0xfc1e6d11, /* (B|M|E) - A, E, I, K, L, N, O, R, S, T, U, ä, ö, ü */
			/* L : */ 0xff7effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ö, ü */
			/* M : */ 0xfe1efd7b, /* (B|M|E) - A, B, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, Z, ä, ö, ü */
			/* N : */ 0xff7fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y, Z, ä, ö, ü */
			/* O : */ 0xc00ebc6e, /* ( |M|E) - B, C, D, F, G, K, L, M, N, P, R, S, T */
			/* P : */ 0xe01ec9b1, /* (B|M|E) - A, E, F, H, I, L, O, P, R, S, T, U */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0xff7fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y, Z, ä, ö, ü */
			/* S : */ 0xf77efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ü */
			/* T : */ 0xff5e7df7, /* (B|M|E) - A, B, C, E, F, G, H, I, K, L, M, N, O, R, S, T, U, W, Y, Z, ä, ö, ü */
			/* U : */ 0xe25ebdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, W, Z */
			/* V : */ 0xe8004111, /* (B|M|E) - A, E, I, O, ö */
			/* W : */ 0xfc104111, /* (B|M|E) - A, E, I, O, U, ä, ö, ü */
			/* X : */ 0xe21cd593, /* (B|M|E) - A, B, E, H, I, K, M, O, P, S, T, U, Z */
			/* Y : */ 0xe004399a, /* (B|M|E) - B, D, E, H, I, L, M, N, S */
			/* Z : */ 0xd6504111, /* ( |M|E) - A, E, I, O, U, W, Z, ä, ü */
			/* ä : */ 0x40000080, /* ( |M| ) - H */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x40000002  /* ( |M| ) - B */
		},
		{ /* Rules following letter "F" */
			/* A : */ 0xe3be3dde, /* (B|M|E) - B, C, D, E, G, H, I, K, L, M, N, R, S, T, U, V, X, Y, Z */
			/* B : */ 0x40024911, /* ( |M| ) - A, E, I, L, O, R */
			/* C : */ 0x40000080, /* ( |M| ) - H */
			/* D : */ 0x40020000, /* ( |M| ) - R */
			/* E : */ 0xe21e3dfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, R, S, T, U, Z */
			/* F : */ 0xd45eedd3, /* ( |M|E) - A, B, E, G, H, I, K, L, N, O, P, R, S, T, U, W, ä, ü */
			/* G : */ 0x40020991, /* ( |M| ) - A, E, H, I, L, R */
			/* H : */ 0x44104111, /* ( |M| ) - A, E, I, O, U, ä */
			/* I : */ 0xe28e6c7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, N, O, R, S, T, X, Z */
			/* J : */ 0x60004000, /* (B|M| ) - O */
			/* K : */ 0x40024801, /* ( |M| ) - A, L, O, R */
			/* L : */ 0x7c104111, /* (B|M| ) - A, E, I, O, U, ä, ö, ü */
			/* M : */ 0x50004111, /* ( |M| ) - A, E, I, O, ü */
			/* N : */ 0x40100111, /* ( |M| ) - A, E, I, U */
			/* O : */ 0xe01e6c84, /* (B|M|E) - C, H, K, L, N, O, R, S, T, U */
			/* P : */ 0x40020000, /* ( |M| ) - R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x7c104111, /* (B|M| ) - A, E, I, O, U, ä, ö, ü */
			/* S : */ 0xc44c95f7, /* ( |M|E) - A, B, C, E, F, G, H, I, K, M, P, S, T, W, ä */
			/* T : */ 0xc05eddf3, /* ( |M|E) - A, B, E, F, G, H, I, K, L, M, O, P, R, S, T, U, W */
			/* U : */ 0x600e38c4, /* (B|M| ) - C, G, H, L, M, N, R, S, T */
			/* V : */ 0x40000010, /* ( |M| ) - E */
			/* W : */ 0x44000111, /* ( |M| ) - A, E, I, ä */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x40100110, /* ( |M| ) - E, I, U */
			/* ä : */ 0x6016288c, /* (B|M| ) - C, D, H, L, N, R, S, U */
			/* ö : */ 0x60020088, /* (B|M| ) - D, H, R */
			/* ü : */ 0x600e38c0  /* (B|M| ) - G, H, L, M, N, R, S, T */
		},
		{ /* Rules following letter "G" */
			/* A : */ 0xe23eb96a, /* (B|M|E) - B, D, F, G, I, L, M, N, P, R, S, T, U, V, Z */
			/* B : */ 0x40024811, /* ( |M| ) - A, E, L, O, R */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0xc0020011, /* ( |M|E) - A, E, R */
			/* E : */ 0xe67fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Z, ä */
			/* F : */ 0x54020911, /* ( |M| ) - A, E, I, L, R, ä, ü */
			/* G : */ 0xc0024910, /* ( |M|E) - E, I, L, O, R */
			/* H : */ 0xe0104111, /* (B|M|E) - A, E, I, O, U */
			/* I : */ 0xea1efc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, Z, ö */
			/* J : */ 0x44000000, /* ( |M| ) - ä */
			/* K : */ 0x40024011, /* ( |M| ) - A, E, O, R */
			/* L : */ 0x7d104111, /* (B|M| ) - A, E, I, O, U, Y, ä, ö, ü */
			/* M : */ 0x50104111, /* ( |M| ) - A, E, I, O, U, ü */
			/* N : */ 0x70104111, /* (B|M| ) - A, E, I, O, U, ü */
			/* O : */ 0xe01eb85a, /* (B|M|E) - B, D, E, G, L, M, N, P, R, S, T, U */
			/* P : */ 0x40020801, /* ( |M| ) - A, L, R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x7c104111, /* (B|M| ) - A, E, I, O, U, ä, ö, ü */
			/* S : */ 0xd67ffdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, Z, ä, ü */
			/* T : */ 0xc0124091, /* ( |M|E) - A, E, H, O, R, U */
			/* U : */ 0x600e3915, /* (B|M| ) - A, C, E, I, L, M, N, R, S, T */
			/* V : */ 0x40000010, /* ( |M| ) - E */
			/* W : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x60009000, /* (B|M| ) - M, P */
			/* Z : */ 0x40100010, /* ( |M| ) - E, U */
			/* ä : */ 0x60162002, /* (B|M| ) - B, N, R, S, U */
			/* ö : */ 0x600e2008, /* (B|M| ) - D, N, R, S, T */
			/* ü : */ 0x600a2800  /* (B|M| ) - L, N, R, T */
		},
		{ /* Rules following letter "H" */
			/* A : */ 0xe37ebfff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, P, R, S, T, U, V, W, Y, Z */
			/* B : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R, U */
			/* C : */ 0x40000080, /* ( |M| ) - H */
			/* D : */ 0x40024111, /* ( |M| ) - A, E, I, O, R */
			/* E : */ 0xe3fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y, Z */
			/* F : */ 0x54124911, /* ( |M| ) - A, E, I, L, O, R, U, ä, ü */
			/* G : */ 0x40020911, /* ( |M| ) - A, E, I, L, R */
			/* H : */ 0x44004111, /* ( |M| ) - A, E, I, O, ä */
			/* I : */ 0xe23efc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, V, Z */
			/* J : */ 0x40000001, /* ( |M| ) - A */
			/* K : */ 0x44126911, /* ( |M| ) - A, E, I, L, N, O, R, U, ä */
			/* L : */ 0xde7eddfb, /* ( |M|E) - A, B, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U, V, W, Z, ä, ö, ü */
			/* M : */ 0xdc1c4913, /* ( |M|E) - A, B, E, I, L, O, S, T, U, ä, ö, ü */
			/* N : */ 0xdc5c5ff7, /* ( |M|E) - A, B, C, E, F, G, H, I, J, K, L, M, O, S, T, U, W, ä, ö, ü */
			/* O : */ 0xe07efdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W */
			/* P : */ 0x40124821, /* ( |M| ) - A, F, L, O, R, U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xd77efdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ü */
			/* S : */ 0xd65eddf7, /* ( |M|E) - A, B, C, E, F, G, H, I, K, L, M, O, P, R, S, T, U, W, Z, ä, ü */
			/* T : */ 0xd27efdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Z, ü */
			/* U : */ 0xe00eb8eb, /* (B|M|E) - A, B, D, F, G, H, L, M, N, P, R, S, T */
			/* V : */ 0x40004010, /* ( |M| ) - E, O */
			/* W : */ 0x5c104111, /* ( |M| ) - A, E, I, O, U, ä, ö, ü */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe40e984b, /* (B|M|E) - A, B, D, G, L, M, P, R, S, T, ä */
			/* Z : */ 0x40100111, /* ( |M| ) - A, E, I, U */
			/* ä : */ 0x601e7cac, /* (B|M| ) - C, D, F, H, K, L, M, N, O, R, S, T, U */
			/* ö : */ 0x6006a8a6, /* (B|M| ) - B, C, F, H, L, N, P, R, S */
			/* ü : */ 0x600ea8e6  /* (B|M| ) - B, C, F, G, H, L, N, P, R, S, T */
		},
		{ /* Rules following letter "I" */
			/* A : */ 0xe23ebc6e, /* (B|M|E) - B, C, D, F, G, K, L, M, N, P, R, S, T, U, V, Z */
			/* B : */ 0xf15e4917, /* (B|M|E) - A, B, C, E, I, L, O, R, S, T, U, W, Y, ü */
			/* C : */ 0xe0184591, /* (B|M|E) - A, E, H, I, K, O, T, U */
			/* D : */ 0xe13e7dfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, R, S, T, U, V, Y */
			/* E : */ 0xc27ebdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, Z */
			/* F : */ 0xe01e49b1, /* (B|M|E) - A, E, F, H, I, L, O, R, S, T, U */
			/* G : */ 0xe25efff3, /* (B|M|E) - A, B, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, W, Z */
			/* H : */ 0xe01e2911, /* (B|M|E) - A, E, I, L, N, R, S, T, U */
			/* I : */ 0x40002010, /* ( |M| ) - E, N */
			/* J : */ 0xc0004011, /* ( |M|E) - A, E, O */
			/* K : */ 0xec5eddbb, /* (B|M|E) - A, B, D, E, F, H, I, K, L, M, O, P, R, S, T, U, W, ä, ö */
			/* L : */ 0xe67effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Z, ä */
			/* M : */ 0xe65cfd73, /* (B|M|E) - A, B, E, F, G, I, K, L, M, N, O, P, S, T, U, W, Z, ä */
			/* N : */ 0xff7fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y, Z, ä, ö, ü */
			/* O : */ 0xe2aebc4f, /* (B|M|E) - A, B, C, D, G, K, L, M, N, P, R, S, T, V, X, Z */
			/* P : */ 0xc01ec9b1, /* ( |M|E) - A, E, F, H, I, L, O, P, R, S, T, U */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0xe01efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U */
			/* S : */ 0xe77efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä */
			/* T : */ 0xf77efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ü */
			/* U : */ 0x40043000, /* ( |M| ) - M, N, S */
			/* V : */ 0xe07ecd73, /* (B|M|E) - A, B, E, F, G, I, K, L, O, P, R, S, T, U, V, W */
			/* W : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* X : */ 0xc0080110, /* ( |M|E) - E, I, T */
			/* Y : */ 0x40000011, /* ( |M| ) - A, E */
			/* Z : */ 0xe21c4593, /* (B|M|E) - A, B, E, H, I, K, O, S, T, U, Z */
			/* ä : */ 0x40020000, /* ( |M| ) - R */
			/* ö : */ 0x40040000, /* ( |M| ) - S */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "J" */
			/* A : */ 0xe236bccc, /* (B|M|E) - C, D, G, H, K, L, M, N, P, R, S, U, V, Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe00e3c09, /* (B|M|E) - A, D, K, L, M, N, R, S, T */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x20000000, /* (B| | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe01628cf, /* (B|M|E) - A, B, C, D, G, H, L, N, R, S, U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x6006a84e, /* (B|M| ) - B, C, D, G, L, N, P, R, S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x600000c0, /* (B|M| ) - G, H */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x60002008  /* (B|M| ) - D, N */
		},
		{ /* Rules following letter "K" */
			/* A : */ 0xe33effef, /* (B|M|E) - A, B, C, D, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, Y, Z */
			/* B : */ 0x40020911, /* ( |M| ) - A, E, I, L, R */
			/* C : */ 0x40000080, /* ( |M| ) - H */
			/* D : */ 0x40004000, /* ( |M| ) - O */
			/* E : */ 0xe13ebdfa, /* (B|M|E) - B, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, Y */
			/* F : */ 0x44120911, /* ( |M| ) - A, E, I, L, R, U, ä */
			/* G : */ 0x40020011, /* ( |M| ) - A, E, R */
			/* H : */ 0x60004011, /* (B|M| ) - A, E, O */
			/* I : */ 0xe30eb87f, /* (B|M|E) - A, B, C, D, E, F, G, L, M, N, P, R, S, T, Y, Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R, U */
			/* L : */ 0x7c104111, /* (B|M| ) - A, E, I, O, U, ä, ö, ü */
			/* M : */ 0x44000011, /* ( |M| ) - A, E, ä */
			/* N : */ 0x7c104111, /* (B|M| ) - A, E, I, O, U, ä, ö, ü */
			/* O : */ 0xe27efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Z */
			/* P : */ 0x40024900, /* ( |M| ) - I, L, O, R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x7d104111, /* (B|M| ) - A, E, I, O, U, Y, ä, ö, ü */
			/* S : */ 0xc43cddff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, S, T, U, V, ä */
			/* T : */ 0xd03649f3, /* ( |M|E) - A, B, E, F, G, H, I, L, O, R, S, U, V, ü */
			/* U : */ 0xe00eb9c6, /* (B|M|E) - B, C, G, H, I, L, M, N, P, R, S, T */
			/* V : */ 0x40004010, /* ( |M| ) - E, O */
			/* W : */ 0x54000111, /* ( |M| ) - A, E, I, ä, ü */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0020800, /* (B|M|E) - L, R */
			/* Z : */ 0x40100111, /* ( |M| ) - A, E, I, U */
			/* ä : */ 0x60163820, /* (B|M| ) - F, L, M, N, R, S, U */
			/* ö : */ 0x6002b80c, /* (B|M| ) - C, D, L, M, N, P, R */
			/* ü : */ 0x60063886  /* (B|M| ) - B, C, H, L, M, N, R, S */
		},
		{ /* Rules following letter "L" */
			/* A : */ 0xe3feffee, /* (B|M|E) - B, C, D, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X, Y, Z */
			/* B : */ 0xdc7e5dfb, /* ( |M|E) - A, B, D, E, F, G, H, I, K, L, M, O, R, S, T, U, V, W, ä, ö, ü */
			/* C : */ 0x40000181, /* ( |M| ) - A, H, I */
			/* D : */ 0xc67e79fb, /* ( |M|E) - A, B, D, E, F, G, H, I, L, M, N, O, R, S, T, U, V, W, Z, ä */
			/* E : */ 0xe1fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y */
			/* F : */ 0xdc1e5991, /* ( |M|E) - A, E, H, I, L, M, O, R, S, T, U, ä, ö, ü */
			/* G : */ 0xc41e4911, /* ( |M|E) - A, E, I, L, O, R, S, T, U, ä */
			/* H : */ 0x4c004111, /* ( |M| ) - A, E, I, O, ä, ö */
			/* I : */ 0xe2bffeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, Q, R, S, T, U, V, X, Z */
			/* J : */ 0x60100001, /* (B|M| ) - A, U */
			/* K : */ 0xdd1e7911, /* ( |M|E) - A, E, I, L, M, N, O, R, S, T, U, Y, ä, ö, ü */
			/* L : */ 0xd77ef5ff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ü */
			/* M : */ 0xd45c451b, /* ( |M|E) - A, B, D, E, I, K, O, S, T, U, W, ä, ü */
			/* N : */ 0xc0104119, /* ( |M|E) - A, D, E, I, O, U */
			/* O : */ 0xe3fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y, Z */
			/* P : */ 0x401a49b1, /* ( |M| ) - A, E, F, H, I, L, O, R, T, U */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0x5c104111, /* ( |M| ) - A, E, I, O, U, ä, ö, ü */
			/* S : */ 0xd57eddff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U, V, W, Y, ä, ü */
			/* T : */ 0xdf7efdf7, /* ( |M|E) - A, B, C, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ö, ü */
			/* U : */ 0xe2aefd7e, /* (B|M|E) - B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, V, X, Z */
			/* V : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* W : */ 0x54104111, /* ( |M| ) - A, E, I, O, U, ä, ü */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe24ebc5d, /* (B|M|E) - A, C, D, E, G, K, L, M, N, P, R, S, T, W, Z */
			/* Z : */ 0xd05cc9f3, /* ( |M|E) - A, B, E, F, G, H, I, L, O, P, S, T, U, W, ü */
			/* ä : */ 0x601e70ec, /* (B|M| ) - C, D, F, G, H, M, N, O, R, S, T, U */
			/* ö : */ 0x604c88ee, /* (B|M| ) - B, C, D, F, G, H, L, P, S, T, W */
			/* ü : */ 0x600cb0ee  /* (B|M| ) - B, C, D, F, G, H, M, N, P, S, T */
		},
		{ /* Rules following letter "M" */
			/* A : */ 0xe39effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, X, Y, Z */
			/* B : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R, U */
			/* C : */ 0x40000081, /* ( |M| ) - A, H */
			/* D : */ 0xc4160111, /* ( |M|E) - A, E, I, R, S, U, ä */
			/* E : */ 0xe31ebdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, Y, Z */
			/* F : */ 0x54024911, /* ( |M| ) - A, E, I, L, O, R, ä, ü */
			/* G : */ 0x40020011, /* ( |M| ) - A, E, R */
			/* H : */ 0x44004111, /* ( |M| ) - A, E, I, O, ä */
			/* I : */ 0xe39e7cfd, /* (B|M|E) - A, C, D, E, F, G, H, K, L, M, N, O, R, S, T, U, X, Y, Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x44124811, /* ( |M| ) - A, E, L, O, R, U, ä */
			/* L : */ 0x4c104111, /* ( |M| ) - A, E, I, O, U, ä, ö */
			/* M : */ 0xc31e6df3, /* ( |M|E) - A, B, E, F, G, H, I, K, L, N, O, R, S, T, U, Y, Z */
			/* N : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* O : */ 0xe29efdde, /* (B|M|E) - B, C, D, E, G, H, I, K, L, M, N, O, P, R, S, T, U, X, Z */
			/* P : */ 0xc01e49b1, /* ( |M|E) - A, E, F, H, I, L, O, R, S, T, U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* S : */ 0xc45eddff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U, W, ä */
			/* T : */ 0xc0160991, /* ( |M|E) - A, E, H, I, L, R, S, U */
			/* U : */ 0x600e3ced, /* (B|M| ) - A, C, D, F, G, H, K, L, M, N, R, S, T */
			/* V : */ 0x40004010, /* ( |M| ) - E, O */
			/* W : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00e4c10, /* (B|M|E) - E, K, L, O, R, S, T */
			/* Z : */ 0x40100111, /* ( |M| ) - A, E, I, U */
			/* ä : */ 0x6016288d, /* (B|M| ) - A, C, D, H, L, N, R, S, U */
			/* ö : */ 0x604268ca, /* (B|M| ) - B, D, G, H, L, N, O, R, W */
			/* ü : */ 0x600e288c  /* (B|M| ) - C, D, H, L, N, R, S, T */
		},
		{ /* Rules following letter "N" */
			/* A : */ 0xe37ebdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, Y, Z */
			/* B : */ 0x5c124911, /* ( |M| ) - A, E, I, L, O, R, U, ä, ö, ü */
			/* C : */ 0xc0004191, /* ( |M|E) - A, E, H, I, O */
			/* D : */ 0xd77effff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ü */
			/* E : */ 0xe3fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y, Z */
			/* F : */ 0x5e1e4911, /* ( |M| ) - A, E, I, L, O, R, S, T, U, Z, ä, ö, ü */
			/* G : */ 0xd67efdf3, /* ( |M|E) - A, B, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Z, ä, ü */
			/* H : */ 0x5d104111, /* ( |M| ) - A, E, I, O, U, Y, ä, ö, ü */
			/* I : */ 0xe2befcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, O, P, R, S, T, U, V, X, Z */
			/* J : */ 0x44100011, /* ( |M| ) - A, E, U, ä */
			/* K : */ 0xde5ef9f3, /* ( |M|E) - A, B, E, F, G, H, I, L, M, N, O, P, R, S, T, U, W, Z, ä, ö, ü */
			/* L : */ 0x4c104111, /* ( |M| ) - A, E, I, O, U, ä, ö */
			/* M : */ 0x5c104111, /* ( |M| ) - A, E, I, O, U, ä, ö, ü */
			/* N : */ 0xc77e4dff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, O, R, S, T, U, V, W, Y, Z, ä */
			/* O : */ 0xe2febd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, P, R, S, T, U, V, W, X, Z */
			/* P : */ 0x401249b1, /* ( |M| ) - A, E, F, H, I, L, O, R, U */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0x5c104111, /* ( |M| ) - A, E, I, O, U, ä, ö, ü */
			/* S : */ 0xdf7effff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ö, ü */
			/* T : */ 0xdf7efffb, /* ( |M|E) - A, B, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ö, ü */
			/* U : */ 0xe00ebd3f, /* (B|M|E) - A, B, C, D, E, F, I, K, L, M, N, P, R, S, T */
			/* V : */ 0x48004111, /* ( |M| ) - A, E, I, O, ö */
			/* W : */ 0x5c104111, /* ( |M| ) - A, E, I, O, U, ä, ö, ü */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0041800, /* (B|M|E) - L, M, S */
			/* Z : */ 0xdf7efff3, /* ( |M|E) - A, B, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ö, ü */
			/* ä : */ 0x60169886, /* (B|M| ) - B, C, H, L, M, P, R, S, U */
			/* ö : */ 0x602e880c, /* (B|M| ) - C, D, L, P, R, S, T, V */
			/* ü : */ 0x600a8062  /* (B|M| ) - B, F, G, P, R, T */
		},
		{ /* Rules following letter "O" */
			/* A : */ 0x401e3c0e, /* ( |M| ) - B, C, D, K, L, M, N, R, S, T, U */
			/* B : */ 0xe11e4b1b, /* (B|M|E) - A, B, D, E, I, J, L, O, R, S, T, U, Y */
			/* C : */ 0x61000495, /* (B|M| ) - A, C, E, H, K, Y */
			/* D : */ 0xe15e4119, /* (B|M|E) - A, D, E, I, O, R, S, T, U, W, Y */
			/* E : */ 0xe00e2828, /* (B|M|E) - D, F, L, N, R, S, T */
			/* F : */ 0xe00e4931, /* (B|M|E) - A, E, F, I, L, O, R, S, T */
			/* G : */ 0xe01e7953, /* (B|M|E) - A, B, E, G, I, L, M, N, O, R, S, T, U */
			/* H : */ 0xe50e7911, /* (B|M|E) - A, E, I, L, M, N, O, R, S, T, Y, ä */
			/* I : */ 0xc00e2c0c, /* ( |M|E) - C, D, K, L, N, R, S, T */
			/* J : */ 0x40000011, /* ( |M| ) - A, E */
			/* K : */ 0xe21e4d11, /* (B|M|E) - A, E, I, K, L, O, R, S, T, U, Z */
			/* L : */ 0xe33cfd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, S, T, U, V, Y, Z */
			/* M : */ 0xed3ef533, /* (B|M|E) - A, B, E, F, I, K, M, N, O, P, R, S, T, U, V, Y, ä, ö */
			/* N : */ 0xe77effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä */
			/* O : */ 0x400ebc48, /* ( |M| ) - D, G, K, L, M, N, P, R, S, T */
			/* P : */ 0xe51ecdb1, /* (B|M|E) - A, E, F, H, I, K, L, O, P, R, S, T, U, Y, ä */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0xf77efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ü */
			/* S : */ 0xe75efdf7, /* (B|M|E) - A, B, C, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W, Y, Z, ä */
			/* T : */ 0xe35eddf3, /* (B|M|E) - A, B, E, F, G, H, I, K, L, M, O, P, R, S, T, U, W, Y, Z */
			/* U : */ 0xe02eb96f, /* (B|M|E) - A, B, C, D, F, G, I, L, M, N, P, R, S, T, V */
			/* V : */ 0x60004111, /* (B|M| ) - A, E, I, O */
			/* W : */ 0xc0042b11, /* ( |M|E) - A, E, I, J, L, N, S */
			/* X : */ 0xe1000111, /* (B|M|E) - A, E, I, Y */
			/* Y : */ 0xc0000011, /* ( |M|E) - A, E */
			/* Z : */ 0x65004111, /* (B|M| ) - A, E, I, O, Y, ä */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "P" */
			/* A : */ 0xe23ebc4d, /* (B|M|E) - A, C, D, G, K, L, M, N, P, R, S, T, U, V, Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40000080, /* ( |M| ) - H */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe21ead5d, /* (B|M|E) - A, C, D, E, G, I, K, L, N, P, R, S, T, U, Z */
			/* F : */ 0xf45e7df7, /* (B|M|E) - A, B, C, E, F, G, H, I, K, L, M, N, O, R, S, T, U, W, ä, ü */
			/* G : */ 0x40000010, /* ( |M| ) - E */
			/* H : */ 0xe51a4911, /* (B|M|E) - A, E, I, L, O, R, T, U, Y, ä */
			/* I : */ 0xe21efc5d, /* (B|M|E) - A, C, D, E, G, K, L, M, N, O, P, R, S, T, U, Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000001, /* ( |M| ) - A */
			/* L : */ 0x74104111, /* (B|M| ) - A, E, I, O, U, ä, ü */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x60004010, /* (B|M| ) - E, O */
			/* O : */ 0xe0cefd5c, /* (B|M|E) - C, D, E, G, I, K, L, M, N, O, P, R, S, T, W, X */
			/* P : */ 0xc11e4d95, /* ( |M|E) - A, C, E, H, I, K, L, O, R, S, T, U, Y */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x7c104111, /* (B|M| ) - A, E, I, O, U, ä, ö, ü */
			/* S : */ 0xe108c115, /* (B|M|E) - A, C, E, I, O, P, T, Y */
			/* T : */ 0xc0144911, /* ( |M|E) - A, E, I, L, O, S, U */
			/* U : */ 0x620eb82e, /* (B|M| ) - B, C, D, F, L, M, N, P, R, S, T, Z */
			/* V : */ 0x40000010, /* ( |M| ) - E */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x60020800, /* (B|M| ) - L, R */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x600aa18c, /* (B|M| ) - C, D, H, I, N, P, R, T */
			/* ö : */ 0x60000402, /* (B|M| ) - B, K */
			/* ü : */ 0x60020800  /* (B|M| ) - L, R */
		},
		{ /* Rules following letter "Q" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x64004111, /* (B|M| ) - A, E, I, O, ä */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "R" */
			/* A : */ 0xe3feffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X, Y, Z */
			/* B : */ 0xdc1e4953, /* ( |M|E) - A, B, E, G, I, L, O, R, S, T, U, ä, ö, ü */
			/* C : */ 0x40104591, /* ( |M| ) - A, E, H, I, K, O, U */
			/* D : */ 0xdd5efdfb, /* ( |M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W, Y, ä, ö, ü */
			/* E : */ 0xe3fffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z */
			/* F : */ 0xdc1e6911, /* ( |M|E) - A, E, I, L, N, O, R, S, T, U, ä, ö, ü */
			/* G : */ 0xdc5e6db3, /* ( |M|E) - A, B, E, F, H, I, K, L, N, O, R, S, T, U, W, ä, ö, ü */
			/* H : */ 0x7d104111, /* (B|M| ) - A, E, I, O, U, Y, ä, ö, ü */
			/* I : */ 0xe3bcfe7f, /* (B|M|E) - A, B, C, D, E, F, G, J, K, L, M, N, O, P, S, T, U, V, X, Y, Z */
			/* J : */ 0x44100001, /* ( |M| ) - A, U, ä */
			/* K : */ 0xde5e7dd3, /* ( |M|E) - A, B, E, G, H, I, K, L, M, N, O, R, S, T, U, W, Z, ä, ö, ü */
			/* L : */ 0xcc144111, /* ( |M|E) - A, E, I, O, S, U, ä, ö */
			/* M : */ 0xde5c49f7, /* ( |M|E) - A, B, C, E, F, G, H, I, L, O, S, T, U, W, Z, ä, ö, ü */
			/* N : */ 0xc67eddff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U, V, W, Z, ä */
			/* O : */ 0xe3feffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X, Y, Z */
			/* P : */ 0x401e49b1, /* ( |M| ) - A, E, F, H, I, L, O, R, S, T, U */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0xd51c6dd1, /* ( |M|E) - A, E, G, H, I, K, L, N, O, S, T, U, Y, ä, ü */
			/* S : */ 0xdf5eddff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U, W, Y, Z, ä, ö, ü */
			/* T : */ 0xdf7effff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ö, ü */
			/* U : */ 0xe20ebdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, Z */
			/* V : */ 0x48004111, /* ( |M| ) - A, E, I, O, ö */
			/* W : */ 0x5c104111, /* ( |M| ) - A, E, I, O, U, ä, ö, ü */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc00ce801, /* ( |M|E) - A, L, N, O, P, S, T */
			/* Z : */ 0xde5cddf3, /* ( |M|E) - A, B, E, F, G, H, I, K, L, M, O, P, S, T, U, W, Z, ä, ö, ü */
			/* ä : */ 0x623ebcee, /* (B|M| ) - B, C, D, F, G, H, K, L, M, N, P, R, S, T, U, V, Z */
			/* ö : */ 0x600eb8ae, /* (B|M| ) - B, C, D, F, H, L, M, N, P, R, S, T */
			/* ü : */ 0x600cb8ee  /* (B|M| ) - B, C, D, F, G, H, L, M, N, P, S, T */
		},
		{ /* Rules following letter "S" */
			/* A : */ 0xe1bebdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, X, Y */
			/* B : */ 0x5c124911, /* ( |M| ) - A, E, I, L, O, R, U, ä, ö, ü */
			/* C : */ 0x60024181, /* (B|M| ) - A, H, I, O, R */
			/* D : */ 0x44124111, /* ( |M| ) - A, E, I, O, R, U, ä */
			/* E : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z */
			/* F : */ 0x5c124911, /* ( |M| ) - A, E, I, L, O, R, U, ä, ö, ü */
			/* G : */ 0x54124911, /* ( |M| ) - A, E, I, L, O, R, U, ä, ü */
			/* H : */ 0xfd104111, /* (B|M|E) - A, E, I, O, U, Y, ä, ö, ü */
			/* I : */ 0xe23efcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, O, P, R, S, T, U, V, Z */
			/* J : */ 0x40100001, /* ( |M| ) - A, U */
			/* K : */ 0xfd126911, /* (B|M|E) - A, E, I, L, N, O, R, U, Y, ä, ö, ü */
			/* L : */ 0x7c104111, /* (B|M| ) - A, E, I, O, U, ä, ö, ü */
			/* M : */ 0x7c104111, /* (B|M| ) - A, E, I, O, U, ä, ö, ü */
			/* N : */ 0x44104111, /* ( |M| ) - A, E, I, O, U, ä */
			/* O : */ 0xe2febcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, P, R, S, T, U, V, W, X, Z */
			/* P : */ 0x7c1649b1, /* (B|M| ) - A, E, F, H, I, L, O, R, S, U, ä, ö, ü */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0x54104111, /* ( |M| ) - A, E, I, O, U, ä, ü */
			/* S : */ 0xd77afdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, T, U, V, W, Y, Z, ä, ü */
			/* T : */ 0xff7effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ö, ü */
			/* U : */ 0xe00ebdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T */
			/* V : */ 0x60004111, /* (B|M| ) - A, E, I, O */
			/* W : */ 0x7c104111, /* (B|M| ) - A, E, I, O, U, ä, ö, ü */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe006b804, /* (B|M|E) - C, L, M, N, P, R, S */
			/* Z : */ 0x65504111, /* (B|M| ) - A, E, I, O, U, W, Y, ä */
			/* ä : */ 0x601e3cc6, /* (B|M| ) - B, C, G, H, K, L, M, N, R, S, T, U */
			/* ö : */ 0x600028a0, /* (B|M| ) - F, H, L, N */
			/* ü : */ 0x6004208e  /* (B|M| ) - B, C, D, H, N, S */
		},
		{ /* Rules following letter "T" */
			/* A : */ 0xe3bebdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, X, Y, Z */
			/* B : */ 0x5c124911, /* ( |M| ) - A, E, I, L, O, R, U, ä, ö, ü */
			/* C : */ 0x40104080, /* ( |M| ) - H, O, U */
			/* D : */ 0x54124111, /* ( |M| ) - A, E, I, O, R, U, ä, ü */
			/* E : */ 0xe2fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Z */
			/* F : */ 0x5c124911, /* ( |M| ) - A, E, I, L, O, R, U, ä, ö, ü */
			/* G : */ 0x44024911, /* ( |M| ) - A, E, I, L, O, R, ä */
			/* H : */ 0xfd127911, /* (B|M|E) - A, E, I, L, M, N, O, R, U, Y, ä, ö, ü */
			/* I : */ 0xee3fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, Z, ä, ö */
			/* J : */ 0x64100011, /* (B|M| ) - A, E, U, ä */
			/* K : */ 0x4c124911, /* ( |M| ) - A, E, I, L, O, R, U, ä, ö */
			/* L : */ 0xcc104111, /* ( |M|E) - A, E, I, O, U, ä, ö */
			/* M : */ 0x5c104111, /* ( |M| ) - A, E, I, O, U, ä, ö, ü */
			/* N : */ 0x44104111, /* ( |M| ) - A, E, I, O, U, ä */
			/* O : */ 0xe2fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Z */
			/* P : */ 0x401249b1, /* ( |M| ) - A, E, F, H, I, L, O, R, U */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0x7d104111, /* (B|M| ) - A, E, I, O, U, Y, ä, ö, ü */
			/* S : */ 0xf77efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ü */
			/* T : */ 0xd776fdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, U, V, W, Y, Z, ä, ü */
			/* U : */ 0xe00ef9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, R, S, T */
			/* V : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* W : */ 0x5c104111, /* ( |M| ) - A, E, I, O, U, ä, ö, ü */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0029800, /* (B|M|E) - L, M, P, R */
			/* Z : */ 0xd15efdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W, Y, ü */
			/* ä : */ 0x601e38ee, /* (B|M| ) - B, C, D, F, G, H, L, M, N, R, S, T, U */
			/* ö : */ 0x600ea006, /* (B|M| ) - B, C, N, P, R, S, T */
			/* ü : */ 0x600a3886  /* (B|M| ) - B, C, H, L, M, N, R, T */
		},
		{ /* Rules following letter "U" */
			/* A : */ 0xc00e2c08, /* ( |M|E) - D, K, L, N, R, S, T */
			/* B : */ 0xf03e6f93, /* (B|M|E) - A, B, E, H, I, J, K, L, N, O, R, S, T, U, V, ü */
			/* C : */ 0x40004595, /* ( |M| ) - A, C, E, H, I, K, O */
			/* D : */ 0xd4564959, /* ( |M|E) - A, D, E, G, I, L, O, R, S, U, W, ä, ü */
			/* E : */ 0xe00e3906, /* (B|M|E) - B, C, I, L, M, N, R, S, T */
			/* F : */ 0xf65efdfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W, Z, ä, ü */
			/* G : */ 0xc61e79f3, /* ( |M|E) - A, B, E, F, G, H, I, L, M, N, O, R, S, T, U, Z, ä */
			/* H : */ 0xfc167911, /* (B|M|E) - A, E, I, L, M, N, O, R, S, U, ä, ö, ü */
			/* I : */ 0xc22e285c, /* ( |M|E) - C, D, E, G, L, N, R, S, T, V, Z */
			/* J : */ 0x40000001, /* ( |M| ) - A */
			/* K : */ 0xdc1a6d11, /* ( |M|E) - A, E, I, K, L, N, O, R, T, U, ä, ö, ü */
			/* L : */ 0xfe7efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V, W, Z, ä, ö, ü */
			/* M : */ 0xf67efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Z, ä, ü */
			/* N : */ 0xf67efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Z, ä, ü */
			/* O : */ 0x400e0800, /* ( |M| ) - L, R, S, T */
			/* P : */ 0xc01ec9b1, /* ( |M|E) - A, E, F, H, I, L, O, P, R, S, T, U */
			/* Q : */ 0x40100000, /* ( |M| ) - U */
			/* R : */ 0xf77efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y, Z, ä, ü */
			/* S : */ 0xe67ffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, Z, ä */
			/* T : */ 0xe27efdfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Z */
			/* U : */ 0x40003000, /* ( |M| ) - M, N */
			/* V : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* W : */ 0x54104111, /* ( |M| ) - A, E, I, O, U, ä, ü */
			/* X : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0xd6584113, /* ( |M|E) - A, B, E, I, O, T, U, W, Z, ä, ü */
			/* ä : */ 0x40000800, /* ( |M| ) - L */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "V" */
			/* A : */ 0xe00e3dc8, /* (B|M|E) - D, G, H, I, K, L, M, N, R, S, T */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe08e2d4d, /* (B|M|E) - A, C, D, G, I, K, L, N, R, S, T, X */
			/* F : */ 0x40004000, /* ( |M| ) - O */
			/* G : */ 0x40000010, /* ( |M| ) - E */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe22e6c5f, /* (B|M|E) - A, B, C, D, E, G, K, L, N, O, R, S, T, V, Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40004000, /* ( |M| ) - O */
			/* L : */ 0x60000101, /* (B|M| ) - A, I */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe00a2d40, /* (B|M|E) - G, I, K, L, N, R, T */
			/* P : */ 0x40020000, /* ( |M| ) - R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40000100, /* ( |M| ) - I */
			/* S : */ 0x40080000, /* ( |M| ) - T */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60021800, /* (B|M| ) - L, M, R */
			/* V : */ 0x40000010, /* ( |M| ) - E */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x60080000, /* (B|M| ) - T */
			/* ö : */ 0x60000840, /* (B|M| ) - G, L */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "W" */
			/* A : */ 0xe10ebdef, /* (B|M|E) - A, B, C, D, F, G, H, I, K, L, M, N, P, R, S, T, Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe00e39ee, /* (B|M|E) - B, C, D, F, G, H, I, L, M, N, R, S, T */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x20000000, /* (B| | ) - None are allowed */
			/* I : */ 0x600ebc5c, /* (B|M| ) - C, D, E, G, K, L, M, N, P, R, S, T */
			/* J : */ 0x40000010, /* ( |M| ) - E */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x60000011, /* (B|M| ) - A, E */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x80000000, /* ( | |E) - None are allowed */
			/* O : */ 0x600278c6, /* (B|M| ) - B, C, G, H, L, M, N, O, R */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60000001, /* (B|M| ) - A */
			/* S : */ 0x40000410, /* ( |M| ) - E, K */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x600e2804, /* (B|M| ) - C, L, N, R, S, T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x600e38c4, /* (B|M| ) - C, G, H, L, M, N, R, S, T */
			/* ö : */ 0x60021884, /* (B|M| ) - C, H, L, M, R */
			/* ü : */ 0x600e2884  /* (B|M| ) - C, H, L, N, R, S, T */
		},
		{ /* Rules following letter "X" */
			/* A : */ 0x60003800, /* (B|M| ) - L, M, N */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe0023c00, /* (B|M|E) - K, L, M, N, R */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40000001, /* ( |M| ) - A */
			/* I : */ 0x40247c19, /* ( |M| ) - A, D, E, K, L, M, N, O, S, V */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40024000, /* ( |M| ) - O, R */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x400a2000, /* ( |M| ) - N, R, T */
			/* P : */ 0x40024811, /* ( |M| ) - A, E, L, O, R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0xc0120110, /* ( |M|E) - E, I, R, U */
			/* U : */ 0x40000011, /* ( |M| ) - A, E */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x60000808, /* (B|M| ) - D, L */
			/* Z : */ 0x40000010, /* ( |M| ) - E */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Y" */
			/* A : */ 0xe2063c0c, /* (B|M|E) - C, D, K, L, M, N, R, S, Z */
			/* B : */ 0x40124011, /* ( |M| ) - A, E, O, R, U */
			/* C : */ 0x40000890, /* ( |M| ) - E, H, L */
			/* D : */ 0xc0020111, /* ( |M|E) - A, E, I, R */
			/* E : */ 0xe00e2808, /* (B|M|E) - D, L, N, R, S, T */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x40024100, /* ( |M| ) - I, O, R */
			/* H : */ 0x40000001, /* ( |M| ) - A */
			/* I : */ 0xe0002800, /* (B|M|E) - L, N */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40004810, /* ( |M| ) - E, L, O */
			/* L : */ 0xc0244955, /* ( |M|E) - A, C, E, G, I, L, O, S, V */
			/* M : */ 0xc010f113, /* ( |M|E) - A, B, E, I, M, N, O, P, U */
			/* N : */ 0xc008451d, /* ( |M|E) - A, C, D, E, I, K, O, T */
			/* O : */ 0xe01c2040, /* (B|M|E) - G, N, S, T, U */
			/* P : */ 0xc01e6190, /* ( |M|E) - E, H, I, N, O, R, S, T, U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x401a4111, /* ( |M| ) - A, E, I, O, R, T, U */
			/* S : */ 0xc00c0115, /* ( |M|E) - A, C, E, I, S, T */
			/* T : */ 0xc0004190, /* ( |M|E) - E, H, I, O */
			/* U : */ 0x60022000, /* (B|M| ) - N, R */
			/* V : */ 0x20000000, /* (B| | ) - None are allowed */
			/* W : */ 0x40004000, /* ( |M| ) - O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x40000011, /* ( |M| ) - A, E */
			/* ä : */ 0x40002000, /* ( |M| ) - N */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Z" */
			/* A : */ 0xe01abdee, /* (B|M|E) - B, C, D, F, G, H, I, K, L, M, N, P, R, T, U */
			/* B : */ 0x50120911, /* ( |M| ) - A, E, I, L, R, U, ü */
			/* C : */ 0x40000080, /* ( |M| ) - H */
			/* D : */ 0x40000110, /* ( |M| ) - E, I */
			/* E : */ 0xe11ebdaf, /* (B|M|E) - A, B, C, D, F, H, I, K, L, M, N, P, R, S, T, U, Y */
			/* F : */ 0x44024811, /* ( |M| ) - A, E, L, O, R, ä */
			/* G : */ 0x40020011, /* ( |M| ) - A, E, R */
			/* H : */ 0x40004011, /* ( |M| ) - A, E, O */
			/* I : */ 0xe93efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V, Y, ö */
			/* J : */ 0x40000001, /* ( |M| ) - A */
			/* K : */ 0x48124101, /* ( |M| ) - A, I, O, R, U, ö */
			/* L : */ 0x40004111, /* ( |M| ) - A, E, I, O */
			/* M : */ 0x40000111, /* ( |M| ) - A, E, I */
			/* N : */ 0x40000001, /* ( |M| ) - A */
			/* O : */ 0xe00ee964, /* (B|M|E) - C, F, G, I, L, N, O, P, R, S, T */
			/* P : */ 0x40120820, /* ( |M| ) - F, L, R, U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40000011, /* ( |M| ) - A, E */
			/* S : */ 0x40088115, /* ( |M| ) - A, C, E, I, P, T */
			/* T : */ 0xc0020891, /* ( |M|E) - A, E, H, L, R */
			/* U : */ 0xe66fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, V, W, Z, ä */
			/* V : */ 0x40004010, /* ( |M| ) - E, O */
			/* W : */ 0x6c100111, /* (B|M| ) - A, E, I, U, ä, ö */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x600c9c00, /* (B|M| ) - K, L, M, P, S, T */
			/* Z : */ 0x40104111, /* ( |M| ) - A, E, I, O, U */
			/* ä : */ 0x60102080, /* (B|M| ) - H, N, U */
			/* ö : */ 0x60040840, /* (B|M| ) - G, L, S */
			/* ü : */ 0x60022044  /* (B|M| ) - C, G, N, R */
		},
		{ /* Rules following letter "ä" */
			/* A : */ 0x40002000, /* ( |M| ) - N */
			/* B : */ 0x40080110, /* ( |M| ) - E, I, T */
			/* C : */ 0x40000480, /* ( |M| ) - H, K */
			/* D : */ 0x40080915, /* ( |M| ) - A, C, E, I, L, T */
			/* E : */ 0x40022000, /* ( |M| ) - N, R */
			/* F : */ 0x40080930, /* ( |M| ) - E, F, I, L, T */
			/* G : */ 0x61182910, /* (B|M| ) - E, I, L, N, T, U, Y */
			/* H : */ 0x600a3910, /* (B|M| ) - E, I, L, M, N, R, T */
			/* I : */ 0x40040000, /* ( |M| ) - S */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40100010, /* ( |M| ) - E, U */
			/* L : */ 0x420c083e, /* ( |M| ) - B, C, D, E, F, L, S, T, Z */
			/* M : */ 0x4008d911, /* ( |M| ) - A, E, I, L, M, O, P, T */
			/* N : */ 0xc20c6558, /* ( |M|E) - D, E, G, I, K, N, O, S, T, Z */
			/* O : */ 0x40000840, /* ( |M| ) - G, L */
			/* P : */ 0x40008021, /* ( |M| ) - A, F, P */
			/* Q : */ 0x60100000, /* (B|M| ) - U */
			/* R : */ 0xe25e9d7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, P, R, S, T, U, W, Z */
			/* S : */ 0x601c0114, /* (B|M| ) - C, E, I, S, T, U */
			/* T : */ 0xe20c4590, /* (B|M|E) - E, H, I, K, O, S, T, Z */
			/* U : */ 0x620eb87e, /* (B|M| ) - B, C, D, E, F, G, L, M, N, P, R, S, T, Z */
			/* V : */ 0x40000010, /* ( |M| ) - E */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x40000110, /* ( |M| ) - E, I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ö" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x40000810, /* ( |M| ) - E, L */
			/* C : */ 0x40000480, /* ( |M| ) - H, K */
			/* D : */ 0x60000110, /* (B|M| ) - E, I */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x60000830, /* (B|M| ) - E, F, L */
			/* G : */ 0x40000810, /* ( |M| ) - E, L */
			/* H : */ 0x40123810, /* ( |M| ) - E, L, M, N, R, U */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x60004010, /* (B|M| ) - E, O */
			/* L : */ 0xe2000c3a, /* (B|M|E) - B, D, E, F, K, L, Z */
			/* M : */ 0x40181110, /* ( |M| ) - E, I, M, T, U */
			/* N : */ 0xc01c2954, /* ( |M|E) - C, E, G, I, L, N, S, T, U */
			/* O : */ 0x40008000, /* ( |M| ) - P */
			/* P : */ 0x40048020, /* ( |M| ) - F, P, S */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xc01eb57a, /* ( |M|E) - B, D, E, F, G, I, K, M, N, P, R, S, T, U */
			/* S : */ 0xe01c0914, /* (B|M|E) - C, E, I, L, S, T, U */
			/* T : */ 0x42180114, /* ( |M| ) - C, E, I, T, U, Z */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x40020010, /* ( |M| ) - E, R */
			/* W : */ 0x40000010, /* ( |M| ) - E */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ü" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x60160810, /* (B|M| ) - E, L, R, S, U */
			/* C : */ 0x40000480, /* ( |M| ) - H, K */
			/* D : */ 0x40444111, /* ( |M| ) - A, E, I, O, S, W */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x40180030, /* ( |M| ) - E, F, T, U */
			/* G : */ 0x40180952, /* ( |M| ) - B, E, G, I, L, T, U */
			/* H : */ 0x400e3a12, /* ( |M| ) - B, E, J, L, M, N, R, S, T */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0xc00c8810, /* ( |M|E) - E, L, P, S, T */
			/* M : */ 0xc0009810, /* ( |M|E) - E, L, M, P */
			/* N : */ 0xc204247c, /* ( |M|E) - C, D, E, F, G, K, N, S, Z */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x40008020, /* ( |M| ) - F, P */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xc24e5d7e, /* ( |M|E) - B, C, D, E, F, G, I, K, L, M, O, R, S, T, W, Z */
			/* S : */ 0x400c0114, /* ( |M| ) - C, E, I, S, T */
			/* T : */ 0x420c0910, /* ( |M| ) - E, I, L, S, T, Z */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		}
	}, /* End of Enum 3 / 5 */
	/* ---------------------------------- *
	 * --- texts / en => NST_TEXTS_EN --- *
	 * ---------------------------------- */
	{
		{ /* Rules following letter "A" */
			/* A : */ 0x60022800, /* (B|M| ) - L, N and R */
			/* B : */ 0xe1166b9b, /* (B|M|E) - A, B, D, E, H, I, J, L, N, O, R, S, U and Y */
			/* C : */ 0xe11f4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, Q, R, S, T, U and Y */
			/* D : */ 0xe17efbff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* E : */ 0xe03e784d, /* (B|M|E) - A, C, D, G, L, M, N, O, R, S, T, U and V */
			/* F : */ 0xe00a4931, /* (B|M|E) - A, E, F, I, L, O, R and T */
			/* G : */ 0xe01679d9, /* (B|M|E) - A, D, E, G, H, I, L, M, N, O, R, S and U */
			/* H : */ 0xe0147911, /* (B|M|E) - A, E, I, L, M, N, O, S and U */
			/* I : */ 0xe02ebc5f, /* (B|M|E) - A, B, C, D, E, G, K, L, M, N, P, R, S, T and V */
			/* J : */ 0x40004001, /* ( |M| ) - A and O */
			/* K : */ 0xe1144db1, /* (B|M|E) - A, E, F, H, I, K, L, O, S, U and Y */
			/* L : */ 0xe17efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* M : */ 0xe156f913, /* (B|M|E) - A, B, E, I, L, M, N, O, P, R, S, U, W and Y */
			/* N : */ 0xe3fd7fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, Q, S, T, U, V, W, X, Y and Z */
			/* O : */ 0xc0020800, /* ( |M|E) - L and R */
			/* P : */ 0xe11ec991, /* (B|M|E) - A, E, H, I, L, O, P, R, S, T, U and Y */
			/* Q : */ 0x60100000, /* (B|M| ) - U,  */
			/* R : */ 0xe17fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W and Y */
			/* S : */ 0xe11ddd97, /* (B|M|E) - A, B, C, E, H, I, K, L, M, O, P, Q, S, T, U and Y */
			/* T : */ 0xe35e7db7, /* (B|M|E) - A, B, C, E, F, H, I, K, L, M, N, O, R, S, T, U, W, Y and Z */
			/* U : */ 0xe0aebc7e, /* (B|M|E) - B, C, D, E, F, G, K, L, M, N, P, R, S, T, V and X */
			/* V : */ 0xe1124911, /* (B|M|E) - A, E, I, L, O, R, U and Y */
			/* W : */ 0xe10e7d3b, /* (B|M|E) - A, B, D, E, F, I, K, L, M, N, O, R, S, T and Y */
			/* X : */ 0xe0084111, /* (B|M|E) - A, E, I, O and T */
			/* Y : */ 0xe04ef9bf, /* (B|M|E) - A, B, C, D, E, F, H, I, L, M, N, O, P, R, S, T and W */
			/* Z : */ 0xe3005111, /* (B|M|E) - A, E, I, M, O, Y and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "B" */
			/* A : */ 0xe15ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, W and Y */
			/* B : */ 0xc1024911, /* ( |M|E) - A, E, I, L, O, R and Y */
			/* C : */ 0x40004001, /* ( |M| ) - A and O */
			/* D : */ 0x40100100, /* ( |M| ) - I and U */
			/* E : */ 0xe36f7dfd, /* (B|M|E) - A, C, D, E, F, G, H, I, K, L, M, N, O, Q, R, S, T, V, W, Y and Z */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40004001, /* ( |M| ) - A and O */
			/* I : */ 0xe23efc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, V and Z */
			/* J : */ 0x40100010, /* ( |M| ) - E and U */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* N : */ 0x40004010, /* ( |M| ) - E and O */
			/* O : */ 0xe1def9df, /* (B|M|E) - A, B, C, D, E, G, H, I, L, M, N, O, P, R, S, T, U, W, X and Y */
			/* P : */ 0x40020000, /* ( |M| ) - R,  */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc0184114, /* ( |M|E) - C, E, I, O, T and U */
			/* T : */ 0x40020811, /* ( |M| ) - A, E, L and R */
			/* U : */ 0x630e7d7e, /* (B|M| ) - B, C, D, E, F, G, I, K, L, M, N, O, R, S, T, Y and Z */
			/* V : */ 0x40000010, /* ( |M| ) - E,  */
			/* W : */ 0x40000010, /* ( |M| ) - E,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00e2810, /* (B|M|E) - E, L, N, R, S and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "C" */
			/* A : */ 0xe13ebffe, /* (B|M|E) - B, C, D, E, F, G, H, I, J, K, L, M, N, P, R, S, T, U, V and Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40124991, /* ( |M| ) - A, E, H, I, L, O, R and U */
			/* D : */ 0x40004000, /* ( |M| ) - O,  */
			/* E : */ 0xe14ef93f, /* (B|M|E) - A, B, C, D, E, F, I, L, M, N, O, P, R, S, T, W and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe15ef93b, /* (B|M|E) - A, B, D, E, F, I, L, M, N, O, P, R, S, T, U, W and Y */
			/* I : */ 0xe23ef87f, /* (B|M|E) - A, B, C, D, E, F, G, L, M, N, O, P, R, S, T, U, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc15ef9bf, /* ( |M|E) - A, B, C, D, E, F, H, I, L, M, N, O, P, R, S, T, U, W and Y */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000001, /* ( |M| ) - A,  */
			/* N : */ 0x40000010, /* ( |M| ) - E,  */
			/* O : */ 0xe3fffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc0004000, /* ( |M|E) - O,  */
			/* T : */ 0xc1167931, /* ( |M|E) - A, E, F, I, L, M, N, O, R, S, U and Y */
			/* U : */ 0x600ef93f, /* (B|M| ) - A, B, C, D, E, F, I, L, M, N, O, P, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00af80f, /* (B|M|E) - A, B, C, D, L, M, N, O, P, R and T */
			/* Z : */ 0x20000010, /* (B| | ) - E,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "D" */
			/* A : */ 0xe37eb9fe, /* (B|M|E) - B, C, D, E, F, G, H, I, L, M, N, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x41124111, /* ( |M| ) - A, E, I, O, R, U and Y */
			/* C : */ 0x40104881, /* ( |M| ) - A, H, L, O and U */
			/* D : */ 0xc1124911, /* ( |M|E) - A, E, I, L, O, R, U and Y */
			/* E : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0x40104101, /* ( |M| ) - A, I, O and U */
			/* G : */ 0x41001111, /* ( |M| ) - A, E, I, M and Y */
			/* H : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* I : */ 0xe2befc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, V, X and Z */
			/* J : */ 0x40104011, /* ( |M| ) - A, E, O and U */
			/* K : */ 0x40000100, /* ( |M| ) - I,  */
			/* L : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* M : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* N : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* O : */ 0xe2fef95d, /* (B|M|E) - A, C, D, E, G, I, L, M, N, O, P, R, S, T, U, V, W, X and Z */
			/* P : */ 0x40004101, /* ( |M| ) - A, I and O */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc048d995, /* ( |M|E) - A, C, E, H, I, L, M, O, P, T and W */
			/* T : */ 0xc0000080, /* ( |M|E) - H,  */
			/* U : */ 0xe00efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S and T */
			/* V : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* W : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00c3911, /* (B|M|E) - A, E, I, L, M, N, S and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "E" */
			/* A : */ 0xe27ebcfe, /* (B|M|E) - B, C, D, E, F, G, H, K, L, M, N, P, R, S, T, U, V, W and Z */
			/* B : */ 0xe0164913, /* (B|M|E) - A, B, E, I, L, O, R, S and U */
			/* C : */ 0xe11e4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, R, S, T, U and Y */
			/* D : */ 0xe156797f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, O, R, S, U, W and Y */
			/* E : */ 0xe26ebdef, /* (B|M|E) - A, B, C, D, F, G, H, I, K, L, M, N, P, R, S, T, V, W and Z */
			/* F : */ 0xe11e4931, /* (B|M|E) - A, E, F, I, L, O, R, S, T, U and Y */
			/* G : */ 0xe11e79d3, /* (B|M|E) - A, B, E, G, H, I, L, M, N, O, R, S, T, U and Y */
			/* H : */ 0xc1124111, /* ( |M|E) - A, E, I, O, R, U and Y */
			/* I : */ 0xe22e7c6d, /* (B|M|E) - A, C, D, F, G, K, L, M, N, O, R, S, T, V and Z */
			/* J : */ 0x60104011, /* (B|M| ) - A, E, O and U */
			/* K : */ 0xe0042911, /* (B|M|E) - A, E, I, L, N and S */
			/* L : */ 0xe17efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* M : */ 0xe114f913, /* (B|M|E) - A, B, E, I, L, M, N, O, P, S, U and Y */
			/* N : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe07eb86c, /* (B|M|E) - C, D, F, G, L, M, N, P, R, S, T, U, V and W */
			/* P : */ 0xe11ec991, /* (B|M|E) - A, E, H, I, L, O, P, R, S, T, U and Y */
			/* Q : */ 0x60100000, /* (B|M| ) - U,  */
			/* R : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* S : */ 0xe15ddd9f, /* (B|M|E) - A, B, C, D, E, H, I, K, L, M, O, P, Q, S, T, U, W and Y */
			/* T : */ 0xe35e69b7, /* (B|M|E) - A, B, C, E, F, H, I, L, N, O, R, S, T, U, W, Y and Z */
			/* U : */ 0xe02eb85e, /* (B|M|E) - B, C, D, E, G, L, M, N, P, R, S, T and V */
			/* V : */ 0xe1024111, /* (B|M|E) - A, E, I, O, R and Y */
			/* W : */ 0xe10e699b, /* (B|M|E) - A, B, D, E, H, I, L, N, O, R, S, T and Y */
			/* X : */ 0xe018c195, /* (B|M|E) - A, C, E, H, I, O, P, T and U */
			/* Y : */ 0xe004f91f, /* (B|M|E) - A, B, C, D, E, I, L, M, N, O, P and S */
			/* Z : */ 0xe2004110, /* (B|M|E) - E, I, O and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "F" */
			/* A : */ 0xe1fe395e, /* (B|M|E) - B, C, D, E, G, I, L, M, N, R, S, T, U, V, W, X and Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40004000, /* ( |M| ) - O,  */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe12e391f, /* (B|M|E) - A, B, C, D, E, I, L, M, N, R, S, T, V and Y */
			/* F : */ 0xc1166911, /* ( |M|E) - A, E, I, L, N, O, R, S, U and Y */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe28e687f, /* (B|M|E) - A, B, C, D, E, F, G, L, N, O, R, S, T, X and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x40000010, /* ( |M| ) - E,  */
			/* O : */ 0xe0d67955, /* (B|M|E) - A, C, E, G, I, L, M, N, O, R, S, U, W and X */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0x80000000, /* ( | |E) - None are allowed */
			/* T : */ 0xc1044191, /* ( |M|E) - A, E, H, I, O, S and Y */
			/* U : */ 0x620e3844, /* (B|M| ) - C, G, L, M, N, R, S, T and Z */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0000100, /* ( |M|E) - I,  */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "G" */
			/* A : */ 0xe37eb95e, /* (B|M|E) - B, C, D, E, G, I, L, M, N, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x40004001, /* ( |M| ) - A and O */
			/* E : */ 0xe14e79bb, /* (B|M|E) - A, B, D, E, F, H, I, L, M, N, O, R, S, T, W and Y */
			/* F : */ 0x40104100, /* ( |M| ) - I, O and U */
			/* G : */ 0xc1064911, /* ( |M|E) - A, E, I, L, O, R, S and Y */
			/* H : */ 0xe04c6913, /* (B|M|E) - A, B, E, I, L, N, O, S, T and W */
			/* I : */ 0xe23e787f, /* (B|M|E) - A, B, C, D, E, F, G, L, M, N, O, R, S, T, U, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0xe0145111, /* (B|M|E) - A, E, I, M, O, S and U */
			/* O : */ 0xe07e795b, /* (B|M|E) - A, B, D, E, G, I, L, M, N, O, R, S, T, U, V and W */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* S : */ 0xc0080080, /* ( |M|E) - H and T */
			/* T : */ 0x40004080, /* ( |M| ) - H and O */
			/* U : */ 0x600e7911, /* (B|M| ) - A, E, I, L, M, N, O, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe002b000, /* (B|M|E) - M, N, P and R */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "H" */
			/* A : */ 0xe37ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x40104011, /* ( |M| ) - A, E, O and U */
			/* C : */ 0x40000801, /* ( |M| ) - A and L */
			/* D : */ 0x40024000, /* ( |M| ) - O and R */
			/* E : */ 0xe1fff97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, O, P, Q, R, S, T, U, V, W, X and Y */
			/* F : */ 0x40104100, /* ( |M| ) - I, O and U */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40004000, /* ( |M| ) - O,  */
			/* I : */ 0xe23efc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0xc1004111, /* ( |M|E) - A, E, I, O and Y */
			/* M : */ 0xc0004111, /* ( |M|E) - A, E, I and O */
			/* N : */ 0xc0046111, /* ( |M|E) - A, E, I, N, O and S */
			/* O : */ 0xe17efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* P : */ 0x40004001, /* ( |M| ) - A and O */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc0080080, /* ( |M|E) - H and T */
			/* T : */ 0xc11469b1, /* ( |M|E) - A, E, F, H, I, L, N, O, S, U and Y */
			/* U : */ 0xe00e387f, /* (B|M|E) - A, B, C, D, E, F, G, L, M, N, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00e985b, /* (B|M|E) - A, B, D, E, G, L, M, P, R, S and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "I" */
			/* A : */ 0xe00eb8de, /* (B|M|E) - B, C, D, E, G, H, L, M, N, P, R, S and T */
			/* B : */ 0xe1164913, /* (B|M|E) - A, B, E, I, L, O, R, S, U and Y */
			/* C : */ 0xe11e6d95, /* (B|M|E) - A, C, E, H, I, K, L, N, O, R, S, T, U and Y */
			/* D : */ 0xe15e6959, /* (B|M|E) - A, D, E, G, I, L, N, O, R, S, T, U, W and Y */
			/* E : */ 0xc07e3d6e, /* ( |M|E) - B, C, D, F, G, I, K, L, M, N, R, S, T, U, V and W */
			/* F : */ 0xe11a4931, /* (B|M|E) - A, E, F, I, L, O, R, T, U and Y */
			/* G : */ 0xe15679d3, /* (B|M|E) - A, B, E, G, H, I, L, M, N, O, R, S, U, W and Y */
			/* H : */ 0x40000100, /* ( |M| ) - I,  */
			/* I : */ 0x80000000, /* ( | |E) - None are allowed */
			/* J : */ 0x40004001, /* ( |M| ) - A and O */
			/* K : */ 0xc0044591, /* ( |M|E) - A, E, H, I, K, O and S */
			/* L : */ 0xe17efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* M : */ 0xe014f913, /* (B|M|E) - A, B, E, I, L, M, N, O, P, S and U */
			/* N : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe03eb85e, /* (B|M|E) - B, C, D, E, G, L, M, N, P, R, S, T, U and V */
			/* P : */ 0xe05ed997, /* (B|M|E) - A, B, C, E, H, I, L, M, O, P, R, S, T, U and W */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe17efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* S : */ 0xe15fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, W and Y */
			/* T : */ 0xe35e79b7, /* (B|M|E) - A, B, C, E, F, H, I, L, M, N, O, R, S, T, U, W, Y and Z */
			/* U : */ 0x40041000, /* ( |M| ) - M and S */
			/* V : */ 0xe1204111, /* (B|M|E) - A, E, I, O, V and Y */
			/* W : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* X : */ 0xc0084111, /* ( |M|E) - A, E, I, O and T */
			/* Y : */ 0x40000001, /* ( |M| ) - A,  */
			/* Z : */ 0xe2004111, /* (B|M|E) - A, E, I, O and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "J" */
			/* A : */ 0xe367bdde, /* (B|M|E) - B, C, D, E, G, H, I, K, L, M, N, P, Q, R, S, V, W, Y and Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x604e783d, /* (B|M| ) - A, C, D, E, F, L, M, N, O, R, S, T and W */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe0083842, /* (B|M|E) - B, G, L, M, N and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe13e2ddf, /* (B|M|E) - A, B, C, D, E, G, H, I, K, L, N, R, S, T, U, V and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x602e394b, /* (B|M| ) - A, B, D, G, I, L, M, N, R, S, T and V */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "K" */
			/* A : */ 0xe14ef9de, /* (B|M|E) - B, C, D, E, G, H, I, L, M, N, O, P, R, S, T, W and Y */
			/* B : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* C : */ 0x40000080, /* ( |M| ) - H,  */
			/* D : */ 0x40000001, /* ( |M| ) - A,  */
			/* E : */ 0xe16ef97b, /* (B|M|E) - A, B, D, E, F, G, I, L, M, N, O, P, R, S, T, V, W and Y */
			/* F : */ 0x40104101, /* ( |M| ) - A, I, O and U */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x60004011, /* (B|M| ) - A, E and O */
			/* I : */ 0xe00eb81d, /* (B|M|E) - A, C, D, E, L, M, N, P, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* O : */ 0xe076e822, /* (B|M|E) - B, F, L, N, O, P, R, S, U, V and W */
			/* P : */ 0x40004000, /* ( |M| ) - O,  */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc0185991, /* ( |M|E) - A, E, H, I, L, M, O, T and U */
			/* T : */ 0x40004001, /* ( |M| ) - A and O */
			/* U : */ 0x60069800, /* (B|M| ) - L, M, P, R and S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0020801, /* (B|M|E) - A, L and R */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "L" */
			/* A : */ 0xe3fefdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0x41124111, /* ( |M| ) - A, E, I, O, R, U and Y */
			/* C : */ 0x41104191, /* ( |M| ) - A, E, H, I, O, U and Y */
			/* D : */ 0xc15e6937, /* ( |M|E) - A, B, C, E, F, I, L, N, O, R, S, T, U, W and Y */
			/* E : */ 0xe1feffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X and Y */
			/* F : */ 0xc01e4915, /* ( |M|E) - A, C, E, I, L, O, R, S, T and U */
			/* G : */ 0x40024111, /* ( |M| ) - A, E, I, O and R */
			/* H : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* I : */ 0xe3fffc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc1040111, /* ( |M|E) - A, E, I, S and Y */
			/* L : */ 0xe15cf1bf, /* (B|M|E) - A, B, C, D, E, F, H, I, M, N, O, P, S, T, U, W and Y */
			/* M : */ 0xc0044111, /* ( |M|E) - A, E, I, O and S */
			/* N : */ 0xc0000011, /* ( |M|E) - A and E */
			/* O : */ 0xe17ff9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, Q, R, S, T, U, V, W and Y */
			/* P : */ 0xc00c4991, /* ( |M|E) - A, E, H, I, L, O, S and T */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x41004100, /* ( |M| ) - I, O and Y */
			/* S : */ 0xc108c193, /* ( |M|E) - A, B, E, H, I, O, P, T and Y */
			/* T : */ 0xc3164991, /* ( |M|E) - A, E, H, I, L, O, R, S, U, Y and Z */
			/* U : */ 0xe0aefd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, V and X */
			/* V : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* W : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe24eb9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, P, R, S, T, W and Z */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "M" */
			/* A : */ 0xe3febfde, /* (B|M|E) - B, C, D, E, G, H, I, J, K, L, M, N, P, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0xc1164911, /* ( |M|E) - A, E, I, L, O, R, S, U and Y */
			/* C : */ 0x60000d45, /* (B|M| ) - A, C, G, I, K and L */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe3cefd5f, /* (B|M|E) - A, B, C, D, E, G, I, K, L, M, N, O, P, R, S, T, W, X, Y and Z */
			/* F : */ 0x40004100, /* ( |M| ) - I and O */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe29efc7d, /* (B|M|E) - A, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, X and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000100, /* ( |M| ) - I,  */
			/* L : */ 0x41004110, /* ( |M| ) - E, I, O and Y */
			/* M : */ 0xc1104111, /* ( |M|E) - A, E, I, O, U and Y */
			/* N : */ 0xc0004111, /* ( |M|E) - A, E, I and O */
			/* O : */ 0xe37efddf, /* (B|M|E) - A, B, C, D, E, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* P : */ 0xc11e4991, /* ( |M|E) - A, E, H, I, L, O, R, S, T, U and Y */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40004001, /* ( |M| ) - A and O */
			/* S : */ 0xc0084190, /* ( |M|E) - E, H, I, O and T */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x620e387c, /* (B|M| ) - C, D, E, F, G, L, M, N, R, S, T and Z */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00e6815, /* (B|M|E) - A, C, E, L, N, O, R, S and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "N" */
			/* A : */ 0xe37ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* C : */ 0xc11a4d91, /* ( |M|E) - A, E, H, I, K, L, O, R, T, U and Y */
			/* D : */ 0xc15ef9bf, /* ( |M|E) - A, B, C, D, E, F, H, I, L, M, N, O, P, R, S, T, U, W and Y */
			/* E : */ 0xe3fff9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* G : */ 0xc15e79bb, /* ( |M|E) - A, B, D, E, F, H, I, L, M, N, O, R, S, T, U, W and Y */
			/* H : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* I : */ 0xe2bffcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, O, P, Q, R, S, T, U, V, X and Z */
			/* J : */ 0x40104011, /* ( |M| ) - A, E, O and U */
			/* K : */ 0xc10469b1, /* ( |M|E) - A, E, F, H, I, L, N, O, S and Y */
			/* L : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* M : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* N : */ 0xc1144111, /* ( |M|E) - A, E, I, O, S, U and Y */
			/* O : */ 0xe3fef97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc17cddf7, /* ( |M|E) - A, B, C, E, F, G, H, I, K, L, M, O, P, S, T, U, V, W and Y */
			/* T : */ 0xc35679b1, /* ( |M|E) - A, E, F, H, I, L, M, N, O, R, S, U, W, Y and Z */
			/* U : */ 0xe00efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S and T */
			/* V : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* W : */ 0x40024111, /* ( |M| ) - A, E, I, O and R */
			/* X : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Y : */ 0xe0445911, /* (B|M|E) - A, E, I, L, M, O, S and W */
			/* Z : */ 0xc1004111, /* ( |M|E) - A, E, I, O and Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "O" */
			/* A : */ 0xe08ebc6c, /* (B|M|E) - C, D, F, G, K, L, M, N, P, R, S, T and X */
			/* B : */ 0xe11e4b1b, /* (B|M|E) - A, B, D, E, I, J, L, O, R, S, T, U and Y */
			/* C : */ 0xe11a4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, R, T, U and Y */
			/* D : */ 0xe15659ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, O, R, S, U, W and Y */
			/* E : */ 0xe17e792f, /* (B|M|E) - A, B, C, D, F, I, L, M, N, O, R, S, T, U, V, W and Y */
			/* F : */ 0xe01c4931, /* (B|M|E) - A, E, F, I, L, O, S, T and U */
			/* G : */ 0xe11e79d9, /* (B|M|E) - A, D, E, G, H, I, L, M, N, O, R, S, T, U and Y */
			/* H : */ 0xe0006111, /* (B|M|E) - A, E, I, N and O */
			/* I : */ 0xe00e284c, /* (B|M|E) - C, D, G, L, N, R, S and T */
			/* J : */ 0x40004010, /* ( |M| ) - E and O */
			/* K : */ 0xe1144d11, /* (B|M|E) - A, E, I, K, L, O, S, U and Y */
			/* L : */ 0xe17cfd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, S, T, U, V, W and Y */
			/* M : */ 0xe114f933, /* (B|M|E) - A, B, E, F, I, L, M, N, O, P, S, U and Y */
			/* N : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe22ebd7c, /* (B|M|E) - C, D, E, F, G, I, K, L, M, N, P, R, S, T, V and Z */
			/* P : */ 0xe11edd91, /* (B|M|E) - A, E, H, I, K, L, M, O, P, R, S, T, U and Y */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe17ffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W and Y */
			/* S : */ 0xe15cd997, /* (B|M|E) - A, B, C, E, H, I, L, M, O, P, S, T, U, W and Y */
			/* T : */ 0xe15ec997, /* (B|M|E) - A, B, C, E, H, I, L, O, P, R, S, T, U, W and Y */
			/* U : */ 0xe02ea97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, N, P, R, S, T and V */
			/* V : */ 0xe0004111, /* (B|M|E) - A, E, I and O */
			/* W : */ 0xe30ef9bf, /* (B|M|E) - A, B, C, D, E, F, H, I, L, M, N, O, P, R, S, T, Y and Z */
			/* X : */ 0xe1080111, /* (B|M|E) - A, E, I, T and Y */
			/* Y : */ 0xc004591d, /* ( |M|E) - A, C, D, E, I, L, M, O and S */
			/* Z : */ 0xe3004111, /* (B|M|E) - A, E, I, O, Y and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "P" */
			/* A : */ 0xe1feb9de, /* (B|M|E) - B, C, D, E, G, H, I, L, M, N, P, R, S, T, U, V, W, X and Y */
			/* B : */ 0x40004000, /* ( |M| ) - O,  */
			/* C : */ 0x40004080, /* ( |M| ) - H and O */
			/* D : */ 0x40000001, /* ( |M| ) - A,  */
			/* E : */ 0xe35efd7d, /* (B|M|E) - A, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, W, Y and Z */
			/* F : */ 0x40120000, /* ( |M| ) - R and U */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe11e4911, /* (B|M|E) - A, E, I, L, O, R, S, T, U and Y */
			/* I : */ 0xe21ffc7d, /* (B|M|E) - A, C, D, E, F, G, K, L, M, N, O, P, Q, R, S, T, U and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000100, /* ( |M| ) - I,  */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0x40004010, /* ( |M| ) - E and O */
			/* O : */ 0xe0fefd5d, /* (B|M|E) - A, C, D, E, G, I, K, L, M, N, O, P, R, S, T, U, V, W and X */
			/* P : */ 0xc1124911, /* ( |M|E) - A, E, I, L, O, R, U and Y */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xe1184191, /* (B|M|E) - A, E, H, I, O, T, U and Y */
			/* T : */ 0xe1166191, /* (B|M|E) - A, E, H, I, N, O, R, S, U and Y */
			/* U : */ 0x600eb87e, /* (B|M| ) - B, C, D, E, F, G, L, M, N, P, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000101, /* ( |M| ) - A and I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00a0900, /* (B|M|E) - I, L, R and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Q" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "R" */
			/* A : */ 0xe3fffffe, /* (B|M|E) - B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0xc1164911, /* ( |M|E) - A, E, I, L, O, R, S, U and Y */
			/* C : */ 0xc11a4991, /* ( |M|E) - A, E, H, I, L, O, R, T, U and Y */
			/* D : */ 0xc15e7993, /* ( |M|E) - A, B, E, H, I, L, M, N, O, R, S, T, U, W and Y */
			/* E : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0xc0104911, /* ( |M|E) - A, E, I, L, O and U */
			/* G : */ 0xc1164991, /* ( |M|E) - A, E, H, I, L, O, R, S, U and Y */
			/* H : */ 0x61184111, /* (B|M| ) - A, E, I, O, T, U and Y */
			/* I : */ 0xe2bdfc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, Q, S, T, U, V, X and Z */
			/* J : */ 0x40104010, /* ( |M| ) - E, O and U */
			/* K : */ 0xc1445991, /* ( |M|E) - A, E, H, I, L, M, O, S, W and Y */
			/* L : */ 0xc1144119, /* ( |M|E) - A, D, E, I, O, S, U and Y */
			/* M : */ 0xc1144991, /* ( |M|E) - A, E, H, I, L, O, S, U and Y */
			/* N : */ 0xc11c59b3, /* ( |M|E) - A, B, E, F, H, I, L, M, O, S, T, U and Y */
			/* O : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0xc01e4991, /* ( |M|E) - A, E, H, I, L, O, R, S, T and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xc1104191, /* ( |M|E) - A, E, H, I, O, U and Y */
			/* S : */ 0xc058d597, /* ( |M|E) - A, B, C, E, H, I, K, M, O, P, T, U and W */
			/* T : */ 0xc35679b7, /* ( |M|E) - A, B, C, E, F, H, I, L, M, N, O, R, S, U, W, Y and Z */
			/* U : */ 0xe00eb97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, P, R, S and T */
			/* V : */ 0xc1004111, /* ( |M|E) - A, E, I, O and Y */
			/* W : */ 0x41024191, /* ( |M| ) - A, E, H, I, O, R and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe04cf95f, /* (B|M|E) - A, B, C, D, E, G, I, L, M, N, O, P, S, T and W */
			/* Z : */ 0x40000011, /* ( |M| ) - A and E */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "S" */
			/* A : */ 0xe1febdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, X and Y */
			/* B : */ 0x41124011, /* ( |M| ) - A, E, O, R, U and Y */
			/* C : */ 0xe1124991, /* (B|M|E) - A, E, H, I, L, O, R, U and Y */
			/* D : */ 0x40024111, /* ( |M| ) - A, E, I, O and R */
			/* E : */ 0xe1fffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, X and Y */
			/* F : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* G : */ 0x40124101, /* ( |M| ) - A, I, O, R and U */
			/* H : */ 0xe15afd37, /* (B|M|E) - A, B, C, E, F, I, K, L, M, N, O, P, R, T, U, W and Y */
			/* I : */ 0xe2befc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, V, X and Z */
			/* J : */ 0x40104000, /* ( |M| ) - O and U */
			/* K : */ 0xe1164111, /* (B|M|E) - A, E, I, O, R, S, U and Y */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0xe1144111, /* (B|M|E) - A, E, I, O, S, U and Y */
			/* N : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* O : */ 0xe07ef97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, O, P, R, S, T, U, V and W */
			/* P : */ 0xe1124991, /* (B|M|E) - A, E, H, I, L, O, R, U and Y */
			/* Q : */ 0x60100000, /* (B|M| ) - U,  */
			/* R : */ 0x40104011, /* ( |M| ) - A, E, O and U */
			/* S : */ 0xc158f9b7, /* ( |M|E) - A, B, C, E, F, H, I, L, M, N, O, P, T, U, W and Y */
			/* T : */ 0xe156f9b7, /* (B|M|E) - A, B, C, E, F, H, I, L, M, N, O, P, R, S, U, W and Y */
			/* U : */ 0xe20ebd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, P, R, S, T and Z */
			/* V : */ 0x60000110, /* (B|M| ) - E and I */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe006b80e, /* (B|M|E) - B, C, D, L, M, N, P, R and S */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "T" */
			/* A : */ 0xe1febdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, X and Y */
			/* B : */ 0x40124111, /* ( |M| ) - A, E, I, O, R and U */
			/* C : */ 0x40024881, /* ( |M| ) - A, H, L, O and R */
			/* D : */ 0x40004000, /* ( |M| ) - O,  */
			/* E : */ 0xe1fef9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, R, S, T, U, V, W, X and Y */
			/* F : */ 0x40104101, /* ( |M| ) - A, I, O and U */
			/* G : */ 0x40124010, /* ( |M| ) - E, O, R and U */
			/* H : */ 0xe15ef93f, /* (B|M|E) - A, B, C, D, E, F, I, L, M, N, O, P, R, S, T, U, W and Y */
			/* I : */ 0xe27ffc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, Q, R, S, T, U, V, W and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000100, /* ( |M| ) - I,  */
			/* L : */ 0xc1004111, /* ( |M|E) - A, E, I, O and Y */
			/* M : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* N : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* O : */ 0xe3fefd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0x40024011, /* ( |M| ) - A, E, O and R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xe158d595, /* (B|M|E) - A, C, E, H, I, K, M, O, P, T, U, W and Y */
			/* T : */ 0xc1166991, /* ( |M|E) - A, E, H, I, L, N, O, R, S, U and Y */
			/* U : */ 0xe00ef97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, O, P, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe006b801, /* (B|M|E) - A, L, M, N, P, R and S */
			/* Z : */ 0xc0000111, /* ( |M|E) - A, E and I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "U" */
			/* A : */ 0xc12e2d4e, /* ( |M|E) - B, C, D, G, I, K, L, N, R, S, T, V and Y */
			/* B : */ 0xc03efb3f, /* ( |M|E) - A, B, C, D, E, F, I, J, L, M, N, O, P, R, S, T, U and V */
			/* C : */ 0x401a4d95, /* ( |M| ) - A, C, E, H, I, K, L, O, R, T and U */
			/* D : */ 0xe1064959, /* (B|M|E) - A, D, E, G, I, L, O, R, S and Y */
			/* E : */ 0xc21e293b, /* ( |M|E) - A, B, D, E, F, I, L, N, R, S, T, U and Z */
			/* F : */ 0x40084021, /* ( |M| ) - A, F, O and T */
			/* G : */ 0xe01479d1, /* (B|M|E) - A, E, G, H, I, L, M, N, O, S and U */
			/* H : */ 0x40000001, /* ( |M| ) - A,  */
			/* I : */ 0xc22ea81f, /* ( |M|E) - A, B, C, D, E, L, N, P, R, S, T, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc0000511, /* ( |M|E) - A, E, I and K */
			/* L : */ 0xe13efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V and Y */
			/* M : */ 0xe014f933, /* (B|M|E) - A, B, E, F, I, L, M, N, O, P, S and U */
			/* N : */ 0xe17fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W and Y */
			/* O : */ 0x411a0000, /* ( |M| ) - R, T, U and Y */
			/* P : */ 0xe15ec997, /* (B|M|E) - A, B, C, E, H, I, L, O, P, R, S, T, U, W and Y */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xe17efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* S : */ 0xe11ced97, /* (B|M|E) - A, B, C, E, H, I, K, L, N, O, P, S, T, U and Y */
			/* T : */ 0xe35ef9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, R, S, T, U, W, Y and Z */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x40020111, /* ( |M| ) - A, E, I and R */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0xc0180110, /* ( |M|E) - E, I, T and U */
			/* Y : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Z : */ 0xc2000011, /* ( |M|E) - A, E and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "V" */
			/* A : */ 0xe01ead4e, /* (B|M|E) - B, C, D, G, I, K, L, N, P, R, S, T and U */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe18e39dd, /* (B|M|E) - A, C, D, E, G, H, I, L, M, N, R, S, T, X and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe03eec7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, N, O, P, R, S, T, U and V */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x40000001, /* ( |M| ) - A,  */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe15e2d04, /* (B|M|E) - C, I, K, L, N, R, S, T, U, W and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* S : */ 0x40000400, /* ( |M| ) - K,  */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60000800, /* (B|M| ) - L,  */
			/* V : */ 0x41000100, /* ( |M| ) - I and Y */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0002000, /* ( |M|E) - N,  */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "W" */
			/* A : */ 0xe1bebd7e, /* (B|M|E) - B, C, D, E, F, G, I, K, L, M, N, P, R, S, T, U, V, X and Y */
			/* B : */ 0x40024011, /* ( |M| ) - A, E, O and R */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0xc0020910, /* ( |M|E) - E, I, L and R */
			/* E : */ 0xe10e295b, /* (B|M|E) - A, B, D, E, G, I, L, N, R, S, T and Y */
			/* F : */ 0x40104000, /* ( |M| ) - O and U */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* I : */ 0x622eb87c, /* (B|M| ) - C, D, E, F, G, L, M, N, P, R, S, T, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc0000110, /* ( |M|E) - E and I */
			/* L : */ 0xc1040111, /* ( |M|E) - A, E, I, S and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0xc1040999, /* ( |M|E) - A, D, E, H, I, L, S and Y */
			/* O : */ 0x601a7c1a, /* (B|M| ) - B, D, E, K, L, M, N, O, R, T and U */
			/* P : */ 0x40004000, /* ( |M| ) - O,  */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* S : */ 0xc048c090, /* ( |M|E) - E, H, O, P, T and W */
			/* T : */ 0x40004080, /* ( |M| ) - H and O */
			/* U : */ 0x60020800, /* (B|M| ) - L and R */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0002814, /* (B|M|E) - C, E, L and N */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "X" */
			/* A : */ 0x600c3844, /* (B|M| ) - C, G, L, M, N, S and T */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40124991, /* ( |M| ) - A, E, H, I, L, O, R and U */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe006300c, /* (B|M|E) - C, D, M, N, R and S */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40004101, /* ( |M| ) - A, I and O */
			/* I : */ 0xe02c785f, /* (B|M|E) - A, B, C, D, E, G, L, M, N, O, S, T and V */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x40000010, /* ( |M| ) - E,  */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x400a2000, /* ( |M| ) - N, R and T */
			/* P : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0xc0124111, /* ( |M|E) - A, E, I, O, R and U */
			/* U : */ 0x40060801, /* ( |M| ) - A, L, R and S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0000800, /* (B|M|E) - L,  */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Y" */
			/* A : */ 0xe04e3cce, /* (B|M|E) - B, C, D, G, H, K, L, M, N, R, S, T and W */
			/* B : */ 0x40124111, /* ( |M| ) - A, E, I, O, R and U */
			/* C : */ 0x40024991, /* ( |M| ) - A, E, H, I, L, O and R */
			/* D : */ 0xc0026111, /* ( |M|E) - A, E, I, N, O and R */
			/* E : */ 0xe00e6809, /* (B|M|E) - A, D, L, N, O, R, S and T */
			/* F : */ 0xc0104000, /* ( |M|E) - O and U */
			/* G : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* H : */ 0x40004010, /* ( |M| ) - E and O */
			/* I : */ 0x60042010, /* (B|M| ) - E, N and S */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc0000010, /* ( |M|E) - E,  */
			/* L : */ 0xc1304911, /* ( |M|E) - A, E, I, L, O, U, V and Y */
			/* M : */ 0xc010f113, /* ( |M|E) - A, B, E, I, M, N, O, P and U */
			/* N : */ 0xc008615d, /* ( |M|E) - A, C, D, E, G, I, N, O and T */
			/* O : */ 0xe01e2c40, /* (B|M|E) - G, K, L, N, R, S, T and U */
			/* P : */ 0x401e6191, /* ( |M| ) - A, E, H, I, N, O, R, S, T and U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xc01a6111, /* ( |M|E) - A, E, I, N, O, R, T and U */
			/* S : */ 0xc00c5195, /* ( |M|E) - A, C, E, H, I, M, O, S and T */
			/* T : */ 0x40004191, /* ( |M| ) - A, E, H, I and O */
			/* U : */ 0x60040800, /* (B|M| ) - L and S */
			/* V : */ 0x60000010, /* (B|M| ) - E,  */
			/* W : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* X : */ 0xc0000100, /* ( |M|E) - I,  */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x40000010, /* ( |M| ) - E,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Z" */
			/* A : */ 0xe00abd46, /* (B|M|E) - B, C, G, I, K, L, M, N, P, R and T */
			/* B : */ 0x40000010, /* ( |M| ) - E,  */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe00ea81f, /* (B|M|E) - A, B, C, D, E, L, N, P, R, S and T */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40004000, /* ( |M| ) - O,  */
			/* I : */ 0xe008f851, /* (B|M|E) - A, E, G, L, M, N, O, P and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x40000110, /* ( |M| ) - E and I */
			/* M : */ 0x40000101, /* ( |M| ) - A and I */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe0027108, /* (B|M|E) - D, I, M, N, O and R */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60020000, /* (B|M| ) - R,  */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0001000, /* ( |M|E) - M,  */
			/* Z : */ 0xc1000911, /* ( |M|E) - A, E, I, L and Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ä" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ö" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ü" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		}
	}, /* End of Enum 4 / 5 */
	/* ------------------------------------ *
	 * --- texts / all => NST_TEXTS_ALL --- *
	 * ------------------------------------ */
	{
		{ /* Rules following letter "A" */
			/* A : */ 0xe00e2c4e, /* (B|M|E) - B, C, D, G, K, L, N, R, S and T */
			/* B : */ 0xe35efffb, /* (B|M|E) - A, B, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, W, Y and Z */
			/* C : */ 0xe11f4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, Q, R, S, T, U and Y */
			/* D : */ 0xe37efbff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* E : */ 0xe01e784d, /* (B|M|E) - A, C, D, G, L, M, N, O, R, S, T and U */
			/* F : */ 0xe00e4971, /* (B|M|E) - A, E, F, G, I, L, O, R, S and T */
			/* G : */ 0xe25ef9fb, /* (B|M|E) - A, B, D, E, F, G, H, I, L, M, N, O, P, R, S, T, U, W and Z */
			/* H : */ 0xe03e7d1b, /* (B|M|E) - A, B, D, E, I, K, L, M, N, O, R, S, T, U and V */
			/* I : */ 0xe26efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, V, W and Z */
			/* J : */ 0xc0004119, /* ( |M|E) - A, D, E, I and O */
			/* K : */ 0xe31e6db1, /* (B|M|E) - A, E, F, H, I, K, L, N, O, R, S, T, U, Y and Z */
			/* L : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* M : */ 0xe15efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W and Y */
			/* N : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xc00ebd02, /* ( |M|E) - B, I, K, L, M, N, P, R, S and T */
			/* P : */ 0xe11ee9b1, /* (B|M|E) - A, E, F, H, I, L, N, O, P, R, S, T, U and Y */
			/* Q : */ 0x60100000, /* (B|M| ) - U,  */
			/* R : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* S : */ 0xe37ffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* T : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* U : */ 0xe2febdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, X and Z */
			/* V : */ 0xe1124911, /* (B|M|E) - A, E, I, L, O, R, U and Y */
			/* W : */ 0xe10e6d3b, /* (B|M|E) - A, B, D, E, F, I, K, L, N, O, R, S, T and Y */
			/* X : */ 0xe0484111, /* (B|M|E) - A, E, I, O, T and W */
			/* Y : */ 0xe05e79bf, /* (B|M|E) - A, B, C, D, E, F, H, I, L, M, N, O, R, S, T, U and W */
			/* Z : */ 0xe3585911, /* (B|M|E) - A, E, I, L, M, O, T, U, W, Y and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "B" */
			/* A : */ 0xe35ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, W, Y and Z */
			/* B : */ 0xc1124911, /* ( |M|E) - A, E, I, L, O, R, U and Y */
			/* C : */ 0x40004080, /* ( |M| ) - H and O */
			/* D : */ 0x40124111, /* ( |M| ) - A, E, I, O, R and U */
			/* E : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* F : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* G : */ 0x40020811, /* ( |M| ) - A, E, L and R */
			/* H : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* I : */ 0xe27efcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, O, P, R, S, T, U, V, W and Z */
			/* J : */ 0x40100010, /* ( |M| ) - E and U */
			/* K : */ 0x40126801, /* ( |M| ) - A, L, N, O, R and U */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* N : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* O : */ 0xe3deffdf, /* (B|M|E) - A, B, C, D, E, G, H, I, J, K, L, M, N, O, P, R, S, T, U, W, X, Y and Z */
			/* P : */ 0x40020001, /* ( |M| ) - A and R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc21ccd77, /* ( |M|E) - A, B, C, E, F, G, I, K, L, O, P, S, T, U and Z */
			/* T : */ 0xc0064991, /* ( |M|E) - A, E, H, I, L, O, R and S */
			/* U : */ 0xe30e7dfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, O, R, S, T, Y and Z */
			/* V : */ 0x40000010, /* ( |M| ) - E,  */
			/* W : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe20e0810, /* (B|M|E) - E, L, R, S, T and Z */
			/* Z : */ 0x40100110, /* ( |M| ) - E, I and U */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "C" */
			/* A : */ 0xe13ebffe, /* (B|M|E) - B, C, D, E, F, G, H, I, J, K, L, M, N, P, R, S, T, U, V and Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40124991, /* ( |M| ) - A, E, H, I, L, O, R and U */
			/* D : */ 0x40004000, /* ( |M| ) - O,  */
			/* E : */ 0xe16efdbf, /* (B|M|E) - A, B, C, D, E, F, H, I, K, L, M, N, O, P, R, S, T, V, W and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* I : */ 0xe23ef8ff, /* (B|M|E) - A, B, C, D, E, F, G, H, L, M, N, O, P, R, S, T, U, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc37efdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x40000010, /* ( |M| ) - E,  */
			/* O : */ 0xe17efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0x80000000, /* ( | |E) - None are allowed */
			/* T : */ 0xc0166931, /* ( |M|E) - A, E, F, I, L, N, O, R, S and U */
			/* U : */ 0x600ef93b, /* (B|M| ) - A, B, D, E, F, I, L, M, N, O, P, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00eb807, /* (B|M|E) - A, B, C, L, M, N, P, R, S and T */
			/* Z : */ 0x60000010, /* (B|M| ) - E,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "D" */
			/* A : */ 0xe37ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x41124911, /* ( |M| ) - A, E, I, L, O, R, U and Y */
			/* C : */ 0x40104081, /* ( |M| ) - A, H, O and U */
			/* D : */ 0xc1124991, /* ( |M|E) - A, E, H, I, L, O, R, U and Y */
			/* E : */ 0xe3feffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* G : */ 0x41021911, /* ( |M| ) - A, E, I, L, M, R and Y */
			/* H : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* I : */ 0xe3befeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, R, S, T, U, V, X, Y and Z */
			/* J : */ 0x60104011, /* (B|M| ) - A, E, O and U */
			/* K : */ 0x40024111, /* ( |M| ) - A, E, I, O and R */
			/* L : */ 0xc1104111, /* ( |M|E) - A, E, I, O, U and Y */
			/* M : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* N : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* O : */ 0xe2fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X and Z */
			/* P : */ 0x40024921, /* ( |M| ) - A, F, I, L, O and R */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc17cddf7, /* ( |M|E) - A, B, C, E, F, G, H, I, K, L, M, O, P, S, T, U, V, W and Y */
			/* T : */ 0xc0160191, /* ( |M|E) - A, E, H, I, R, S and U */
			/* U : */ 0xe20efd3f, /* (B|M|E) - A, B, C, D, E, F, I, K, L, M, N, O, P, R, S, T and Z */
			/* V : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* W : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00c3913, /* (B|M|E) - A, B, E, I, L, M, N, S and T */
			/* Z : */ 0x40500111, /* ( |M| ) - A, E, I, U and W */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "E" */
			/* A : */ 0xe27ebcee, /* (B|M|E) - B, C, D, F, G, H, K, L, M, N, P, R, S, T, U, V, W and Z */
			/* B : */ 0xe21e6df3, /* (B|M|E) - A, B, E, F, G, H, I, K, L, N, O, R, S, T, U and Z */
			/* C : */ 0xe11a4d9d, /* (B|M|E) - A, C, D, E, H, I, K, L, O, R, T, U and Y */
			/* D : */ 0xe37e7dff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, R, S, T, U, V, W, Y and Z */
			/* E : */ 0xe26ebdef, /* (B|M|E) - A, B, C, D, F, G, H, I, K, L, M, N, P, R, S, T, V, W and Z */
			/* F : */ 0xe23e5d73, /* (B|M|E) - A, B, E, F, G, I, K, L, M, O, R, S, T, U, V and Z */
			/* G : */ 0xe35e7dfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, R, S, T, U, W, Y and Z */
			/* H : */ 0xe35efd7b, /* (B|M|E) - A, B, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, W, Y and Z */
			/* I : */ 0xe27effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W and Z */
			/* J : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* K : */ 0xe01e6d19, /* (B|M|E) - A, D, E, I, K, L, N, O, R, S, T and U */
			/* L : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* M : */ 0xe31efd7b, /* (B|M|E) - A, B, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, Y and Z */
			/* N : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe25ebd6e, /* (B|M|E) - B, C, D, F, G, I, K, L, M, N, P, R, S, T, U, W and Z */
			/* P : */ 0xe01ec9b1, /* (B|M|E) - A, E, F, H, I, L, O, P, R, S, T and U */
			/* Q : */ 0x60100000, /* (B|M| ) - U,  */
			/* R : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* S : */ 0xe37ffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* T : */ 0xe35e7df7, /* (B|M|E) - A, B, C, E, F, G, H, I, K, L, M, N, O, R, S, T, U, W, Y and Z */
			/* U : */ 0xe27ebdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W and Z */
			/* V : */ 0xe1164911, /* (B|M|E) - A, E, I, L, O, R, S, U and Y */
			/* W : */ 0xe11e69d9, /* (B|M|E) - A, D, E, G, H, I, L, N, O, R, S, T, U and Y */
			/* X : */ 0xe21cd597, /* (B|M|E) - A, B, C, E, H, I, K, M, O, P, S, T, U and Z */
			/* Y : */ 0xe00e79bf, /* (B|M|E) - A, B, C, D, E, F, H, I, L, M, N, O, R, S and T */
			/* Z : */ 0xe3504111, /* (B|M|E) - A, E, I, O, U, W, Y and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "F" */
			/* A : */ 0xe3fe3dfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0x40024911, /* ( |M| ) - A, E, I, L, O and R */
			/* C : */ 0x40000080, /* ( |M| ) - H,  */
			/* D : */ 0x40020001, /* ( |M| ) - A and R */
			/* E : */ 0xe33e3dff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, R, S, T, U, V, Y and Z */
			/* F : */ 0xc15efdd3, /* ( |M|E) - A, B, E, G, H, I, K, L, M, N, O, P, R, S, T, U, W and Y */
			/* G : */ 0x40020991, /* ( |M| ) - A, E, H, I, L and R */
			/* H : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* I : */ 0xe28e7c7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, R, S, T, X and Z */
			/* J : */ 0x60004000, /* (B|M| ) - O,  */
			/* K : */ 0x40024801, /* ( |M| ) - A, L, O and R */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* N : */ 0x40100111, /* ( |M| ) - A, E, I and U */
			/* O : */ 0xe0defdd5, /* (B|M|E) - A, C, E, G, H, I, K, L, M, N, O, P, R, S, T, U, W and X */
			/* P : */ 0x40020010, /* ( |M| ) - E and R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc04cd5f7, /* ( |M|E) - A, B, C, E, F, G, H, I, K, M, O, P, S, T and W */
			/* T : */ 0xc15eddf3, /* ( |M|E) - A, B, E, F, G, H, I, K, L, M, O, P, R, S, T, U, W and Y */
			/* U : */ 0x620e38cc, /* (B|M| ) - C, D, G, H, L, M, N, R, S, T and Z */
			/* V : */ 0x40000010, /* ( |M| ) - E,  */
			/* W : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0000100, /* ( |M|E) - I,  */
			/* Z : */ 0x40100110, /* ( |M| ) - E, I and U */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "G" */
			/* A : */ 0xe37eb9fe, /* (B|M|E) - B, C, D, E, F, G, H, I, L, M, N, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x41124911, /* ( |M| ) - A, E, I, L, O, R, U and Y */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0xc0024011, /* ( |M|E) - A, E, O and R */
			/* E : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* F : */ 0x40120911, /* ( |M| ) - A, E, I, L, R and U */
			/* G : */ 0xc1064911, /* ( |M|E) - A, E, I, L, O, R, S and Y */
			/* H : */ 0xe01c6913, /* (B|M|E) - A, B, E, I, L, N, O, S, T and U */
			/* I : */ 0xe23efc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40024011, /* ( |M| ) - A, E, O and R */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* N : */ 0xe0144111, /* (B|M|E) - A, E, I, O, S and U */
			/* O : */ 0xe07ef95f, /* (B|M|E) - A, B, C, D, E, G, I, L, M, N, O, P, R, S, T, U, V and W */
			/* P : */ 0x40024801, /* ( |M| ) - A, L, O and R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* S : */ 0xc27ffdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W and Z */
			/* T : */ 0xc0124091, /* ( |M|E) - A, E, H, O, R and U */
			/* U : */ 0x600e7915, /* (B|M| ) - A, C, E, I, L, M, N, O, R, S and T */
			/* V : */ 0x40000010, /* ( |M| ) - E,  */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe002b001, /* (B|M|E) - A, M, N, P and R */
			/* Z : */ 0x40100010, /* ( |M| ) - E and U */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "H" */
			/* A : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* C : */ 0x40004880, /* ( |M| ) - H, L and O */
			/* D : */ 0x40024111, /* ( |M| ) - A, E, I, O and R */
			/* E : */ 0xe3fffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* G : */ 0x40120911, /* ( |M| ) - A, E, I, L, R and U */
			/* H : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* I : */ 0xe23efc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, V and Z */
			/* J : */ 0x40000001, /* ( |M| ) - A,  */
			/* K : */ 0x40126911, /* ( |M| ) - A, E, I, L, N, O, R and U */
			/* L : */ 0xc37eddfb, /* ( |M|E) - A, B, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U, V, W, Y and Z */
			/* M : */ 0xc01c4913, /* ( |M|E) - A, B, E, I, L, O, S, T and U */
			/* N : */ 0xc05c5ff7, /* ( |M|E) - A, B, C, E, F, G, H, I, J, K, L, M, O, S, T, U and W */
			/* O : */ 0xe17efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* P : */ 0x40124831, /* ( |M| ) - A, E, F, L, O, R and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xc37efdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* S : */ 0xc25eddf7, /* ( |M|E) - A, B, C, E, F, G, H, I, K, L, M, O, P, R, S, T, U, W and Z */
			/* T : */ 0xc37efdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* U : */ 0xe00ebcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, P, R, S and T */
			/* V : */ 0x40004010, /* ( |M| ) - E and O */
			/* W : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00ed85b, /* (B|M|E) - A, B, D, E, G, L, M, O, P, R, S and T */
			/* Z : */ 0x40100111, /* ( |M| ) - A, E, I and U */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "I" */
			/* A : */ 0xe23ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V and Z */
			/* B : */ 0xe15e6917, /* (B|M|E) - A, B, C, E, I, L, N, O, R, S, T, U, W and Y */
			/* C : */ 0xe11e6d95, /* (B|M|E) - A, C, E, H, I, K, L, N, O, R, S, T, U and Y */
			/* D : */ 0xe17efdfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* E : */ 0xc27ebdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W and Z */
			/* F : */ 0xe11e49b1, /* (B|M|E) - A, E, F, H, I, L, O, R, S, T, U and Y */
			/* G : */ 0xe25efff3, /* (B|M|E) - A, B, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, W and Z */
			/* H : */ 0xe01e6911, /* (B|M|E) - A, E, I, L, N, O, R, S, T and U */
			/* I : */ 0xc0002010, /* ( |M|E) - E and N */
			/* J : */ 0xc0004011, /* ( |M|E) - A, E and O */
			/* K : */ 0xe05eddbb, /* (B|M|E) - A, B, D, E, F, H, I, K, L, M, O, P, R, S, T, U and W */
			/* L : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* M : */ 0xe25efd73, /* (B|M|E) - A, B, E, F, G, I, K, L, M, N, O, P, R, S, T, U, W and Z */
			/* N : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe2bebc5f, /* (B|M|E) - A, B, C, D, E, G, K, L, M, N, P, R, S, T, U, V, X and Z */
			/* P : */ 0xe01ed9b3, /* (B|M|E) - A, B, E, F, H, I, L, M, O, P, R, S, T and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe17efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* S : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* T : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* U : */ 0x40063800, /* ( |M| ) - L, M, N, R and S */
			/* V : */ 0xe17ecd73, /* (B|M|E) - A, B, E, F, G, I, K, L, O, P, R, S, T, U, V, W and Y */
			/* W : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* X : */ 0xc00cc111, /* ( |M|E) - A, E, I, O, P, S and T */
			/* Y : */ 0x40000011, /* ( |M| ) - A and E */
			/* Z : */ 0xe21c5593, /* (B|M|E) - A, B, E, H, I, K, M, O, S, T, U and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "J" */
			/* A : */ 0xe377bdce, /* (B|M|E) - B, C, D, G, H, I, K, L, M, N, P, Q, R, S, U, V, W, Y and Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe04e3c3d, /* (B|M|E) - A, C, D, E, F, K, L, M, N, R, S, T and W */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe0083842, /* (B|M|E) - B, G, L, M, N and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe13e2ddf, /* (B|M|E) - A, B, C, D, E, G, H, I, K, L, N, R, S, T, U, V and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x602eb94f, /* (B|M| ) - A, B, C, D, G, I, L, M, N, P, R, S, T and V */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "K" */
			/* A : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* C : */ 0x40000081, /* ( |M| ) - A and H */
			/* D : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* E : */ 0xe17efdfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* F : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* G : */ 0x40020011, /* ( |M| ) - A, E and R */
			/* H : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* I : */ 0xe30eb87f, /* (B|M|E) - A, B, C, D, E, F, G, L, M, N, P, R, S, T, Y and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* O : */ 0xe27efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Z */
			/* P : */ 0x40024900, /* ( |M| ) - I, L, O and R */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc07cddff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, S, T, U, V and W */
			/* T : */ 0xc03649f3, /* ( |M|E) - A, B, E, F, G, H, I, L, O, R, S, U and V */
			/* U : */ 0xe00eb9c6, /* (B|M|E) - B, C, G, H, I, L, M, N, P, R, S and T */
			/* V : */ 0x40004010, /* ( |M| ) - E and O */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0021800, /* (B|M|E) - L, M and R */
			/* Z : */ 0x40100111, /* ( |M| ) - A, E, I and U */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "L" */
			/* A : */ 0xe3feffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0xc17e5dfb, /* ( |M|E) - A, B, D, E, F, G, H, I, K, L, M, O, R, S, T, U, V, W and Y */
			/* C : */ 0x40104191, /* ( |M| ) - A, E, H, I, O and U */
			/* D : */ 0xc37e79ff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, R, S, T, U, V, W, Y and Z */
			/* E : */ 0xe3feffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0xc01e59b5, /* ( |M|E) - A, C, E, F, H, I, L, M, O, R, S, T and U */
			/* G : */ 0xc01e4911, /* ( |M|E) - A, E, I, L, O, R, S, T and U */
			/* H : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* I : */ 0xe3fffeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* J : */ 0x60100001, /* (B|M| ) - A and U */
			/* K : */ 0xc15e7991, /* ( |M|E) - A, E, H, I, L, M, N, O, R, S, T, U, W and Y */
			/* L : */ 0xc37ef5ff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* M : */ 0xc15c451b, /* ( |M|E) - A, B, D, E, I, K, O, S, T, U, W and Y */
			/* N : */ 0xc0104119, /* ( |M|E) - A, D, E, I, O and U */
			/* O : */ 0xe3fffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0xc01e49b1, /* ( |M|E) - A, E, F, H, I, L, O, R, S, T and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc17eddff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U, V, W and Y */
			/* T : */ 0xc37efdf7, /* ( |M|E) - A, B, C, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* U : */ 0xe2aefd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, V, X and Z */
			/* V : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* W : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe26efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, V, W and Z */
			/* Z : */ 0xc05cc9f3, /* ( |M|E) - A, B, E, F, G, H, I, L, O, P, S, T, U and W */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "M" */
			/* A : */ 0xe3feffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0xc1164911, /* ( |M|E) - A, E, I, L, O, R, S, U and Y */
			/* C : */ 0x600004c5, /* (B|M| ) - A, C, G, H and K */
			/* D : */ 0xc0160111, /* ( |M|E) - A, E, I, R, S and U */
			/* E : */ 0xe3defdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W, X, Y and Z */
			/* F : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* G : */ 0x40020011, /* ( |M| ) - A, E and R */
			/* H : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* I : */ 0xe39efcfd, /* (B|M|E) - A, C, D, E, F, G, H, K, L, M, N, O, P, R, S, T, U, X, Y and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* L : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* M : */ 0xc31e6df3, /* ( |M|E) - A, B, E, F, G, H, I, K, L, N, O, R, S, T, U, Y and Z */
			/* N : */ 0xe0104111, /* (B|M|E) - A, E, I, O and U */
			/* O : */ 0xe3fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0xc11e49b1, /* ( |M|E) - A, E, F, H, I, L, O, R, S, T, U and Y */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* S : */ 0xc05eddff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U and W */
			/* T : */ 0xc0164991, /* ( |M|E) - A, E, H, I, L, O, R, S and U */
			/* U : */ 0x620e3cfd, /* (B|M| ) - A, C, D, E, F, G, H, K, L, M, N, R, S, T and Z */
			/* V : */ 0x40004010, /* ( |M| ) - E and O */
			/* W : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00e6c14, /* (B|M|E) - C, E, K, L, N, O, R, S and T */
			/* Z : */ 0x40100111, /* ( |M| ) - A, E, I and U */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "N" */
			/* A : */ 0xe3febdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* C : */ 0xc11a4d91, /* ( |M|E) - A, E, H, I, K, L, O, R, T, U and Y */
			/* D : */ 0xc37effff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* E : */ 0xe3fffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0xc21e4911, /* ( |M|E) - A, E, I, L, O, R, S, T, U and Z */
			/* G : */ 0xc37efdfb, /* ( |M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* H : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* I : */ 0xe3bffeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, Q, R, S, T, U, V, X, Y and Z */
			/* J : */ 0x40104011, /* ( |M| ) - A, E, O and U */
			/* K : */ 0xc35ef9f3, /* ( |M|E) - A, B, E, F, G, H, I, L, M, N, O, P, R, S, T, U, W, Y and Z */
			/* L : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* N : */ 0xc37e4dff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, O, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe3fefd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0x401249b1, /* ( |M| ) - A, E, F, H, I, L, O, R and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc37effff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* T : */ 0xc37effff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* U : */ 0xe00efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S and T */
			/* V : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* W : */ 0x40124111, /* ( |M| ) - A, E, I, O, R and U */
			/* X : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Y : */ 0xe0445911, /* (B|M|E) - A, E, I, L, M, O, S and W */
			/* Z : */ 0xc37efff3, /* ( |M|E) - A, B, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "O" */
			/* A : */ 0xe09ebcce, /* (B|M|E) - B, C, D, G, H, K, L, M, N, P, R, S, T, U and X */
			/* B : */ 0xe11e4b9b, /* (B|M|E) - A, B, D, E, H, I, J, L, O, R, S, T, U and Y */
			/* C : */ 0xe11a4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, R, T, U and Y */
			/* D : */ 0xe15e59ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, O, R, S, T, U, W and Y */
			/* E : */ 0xe1be792f, /* (B|M|E) - A, B, C, D, F, I, L, M, N, O, R, S, T, U, V, X and Y */
			/* F : */ 0xe01e4931, /* (B|M|E) - A, E, F, I, L, O, R, S, T and U */
			/* G : */ 0xe11e79db, /* (B|M|E) - A, B, D, E, G, H, I, L, M, N, O, R, S, T, U and Y */
			/* H : */ 0xe11e7911, /* (B|M|E) - A, E, I, L, M, N, O, R, S, T, U and Y */
			/* I : */ 0xe00e2c4d, /* (B|M|E) - A, C, D, G, K, L, N, R, S and T */
			/* J : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* K : */ 0xe31e5d13, /* (B|M|E) - A, B, E, I, K, L, M, O, R, S, T, U, Y and Z */
			/* L : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* M : */ 0xe17efd33, /* (B|M|E) - A, B, E, F, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* N : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe22ebd78, /* (B|M|E) - D, E, F, G, I, K, L, M, N, P, R, S, T, V and Z */
			/* P : */ 0xe11eddb1, /* (B|M|E) - A, E, F, H, I, K, L, M, O, P, R, S, T, U and Y */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe37ffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* S : */ 0xe37efdf7, /* (B|M|E) - A, B, C, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* T : */ 0xe35eddf7, /* (B|M|E) - A, B, C, E, F, G, H, I, K, L, M, O, P, R, S, T, U, W, Y and Z */
			/* U : */ 0xe22ebd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, P, R, S, T, V and Z */
			/* V : */ 0xe0104111, /* (B|M|E) - A, E, I, O and U */
			/* W : */ 0xe10cfbbf, /* (B|M|E) - A, B, C, D, E, F, H, I, J, L, M, N, O, P, S, T and Y */
			/* X : */ 0xe1084111, /* (B|M|E) - A, E, I, O, T and Y */
			/* Y : */ 0xc004591d, /* ( |M|E) - A, C, D, E, I, L, M, O and S */
			/* Z : */ 0xe3004111, /* (B|M|E) - A, E, I, O, Y and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "P" */
			/* A : */ 0xe3fefddf, /* (B|M|E) - A, B, C, D, E, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0x40004000, /* ( |M| ) - O,  */
			/* C : */ 0x40004080, /* ( |M| ) - H and O */
			/* D : */ 0x40000001, /* ( |M| ) - A,  */
			/* E : */ 0xe35efd7d, /* (B|M|E) - A, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, W, Y and Z */
			/* F : */ 0xe05e7df7, /* (B|M|E) - A, B, C, E, F, G, H, I, K, L, M, N, O, R, S, T, U and W */
			/* G : */ 0x40000010, /* ( |M| ) - E,  */
			/* H : */ 0xe11e4911, /* (B|M|E) - A, E, I, L, O, R, S, T, U and Y */
			/* I : */ 0xe21ffc5d, /* (B|M|E) - A, C, D, E, G, K, L, M, N, O, P, Q, R, S, T, U and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0x60004010, /* (B|M| ) - E and O */
			/* O : */ 0xe2fefd5d, /* (B|M|E) - A, C, D, E, G, I, K, L, M, N, O, P, R, S, T, U, V, W, X and Z */
			/* P : */ 0xc11e4d95, /* ( |M|E) - A, C, E, H, I, K, L, O, R, S, T, U and Y */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xe118c195, /* (B|M|E) - A, C, E, H, I, O, P, T, U and Y */
			/* T : */ 0xe1165993, /* (B|M|E) - A, B, E, H, I, L, M, O, R, S, U and Y */
			/* U : */ 0x620ebc7e, /* (B|M| ) - B, C, D, E, F, G, K, L, M, N, P, R, S, T and Z */
			/* V : */ 0x40000010, /* ( |M| ) - E,  */
			/* W : */ 0x40000001, /* ( |M| ) - A,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00a0900, /* (B|M|E) - I, L, R and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Q" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "R" */
			/* A : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0xc11e4953, /* ( |M|E) - A, B, E, G, I, L, O, R, S, T, U and Y */
			/* C : */ 0xc11a4d91, /* ( |M|E) - A, E, H, I, K, L, O, R, T, U and Y */
			/* D : */ 0xc15efdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W and Y */
			/* E : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0xc01e6911, /* ( |M|E) - A, E, I, L, N, O, R, S, T and U */
			/* G : */ 0xc15e6db3, /* ( |M|E) - A, B, E, F, H, I, K, L, N, O, R, S, T, U, W and Y */
			/* H : */ 0xe1184111, /* (B|M|E) - A, E, I, O, T, U and Y */
			/* I : */ 0xe3fffeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* J : */ 0x40104011, /* ( |M| ) - A, E, O and U */
			/* K : */ 0xc35efdd3, /* ( |M|E) - A, B, E, G, H, I, K, L, M, N, O, P, R, S, T, U, W, Y and Z */
			/* L : */ 0xc114411b, /* ( |M|E) - A, B, D, E, I, O, S, U and Y */
			/* M : */ 0xc35cc9f7, /* ( |M|E) - A, B, C, E, F, G, H, I, L, O, P, S, T, U, W, Y and Z */
			/* N : */ 0xc37eddff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0xc01e49b1, /* ( |M|E) - A, E, F, H, I, L, O, R, S, T and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xc11c6dd1, /* ( |M|E) - A, E, G, H, I, K, L, N, O, S, T, U and Y */
			/* S : */ 0xc35efdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W, Y and Z */
			/* T : */ 0xc37effff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* U : */ 0xe20ebdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T and Z */
			/* V : */ 0xc1004111, /* ( |M|E) - A, E, I, O and Y */
			/* W : */ 0x41124191, /* ( |M| ) - A, E, H, I, O, R, U and Y */
			/* X : */ 0x40000100, /* ( |M| ) - I,  */
			/* Y : */ 0xe04cf95f, /* (B|M|E) - A, B, C, D, E, G, I, L, M, N, O, P, S, T and W */
			/* Z : */ 0xc35cddf3, /* ( |M|E) - A, B, E, F, G, H, I, K, L, M, O, P, S, T, U, W, Y and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "S" */
			/* A : */ 0xe1febdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, X and Y */
			/* B : */ 0x41124911, /* ( |M| ) - A, E, I, L, O, R, U and Y */
			/* C : */ 0xe1124991, /* (B|M|E) - A, E, H, I, L, O, R, U and Y */
			/* D : */ 0x40124111, /* ( |M| ) - A, E, I, O, R and U */
			/* E : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* G : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* H : */ 0xe15a7d37, /* (B|M|E) - A, B, C, E, F, I, K, L, M, N, O, R, T, U, W and Y */
			/* I : */ 0xe2befcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, O, P, R, S, T, U, V, X and Z */
			/* J : */ 0x40104001, /* ( |M| ) - A, O and U */
			/* K : */ 0xe1166911, /* (B|M|E) - A, E, I, L, N, O, R, S, U and Y */
			/* L : */ 0xe1104111, /* (B|M|E) - A, E, I, O, U and Y */
			/* M : */ 0xe1144111, /* (B|M|E) - A, E, I, O, S, U and Y */
			/* N : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* O : */ 0xe3fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0xe11649b1, /* (B|M|E) - A, E, F, H, I, L, O, R, S, U and Y */
			/* Q : */ 0x60100000, /* (B|M| ) - U,  */
			/* R : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* S : */ 0xc37afdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, T, U, V, W, Y and Z */
			/* T : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* U : */ 0xe22ebdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, V and Z */
			/* V : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* W : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe006b80e, /* (B|M|E) - B, C, D, L, M, N, P, R and S */
			/* Z : */ 0xe1504111, /* (B|M|E) - A, E, I, O, U, W and Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "T" */
			/* A : */ 0xe3febdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* C : */ 0x40124881, /* ( |M| ) - A, H, L, O, R and U */
			/* D : */ 0x40124111, /* ( |M| ) - A, E, I, O, R and U */
			/* E : */ 0xe3fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* G : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* H : */ 0xe15ef93f, /* (B|M|E) - A, B, C, D, E, F, I, L, M, N, O, P, R, S, T, U, W and Y */
			/* I : */ 0xe27fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W and Z */
			/* J : */ 0x60100011, /* (B|M| ) - A, E and U */
			/* K : */ 0x40126911, /* ( |M| ) - A, E, I, L, N, O, R and U */
			/* L : */ 0xc1104111, /* ( |M|E) - A, E, I, O, U and Y */
			/* M : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* N : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* O : */ 0xe3fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0x401249b1, /* ( |M| ) - A, E, F, H, I, L, O, R and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* T : */ 0xc376fdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, U, V, W, Y and Z */
			/* U : */ 0xe00ef9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, R, S and T */
			/* V : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* W : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe006b805, /* (B|M|E) - A, C, L, M, N, P, R and S */
			/* Z : */ 0xc17efdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "U" */
			/* A : */ 0xc13ebd4e, /* ( |M|E) - B, C, D, G, I, K, L, M, N, P, R, S, T, U, V and Y */
			/* B : */ 0xe07effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V and W */
			/* C : */ 0xe01a4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, R, T and U */
			/* D : */ 0xe15649d9, /* (B|M|E) - A, D, E, G, H, I, L, O, R, S, U, W and Y */
			/* E : */ 0xe21e393f, /* (B|M|E) - A, B, C, D, E, F, I, L, M, N, R, S, T, U and Z */
			/* F : */ 0xe25efdfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W and Z */
			/* G : */ 0xe21e79f3, /* (B|M|E) - A, B, E, F, G, H, I, L, M, N, O, R, S, T, U and Z */
			/* H : */ 0xe0167911, /* (B|M|E) - A, E, I, L, M, N, O, R, S and U */
			/* I : */ 0xc22ea85f, /* ( |M|E) - A, B, C, D, E, G, L, N, P, R, S, T, V and Z */
			/* J : */ 0x40100101, /* ( |M| ) - A, I and U */
			/* K : */ 0xe01a6d11, /* (B|M|E) - A, E, I, K, L, N, O, R, T and U */
			/* L : */ 0xe37efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* M : */ 0xe27efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Z */
			/* N : */ 0xe27fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W and Z */
			/* O : */ 0xc11e0808, /* ( |M|E) - D, L, R, S, T, U and Y */
			/* P : */ 0xe05ec9bd, /* (B|M|E) - A, C, D, E, F, H, I, L, O, P, R, S, T, U and W */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* S : */ 0xe37ffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* T : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* U : */ 0x40003000, /* ( |M| ) - M and N */
			/* V : */ 0x40024111, /* ( |M| ) - A, E, I, O and R */
			/* W : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* X : */ 0xc0180110, /* ( |M|E) - E, I, T and U */
			/* Y : */ 0xc0000001, /* ( |M|E) - A,  */
			/* Z : */ 0xc2584193, /* ( |M|E) - A, B, E, H, I, O, T, U, W and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "V" */
			/* A : */ 0xe01ebdcf, /* (B|M|E) - A, B, C, D, G, H, I, K, L, M, N, P, R, S, T and U */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe18e3ddd, /* (B|M|E) - A, C, D, E, G, H, I, K, L, M, N, R, S, T, X and Y */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x40000010, /* ( |M| ) - E,  */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe23efc5f, /* (B|M|E) - A, B, C, D, E, G, K, L, M, N, O, P, R, S, T, U, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40004001, /* ( |M| ) - A and O */
			/* L : */ 0x60004101, /* (B|M| ) - A, I and O */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe15e3d44, /* (B|M|E) - C, G, I, K, L, M, N, R, S, T, U, W and Y */
			/* P : */ 0x40020000, /* ( |M| ) - R,  */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* S : */ 0x40080401, /* ( |M| ) - A, K and T */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60021800, /* (B|M| ) - L, M and R */
			/* V : */ 0x41000010, /* ( |M| ) - E and Y */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0002000, /* ( |M|E) - N,  */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "W" */
			/* A : */ 0xe3bebdef, /* (B|M|E) - A, B, C, D, F, G, H, I, K, L, M, N, P, R, S, T, U, V, X, Y and Z */
			/* B : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0xc0000010, /* ( |M|E) - E,  */
			/* E : */ 0xe10e39ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, R, S, T and Y */
			/* F : */ 0x40104000, /* ( |M| ) - O and U */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* I : */ 0xe02ebc7c, /* (B|M|E) - C, D, E, F, G, K, L, M, N, P, R, S, T and V */
			/* J : */ 0x40000010, /* ( |M| ) - E,  */
			/* K : */ 0x40000100, /* ( |M| ) - I,  */
			/* L : */ 0xe1040111, /* (B|M|E) - A, E, I, S and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0xc0040910, /* ( |M|E) - E, I, L and S */
			/* O : */ 0xe0127cee, /* (B|M|E) - B, C, D, F, G, H, K, L, M, N, O, R and U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* S : */ 0xc0084490, /* ( |M|E) - E, H, K, O and T */
			/* T : */ 0x40004080, /* ( |M| ) - H and O */
			/* U : */ 0x600ea804, /* (B|M| ) - C, L, N, P, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0002010, /* (B|M|E) - E and N */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "X" */
			/* A : */ 0x600c3844, /* (B|M| ) - C, G, L, M, N, S and T */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40124991, /* ( |M| ) - A, E, H, I, L, O, R and U */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe0063c4c, /* (B|M|E) - C, D, G, K, L, M, N, R and S */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40004101, /* ( |M| ) - A, I and O */
			/* I : */ 0xc02c7c1f, /* ( |M|E) - A, B, C, D, E, K, L, M, N, O, S, T and V */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40024000, /* ( |M| ) - O and R */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x40000001, /* ( |M| ) - A,  */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x400aa000, /* ( |M| ) - N, P, R and T */
			/* P : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x40080000, /* ( |M| ) - T,  */
			/* T : */ 0xc0164111, /* ( |M|E) - A, E, I, O, R, S and U */
			/* U : */ 0x40060011, /* ( |M| ) - A, E, R and S */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40000010, /* ( |M| ) - E,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0000808, /* (B|M|E) - D and L */
			/* Z : */ 0x40000010, /* ( |M| ) - E,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Y" */
			/* A : */ 0xe24e3cce, /* (B|M|E) - B, C, D, G, H, K, L, M, N, R, S, T, W and Z */
			/* B : */ 0x40124119, /* ( |M| ) - A, D, E, I, O, R and U */
			/* C : */ 0x40004991, /* ( |M| ) - A, E, H, I, L and O */
			/* D : */ 0xc0026111, /* ( |M|E) - A, E, I, N, O and R */
			/* E : */ 0xe01e2809, /* (B|M|E) - A, D, L, N, R, S, T and U */
			/* F : */ 0x40104110, /* ( |M| ) - E, I, O and U */
			/* G : */ 0x40024111, /* ( |M| ) - A, E, I, O and R */
			/* H : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* I : */ 0xe0042810, /* (B|M|E) - E, L, N and S */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc0004810, /* ( |M|E) - E, L and O */
			/* L : */ 0xc134c955, /* ( |M|E) - A, C, E, G, I, L, O, P, S, U, V and Y */
			/* M : */ 0xc010f113, /* ( |M|E) - A, B, E, I, M, N, O, P and U */
			/* N : */ 0xc008655d, /* ( |M|E) - A, C, D, E, G, I, K, N, O and T */
			/* O : */ 0xe01eac40, /* (B|M|E) - G, K, L, N, P, R, S, T and U */
			/* P : */ 0xc01e6191, /* ( |M|E) - A, E, H, I, N, O, R, S, T and U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xc01a6111, /* ( |M|E) - A, E, I, N, O, R, T and U */
			/* S : */ 0xc00cd995, /* ( |M|E) - A, C, E, H, I, L, M, O, P, S and T */
			/* T : */ 0xc0084191, /* ( |M|E) - A, E, H, I, O and T */
			/* U : */ 0x60062400, /* (B|M| ) - K, N, R and S */
			/* V : */ 0x60004010, /* (B|M| ) - E and O */
			/* W : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* X : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x40000011, /* ( |M| ) - A and E */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Z" */
			/* A : */ 0xe01abdee, /* (B|M|E) - B, C, D, F, G, H, I, K, L, M, N, P, R, T and U */
			/* B : */ 0x40120911, /* ( |M| ) - A, E, I, L, R and U */
			/* C : */ 0x40000080, /* ( |M| ) - H,  */
			/* D : */ 0x40000110, /* ( |M| ) - E and I */
			/* E : */ 0xe11ebdbf, /* (B|M|E) - A, B, C, D, E, F, H, I, K, L, M, N, P, R, S, T, U and Y */
			/* F : */ 0x40024811, /* ( |M| ) - A, E, L, O and R */
			/* G : */ 0x40020011, /* ( |M| ) - A, E and R */
			/* H : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* I : */ 0xe13efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V and Y */
			/* J : */ 0x40000001, /* ( |M| ) - A,  */
			/* K : */ 0x40124101, /* ( |M| ) - A, I, O, R and U */
			/* L : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* M : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* N : */ 0x40000011, /* ( |M| ) - A and E */
			/* O : */ 0xe00ef974, /* (B|M|E) - C, E, F, G, I, L, M, N, O, P, R, S and T */
			/* P : */ 0x40120821, /* ( |M| ) - A, F, L, R and U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40000011, /* ( |M| ) - A and E */
			/* S : */ 0x40088115, /* ( |M| ) - A, C, E, I, P and T */
			/* T : */ 0xc0020891, /* ( |M|E) - A, E, H, L and R */
			/* U : */ 0xe26fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, V, W and Z */
			/* V : */ 0x40004010, /* ( |M| ) - E and O */
			/* W : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00c9c40, /* (B|M|E) - G, K, L, M, P, S and T */
			/* Z : */ 0xc1104911, /* ( |M|E) - A, E, I, L, O, U and Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ä" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ö" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "ü" */
			/* A : */ 0x00000000, /* ( | | ) - None are allowed */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x00000000, /* ( | | ) - None are allowed */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x00000000, /* ( | | ) - None are allowed */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		}
	} /* End of Enum 5 / 5 */
};


} // namespace NameConstants

} // namespace pwx

/** @file CRandomWordConstants.h
  *
  @verbatim
  * ---   Information about this generated file   --- *
  * ------------------------------------------------- *
  * This file has been generated by analyzing text    *
  * files for (hopefully) valid words which then got  *
  * processed.                                        *
  * First those words got split into their characters,*
  * then these character sequences got recorded.      *
  * Sequences with very little occurrences are        *
  * filtered out, as they might indicate typos.       *
  * All character occurrences are recorded as well,   *
  * and the resulting consonant and vowel strings     *
  * have then been generated by randomly mixing the   *
  * characters. Due to this mixing the program will   *
  * produce different strings every time it is called!*
  * ------------------------------------------------- *
  * Q: Why are all those constants mixed case?        *
  *    They should be all uppercase!                  *
  * A: Yes, if they where defines or in any other way *
  *    global. But here they have their own namespace *
  *    and can have readable names.                   *
  * ------------------------------------------------- *
  * Start   : 07.06.2013 19:27:16                     *
  * Stop    : 07.06.2013 19:33:43                     *
  * Duration:    0 h,  6 m, 27 s                      *
  * ------------------------------------------------- *
  * textAnalyzer (c) Sven Eden, PrydeWorX 2010 - 2013 *
  @endverbatim
**/

#endif // PWX_LIBPWX_PWX_INTERNAL_CRANDOMWORDCONSTANTS_H_INCLUDED

