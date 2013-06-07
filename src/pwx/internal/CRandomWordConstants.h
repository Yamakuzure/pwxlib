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
  * -- Number of text files that have been analyzed:            33           -- *
  * -- Number of words in all of these files       :        909814           -- *
  * --                  names / german             :         13254           -- *
  * --                  names / english            :          9658           -- *
  * --                  texts / german             :        141757           -- *
  * --                  texts / english            :        746181           -- *
  * -- Number of unique words found                :        237200           -- *
  * --                  names / german             :         13253           -- *
  * --                  names / english            :          8043           -- *
  * --                  texts / german             :        139268           -- *
  * --                  texts / english            :         73720           -- *
  * -- Number of names from name list files        :         21330           -- *
  * -- Number of ignored words (illegal content)   :          1208           -- *
  * -- Resulting number of analyzed words          :        235992           -- *
  * -- ( ==>         11 E-Mail addresses have been filtered out)             -- *
  * -- ( ==>          7 Internet addresses have been filtered out)           -- *
  * -- ( ==>          5 File paths have been filtered out)                   -- *
  * -- ( ==>          9 Triple Threats have been filtered out)               -- *
  * -- ( ==>          9 Roman numerals have been filtered out)               -- *
  * -- ( ==>       1175 times Captain Caps stroke back)                      -- *
  * -- ( ==>          1 Weird letter combinations have been filtered out)    -- *
  * -- ( ==>          3 CamelCase words have been split)                     -- *
  * --------------------------------------------------------------------------- *
  * Top Hundred statistics - The most and fewest words that got accepted or denied: *
  * Hundred most often found accepted words  | Hundred most often found denied words    *
  * -------------------------------------+------------------------------------- *
  * ----                        Language de                                ---- *
  * -------------------------------------+------------------------------------- *
  *   1.:                und (       94) |   1.:               gehn (        3) *
  *   2.:                 er (       72) |   2.:                pst (        2) *
  *   3.:                der (       61) |   3.:                 aß (        2) *
  *   4.:                die (       55) |   4.:              kömmt (        2) *
  *   5.:                 da (       37) |   5.:                 ge (        2) *
  *   6.:                ein (       30) |   6.:               läßt (        2) *
  *   7.:              nicht (       30) |   7.:              hopßa (        2) *
  *   8.:                das (       29) |   8.:                 hu (        2) *
  *   9.:                 es (       27) |   9.:                 ho (        2) *
  *  10.:                wie (       26) |  10.:              tlein (        2) *
  *  11.:                den (       25) |  11.:                 su (        1) *
  *  12.:                ist (       25) |  12.:                oya (        1) *
  *  13.:                von (       25) |  13.:                 le (        1) *
  *  14.:                ich (       23) |  14.:                 ok (        1) *
  *  15.:                 in (       23) |  15.:                liv (        1) *
  *  16.:                hat (       22) |  16.:                 jo (        1) *
  *  17.:               doch (       21) |  17.:                yui (        1) *
  *  18.:                 du (       19) |  18.:                 ib (        1) *
  *  19.:                 im (       19) |  19.:                yao (        1) *
  *  20.:                 zu (       19) |  20.:                 re (        1) *
  *  21.:               sich (       18) |  21.:                 ly (        1) *
  *  22.:             weiter (       18) |  22.:                lrs (        1) *
  *  23.:                 an (       17) |  23.:                 ed (        1) *
  *  24.:               herr (       17) |  24.:                 uz (        1) *
  *  25.:               übel (       17) |  25.:                 el (        1) *
  *  26.:               dran (       16) |  26.:                 lu (        1) *
  *  27.:                gar (       15) |  27.:                 sp (        1) *
  *  28.:              getan (       15) |  28.:                 ah (        1) *
  *  29.:              tutti (       15) |  29.:               fuss (        1) *
  *  30.:              urian (       15) |  30.:                 nu (        1) *
  *  31.:                auf (       14) |  31.:              eimal (        1) *
  *  32.:                aus (       14) |  32.:             menßen (        1) *
  *  33.:                sie (       14) |  33.:                 iv (        1) *
  *  34.:                 so (       14) |  34.:           hopßassa (        1) *
  *  35.:            verzähl (       14) |  35.:               fluß (        1) *
  *  36.:                des (       13) |  36.:                 co (        1) *
  *  37.:               sein (       13) |  37.:               itzt (        1) *
  *  38.:               wenn (       13) |  38.:                 hm (        1) *
  *  39.:                als (       11) |  39.:                 al (        1) *
  *  40.:                 am (       11) |  40.:            tönigin (        1) *
  *  41.:                dem (       11) |  41.:              hoppß (        1) *
  *  42.:         eingesandt (       10) |  42.:                 ak (        1) *
  *  43.:        verfügbaren (       10) |  43.:               tuhl (        1) *
  *  44.:                der (       10) |  44.:               asia (        1) *
  *  45.:        verzeichnis (       10) |  45.:                 ip (        1) *
  *  46.:               list (       10) |  46.:                 af (        1) *
  *  47.:           gedichte (       10) |  47.:                 eu (        1) *
  *  48.:                ihn (        9) |  48.:               zvei (        1) *
  *  49.:                mit (        9) |  49.:                sch (        1) *
  *  50.:                nur (        9) |  50.:                 it (        1) *
  *  51.:                uns (        9) |  51.:              tuhle (        1) *
  *  52.:                war (        9) |  52.:                iii (        1) *
  *  53.:                 ja (        8) |  53.:            kikriki (        1) *
  *  54.:               nach (        8) |  54.:            otaheit (        1) *
  *  55.:               noch (        8) |  55.:                 iq (        1) *
  *  56.:               dann (        7) |  56.:                 ai (        1) *
  *  57.:                dir (        7) |  57.:                 uv (        1) *
  *  58.:               dort (        7) |  58.:                 qi (        1) *
  *  59.:              durch (        7) |  59.:               tnix (        1) *
  *  60.:               eine (        7) |  60.:                 hi (        1) *
  *  61.:               hier (        7) |  61.:                 bö (        1) *
  *  62.:              immer (        7) |  62.:                 eg (        1) *
  *  63.:               sind (        7) |  63.:              ßöner (        1) *
  *  64.:                 um (        7) |  64.:                 os (        1) *
  *  65.:                vom (        7) |  65.:            tönidin (        1) *
  *  66.:                was (        7) |  66.:                 fo (        1) *
  *  67.:                wir (        7) |  67.:         geschloßne (        1) *
  *  68.:               denn (        6) |  68.:                 ma (        1) *
  *  69.:               kann (        6) |  69.:           dämmrung (        1) *
  *  70.:                man (        6) |  70.:                 et (        1) *
  *  71.:               moor (        6) |  71.:             heisre (        1) *
  *  72.:              sieht (        6) |  72.:       vitzlibutzki (        1) *
  *  73.:               weiß (        6) |  73.:                 re (        1) *
  *  74.:             allein (        5) |  74.:          wirktlich (        1) *
  *  75.:            annette (        5) |  75.:            haterns (        1) *
  *  76.:                bei (        5) |  76.:                 un (        1) *
  *  77.:                bin (        5) |  77.:            mählich (        1) *
  *  78.:              einen (        5) |  78.:                 op (        1) *
  *  79.:                ihm (        5) |  79.:                 ur (        1) *
  *  80.:                ihr (        5) |  80.:            tleiner (        1) *
  *  81.:              knabe (        5) |  81.:                div (        1) *
  *  82.:             kommen (        5) |  82.:       vitzlibutzli (        1) *
  *  83.:             meinen (        5) |  83.:                 ii (        1) *
  *  84.:               mich (        5) |  84.:      tlinglingling (        1) *
  *  85.:                mir (        5) |  85.:             tnicks (        1) *
  *  86.:                nun (        5) |  86.:                 ad (        1) *
  *  87.:              seele (        5) |  87.:       eingegrabnen (        1) *
  *  88.:                vor (        5) |  88.:                 us (        1) *
  *  89.:               über (        5) |  89.:                 at (        1) *
  *  90.:            deutsch (        5) |  90.:                 te (        1) *
  *  91.:               auch (        4) |  91.:                 he (        1) *
  *  92.:               blau (        4) |  92.:                 ex (        1) *
  *  93.:               blut (        4) |  93.:                 eo (        1) *
  *  94.:           claudius (        4) |  94.:                 ig (        1) *
  *  95.:               dein (        4) |  95.:                 ar (        1) *
  *  96.:              detta (        4) |  96.:          flüchtger (        1) *
  *  97.:               fand (        4) |  97.:            vapeurs (        1) *
  *  98.:         fitzebutze (        4) |  98.:             schloß (        1) *
  *  99.:            fleisch (        4) |  99.:                    (        0) *
  *  100.:               flut (        4) |  100.:                    (        0) *
  * ----                        Language en                                ---- *
  * -------------------------------------+------------------------------------- *
  *   1.:                the (    49495) |   1.:             werper (      315) *
  *   2.:                and (    25932) |   2.:              teeka (      217) *
  *   3.:                 of (    24681) |   3.:              shang (      194) *
  *   4.:                 to (    17710) |   4.:             jeddak (      172) *
  *   5.:                 in (    11502) |   5.:             mbonga (      131) *
  *   6.:                 he (    11107) |   6.:                 la (      123) *
  *   7.:                his (     9723) |   7.:              kulan (      118) *
  *   8.:                was (     9592) |   8.:               dihn (      106) *
  *   9.:               that (     9020) |   9.:                 oh (      103) *
  *  10.:                had (     6929) |  10.:             sheeta (       84) *
  *  11.:                 it (     6853) |  11.:              sabor (       80) *
  *  12.:               with (     5881) |  12.:                 ha (       79) *
  *  13.:                 is (     5831) |  13.:                 ah (       74) *
  *  14.:                for (     5670) |  14.:            jeddaks (       64) *
  *  15.:                 as (     5475) |  15.:             ptarth (       64) *
  *  16.:                but (     4418) |  16.:             waziri (       56) *
  *  17.:                 at (     4345) |  17.:               goro (       49) *
  *  18.:                him (     4131) |  18.:               toog (       48) *
  *  19.:                not (     3987) |  19.:              etext (       45) *
  *  20.:               they (     3686) |  20.:             terkoz (       45) *
  *  21.:                her (     3657) |  21.:               kega (       44) *
  *  22.:               from (     3400) |  22.:               talu (       40) *
  *  23.:              which (     3399) |  23.:              chulk (       38) *
  *  24.:               upon (     3372) |  24.:            kulonga (       38) *
  *  25.:               were (     3358) |  25.:             etexts (       37) *
  *  26.:                 by (     3355) |  26.:             taglat (       36) *
  *  27.:                 be (     3028) |  27.:                 ye (       34) *
  *  28.:                 on (     2874) |  28.:           hekkador (       30) *
  *  29.:               this (     2857) |  29.:              gunto (       29) *
  *  30.:               have (     2791) |  30.:              horta (       28) *
  *  31.:              their (     2693) |  31.:                 dr (       26) *
  *  32.:                you (     2672) |  32.:                 de (       26) *
  *  33.:                she (     2563) |  33.:               boma (       26) *
  *  34.:                 so (     2543) |  34.:               manu (       25) *
  *  35.:                 my (     2502) |  35.:                 cd (       25) *
  *  36.:                one (     2486) |  36.:           frecoult (       22) *
  *  37.:                all (     2478) |  37.:                 eh (       22) *
  *  38.:               them (     2474) |  38.:              thoat (       20) *
  *  39.:              there (     2373) |  39.:              korus (       18) *
  *  40.:                who (     2236) |  40.:            fuwalda (       17) *
  *  41.:              would (     2051) |  41.:             thoats (       12) *
  *  42.:                 an (     2010) |  42.:             kantos (       12) *
  *  43.:               when (     2001) |  43.:               kudu (       10) *
  *  44.:             tarzan (     1979) |  44.:            mcimail (       10) *
  *  45.:                 or (     1937) |  45.:                 ay (        9) *
  *  46.:                 no (     1918) |  46.:                 re (        9) *
  *  47.:                 me (     1895) |  47.:          gaberelle (        8) *
  *  48.:               been (     1876) |  48.:              sojat (        8) *
  *  49.:               king (     1744) |  49.:               mget (        7) *
  *  50.:              great (     1722) |  50.:               cadj (        7) *
  *  51.:               into (     1701) |  51.:              kopje (        7) *
  *  52.:                 we (     1687) |  52.:              ibeto (        6) *
  *  53.:              could (     1659) |  53.:                 ho (        6) *
  *  54.:               said (     1631) |  54.:              bwana (        6) *
  *  55.:               then (     1609) |  55.:                 ax (        6) *
  *  56.:             before (     1443) |  56.:              mamka (        5) *
  *  57.:                out (     1429) |  57.:                 ma (        5) *
  *  58.:                man (     1423) |  58.:                 sa (        5) *
  *  59.:               very (     1405) |  59.:             illall (        4) *
  *  60.:                 up (     1339) |  60.:                 ap (        4) *
  *  61.:                are (     1332) |  61.:             hastor (        4) *
  *  62.:                now (     1300) |  62.:              anjou (        4) *
  *  63.:               more (     1255) |  63.:              gozan (        3) *
  *  64.:               what (     1244) |  64.:                 ex (        3) *
  *  65.:             little (     1211) |  65.:            attmail (        3) *
  *  66.:                 if (     1203) |  66.:              blest (        3) *
  *  67.:               some (     1182) |  67.:                 jo (        2) *
  *  68.:               time (     1155) |  68.:                 bo (        2) *
  *  69.:                its (     1142) |  69.:                 ty (        2) *
  *  70.:                did (     1107) |  70.:                 em (        2) *
  *  71.:                 do (     1093) |  71.:              kamma (        2) *
  *  72.:               came (     1090) |  72.:                 yo (        2) *
  *  73.:                two (     1064) |  73.:               xats (        2) *
  *  74.:               than (     1058) |  74.:           desquerc (        2) *
  *  75.:              other (     1048) |  75.:               dwar (        2) *
  *  76.:                men (     1046) |  76.:             senlac (        2) *
  *  77.:              where (     1042) |  77.:              neeta (        2) *
  *  78.:               made (     1019) |  78.:             diggin (        2) *
  *  79.:              about (      999) |  79.:                 un (        2) *
  *  80.:               down (      989) |  80.:          arternoon (        2) *
  *  81.:            through (      981) |  81.:                 al (        2) *
  *  82.:              again (      979) |  82.:                 en (        2) *
  *  83.:               will (      955) |  83.:                 le (        2) *
  *  84.:              these (      918) |  84.:             vastus (        2) *
  *  85.:               back (      914) |  85.:              omean (        2) *
  *  86.:                any (      869) |  86.:                 di (        1) *
  *  87.:              might (      859) |  87.:                 es (        1) *
  *  88.:              after (      852) |  88.:                viv (        1) *
  *  89.:                way (      852) |  89.:                 os (        1) *
  *  90.:              first (      846) |  90.:                 la (        1) *
  *  91.:            himself (      834) |  91.:                 hy (        1) *
  *  92.:             should (      820) |  92.:                 au (        1) *
  *  93.:                old (      817) |  93.:                 vi (        1) *
  *  94.:               such (      817) |  94.:                 ra (        1) *
  *  95.:                saw (      812) |  95.:                 co (        1) *
  *  96.:                day (      791) |  96.:                 em (        1) *
  *  97.:                own (      786) |  97.:                 ed (        1) *
  *  98.:               only (      782) |  98.:                 el (        1) *
  *  99.:             people (      777) |  99.:                 lu (        1) *
  *  100.:               even (      757) |  100.:          opalesque (        1) *
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
  *  18.:             adelia (        1) |  18.:                 ah (        1) *
  *  19.:            adelina (        1) |  19.:               fuss (        1) *
  *  20.:           adelinde (        1) |  20.:                 nu (        1) *
  *  21.:            adeline (        1) |  21.:              eimal (        1) *
  *  22.:            adelmar (        1) |  22.:             menßen (        1) *
  *  23.:          adeltraud (        1) |  23.:                 iv (        1) *
  *  24.:                adi (        1) |  24.:           hopßassa (        1) *
  *  25.:              adolf (        1) |  25.:               fluß (        1) *
  *  26.:             adrian (        1) |  26.:                 co (        1) *
  *  27.:            adriana (        1) |  27.:               itzt (        1) *
  *  28.:            adriane (        1) |  28.:                 hm (        1) *
  *  29.:           adrianna (        1) |  29.:                 al (        1) *
  *  30.:           adrianne (        1) |  30.:            tönigin (        1) *
  *  31.:            adriano (        1) |  31.:              hoppß (        1) *
  *  32.:           adrianus (        1) |  32.:                 ak (        1) *
  *  33.:            aganeta (        1) |  33.:               tuhl (        1) *
  *  34.:              agata (        1) |  34.:               asia (        1) *
  *  35.:              agate (        1) |  35.:                 ip (        1) *
  *  36.:             agatha (        1) |  36.:                 af (        1) *
  *  37.:             agathe (        1) |  37.:                 eu (        1) *
  *  38.:              agnes (        1) |  38.:               zvei (        1) *
  *  39.:            agnetha (        1) |  39.:                sch (        1) *
  *  40.:              agnis (        1) |  40.:                 it (        1) *
  *  41.:            agustin (        1) |  41.:              tuhle (        1) *
  *  42.:           agustina (        1) |  42.:                iii (        1) *
  *  43.:             aharon (        1) |  43.:            kikriki (        1) *
  *  44.:            ahlbert (        1) |  44.:            otaheit (        1) *
  *  45.:            ahlrich (        1) |  45.:                 iq (        1) *
  *  46.:               aida (        1) |  46.:                 ai (        1) *
  *  47.:               aila (        1) |  47.:                 uv (        1) *
  *  48.:              alain (        1) |  48.:                 qi (        1) *
  *  49.:               alan (        1) |  49.:               tnix (        1) *
  *  50.:               alba (        1) |  50.:                 hi (        1) *
  *  51.:             albert (        1) |  51.:                 bö (        1) *
  *  52.:            alberta (        1) |  52.:                 eg (        1) *
  *  53.:          albertina (        1) |  53.:              ßöner (        1) *
  *  54.:          albertine (        1) |  54.:                 os (        1) *
  *  55.:           albrecht (        1) |  55.:            tönidin (        1) *
  *  56.:             aldina (        1) |  56.:                 fo (        1) *
  *  57.:             aldine (        1) |  57.:         geschloßne (        1) *
  *  58.:               alea (        1) |  58.:                 ma (        1) *
  *  59.:         aleksandar (        1) |  59.:           dämmrung (        1) *
  *  60.:         aleksander (        1) |  60.:                 et (        1) *
  *  61.:         aleksandra (        1) |  61.:             heisre (        1) *
  *  62.:              alena (        1) |  62.:       vitzlibutzki (        1) *
  *  63.:             alenka (        1) |  63.:                 re (        1) *
  *  64.:         alessandra (        1) |  64.:          wirktlich (        1) *
  *  65.:         alessandro (        1) |  65.:            haterns (        1) *
  *  66.:               alex (        1) |  66.:                 un (        1) *
  *  67.:              alexa (        1) |  67.:            mählich (        1) *
  *  68.:          alexander (        1) |  68.:                 op (        1) *
  *  69.:          alexandra (        1) |  69.:                 ur (        1) *
  *  70.:                alf (        1) |  70.:            tleiner (        1) *
  *  71.:             alfons (        1) |  71.:                div (        1) *
  *  72.:             alfred (        1) |  72.:       vitzlibutzli (        1) *
  *  73.:             alfrid (        1) |  73.:                 ii (        1) *
  *  74.:            alfried (        1) |  74.:      tlinglingling (        1) *
  *  75.:              alida (        1) |  75.:             tnicks (        1) *
  *  76.:               alma (        1) |  76.:                 ad (        1) *
  *  77.:             almida (        1) |  77.:       eingegrabnen (        1) *
  *  78.:              almin (        1) |  78.:                 us (        1) *
  *  79.:              alois (        1) |  79.:                 at (        1) *
  *  80.:           altfried (        1) |  80.:                 te (        1) *
  *  81.:            amadeus (        1) |  81.:                 he (        1) *
  *  82.:             amanda (        1) |  82.:                 ex (        1) *
  *  83.:              amber (        1) |  83.:                 eo (        1) *
  *  84.:              ameli (        1) |  84.:                 ig (        1) *
  *  85.:             amelia (        1) |  85.:                 ar (        1) *
  *  86.:               amin (        1) |  86.:          flüchtger (        1) *
  *  87.:              amina (        1) |  87.:            vapeurs (        1) *
  *  88.:               amir (        1) |  88.:             schloß (        1) *
  *  89.:                ana (        1) |  89.:                pst (        2) *
  *  90.:             anabel (        1) |  90.:                 aß (        2) *
  *  91.:          anastasia (        1) |  91.:              kömmt (        2) *
  *  92.:               anca (        1) |  92.:                 ge (        2) *
  *  93.:               andi (        1) |  93.:               läßt (        2) *
  *  94.:              andre (        1) |  94.:              hopßa (        2) *
  *  95.:             andrea (        1) |  95.:                 hu (        2) *
  *  96.:            andreas (        1) |  96.:                 ho (        2) *
  *  97.:           aneliese (        1) |  97.:              tlein (        2) *
  *  98.:           anellise (        1) |  98.:               gehn (        3) *
  *  99.:           anemarie (        1) |  99.:                    ( 999999999) *
  *  100.:             anette (        1) |  100.:                    ( 999999999) *
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
  *  11.:            abigale (        1) |  11.:                 em (        1) *
  *  12.:           abigayle (        1) |  12.:                 ed (        1) *
  *  13.:            abiquiu (        1) |  13.:                 el (        1) *
  *  14.:              abner (        1) |  14.:                 lu (        1) *
  *  15.:              abram (        1) |  15.:          opalesque (        1) *
  *  16.:              abril (        1) |  16.:                 af (        1) *
  *  17.:            acequia (        1) |  17.:           kreegahs (        1) *
  *  18.:               acer (        1) |  18.:              wappi (        1) *
  *  19.:           achilles (        1) |  19.:       gorilephants (        1) *
  *  20.:           ackerley (        1) |  20.:               duro (        1) *
  *  21.:             ackley (        1) |  21.:               advo (        1) *
  *  22.:                ada (        1) |  22.:          perfesser (        1) *
  *  23.:         adalbeorht (        1) |  23.:                ooo (        1) *
  *  24.:        adalbrechta (        1) |  24.:                 aa (        1) *
  *  25.:            adaline (        1) |  25.:                 je (        1) *
  *  26.:            adalson (        1) |  26.:           janviers (        1) *
  *  27.:            adamson (        1) |  27.:                 fi (        1) *
  *  28.:               adan (        1) |  28.:               fcrn (        1) *
  *  29.:               adda (        1) |  29.:         hartebeest (        1) *
  *  30.:           addaneye (        1) |  30.:                 av (        1) *
  *  31.:              addie (        1) |  31.:                 oi (        1) *
  *  32.:              addis (        1) |  32.:                div (        1) *
  *  33.:             addney (        1) |  33.:             arsoof (        1) *
  *  34.:               addy (        1) |  34.:                viv (        1) *
  *  35.:            addyson (        1) |  35.:               topi (        1) *
  *  36.:              adela (        1) |  36.:            aruwimi (        1) *
  *  37.:           adelaide (        1) |  37.:                liv (        1) *
  *  38.:              adele (        1) |  38.:                 fa (        1) *
  *  39.:             adelia (        1) |  39.:                iii (        1) *
  *  40.:            adelina (        1) |  40.:                 bi (        1) *
  *  41.:               aden (        1) |  41.:                 rd (        1) *
  *  42.:               adia (        1) |  42.:                 mo (        1) *
  *  43.:               adie (        1) |  43.:                 gi (        1) *
  *  44.:             aditya (        1) |  44.:             mweeza (        1) *
  *  45.:              adken (        1) |  45.:                 wy (        1) *
  *  46.:             adkins (        1) |  46.:           diferent (        1) *
  *  47.:              adkyn (        1) |  47.:              manus (        1) *
  *  48.:              adler (        1) |  48.:           fusilade (        1) *
  *  49.:              adley (        1) |  49.:                 et (        1) *
  *  50.:              adney (        1) |  50.:            clrcles (        1) *
  *  51.:               adny (        1) |  51.:                pct (        1) *
  *  52.:             adolfo (        1) |  52.:                 ju (        1) *
  *  53.:             adonis (        1) |  53.:                 oc (        1) *
  *  54.:           adrianna (        1) |  54.:                 tu (        1) *
  *  55.:             adriel (        1) |  55.:                 co (        1) *
  *  56.:             adrion (        1) |  56.:                 ee (        1) *
  *  57.:              adron (        1) |  57.:                 au (        1) *
  *  58.:                ady (        1) |  58.:                 po (        1) *
  *  59.:         aeccestane (        1) |  59.:                 jo (        2) *
  *  60.:              aedan (        1) |  60.:                 bo (        2) *
  *  61.:         aegelmaere (        1) |  61.:                 ty (        2) *
  *  62.:         aegelweard (        1) |  62.:                 em (        2) *
  *  63.:              aeker (        1) |  63.:              kamma (        2) *
  *  64.:           aekerley (        1) |  64.:                 yo (        2) *
  *  65.:           aekerman (        1) |  65.:               xats (        2) *
  *  66.:             aekley (        1) |  66.:           desquerc (        2) *
  *  67.:             aeldra (        1) |  67.:               dwar (        2) *
  *  68.:           aelfdane (        1) |  68.:             senlac (        2) *
  *  69.:           aelfdene (        1) |  69.:              neeta (        2) *
  *  70.:           aelfraed (        1) |  70.:             diggin (        2) *
  *  71.:            aelfric (        1) |  71.:                 un (        2) *
  *  72.:           aelfwine (        1) |  72.:          arternoon (        2) *
  *  73.:          aenedleah (        1) |  73.:                 al (        2) *
  *  74.:          aenescumb (        1) |  74.:                 en (        2) *
  *  75.:            aerwyna (        1) |  75.:                 le (        2) *
  *  76.:             aescby (        1) |  76.:             vastus (        2) *
  *  77.:           aescford (        1) |  77.:              omean (        2) *
  *  78.:           aescleah (        1) |  78.:              gozan (        3) *
  *  79.:            aesclin (        1) |  79.:                 ex (        3) *
  *  80.:            aesctun (        1) |  80.:            attmail (        3) *
  *  81.:           aescwine (        1) |  81.:              blest (        3) *
  *  82.:            aescwyn (        1) |  82.:             illall (        4) *
  *  83.:          aesoburne (        1) |  83.:                 ap (        4) *
  *  84.:       aethelbeorht (        1) |  84.:             hastor (        4) *
  *  85.:        aethelbeorn (        1) |  85.:              anjou (        4) *
  *  86.:        aethelberht (        1) |  86.:              mamka (        5) *
  *  87.:         aethelhard (        1) |  87.:                 ma (        5) *
  *  88.:        aethelisdun (        1) |  88.:                 sa (        5) *
  *  89.:         aethelmaer (        1) |  89.:              ibeto (        6) *
  *  90.:        aethelmaere (        1) |  90.:                 ho (        6) *
  *  91.:         aethelreda (        1) |  91.:              bwana (        6) *
  *  92.:         aethelstun (        1) |  92.:                 ax (        6) *
  *  93.:        aethelweard (        1) |  93.:               mget (        7) *
  *  94.:         aethelwine (        1) |  94.:               cadj (        7) *
  *  95.:         aethelwyne (        1) |  95.:              kopje (        7) *
  *  96.:          aetheston (        1) |  96.:          gaberelle (        8) *
  *  97.:          aethretun (        1) |  97.:              sojat (        8) *
  *  98.:             afreda (        1) |  98.:                 ay (        9) *
  *  99.:              aftan (        1) |  99.:                 re (        9) *
  *  100.:              afton (        1) |  100.:               kudu (       10) *
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
  * A :        10292 |      177 | 29.58% | 0.29584 | 12.71%
  * B :         1482 |       25 |  3.21% | 0.03207 |  1.83%
  * C :         1599 |       27 |  3.46% | 0.03460 |  1.97%
  * D :         3137 |       54 |  6.79% | 0.06788 |  3.87%
  * E :         8795 |      151 | 25.28% | 0.25281 | 10.86%
  * F :         1269 |       22 |  2.75% | 0.02746 |  1.57%
  * G :         1630 |       28 |  3.53% | 0.03527 |  2.01%
  * H :         2844 |       49 |  6.15% | 0.06154 |  3.51%
  * I :         8193 |      141 | 23.55% | 0.23551 | 10.11%
  * J :          986 |       16 |  2.13% | 0.02133 |  1.22%
  * K :         1914 |       33 |  4.14% | 0.04141 |  2.36%
  * L :         4584 |       79 |  9.92% | 0.09918 |  5.66%
  * M :         3184 |       54 |  6.89% | 0.06889 |  3.93%
  * N :         5980 |      103 | 12.94% | 0.12939 |  7.38%
  * O :         3869 |       66 | 11.12% | 0.11121 |  4.78%
  * P :          674 |       11 |  1.46% | 0.01458 |  0.83%
  * Q :           64 |        1 |  0.14% | 0.00138 |  0.08%
  * R :         6242 |      107 | 13.51% | 0.13506 |  7.71%
  * S :         4306 |       74 |  9.32% | 0.09317 |  5.32%
  * T :         3816 |       66 |  8.26% | 0.08257 |  4.71%
  * U :         2443 |       42 |  7.02% | 0.07022 |  3.02%
  * V :          875 |       15 |  1.89% | 0.01893 |  1.08%
  * W :          615 |       10 |  1.33% | 0.01331 |  0.76%
  * X :           51 |        1 |  0.11% | 0.00110 |  0.06%
  * Y :         1194 |       20 |  3.43% | 0.03432 |  1.47%
  * Z :          965 |       16 |  2.09% | 0.02088 |  1.19%
  * ä :            2 |        1 |  0.01% | 0.00006 |  0.00%
  * ö :            1 |        1 |  0.00% | 0.00003 |  0.00%
  * ü :            0 |        0 |  0.00% | 0.00000 |  0.00%
  * -----------------------------------------------------
  * Source names, Language en, Type NST_NAMES_EN
  *       char found | recalced |  quote |   rate  |  all
  * A :         6153 |        6 | 28.66% | 0.28660 | 11.99%
  * B :         1120 |       17 |  3.75% | 0.03753 |  2.18%
  * C :         1539 |       24 |  5.16% | 0.05157 |  3.00%
  * D :         2196 |       34 |  7.36% | 0.07359 |  4.28%
  * E :         6312 |        6 | 29.40% | 0.29401 | 12.30%
  * F :          601 |        9 |  2.01% | 0.02014 |  1.17%
  * G :          844 |       13 |  2.83% | 0.02828 |  1.64%
  * H :         1637 |       25 |  5.49% | 0.05485 |  3.19%
  * I :         3572 |        3 | 16.64% | 0.16638 |  6.96%
  * J :          491 |        8 |  1.65% | 0.01645 |  0.96%
  * K :          904 |       14 |  3.03% | 0.03029 |  1.76%
  * L :         3981 |       61 | 13.34% | 0.13340 |  7.76%
  * M :         1280 |       20 |  4.29% | 0.04289 |  2.49%
  * N :         4437 |       68 | 14.87% | 0.14868 |  8.65%
  * O :         2648 |        3 | 12.33% | 0.12334 |  5.16%
  * P :          413 |        6 |  1.38% | 0.01384 |  0.80%
  * Q :           65 |        1 |  0.22% | 0.00218 |  0.13%
  * R :         4190 |       64 | 14.04% | 0.14040 |  8.17%
  * S :         2103 |       32 |  7.05% | 0.07047 |  4.10%
  * T :         2414 |       37 |  8.09% | 0.08089 |  4.70%
  * U :         1033 |        1 |  4.81% | 0.04812 |  2.01%
  * V :          498 |        8 |  1.67% | 0.01669 |  0.97%
  * W :          856 |       13 |  2.87% | 0.02868 |  1.67%
  * X :          114 |        2 |  0.38% | 0.00382 |  0.22%
  * Y :         1751 |        2 |  8.16% | 0.08156 |  3.41%
  * Z :          160 |        2 |  0.54% | 0.00536 |  0.31%
  * -----------------------------------------------------
  * Source names, Language all, Type NST_NAMES_ALL
  *       char found | recalced |  quote |   rate  |  all
  * A :        16445 |        6 | 29.23% | 0.29233 | 12.43%
  * B :         2602 |       20 |  3.42% | 0.03421 |  1.97%
  * C :         3138 |       24 |  4.13% | 0.04126 |  2.37%
  * D :         5333 |       41 |  7.01% | 0.07012 |  4.03%
  * E :        15107 |        5 | 26.85% | 0.26855 | 11.42%
  * F :         1870 |       14 |  2.46% | 0.02459 |  1.41%
  * G :         2474 |       19 |  3.25% | 0.03253 |  1.87%
  * H :         4481 |       35 |  5.89% | 0.05891 |  3.39%
  * I :        11765 |        4 | 20.91% | 0.20914 |  8.89%
  * J :         1477 |       11 |  1.94% | 0.01942 |  1.12%
  * K :         2818 |       22 |  3.70% | 0.03705 |  2.13%
  * L :         8565 |       66 | 11.26% | 0.11261 |  6.47%
  * M :         4464 |       35 |  5.87% | 0.05869 |  3.37%
  * N :        10417 |       81 | 13.70% | 0.13696 |  7.87%
  * O :         6517 |        2 | 11.58% | 0.11585 |  4.93%
  * P :         1087 |        8 |  1.43% | 0.01429 |  0.82%
  * Q :          129 |        1 |  0.17% | 0.00170 |  0.10%
  * R :        10432 |       81 | 13.72% | 0.13715 |  7.88%
  * S :         6409 |       50 |  8.43% | 0.08426 |  4.84%
  * T :         6230 |       48 |  8.19% | 0.08191 |  4.71%
  * U :         3476 |        1 |  6.18% | 0.06179 |  2.63%
  * V :         1373 |       11 |  1.81% | 0.01805 |  1.04%
  * W :         1471 |       11 |  1.93% | 0.01934 |  1.11%
  * X :          165 |        1 |  0.22% | 0.00217 |  0.12%
  * Y :         2945 |        1 |  5.24% | 0.05235 |  2.23%
  * Z :         1125 |        9 |  1.48% | 0.01479 |  0.85%
  * -----------------------------------------------------
  * Source texts, Language de, Type NST_TEXTS_DE
  *       char found | recalced |  quote |   rate  |  all
  * A :       111094 |       21 | 17.58% | 0.17583 |  6.50%
  * B :        38958 |       28 |  3.62% | 0.03618 |  2.28%
  * C :        51133 |       37 |  4.75% | 0.04749 |  2.99%
  * D :        39391 |       29 |  3.66% | 0.03658 |  2.31%
  * E :       232467 |       43 | 36.79% | 0.36792 | 13.61%
  * F :        36069 |       27 |  3.35% | 0.03350 |  2.11%
  * G :        63789 |       47 |  5.92% | 0.05924 |  3.73%
  * H :        80922 |       59 |  7.52% | 0.07515 |  4.74%
  * I :       125699 |       23 | 19.89% | 0.19894 |  7.36%
  * J :         2965 |        2 |  0.28% | 0.00275 |  0.17%
  * K :        40773 |       30 |  3.79% | 0.03787 |  2.39%
  * L :        81643 |       60 |  7.58% | 0.07582 |  4.78%
  * M :        44893 |       33 |  4.17% | 0.04169 |  2.63%
  * N :       142009 |      105 | 13.19% | 0.13189 |  8.31%
  * O :        57327 |       11 |  9.07% | 0.09073 |  3.36%
  * P :        29530 |       22 |  2.74% | 0.02743 |  1.73%
  * Q :         1101 |        1 |  0.10% | 0.00102 |  0.06%
  * R :       134362 |       99 | 12.48% | 0.12479 |  7.86%
  * S :       114038 |       85 | 10.59% | 0.10591 |  6.67%
  * T :       115004 |       85 | 10.68% | 0.10681 |  6.73%
  * U :        68768 |       13 | 10.88% | 0.10884 |  4.02%
  * V :        13363 |        9 |  1.24% | 0.01241 |  0.78%
  * W :        16878 |       12 |  1.57% | 0.01568 |  0.99%
  * X :         2022 |        1 |  0.19% | 0.00188 |  0.12%
  * Y :         5361 |        1 |  0.85% | 0.00848 |  0.31%
  * Z :        27903 |       20 |  2.59% | 0.02591 |  1.63%
  * ä :        14884 |        3 |  2.36% | 0.02356 |  0.87%
  * ö :         5383 |        1 |  0.85% | 0.00852 |  0.32%
  * ü :        10856 |        2 |  1.72% | 0.01718 |  0.64%
  * -----------------------------------------------------
  * Source texts, Language en, Type NST_TEXTS_EN
  *       char found | recalced |  quote |   rate  |  all
  * A :        56952 |        4 | 22.03% | 0.22034 |  8.88%
  * B :        13328 |       12 |  3.48% | 0.03482 |  2.08%
  * C :        25356 |       23 |  6.63% | 0.06625 |  3.95%
  * D :        23146 |       21 |  6.05% | 0.06048 |  3.61%
  * E :        74841 |        6 | 28.96% | 0.28956 | 11.67%
  * F :         8350 |        8 |  2.18% | 0.02182 |  1.30%
  * G :        15940 |       14 |  4.16% | 0.04165 |  2.49%
  * H :        16857 |       15 |  4.40% | 0.04405 |  2.63%
  * I :        53491 |        4 | 20.70% | 0.20695 |  8.34%
  * J :         1977 |        2 |  0.52% | 0.00517 |  0.31%
  * K :         6784 |        6 |  1.77% | 0.01773 |  1.06%
  * L :        37144 |       34 |  9.71% | 0.09705 |  5.79%
  * M :        18561 |       17 |  4.85% | 0.04850 |  2.89%
  * N :        47548 |       43 | 12.42% | 0.12424 |  7.42%
  * O :        39896 |        3 | 15.44% | 0.15436 |  6.22%
  * P :        16595 |       15 |  4.34% | 0.04336 |  2.59%
  * Q :         1105 |        1 |  0.29% | 0.00289 |  0.17%
  * R :        47959 |       43 | 12.53% | 0.12531 |  7.48%
  * S :        43723 |       40 | 11.42% | 0.11424 |  6.82%
  * T :        41171 |       37 | 10.76% | 0.10758 |  6.42%
  * U :        20580 |        2 |  7.96% | 0.07962 |  3.21%
  * V :         6727 |        6 |  1.76% | 0.01758 |  1.05%
  * W :         6223 |        6 |  1.63% | 0.01626 |  0.97%
  * X :         1732 |        2 |  0.45% | 0.00453 |  0.27%
  * Y :        12708 |        1 |  4.92% | 0.04917 |  1.98%
  * Z :         2489 |        2 |  0.65% | 0.00650 |  0.39%
  * -----------------------------------------------------
  * Source texts, Language all, Type NST_TEXTS_ALL
  *       char found | recalced |  quote |   rate  |  all
  * A :       168046 |        9 | 19.56% | 0.19559 |  7.25%
  * B :        52286 |       24 |  3.58% | 0.03583 |  2.26%
  * C :        76489 |       35 |  5.24% | 0.05241 |  3.30%
  * D :        62537 |       28 |  4.28% | 0.04285 |  2.70%
  * E :       307308 |       17 | 35.77% | 0.35767 | 13.25%
  * F :        44419 |       20 |  3.04% | 0.03044 |  1.92%
  * G :        79729 |       36 |  5.46% | 0.05463 |  3.44%
  * H :        97779 |       44 |  6.70% | 0.06700 |  4.22%
  * I :       179190 |       10 | 20.86% | 0.20856 |  7.73%
  * J :         4942 |        2 |  0.34% | 0.00339 |  0.21%
  * K :        47557 |       22 |  3.26% | 0.03259 |  2.05%
  * L :       118787 |       54 |  8.14% | 0.08139 |  5.12%
  * M :        63454 |       29 |  4.35% | 0.04348 |  2.74%
  * N :       189557 |       86 | 12.99% | 0.12988 |  8.18%
  * O :        97223 |        5 | 11.32% | 0.11316 |  4.19%
  * P :        46125 |       21 |  3.16% | 0.03160 |  1.99%
  * Q :         2206 |        1 |  0.15% | 0.00151 |  0.10%
  * R :       182321 |       83 | 12.49% | 0.12492 |  7.86%
  * S :       157761 |       72 | 10.81% | 0.10810 |  6.80%
  * T :       156175 |       71 | 10.70% | 0.10701 |  6.74%
  * U :        89348 |        5 | 10.40% | 0.10399 |  3.85%
  * V :        20090 |        9 |  1.38% | 0.01377 |  0.87%
  * W :        23101 |       10 |  1.58% | 0.01583 |  1.00%
  * X :         3754 |        2 |  0.26% | 0.00257 |  0.16%
  * Y :        18069 |        1 |  2.10% | 0.02103 |  0.78%
  * Z :        30392 |       14 |  2.08% | 0.02082 |  1.31%
  * -----------------------------------------------------
  @endverbatim
**/

/* --- resulting consonant array --- */
const uint32_t conListLen[6] = { 791, 458, 588, 791, 347, 663 };

const char     conList[6][792] = {
  "dzdsbrhlnsbtnrbdbnwrbtmtbdbrtcrtckcnchcbvwcmndcrshcfclgldbdstlrsmkdsdvtlchdr"
  "dndrdndfnpdcdfdcnzdkrltntdrdsdglndfrfnfrflrhfdfsnrsjfsfvgcstkngctzgdvlgrnrhr"
  "gcmrnlglhchdhdtnhrhnlmrnlhntlrthdhbhthththnhvhnhnghrhjhgrjrmtzmkqjrmnktjfnjk"
  "rmnknkgkrkmkckdmlkrnjdkrkgrckmnklrhltlnlslnlglhdrhmrlstlncnlhlnrmltshglmlsds"
  "rfmglrlslglwlflcnrlnjlrlsldgnmlnmskmslrnmhmlkrcmwmnmkpsvrmjrmrsmslmnmnmlmblm"
  "lsprnknjmkmnmnkfkhlntngdrnshmgnrnhnpsrnlnbthnlkdnrnhndwhndnrltdnrsvtnrnlngrn"
  "snbncrbnrndntrlntnknsrgnsnfnbznknwslnlnlplpmptbnkjrsdmrdrlntlrkrnbrcrnprldrl"
  "rnmrdrkrsrmglnrhnmrkrtrbrlsrdrlrlsgsgrsrjrmrlrmrnrdjrnrdrhrtmrdtnrznrpfrsrkr"
  "lrnrmshsdmscnlstfdshszsfshslststsrsdsfhsnsrltnmzsmgszsmtsmjsndsgslsbsmstnlsp"
  "rsnltflthtbthntstbtshtbtrtptltztsmtdtsntghtdbsdtrtctrtljtbtlnsrtlththvnvhvct"
  "vgvnvfvswbwsmwcwvmxzdzrzjzlzrlz",
  "qwxnrcntbkbrlgbsrnbsnlblvdchmlctlntlrclckndcrcldtdtdnlrdrdcdpdldfsnrldrdtdkd"
  "lnrdnfvfvnfrztghgrslnlgdgcghnhtlnknmhsrhvsnhrchwvnhnmjljljmlkrkldcbklkndrndl"
  "dlrlsnklshlstlsltlnlnlgnlmlbklrcdnrbljlhlrnjlrlsnhlbnrlclwlcmcrsmdmtmhmlmgwc"
  "mlmtntnlnhlntnhnmnhnlnsnrnbrdnrbkncnhltfsntbnlnmntntrlndncnrnjnrsrngnbtldpcp"
  "rwrlrlwdbrdlrjrhmrlrnmrnrtcrmrflrsrprdrtnsrwrzrfrnrntdlrknrcrcrhrfskstblswns"
  "nhstspdsrspsmshsxsbsrglntskhtdtgngltnhgtrthjdtrtntrtrtsnctcvdvhdfbvwtwmlwrwr"
  "nw",
  "gznztrmsrvmbtbshrlnbvbrndcdcszscnclcdctcdcnchmlcgdnkldhdldtdrdtdshdrjbdldrdb"
  "dwcmrdjrsfldlkfnhxfsjsntnlfkglrlkrgngsdsrldcglthjhdhfhsmhrhkgchtnrtfvnhphrnr"
  "hfhrhcjtbhjmjfjnmnsrslgklktktpksklkrltkltlklbnlnltdlrlvnbmrlmltlmlnstlnmrlgr"
  "lnlmslmtltlrlkdlnlrndlnrsglmlmcmcmjrmlnrmjmgmnrcmnmlmlmhvnlcbmrnrmnrndhnhjnf"
  "rknrnhvnstnrtnfnmlsrlndnrnlfbnsnclnlnbhdntnsdtrmnlnpmzswlrmnrwcncnrldrlwpspf"
  "nqrntnrwnlrgbhrtcrnrnmrfrdrdrdnrhrsrcmrtsdrkrdrlnkrbgnrprgndrdslbrgnrsrptrhr"
  "nrnrdrhrlsnslsrslsrscsnsnslsztwstsnsbntsrsrhsgshgszksghnthszlktltdzltkhdtstn"
  "btklngthtmtbtntrtstrtntrhktnslvbvmdvhvnvwbwnwdrwpnwfztrn",
  "zgzsbsmgcrktcrbpbsbhbcnhblbwbmbfcskdchcsncncbcncrdscmctcnlctcbdcncsfhdhdbtrd"
  "tdhdtdldpdclcdndwsdspzktmnfnflnrtflfsfbftnslnfrfrtwgmghgtgcgsgsgzgctngprglth"
  "lsgtgtkvgngtnrgdgngsgrlnphnhrhphrhkhrchctbhshnkmhlhshtdhnbhlsfmkrhlhwhdhknth"
  "grsjrktktklkhrknstckznrkgkhnrkhtwlrlnhrzlgwmvwnflslthrnlflfdlhlhlktsflslslgd"
  "lcklblrsnldrhrlrlrsglmtmnmgmshmgndcmrmsmbmgrmtmnmnmtglnmntmfcnfnprnrgrgcjntn"
  "gnhrsnmnkrsncnmrhdnsknglncnfnrvrnsnrlntnhsrsrntstnlndlnrpstkntnrgntntnsnhntn"
  "tnlhnpfpspnpnpnhcprpbrpmpdpsrsrnrlrlrznrprnrnsrbnrgrgrvrsrsrlrnrnrhrgrmnhnrs"
  "ftbrtnlrbszrtrsrtrtrhlrnrhrhrfrnlrhbrnrlzcrftrmnrsrkfzshstsnsbtrsrlscsfscstr"
  "srntgsgschcgstsbslspsdfklscsdbsnsmsnlsnslstnsnsnsgskrfqdslzsrtkmtsgthrsrhtmt"
  "lntdthtghtftskntnbtbtntltlmtzthtgtbtnrntmrntntptgtktrthzrdtrgtnthtwthvlvlvsv"
  "nzwsghlwrtwkwlxzszvznsnpzrkznzt",
  "nxzgbnbsbtblbrbncscpclcrclcrcrscsclbcnldrdbdcdsdsdndcdndpdtdfnfdtmftgrgrglkh"
  "gngstrhghlgthnhtshrhrjclpkpkrlrlvmstlnlhnwlbldtmlclntlklnglptlsrqnlmlmrtrnrm"
  "rmnmntnrntnmngtdntnrnrnwnsnsdsntngcsprnmlnsnrnxcpwplpmpcpwptpgmrjrbscrclhrnk"
  "rnmrlmfrsrmrlrtnrtrnrcrvrbdhrlsgtfsvnrsdsfstnsbdszskslslsvscsndsrsrscshtnlpt"
  "htrftltdtshthntcthtstfdsmtststgvsvptnlwrlwm",
  "szszbrbvbcbrbtbnbhbrbtbrhtbscrbcsnzcdcdczpscsdcgpnclcgtpcsblmlhcfdgdsdcdnrdt"
  "drtncdhdndmdndnlnfpfmftgrflfnlrdsnlgskcgnrhglnhgdpsgcgtnlmrgngtgthrgfrghlnth"
  "phthbtrlshvgbhshwhshbhltmhthththbhsnthnjrksknkrkltgklbknsvkwknklglglthckdtcl"
  "cklhlflmflrlscfrtlplstnrtsnlslstmlrwlrlnrmsmsmrwmgmnmsfmfmtmrmpbfglnwmnrncnk"
  "rnrpfnhsnzmnlngnrndntnfnmpnhgnpncntnrgncntlrngnrtrnlsnlrknhnrdgntgntnzsnrnsn"
  "dnhnpnpsnrsnprprdpgplncpcqrcnclrzhrnrgrcrtrzhrgrlrsrcrlnrkrlrvrhrkhrxngckrlr"
  "nprctlrndbrhdrjnbrsrltsnkrptmrsnrsmstslsrlsrfscrtrskstshzrsksgstrdsltscshlrs"
  "rmskschsntsnshsdlmszstsnstsnstrhtstrntmtmtdtlntcfdtntglstntftltptbrhtltfbntr"
  "sntrtlgstbtltgtrsthtgnstvhrvkvwvhwrmwswfmvwhxzmznsnszdz",
};

/* --- resulting vowel array --- */
const uint32_t vowListLen[6] = { 599, 21, 19, 118, 20, 47 };

const char     vowList[6][600] = {
  "aoaoeyaeoeöeoeieiaeieaieiaiaieieioyaeauieaeayeaieyieieaiueauaeaiaeaeiyaieiea"
  "ioieaiayiaieaoaeieuaeauoiaoaoaieaioiaeaoaeaiaeaiauaoaiaiaoaoaiauaiauiaiaieai"
  "eaoaoaeaeaeoeaeaeaeoaeaeioaeioiaiaoaeiaeaiaeiayeaeaeyeaeaeyeaieiaeaeaueueoao"
  "aeaeaiaoeoaoeieieioueieuaeieaieaeaeieieaeaeoieieoaieoeuaueoaieieaeaoeaoeaieo"
  "aeayeiuaeuyiaeaeaieaieieoieoeiaeiuaiueouiaiaiuieaeaeueäeiaiaieieauiaioauioae"
  "aiaioiaiuiaeiaiaiaieaeuaiaieaoiaieiueiaeyaiaeoaeoeioiaiyieieueaiyieaiaieoiua"
  "eaeioiayieaieauyaeiaioaioaieoaioeioeaiaeoaoiueoeoeaoaoaeieayoiaeoauiaioiaeao"
  "aiauoaeieaeioeaueiueiaiuaeueiaiaeuauiaiaouiuauiaieyaueayaiayaeaiaio",
  "uaeioaoayeyaeieiaeaoe",
  "aoueaeaiaieiyaeioae",
  "aieöüiaeieaeaeaeaiaieaoieaieoeueaeieaeueueuaieueieuäeaieaiaueueieoeaeaeieäüi"
  "aeieieiaoieoiaäieoeaueueioeoeoueueoeoieuey",
  "iuyaeoaeieaoeiueieao",
  "eueaieaiaeauaeueoeioeoaeieyeieaieioeieieoueiaua",
};

/** @file CRandomWordConstants.h
  *
  @verbatim
  * Follow-up statistics :
  * ----------------------
  *  names / de => NST_NAMES_DE
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z | ä | ö | ü |
  * A | 30|129| 83|235| 39| 83| 80|202|150| 43| 95|405|250|999| 15| 25|  8|817|272|277|118| 69| 48|  8|120| 96|  0|  0|  0|
  * B |192| 15|  0|  1|288|  0|  0|  4|125|  2|  3| 12|  0|  2| 80|  0|  0| 64|  3|  0| 47|  0|  0|  0|  8|  0|  1|  0|  0|
  * C |182|  0|  6|  3|114|  0|  1|216|114|  2| 57| 32|  3|  0|108|  0|  1| 10|  1|  7| 19|  0|  0|  0| 10|  4|  0|  0|  0|
  * D |264|  5|  1| 35|333|  2|  8| 15|321| 20|  1| 14| 16|  5|193|  0|  0| 77|  5| 28| 57|  2| 15|  0| 24| 12|  0|  0|  0|
  * E | 47| 68| 74|269| 57| 82| 73| 85|183| 22| 96|616|213|535| 63| 31|  1|889|234|359| 37| 95| 18|  7| 89| 55|  0|  0|  0|
  * F |129|  0|  1|  0|121| 30|  3|  3|109|  1|  3| 19|  0|  1| 35|  0|  0|176|  4| 10| 13|  0|  1|  0|  0|  1|  0|  0|  0|
  * G |143|  4|  0|  9|211|  4| 10| 26|183|  0|  1| 22|  9| 19| 74|  0|  0| 69|  2|  2|104|  1|  6|  0|  6|  0|  0|  0|  0|
  * H |605|  6|  0|  9|320|  1|  0|  2|254|  3|  4| 12| 34| 19| 87|  1|  0| 72| 18| 23| 48|  1|  3|  0| 14|  2|  0|  0|  0|
  * I |304| 67|225|194|363| 47|103| 57|  5| 64|191|470|210|866|157| 46|  9|263|340|207| 44| 45|  4|  6| 74| 61|  0|  0|  0|
  * J |238|  1|  2|  9| 98|  1|  1|  1| 22|  0|  7|  5|  3|  4|107|  0|  0|  7|  1|  3| 60|  0|  0|  0|  0|  0|  0|  0|  0|
  * K |321|  3|  2|  1|178|  1|  0| 32|113|  4| 17| 25|  9|  6|140|  0|  0| 42| 13| 12| 50|  0|  7|  0|  7|  1|  0|  0|  0|
  * L |430| 29| 13|187|312| 70| 30| 29|520| 28| 57|191| 71|  3|164|  8|  0| 10| 31| 54|103| 49|  9|  0| 48|  4|  0|  0|  0|
  * M |610| 16|  4|  8|321|  4|  3|  6|344|  1|  6|  6| 51|  2|131| 12|  0|  9| 15|  6|143|  0|  0|  0| 15|  7|  0|  0|  0|
  * N |566|  6| 68|215|431| 21|102| 41|345| 25| 67|  7|  2|198|179|  1|  0| 24| 77|128| 86|  3|  2|  0| 41| 48|  0|  0|  0|
  * O | 24| 48| 24| 60| 25| 30| 27| 34| 15| 18| 26|248| 86|265| 26| 23|  1|307|225| 74| 92| 27| 15|  2| 16| 17|  0|  0|  0|
  * P | 91|  0|  0|  0| 83|  0|  0| 38| 50|  0|  1|  4|  1|  0| 31| 25|  0| 21|  3|  3|  4|  0|  0|  0|  1|  0|  0|  0|  0|
  * Q |  5|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 26|  0|  0|  0|  0|  0|  0|  0|  0|
  * R |535| 33| 37|212|417| 10|113| 39|753| 16| 64| 64| 75| 85|237|  6|  0| 41| 56|222|129| 17| 16|  0| 55| 25|  0|  0|  0|
  * S |433| 10| 56|  5|421|  5|  4| 80|294|  4| 27| 64| 38| 10|105| 16|  3| 18| 93|240| 91|  9| 13|  0| 30| 13|  0|  0|  0|
  * T |349|  3|  5|  1|263|  8|  5|217|308| 28| 10| 18| 15|  3|146|  1|  0|122| 13|199| 59|  3|  8|  0| 15| 20|  0|  0|  0|
  * U | 33| 44| 28|109| 56| 19| 28| 47| 32|  6| 32|152| 44|164|  3| 13|  1|210|245| 82|  3|  4|  3|  1|  8| 36|  0|  0|  0|
  * V |139|  0|  2|  3|116|  0|  3|  1|150|  0|  6|  9|  1|  1| 39|  0|  0|  7|  3|  1|  7|  1|  0|  0|  2|  4|  0|  0|  0|
  * W |131|  0|  0|  0| 49|  1|  1|  2|118|  0|  0|  5|  0|  1| 22|  0|  0|  0|  1|  1|  4|  0|  1|  0|  1|  1|  0|  0|  0|
  * X |  6|  0|  0|  0|  3|  0|  0|  1|  6|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  1|  0|  0|  0|  0|  0|  0|  0|  0|  0|
  * Y |122|  7|  4| 18| 88|  6|  4|  8| 37|  0| 10| 35| 14| 33| 23|  1|  0| 22| 24|  7| 18|  6|  0|  0|  3|  3|  0|  0|  0|
  * Z |127|  3|  1|  8|103|  0|  3|  6|126|  1|  3| 15|  4|  0| 22|  0|  0|  2|  4|  1| 17|  4|  2|  0|  9|  5|  0|  0|  0|
  * ä |  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  1|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|
  * ö |  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|
  * ü |  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|
  * ------------------------------------------------------------------------------------------------------------
  *  names / en => NST_NAMES_EN
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z |
  * A |  7| 98|116|230|172| 24| 59|138|167|  1| 47|549|218|999|  1| 24| 12|821|149|214|132| 98| 62| 26|225| 17|
  * B |103| 28|  0|  1|278|  0|  0|  2| 74|  0|  0| 49|  0|  0| 76|  0|  0|227|  3|  1|101|  0|  0|  0| 66|  0|
  * C |230|  3|  6|  1|197|  3|  5|234| 96|  0|150| 96|  2|  0|209|  0|  4| 45|  1|  7| 26|  0|  4|  0| 34|  0|
  * D |333| 16|  2| 66|412|  6| 31|  8|151|  0|  4| 37| 24| 13|151|  1|  0|118| 27|  6| 50|  0| 63|  0| 80|  0|
  * E |322| 41| 53|171|180| 48| 51| 16| 93|  3| 25|811| 88|582|100| 18|  2|771|280|235| 23|112| 59| 37|246|  9|
  * F | 82|  0|  1|  1| 67| 49|  0|  0| 53|  0|  0| 17|  0|  1|101|  0|  0| 88|  0| 17| 12|  0|  1|  0|  4|  0|
  * G |128| 12|  0| 13|154|  2| 14|114| 71|  0|  0| 27|  3|  5| 45|  0|  0| 53|  9| 11| 27|  0|  6|  0|  9|  0|
  * H |401| 12|  5|  4|279|  5|  0|  1|132|  0|  1| 44|  9| 12| 94|  0|  0| 62|  4| 48| 63|  0| 18|  0| 24|  0|
  * I |322| 22|220| 94|454| 37|110|  0|  0|  3| 59|305| 56|647| 77| 25|  8|121|317|160| 12| 36|  4|  8| 22| 20|
  * J |170|  0|  0|  0|122|  0|  0|  0| 22|  0|  0|  0|  0|  0|106|  0|  0|  0|  0|  0| 30|  0|  0|  0|  0|  0|
  * K |204|  1|  1|  0|186|  3|  0| 11|113|  0| 11| 23|  1|  6| 31|  0|  0| 28| 15|  0|  4|  0|  8|  0| 33|  0|
  * L |462| 71| 15|177|712| 68|  6| 17|484|  0|  7|486| 46|  8|186| 10|  0| 12| 54| 55| 40| 50| 30|  1|291|  2|
  * M |432| 41|  8|  1|151|  2|  0|  2|163|  0|  1|  6| 38|  2|121| 16|  0|  8| 11|  1| 26|  0|  2|  0| 36|  2|
  * N |463| 18| 76|312|595| 21|105| 13|214|  6| 14| 37|  0|362| 96|  0|  1| 15| 82|140|  6|  1| 27|  0| 67| 18|
  * O | 17| 40| 68|119| 35| 13| 19| 18| 23|  1| 25|217| 58|635| 63| 25|  0|513|111| 70| 96| 24| 65| 16| 25|  4|
  * P | 66|  0|  0|  0| 86|  2|  0| 53| 22|  0|  0| 13|  1|  0| 26| 14|  0| 37| 10|  5|  5|  0|  2|  0|  3|  0|
  * Q |  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 59|  0|  0|  0|  0|  0|
  * R |463| 25| 49|265|498| 12| 37| 45|622|  7| 44|184| 50|154|282| 10|  5|203| 62|212| 60| 36| 54|  0|181|  0|
  * S |189| 23| 71|  8|176|  3|  1|192| 98|  0| 25| 44| 26|  1|138| 24|  1|  6|116|382| 49|  5| 28|  0| 37|  0|
  * T |265|  6| 23|  0|296|  5|  0|301|195|  1|  4| 48|  6| 14|264|  0|  0|112| 18|212| 78|  0| 26|  0| 76| 18|
  * U | 16| 14| 41| 61| 58| 10| 49|  8| 41|  0|  9| 86| 33|149|  0| 13|  0|192| 80| 45|  0|  2|  1|  7|  7|  7|
  * V | 86|  0|  0|  0|151|  0|  0|  0|147|  0|  0|  1|  0|  0| 32|  0|  0|  9|  0|  0|  0|  1|  0|  0| 25|  0|
  * W |169|  1|  0|  2|146|  2|  0| 21|187|  0|  0| 15|  0| 22| 80|  1|  0|  8|  5|  1|  9|  0|  0|  0| 75|  0|
  * X | 18|  0|  0|  0|  2|  4|  0|  0| 21|  0|  0|  4|  0|  2|  6|  0|  0|  0|  0| 10|  1|  0|  1|  0|  2|  1|
  * Y | 94|  8| 37| 42| 40| 14|  9|  6|  2|  1| 10|151| 23|323| 18|  4|  0| 41| 69| 33|  2|  8|  2|  0|  0|  3|
  * Z | 43|  2|  0|  0| 25|  0|  2|  0| 16|  0|  0|  1|  7|  0|  7|  0|  0|  0|  2|  0|  0|  0|  1|  0|  7|  7|
  * ------------------------------------------------------------------------------------------------------------
  *  names / all => NST_NAMES_ALL
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z |
  * A | 21|117| 96|233| 92| 60| 72|177|157| 27| 76|462|238|998| 10| 24|  9|819|223|252|123| 80| 54| 15|161| 65|
  * B |157| 20|  0|  1|284|  0|  0|  3|105|  1|  1| 27|  0|  1| 79|  0|  0|128|  3|  1| 68|  0|  1|  0| 31|  0|
  * C |201|  1|  6|  2|147|  1|  2|223|107|  1| 93| 57|  3|  0|148|  0|  2| 24|  1|  7| 22|  0|  1|  0| 19|  2|
  * D |291|  9|  1| 47|364|  4| 17| 12|254| 12|  2| 23| 19|  8|177|  1|  0| 93| 14| 19| 54|  1| 34|  0| 46|  8|
  * E |156| 57| 66|230|106| 69| 65| 58|147| 14| 68|693|164|554| 77| 26|  2|842|252|310| 31|102| 34| 19|151| 37|
  * F |111|  0|  1|  1|100| 37|  2|  2| 87|  1|  2| 18|  0|  1| 61|  0|  0|141|  2| 13| 12|  1|  1|  0|  2|  1|
  * G |137|  7|  0| 10|189|  4| 12| 61|138|  0|  1| 24|  7| 14| 63|  0|  0| 63|  5|  5| 74|  1|  6|  0|  7|  0|
  * H |525|  8|  2|  7|304|  3|  0|  2|206|  1|  3| 25| 24| 16| 90|  1|  0| 68| 13| 33| 54|  1|  9|  0| 18|  1|
  * I |311| 49|223|155|399| 43|106| 35|  3| 40|139|405|149|779|126| 38|  9|207|331|189| 31| 41|  4|  6| 53| 44|
  * J |211|  1|  1|  5|107|  1|  1|  1| 22|  0|  4|  3|  2|  2|107|  0|  0|  4|  1|  1| 48|  0|  0|  0|  1|  0|
  * K |275|  2|  2|  1|181|  2|  0| 23|113|  2| 14| 24|  6|  6| 97|  0|  0| 36| 14|  7| 32|  0|  7|  0| 17|  1|
  * L |443| 46| 14|183|470| 69| 21| 24|506| 17| 37|307| 61|  5|173|  9|  0| 11| 40| 55| 78| 50| 17|  1|144|  3|
  * M |540| 26|  6|  5|254|  4|  2|  4|272|  1|  4|  6| 46|  2|127| 13|  0|  9| 13|  4| 97|  0|  1|  0| 23|  5|
  * N |525| 10| 72|253|496| 21|103| 30|293| 18| 46| 19|  1|263|146|  1|  1| 21| 79|133| 54|  2| 12|  0| 51| 36|
  * O | 21| 45| 41| 83| 29| 23| 24| 28| 18| 11| 26|236| 75|411| 40| 24|  1|388|180| 73| 93| 26| 35|  8| 20| 12|
  * P | 81|  0|  1|  0| 84|  1|  0| 44| 39|  0|  1|  8|  1|  0| 29| 21|  0| 27|  5|  4|  4|  0|  1|  0|  2|  0|
  * Q |  3|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 39|  0|  0|  0|  0|  0|
  * R |507| 30| 42|233|449| 11| 83| 42|702| 12| 56|111| 65|112|255|  7|  2|105| 58|218|102| 24| 31|  0|105| 15|
  * S |337| 15| 62|  6|324|  4|  3|125|217|  2| 26| 56| 33|  7|118| 19|  2| 13|102|296| 74|  8| 19|  0| 33|  8|
  * T |316|  4| 12|  1|276|  7|  3|250|263| 18|  8| 30| 11|  7|193|  1|  0|118| 15|204| 67|  2| 15|  0| 39| 19|
  * U | 26| 32| 34| 90| 57| 16| 36| 31| 36|  4| 23|126| 39|158|  2| 13|  1|203|180| 67|  2|  4|  2|  3|  8| 25|
  * V |118|  0|  1|  2|130|  0|  1|  1|149|  0|  4|  6|  1|  1| 36|  0|  0|  8|  2|  1|  4|  1|  0|  0| 11|  2|
  * W |146|  1|  0|  1| 87|  2|  1|  9|145|  0|  1|  9|  0|  9| 45|  1|  0|  3|  3|  1|  6|  0|  1|  0| 30|  1|
  * X | 10|  0|  0|  0|  3|  2|  0|  1| 12|  0|  0|  1|  0|  1|  2|  0|  0|  0|  0|  5|  1|  0|  1|  0|  1|  1|
  * Y |111|  7| 17| 28| 69|  9|  6|  7| 23|  1| 10| 80| 17|148| 21|  2|  0| 30| 42| 17| 12|  7|  1|  1|  2|  3|
  * Z | 94|  3|  1|  5| 72|  0|  3|  4| 83|  1|  2|  9|  5|  0| 16|  1|  0|  1|  3|  1| 10|  2|  2|  0|  8|  6|
  * ------------------------------------------------------------------------------------------------------------
  *  texts / de => NST_TEXTS_DE
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z | ä | ö | ü |
  * A | 12|102| 94| 51|  4| 52| 71| 63| 22|  2| 37|212| 84|326|  2| 39|  1|207|114|178|235| 12|  4|  4|  6| 13|  0|  0|  0|
  * B | 77|  8|  1|  2|281|  3| 13|  6| 57|  1|  3| 40|  1|  3| 28|  1|  0| 55| 24| 10| 33|  1|  4|  0|  2| 12|  8|  3| 10|
  * C | 14|  0|  1|  1| 10|  0|  0|772|  7|  0|102|  3|  0|  0| 12|  0|  0|  2|  1|  1|  3|  0|  0|  0|  1|  1|  1|  0|  0|
  * D | 57|  5|  1|  5|239|  3|  5|  5| 99|  1|  2| 15|  3|  5| 33|  1|  0| 42| 19| 11| 51|  2|  4|  0|  2|  2|  4|  1|  5|
  * E | 20| 83| 66| 75| 26| 54|102| 79|440|  2| 55|289| 75|948| 16| 32|  2|998|213|141| 62| 14| 35| 20|  5| 23|  1|  0|  1|
  * F | 62|  4|  1|  1|131| 38| 12|  2| 50|  1|  2| 54|  2|  2| 38|  1|  0| 59| 14| 63| 20|  1|  3|  0|  0| 12| 20|  8| 20|
  * G | 50|  3|  0|  1|478|  2| 11|  4| 51|  1| 22| 40|  5| 13| 18|  1|  0| 74|101| 20| 31|  1|  2|  0|  1|  5|  5|  1|  2|
  * H |158|  8|  1|  2|325|  6| 12|  3|141|  1| 15| 85| 30| 56| 68|  2|  0| 87| 35|106| 35|  2| 34|  0| 13| 12| 29| 15| 11|
  * I | 47| 29|159| 50|332| 46|179|  8|  1|  2| 59|106| 54|401|103| 19|  1| 47|286|245| 17| 28|  1|  2|  2| 23|  1|  1|  0|
  * J | 17|  0|  0|  1|  9|  0|  0|  0|  1|  0|  1|  1|  0|  0|  8|  0|  0|  1|  0|  0| 10|  0|  0|  0|  0|  0|  2|  0|  1|
  * K |103|  3|  1|  1|154|  2|  5|  4| 30|  1|  4| 45|  2| 15| 99|  1|  0| 58| 16| 72| 46|  1|  2|  0|  1|  6|  8|  8|  7|
  * L |181| 34|  5| 42|251| 21| 17|  7|240|  1| 19|134| 16| 29| 85| 10|  1|  5| 36| 87| 57|  7|  5|  0| 11| 17| 31| 12| 13|
  * M |134| 16|  1|  4|173|  4|  8|  4|103|  0|  4|  7| 72|  3| 52| 41|  0|  2| 17| 13| 44|  1|  3|  0|  3|  7| 14|  6| 11|
  * N |161| 41| 11|226|184| 44|305| 31|127|  3| 72| 26| 13| 62| 48| 13|  1| 17|174|191| 36| 15| 20|  0|  3| 86| 10|  4| 11|
  * O |  5| 26| 35| 27|  4| 18| 51| 21|  6|  2| 17|107| 60|194|  6| 45|  1|195| 90| 56| 13|  7|  6|  6|  1| 11|  0|  1|  0|
  * P | 57|  1|  1|  1| 83| 47|  1| 42| 44|  0|  1| 31|  1|  1| 48| 34|  0| 80| 10| 16| 21|  1|  0|  0|  2|  0|  4|  1|  1|
  * Q |  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 19|  0|  0|  0|  0|  0|  0|  0|  0|
  * R |276| 72| 36| 51|363| 48| 80| 29|229|  1| 68| 40| 67| 73|125| 20|  1| 38|113|156|116| 17| 29|  1|  4| 56| 40| 13| 51|
  * S |105| 34|375|  9|243| 24| 37| 17|132|  1| 38| 23| 35|  6| 44| 93|  1| 15|191|395| 37| 14| 16|  0| 13| 27| 15|  2|  6|
  * T |147| 16|  4|  3|430| 11| 22| 59|269|  1|  8| 31|  9|  7| 82|  6|  1|152| 99| 97| 71|  5| 12|  0|  7| 73| 42|  6| 16|
  * U | 14| 37| 44| 25| 39| 85| 38| 17| 11|  1| 25| 61| 96|274|  3| 24|  1|128|212| 70|  1|  3|  9|  1|  1| 16|  1|  0|  0|
  * V | 20|  1|  0|  1|114|  1|  1|  0| 34|  0|  1|  1|  1|  0| 53|  1|  0|  1|  1|  1|  1|  1|  1|  0|  0|  1|  1|  1|  0|
  * W | 60|  0|  0|  0|115|  0|  1|  1| 62|  1|  0|  1|  0|  1| 17|  0|  0|  1|  1|  1| 12|  0|  0|  0|  1|  0| 20|  5|  8|
  * X |  2|  1|  0|  0|  4|  1|  1|  1|  8|  0|  1|  0|  1|  0|  1|  4|  0|  0|  1|  5|  1|  1|  1|  0|  1|  1|  0|  0|  0|
  * Y |  6|  2|  2|  4|  5|  1|  1|  1|  1|  0|  3|  8|  6|  6|  2|  9|  0|  5| 11|  4|  1|  1|  1|  0|  0|  1|  1|  0|  0|
  * Z | 17|  4|  1|  1|108|  2|  2|  2| 65|  1|  2|  3|  1|  1| 15|  1|  0|  1|  4| 12|191|  1| 19|  0|  4|  2|  5|  3|  4|
  * ä |  1|  3| 15|  9|  1|  8| 10| 29|  1|  0|  1| 13| 12| 46|  1|  1|  1| 31| 19| 37| 26|  1|  0|  0|  0|  1|  0|  0|  0|
  * ö |  0|  2|  5|  3|  0|  3|  6|  7|  0|  0|  1|  7|  3|  9|  1|  5|  0| 20| 14|  4|  0|  1|  1|  0|  0|  1|  0|  0|  0|
  * ü |  0| 34| 36|  5|  0|  3|  7| 21|  0|  0|  0|  8|  4| 24|  0|  2|  0| 24| 11| 14|  0|  0|  0|  0|  0|  0|  0|  0|  0|
  * ------------------------------------------------------------------------------------------------------------
  *  texts / en => NST_TEXTS_EN
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z |
  * A |  4|196|203|162| 46| 33|109| 37|134|  4| 49|502|170|640|  5|118|  5|535|208|524| 90| 66| 38| 16| 77| 18|
  * B |135| 35|  2|  2|166|  1|  0|  1|114|  3|  0|184|  2|  1|110|  1|  0|109| 22|  5| 79|  1|  1|  0| 20|  0|
  * C |281|  0| 29|  1|213|  0|  1|259|137|  0|131| 86|  1|  2|314|  0|  4|107| 13|123| 84|  0|  0|  0| 33|  1|
  * D |134|  9|  4| 41|364|  5| 23|  8|243|  3|  1| 39| 12| 22|109|  3|  0| 74| 39|  4| 56|  7| 14|  0| 30|  1|
  * E |253| 40|159|451|135| 60| 62| 20| 67|  5| 14|358|148|605| 54| 88| 13|999|652|241| 36| 69| 46| 73| 70| 10|
  * F | 69|  0|  1|  0| 79| 56|  0|  1|102|  0|  0| 60|  0|  1| 86|  0|  0| 57|  4| 20| 59|  0|  0|  0| 11|  0|
  * G |107|  3|  0|  3|179|  2| 41| 70|100|  0|  0| 55|  8| 36| 64|  1|  0|105| 23|  6| 56|  0|  4|  0| 24|  0|
  * H |236|  6|  2|  2|275|  4|  1|  1|172|  0|  1| 18| 13| 16|175|  1|  0| 46| 25| 42| 53|  0|  9|  0| 50|  0|
  * I |235| 64|368|136|216| 62|119|  2|  1|  2| 29|266|125|906|289| 73|  7|119|420|320| 23|117|  2| 10|  3| 90|
  * J | 42|  0|  0|  0| 33|  0|  0|  0|  9|  0|  0|  0|  0|  0| 36|  0|  0|  0|  0|  0| 27|  0|  0|  0|  0|  0|
  * K | 62|  4|  1|  1|135|  2|  0|  7| 81|  0|  4| 20|  3| 15| 18|  2|  0| 10| 22|  2|  6|  0|  5|  0| 17|  0|
  * L |352| 20| 15| 67|580| 21|  9|  5|457|  0| 15|296| 22| 20|229| 13|  0|  4| 41| 54| 89| 21|  7|  0|225|  1|
  * M |302| 61|  7|  1|268|  4|  0|  1|213|  0|  0|  5| 56| 10|160|103|  0|  2| 16|  1| 57|  0|  1|  0| 21|  0|
  * N |277| 14|165|253|554| 37|426| 14|253|  7| 37| 18| 11|120|157|  9|  5| 12|174|365| 41| 22| 10|  1| 30|  7|
  * O | 49| 56| 97| 96| 35| 30| 93| 12| 49|  2| 32|224|170|641|109|111|  3|434|133|133|237| 70| 85| 17| 20|  8|
  * P |158|  2|  1|  1|224|  2|  0|110|114|  0|  1| 90|  2|  2|137| 70|  0|155| 30| 45| 51|  0|  1|  0| 15|  0|
  * Q |  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 82|  0|  0|  0|  0|  0|
  * R |481| 41| 64|117|586| 20| 53| 21|487|  2| 37| 65| 82| 81|343| 33|  2|105|133|156| 97| 27| 16|  0|115|  1|
  * S |138| 12|116|  5|284|  7|  4|194|224|  1| 32| 50| 72| 50|121|102| 12|  4|373|490|110|  2| 28|  0| 39|  0|
  * T |296|  7| 32|  1|571| 11|  2|236|598|  0|  1| 52| 12| 15|239|  4|  0|240| 67|137| 99|  0| 18|  0|112| 10|
  * U | 56| 54| 62| 53| 61| 16| 44|  2| 61|  1|  7|159| 98|215| 12| 50|  0|208|249|123|  0|  5|  0|  6|  2|  5|
  * V | 79|  0|  0|  0|250|  0|  0|  0|118|  0|  0|  1|  0|  0| 40|  0|  0|  2|  1|  0|  4|  1|  0|  0|  6|  0|
  * W |101|  4|  1|  4| 78|  2|  1| 27| 80|  0|  2| 12|  2| 23| 62|  1|  0| 14| 12|  1|  2|  0|  0|  0| 11|  0|
  * X | 10|  0|  9|  0| 13|  1|  0|  2| 23|  0|  0|  1|  0|  0|  5| 17|  0|  0|  0| 16|  4|  0|  0|  0|  3|  0|
  * Y | 34|  7| 21| 16| 32|  3|  5|  3| 13|  0|  1| 37| 24| 61| 14| 22|  0| 22| 39| 15|  4|  1|  5|  1|  0|  2|
  * Z | 37|  1|  0|  0| 78|  0|  0|  1| 21|  0|  0|  3|  1|  0| 10|  0|  0|  0|  0|  0|  2|  0|  0|  0|  4| 10|
  * ------------------------------------------------------------------------------------------------------------
  *  texts / all => NST_TEXTS_ALL
  *   | A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P | Q | R | S | T | U | V | W | X | Y | Z |
  * A | 11|120|115| 73| 12| 48| 79| 58| 43|  3| 39|268|101|386|  3| 54|  1|271|132|245|207| 22| 11|  7| 20| 14|
  * B | 88| 13|  1|  2|259|  3| 11|  5| 68|  1|  2| 68|  1|  2| 44|  1|  0| 66| 24|  9| 42|  1|  3|  0|  5| 10|
  * C | 65|  0|  6|  1| 49|  1|  1|673| 32|  0|108| 19|  1|  1| 70|  0|  1| 22|  2| 24| 19|  0|  0|  0|  8|  1|
  * D | 72|  6|  1| 12|263|  4|  8|  6|127|  1|  1| 20|  5|  9| 48|  1|  1| 48| 23| 10| 52|  3|  6|  0|  8|  2|
  * E | 65| 75| 84|148| 47| 55| 94| 68|368|  2| 47|302| 89|881| 24| 42|  4|999|298|160| 57| 25| 37| 30| 17| 21|
  * F | 63|  3|  1|  1|121| 41|  9|  2| 60|  1|  2| 55|  1|  2| 47|  1|  0| 59| 12| 54| 28|  1|  2|  0|  2| 10|
  * G | 61|  3|  1|  2|421|  2| 17| 17| 61|  1| 18| 43|  5| 17| 27|  1|  0| 80| 86| 18| 36|  1|  2|  0|  6|  4|
  * H |173|  7|  1|  2|316|  6|  9|  3|147|  1| 12| 72| 27| 48| 89|  2|  1| 79| 33| 93| 38|  1| 29|  0| 20| 10|
  * I | 83| 35|199| 67|310| 49|167|  7|  1|  2| 53|137| 68|499|139| 30|  2| 61|312|260| 18| 45|  2|  4|  2| 36|
  * J | 22|  0|  0|  1| 14|  0|  0|  0|  2|  0|  1|  1|  0|  0| 14|  0|  0|  1|  1|  0| 14|  0|  0|  0|  0|  0|
  * K | 95|  3|  1|  1|150|  2|  4|  4| 40|  1|  4| 40|  2| 15| 83|  1|  1| 48| 17| 59| 38|  1|  3|  0|  4|  5|
  * L |214| 31|  7| 46|314| 21| 15|  7|282|  1| 18|165| 17| 27|113| 10|  1|  5| 37| 81| 63| 10|  6|  0| 52| 14|
  * M |166| 24|  2|  3|192|  4|  7|  4|124|  1|  3|  7| 69|  4| 73| 53|  0|  2| 17| 10| 46|  1|  2|  0|  6|  6|
  * N |183| 35| 41|231|255| 43|328| 28|151|  3| 66| 24| 12| 73| 69| 12|  2| 16|174|224| 37| 16| 18|  1|  8| 71|
  * O | 13| 32| 47| 40| 10| 20| 60| 19| 14|  2| 20|129| 82|280| 26| 58|  1|242| 98| 71| 56| 19| 21|  8|  5| 11|
  * P | 77|  1|  1|  1|110| 38|  1| 55| 57|  0|  1| 43|  1|  1| 65| 41|  0| 94| 14| 22| 27|  1|  1|  0|  5|  1|
  * Q |  1|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0|  0| 31|  0|  0|  0|  0|  0|
  * R |315| 66| 41| 64|406| 43| 75| 28|279|  2| 62| 45| 70| 74|167| 22|  1| 51|117|156|112| 19| 26|  1| 26| 46|
  * S |111| 29|325|  8|251| 21| 31| 51|150|  1| 36| 28| 42| 15| 59| 95|  3| 13|226|413| 51| 12| 19|  0| 18| 22|
  * T |176| 15|  9|  2|457| 11| 18| 93|333|  1|  7| 35| 10|  9|112|  5|  1|169| 93|105| 77|  4| 13|  0| 27| 61|
  * U | 22| 40| 48| 30| 43| 72| 39| 14| 21|  1| 22| 80| 96|262|  5| 29|  1|144|219| 81|  1|  4|  7|  2|  1| 14|
  * V | 31|  1|  0|  1|141|  1|  1|  0| 51|  0|  1|  1|  1|  0| 51|  1|  0|  1|  1|  1|  2|  1|  1|  0|  1|  1|
  * W | 68|  1|  1|  1|107|  1|  1|  5| 66|  1|  1|  3|  1|  4| 25|  1|  0|  3|  3|  1| 10|  0|  1|  0|  2|  0|
  * X |  4|  1|  1|  0|  6|  1|  1|  1| 11|  0|  1|  1|  1|  1|  2|  6|  0|  0|  1|  7|  1|  1|  1|  0|  1|  1|
  * Y | 11|  3|  6|  6| 10|  1|  2|  1|  3|  1|  2| 13| 10| 17|  4| 12|  0|  9| 16|  6|  1|  1|  1|  1|  1|  1|
  * Z | 21|  4|  1|  1|102|  1|  2|  1| 57|  1|  2|  3|  1|  1| 14|  1|  0|  1|  3| 10|154|  1| 15|  0|  4|  4|
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
			/* R : */ 0x62104151, /* (B|M| ) - A, E, G, I, O, U, Z */
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
			/* B : */ 0x40104101, /* ( |M| ) - A, I, O, U */
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
			/* C : */ 0xe0080595, /* (B|M|E) - A, C, E, H, I, K, T */
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
			/* R : */ 0xe35efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W, Y, Z */
			/* S : */ 0xe15edd97, /* (B|M|E) - A, B, C, E, H, I, K, L, M, O, P, R, S, T, U, W, Y */
			/* T : */ 0xe04e43f1, /* (B|M|E) - A, E, F, G, H, I, J, O, R, S, T, W */
			/* U : */ 0xe20e3def, /* (B|M|E) - A, B, C, D, F, G, H, I, K, L, M, N, R, S, T, Z */
			/* V : */ 0xe0000111, /* (B|M|E) - A, E, I */
			/* W : */ 0xc0000111, /* ( |M|E) - A, E, I */
			/* X : */ 0x40000001, /* ( |M| ) - A */
			/* Y : */ 0xc000001d, /* ( |M|E) - A, C, D, E */
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
			/* R : */ 0x60104113, /* (B|M| ) - A, B, E, I, O, U */
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
			/* A : */ 0xe37ebfef, /* (B|M|E) - A, B, C, D, F, G, H, I, J, K, L, M, N, P, R, S, T, U, V, W, Y, Z */
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
			/* L : */ 0xe0100131, /* (B|M|E) - A, E, F, I, U */
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
			/* P : */ 0x60029891, /* (B|M| ) - A, E, H, L, M, P and R */
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
			/* A : */ 0xe11e39fa, /* (B|M|E) - B, D, E, F, G, H, I, L, M, N, R, S, T, U and Y */
			/* B : */ 0x41000000, /* ( |M| ) - Y,  */
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
			/* S : */ 0xe04cd99f, /* (B|M|E) - A, B, C, D, E, H, I, L, M, O, P, S, T and W */
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
			/* W : */ 0x40000010, /* ( |M| ) - E,  */
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
			/* T : */ 0xe1525917, /* (B|M|E) - A, B, C, E, I, L, M, O, R, U, W and Y */
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
			/* C : */ 0x400201d0, /* ( |M| ) - E, G, H, I and R */
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
			/* B : */ 0x41000000, /* ( |M| ) - Y,  */
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
			/* F : */ 0xe00e6d35, /* (B|M|E) - A, C, E, F, I, K, L, N, O, R, S and T */
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
			/* R : */ 0x62104151, /* (B|M| ) - A, E, G, I, O, U and Z */
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
			/* B : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
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
			/* W : */ 0x40000010, /* ( |M| ) - E,  */
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
			/* R : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
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
			/* R : */ 0x60104113, /* (B|M| ) - A, B, E, I, O and U */
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
			/* A : */ 0xe37fbfff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, P, Q, R, S, T, U, V, W, Y and Z */
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
			/* E : */ 0xe10e795b, /* (B|M|E) - A, B, D, E, G, I, L, M, N, O, R, S, T and Y */
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
			/* C : */ 0x400201d1, /* ( |M| ) - A, E, G, H, I and R */
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
			/* L : */ 0xe0100131, /* (B|M|E) - A, E, F, I and U */
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
			/* G : */ 0x40020011, /* ( |M| ) - A, E, R */
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
			/* U : */ 0x600eb8eb, /* (B|M| ) - A, B, D, F, G, H, L, M, N, P, R, S, T */
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
			/* U : */ 0xe20ebdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, Z */
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
			/* S : */ 0xf57eddff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U, V, W, Y, ä, ü */
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
			/* E : */ 0xe00e2906, /* (B|M|E) - B, C, I, L, N, R, S, T */
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
			/* Z : */ 0xd6504113, /* ( |M|E) - A, B, E, I, O, U, W, Z, ä, ü */
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
			/* L : */ 0x60004111, /* (B|M| ) - A, E, I, O */
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
			/* N : */ 0xc0182954, /* ( |M|E) - C, E, G, I, L, N, T, U */
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
			/* A : */ 0x60020000, /* (B|M| ) - R,  */
			/* B : */ 0xe116491b, /* (B|M|E) - A, B, D, E, I, L, O, R, S, U and Y */
			/* C : */ 0xe11f4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, Q, R, S, T, U and Y */
			/* D : */ 0xe17e7bff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, L, M, N, O, R, S, T, U, V, W and Y */
			/* E : */ 0xe01e784d, /* (B|M|E) - A, C, D, G, L, M, N, O, R, S, T and U */
			/* F : */ 0xe00a4131, /* (B|M|E) - A, E, F, I, O, R and T */
			/* G : */ 0xe01679d9, /* (B|M|E) - A, D, E, G, H, I, L, M, N, O, R, S and U */
			/* H : */ 0xe0145911, /* (B|M|E) - A, E, I, L, M, O, S and U */
			/* I : */ 0xe00e3c4d, /* (B|M|E) - A, C, D, G, K, L, M, N, R, S and T */
			/* J : */ 0x40004001, /* ( |M| ) - A and O */
			/* K : */ 0xe1044911, /* (B|M|E) - A, E, I, L, O, S and Y */
			/* L : */ 0xe17efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* M : */ 0xe116f913, /* (B|M|E) - A, B, E, I, L, M, N, O, P, R, S, U and Y */
			/* N : */ 0xe37d6fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, N, O, Q, S, T, U, V, W, Y and Z */
			/* O : */ 0xc0000800, /* ( |M|E) - L,  */
			/* P : */ 0xe11ec991, /* (B|M|E) - A, E, H, I, L, O, P, R, S, T, U and Y */
			/* Q : */ 0x60100000, /* (B|M| ) - U,  */
			/* R : */ 0xe17fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W and Y */
			/* S : */ 0xe11ddd95, /* (B|M|E) - A, C, E, H, I, K, L, M, O, P, Q, S, T, U and Y */
			/* T : */ 0xe15e79b5, /* (B|M|E) - A, C, E, F, H, I, L, M, N, O, R, S, T, U, W and Y */
			/* U : */ 0xe08eb84e, /* (B|M|E) - B, C, D, G, L, M, N, P, R, S, T and X */
			/* V : */ 0xe1024111, /* (B|M|E) - A, E, I, O, R and Y */
			/* W : */ 0xe1046d3b, /* (B|M|E) - A, B, D, E, F, I, K, L, N, O, S and Y */
			/* X : */ 0xe0084111, /* (B|M|E) - A, E, I, O and T */
			/* Y : */ 0xe04e793f, /* (B|M|E) - A, B, C, D, E, F, I, L, M, N, O, R, S, T and W */
			/* Z : */ 0x62004111, /* (B|M| ) - A, E, I, O and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "B" */
			/* A : */ 0xe11ebdce, /* (B|M|E) - B, C, D, G, H, I, K, L, M, N, P, R, S, T, U and Y */
			/* B : */ 0x41004911, /* ( |M| ) - A, E, I, L, O and Y */
			/* C : */ 0x40004000, /* ( |M| ) - O,  */
			/* D : */ 0x40100100, /* ( |M| ) - I and U */
			/* E : */ 0xe16e7dfd, /* (B|M|E) - A, C, D, E, F, G, H, I, K, L, M, N, O, R, S, T, V, W and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe02ee85f, /* (B|M|E) - A, B, C, D, E, G, L, N, O, P, R, S, T and V */
			/* J : */ 0x40000010, /* ( |M| ) - E,  */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000100, /* ( |M| ) - I,  */
			/* N : */ 0x40000010, /* ( |M| ) - E,  */
			/* O : */ 0xe1de795f, /* (B|M|E) - A, B, C, D, E, G, I, L, M, N, O, R, S, T, U, W, X and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc0084114, /* ( |M|E) - C, E, I, O and T */
			/* T : */ 0x40020011, /* ( |M| ) - A, E and R */
			/* U : */ 0x600e3d6e, /* (B|M| ) - B, C, D, F, G, I, K, L, M, N, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00e0810, /* (B|M|E) - E, L, R, S and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "C" */
			/* A : */ 0xe13ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V and Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40124991, /* ( |M| ) - A, E, H, I, L, O, R and U */
			/* D : */ 0x40004000, /* ( |M| ) - O,  */
			/* E : */ 0xe14ef93f, /* (B|M|E) - A, B, C, D, E, F, I, L, M, N, O, P, R, S, T, W and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe15ef93b, /* (B|M|E) - A, B, D, E, F, I, L, M, N, O, P, R, S, T, U, W and Y */
			/* I : */ 0xe23ef83f, /* (B|M|E) - A, B, C, D, E, F, L, M, N, O, P, R, S, T, U, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc14ef9b7, /* ( |M|E) - A, B, C, E, F, H, I, L, M, N, O, P, R, S, T, W and Y */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x40000010, /* ( |M| ) - E,  */
			/* O : */ 0xe17ef9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0x80000000, /* ( | |E) - None are allowed */
			/* T : */ 0xc0166911, /* ( |M|E) - A, E, I, L, N, O, R, S and U */
			/* U : */ 0x600ef93a, /* (B|M| ) - B, D, E, F, I, L, M, N, O, P, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00aa807, /* (B|M|E) - A, B, C, L, N, P, R and T */
			/* Z : */ 0x20000000, /* (B| | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "D" */
			/* A : */ 0xe17eb9fe, /* (B|M|E) - B, C, D, E, F, G, H, I, L, M, N, P, R, S, T, U, V, W and Y */
			/* B : */ 0x40124011, /* ( |M| ) - A, E, O, R and U */
			/* C : */ 0x40000001, /* ( |M| ) - A,  */
			/* D : */ 0xc1024911, /* ( |M|E) - A, E, I, L, O, R and Y */
			/* E : */ 0xe1fefd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V, W, X and Y */
			/* F : */ 0x40104101, /* ( |M| ) - A, I, O and U */
			/* G : */ 0x41001111, /* ( |M| ) - A, E, I, M and Y */
			/* H : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* I : */ 0xe2befc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, V, X and Z */
			/* J : */ 0x40100000, /* ( |M| ) - U,  */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* M : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* N : */ 0x40000011, /* ( |M| ) - A and E */
			/* O : */ 0xe2fef95c, /* (B|M|E) - C, D, E, G, I, L, M, N, O, P, R, S, T, U, V, W, X and Z */
			/* P : */ 0x40000101, /* ( |M| ) - A and I */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc048d190, /* ( |M|E) - E, H, I, M, O, P, T and W */
			/* T : */ 0xc0000080, /* ( |M|E) - H,  */
			/* U : */ 0x600efc17, /* (B|M| ) - A, B, C, E, K, L, M, N, O, P, R, S and T */
			/* V : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* W : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0042911, /* (B|M|E) - A, E, I, L, N and S */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "E" */
			/* A : */ 0xe07ebcee, /* (B|M|E) - B, C, D, F, G, H, K, L, M, N, P, R, S, T, U, V and W */
			/* B : */ 0xe0124913, /* (B|M|E) - A, B, E, I, L, O, R and U */
			/* C : */ 0xe01a4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, R, T and U */
			/* D : */ 0xe156797b, /* (B|M|E) - A, B, D, E, F, G, I, L, M, N, O, R, S, U, W and Y */
			/* E : */ 0xc26ebd2f, /* ( |M|E) - A, B, C, D, F, I, K, L, M, N, P, R, S, T, V, W and Z */
			/* F : */ 0xe01a4931, /* (B|M|E) - A, E, F, I, L, O, R, T and U */
			/* G : */ 0xe0127951, /* (B|M|E) - A, E, G, I, L, M, N, O, R and U */
			/* H : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* I : */ 0xe22e386d, /* (B|M|E) - A, C, D, F, G, L, M, N, R, S, T, V and Z */
			/* J : */ 0x40104011, /* ( |M| ) - A, E, O and U */
			/* K : */ 0xc0042911, /* ( |M|E) - A, E, I, L, N and S */
			/* L : */ 0xe17ef9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* M : */ 0xe114f913, /* (B|M|E) - A, B, E, I, L, M, N, O, P, S, U and Y */
			/* N : */ 0xe37e7fff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe01eb86c, /* (B|M|E) - C, D, F, G, L, M, N, P, R, S, T and U */
			/* P : */ 0xe01ec991, /* (B|M|E) - A, E, H, I, L, O, P, R, S, T and U */
			/* Q : */ 0x60100000, /* (B|M| ) - U,  */
			/* R : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* S : */ 0xe15ddd9f, /* (B|M|E) - A, B, C, D, E, H, I, K, L, M, O, P, Q, S, T, U, W and Y */
			/* T : */ 0xe35e49b7, /* (B|M|E) - A, B, C, E, F, H, I, L, O, R, S, T, U, W, Y and Z */
			/* U : */ 0xe02eb85c, /* (B|M|E) - C, D, E, G, L, M, N, P, R, S, T and V */
			/* V : */ 0xe1004111, /* (B|M|E) - A, E, I, O and Y */
			/* W : */ 0xe0066191, /* (B|M|E) - A, E, H, I, N, O, R and S */
			/* X : */ 0xe018c195, /* (B|M|E) - A, C, E, H, I, O, P, T and U */
			/* Y : */ 0xe0043911, /* (B|M|E) - A, E, I, L, M, N and S */
			/* Z : */ 0xe2000110, /* (B|M|E) - E, I and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "F" */
			/* A : */ 0xe17e395e, /* (B|M|E) - B, C, D, E, G, I, L, M, N, R, S, T, U, V, W and Y */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe00e391d, /* (B|M|E) - A, C, D, E, I, L, M, N, R, S and T */
			/* F : */ 0xc1164911, /* ( |M|E) - A, E, I, L, O, R, S, U and Y */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe08e687f, /* (B|M|E) - A, B, C, D, E, F, G, L, N, O, R, S, T and X */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x40000010, /* ( |M| ) - E,  */
			/* O : */ 0x60966954, /* (B|M| ) - C, E, G, I, L, N, O, R, S, U and X */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0x80000000, /* ( | |E) - None are allowed */
			/* T : */ 0xc1044110, /* ( |M|E) - E, I, O, S and Y */
			/* U : */ 0x600e3844, /* (B|M| ) - C, G, L, M, N, R, S and T */
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
			/* B : */ 0x40004010, /* ( |M| ) - E and O */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x40004001, /* ( |M| ) - A and O */
			/* E : */ 0xe14e799b, /* (B|M|E) - A, B, D, E, H, I, L, M, N, O, R, S, T, W and Y */
			/* F : */ 0x40100000, /* ( |M| ) - U,  */
			/* G : */ 0x41024911, /* ( |M| ) - A, E, I, L, O, R and Y */
			/* H : */ 0xe00c4913, /* (B|M|E) - A, B, E, I, L, O, S and T */
			/* I : */ 0xe23e787f, /* (B|M|E) - A, B, C, D, E, F, G, L, M, N, O, R, S, T, U, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0xe0004111, /* (B|M|E) - A, E, I and O */
			/* O : */ 0xe03e795b, /* (B|M|E) - A, B, D, E, G, I, L, M, N, O, R, S, T, U and V */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* S : */ 0xc0080080, /* ( |M|E) - H and T */
			/* T : */ 0x40004080, /* ( |M| ) - H and O */
			/* U : */ 0x600e7911, /* (B|M| ) - A, E, I, L, M, N, O, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x60004011, /* (B|M| ) - A, E and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe002b000, /* (B|M|E) - M, N, P and R */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "H" */
			/* A : */ 0xe37ebd7e, /* (B|M|E) - B, C, D, E, F, G, I, K, L, M, N, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x40104001, /* ( |M| ) - A, O and U */
			/* C : */ 0x40000800, /* ( |M| ) - L,  */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe1fef97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, O, P, R, S, T, U, V, W, X and Y */
			/* F : */ 0x40104100, /* ( |M| ) - I, O and U */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe22efc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* M : */ 0xc0004111, /* ( |M|E) - A, E, I and O */
			/* N : */ 0xc0004111, /* ( |M|E) - A, E, I and O */
			/* O : */ 0xe07efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V and W */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x41104111, /* ( |M| ) - A, E, I, O, U and Y */
			/* S : */ 0xc0080000, /* ( |M|E) - T,  */
			/* T : */ 0xc10469b1, /* ( |M|E) - A, E, F, H, I, L, N, O, S and Y */
			/* U : */ 0xe00e387f, /* (B|M|E) - A, B, C, D, E, F, G, L, M, N, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00e9809, /* (B|M|E) - A, D, L, M, P, R, S and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "I" */
			/* A : */ 0xe00eb8de, /* (B|M|E) - B, C, D, E, G, H, L, M, N, P, R, S and T */
			/* B : */ 0xe1124913, /* (B|M|E) - A, B, E, I, L, O, R, U and Y */
			/* C : */ 0xe11e6d95, /* (B|M|E) - A, C, E, H, I, K, L, N, O, R, S, T, U and Y */
			/* D : */ 0xe15e6959, /* (B|M|E) - A, D, E, G, I, L, N, O, R, S, T, U, W and Y */
			/* E : */ 0xc07e386e, /* ( |M|E) - B, C, D, F, G, L, M, N, R, S, T, U, V and W */
			/* F : */ 0xc1184931, /* ( |M|E) - A, E, F, I, L, O, T, U and Y */
			/* G : */ 0xe01679d1, /* (B|M|E) - A, E, G, H, I, L, M, N, O, R, S and U */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x00000000, /* ( | | ) - None are allowed */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc0004511, /* ( |M|E) - A, E, I, K and O */
			/* L : */ 0xe17c7dbf, /* (B|M|E) - A, B, C, D, E, F, H, I, K, L, M, N, O, S, T, U, V, W and Y */
			/* M : */ 0xe014f913, /* (B|M|E) - A, B, E, I, L, M, N, O, P, S and U */
			/* N : */ 0xe17fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W and Y */
			/* O : */ 0xe01eb84c, /* (B|M|E) - C, D, G, L, M, N, P, R, S, T and U */
			/* P : */ 0xc01ed991, /* ( |M|E) - A, E, H, I, L, M, O, P, R, S, T and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe16efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, V, W and Y */
			/* S : */ 0xe11edfff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, O, P, R, S, T, U and Y */
			/* T : */ 0xe31e79b5, /* (B|M|E) - A, C, E, F, H, I, L, M, N, O, R, S, T, U, Y and Z */
			/* U : */ 0x40041000, /* ( |M| ) - M and S */
			/* V : */ 0x60204111, /* (B|M| ) - A, E, I, O and V */
			/* W : */ 0x40000101, /* ( |M| ) - A and I */
			/* X : */ 0xc0080110, /* ( |M|E) - E, I and T */
			/* Y : */ 0x40000001, /* ( |M| ) - A,  */
			/* Z : */ 0xe2004111, /* (B|M|E) - A, E, I, O and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "J" */
			/* A : */ 0xe32635ce, /* (B|M|E) - B, C, D, G, H, I, K, M, N, R, S, V, Y and Z */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x604e382d, /* (B|M| ) - A, C, D, F, L, M, N, R, S, T and W */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe0083840, /* (B|M|E) - G, L, M, N and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe1162d9f, /* (B|M|E) - A, B, C, D, E, H, I, K, L, N, R, S, U and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60063848, /* (B|M| ) - D, G, L, M, N, R and S */
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
			/* A : */ 0xe10eb9da, /* (B|M|E) - B, D, E, G, H, I, L, M, N, P, R, S, T and Y */
			/* B : */ 0x40004101, /* ( |M| ) - A, I and O */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe16e7919, /* (B|M|E) - A, D, E, I, L, M, N, O, R, S, T, V, W and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x60004011, /* (B|M| ) - A, E and O */
			/* I : */ 0xe00eb81d, /* (B|M|E) - A, C, D, E, L, M, N, P, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000001, /* ( |M| ) - A,  */
			/* N : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* O : */ 0xe0362800, /* (B|M|E) - L, N, R, S, U and V */
			/* P : */ 0x40004000, /* ( |M| ) - O,  */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61004101, /* (B|M| ) - A, I, O and Y */
			/* S : */ 0xc0084080, /* ( |M|E) - H, O and T */
			/* T : */ 0x40004000, /* ( |M| ) - O,  */
			/* U : */ 0x60028800, /* (B|M| ) - L, P and R */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0000800, /* (B|M|E) - L,  */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "L" */
			/* A : */ 0xe3febdde, /* (B|M|E) - B, C, D, E, G, H, I, K, L, M, N, P, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0x41124111, /* ( |M| ) - A, E, I, O, R, U and Y */
			/* C : */ 0x40104191, /* ( |M| ) - A, E, H, I, O and U */
			/* D : */ 0xc1566933, /* ( |M|E) - A, B, E, F, I, L, N, O, R, S, U, W and Y */
			/* E : */ 0xe1fefbff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, L, M, N, O, P, R, S, T, U, V, W, X and Y */
			/* F : */ 0xc0164111, /* ( |M|E) - A, E, I, O, R, S and U */
			/* G : */ 0x40020111, /* ( |M| ) - A, E, I and R */
			/* H : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* I : */ 0xe3bffc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, Q, R, S, T, U, V, X, Y and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc1040111, /* ( |M|E) - A, E, I, S and Y */
			/* L : */ 0xc154f1bb, /* ( |M|E) - A, B, D, E, F, H, I, M, N, O, P, S, U, W and Y */
			/* M : */ 0xc0044111, /* ( |M|E) - A, E, I, O and S */
			/* N : */ 0x40000010, /* ( |M| ) - E,  */
			/* O : */ 0xe17ff97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, O, P, Q, R, S, T, U, V, W and Y */
			/* P : */ 0x40004191, /* ( |M| ) - A, E, H, I and O */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x41004100, /* ( |M| ) - I, O and Y */
			/* S : */ 0xc1084193, /* ( |M|E) - A, B, E, H, I, O, T and Y */
			/* T : */ 0xc1164191, /* ( |M|E) - A, E, H, I, O, R, S, U and Y */
			/* U : */ 0xe0aefd5f, /* (B|M|E) - A, B, C, D, E, G, I, K, L, M, N, O, P, R, S, T, V and X */
			/* V : */ 0x41000111, /* ( |M| ) - A, E, I and Y */
			/* W : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe24eb1ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, M, N, P, R, S, T, W and Z */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "M" */
			/* A : */ 0xe39ebfde, /* (B|M|E) - B, C, D, E, G, H, I, J, K, L, M, N, P, R, S, T, U, X, Y and Z */
			/* B : */ 0xc0164911, /* ( |M|E) - A, E, I, L, O, R, S and U */
			/* C : */ 0x60000444, /* (B|M| ) - C, G and K */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe14e7d5f, /* (B|M|E) - A, B, C, D, E, G, I, K, L, M, N, O, R, S, T, W and Y */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe29e7c5d, /* (B|M|E) - A, C, D, E, G, K, L, M, N, O, R, S, T, U, X and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x41000110, /* ( |M| ) - E, I and Y */
			/* M : */ 0xc1104111, /* ( |M|E) - A, E, I, O, U and Y */
			/* N : */ 0xc0004111, /* ( |M|E) - A, E, I and O */
			/* O : */ 0xe27efddf, /* (B|M|E) - A, B, C, D, E, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Z */
			/* P : */ 0xc11e4991, /* ( |M|E) - A, E, H, I, L, O, R, S, T, U and Y */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40004000, /* ( |M| ) - O,  */
			/* S : */ 0xc0084090, /* ( |M|E) - E, H, O and T */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x600e387c, /* (B|M| ) - C, D, E, F, G, L, M, N, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00e6804, /* (B|M|E) - C, L, N, O, R, S and T */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "N" */
			/* A : */ 0xe17ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W and Y */
			/* B : */ 0x40124811, /* ( |M| ) - A, E, L, O, R and U */
			/* C : */ 0xc11a4d91, /* ( |M|E) - A, E, H, I, K, L, O, R, T, U and Y */
			/* D : */ 0xc15ef9b7, /* ( |M|E) - A, B, C, E, F, H, I, L, M, N, O, P, R, S, T, U, W and Y */
			/* E : */ 0xe3fef9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* G : */ 0xc15e79bb, /* ( |M|E) - A, B, D, E, F, H, I, L, M, N, O, R, S, T, U, W and Y */
			/* H : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* I : */ 0xe2bffc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, Q, R, S, T, U, V, X and Z */
			/* J : */ 0x40104011, /* ( |M| ) - A, E, O and U */
			/* K : */ 0xc1042911, /* ( |M|E) - A, E, I, L, N, S and Y */
			/* L : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* M : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* N : */ 0xc1104111, /* ( |M|E) - A, E, I, O, U and Y */
			/* O : */ 0xe0fef97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, O, P, R, S, T, U, V, W and X */
			/* P : */ 0x40024911, /* ( |M| ) - A, E, I, L, O and R */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* S : */ 0xc178ddb7, /* ( |M|E) - A, B, C, E, F, H, I, K, L, M, O, P, T, U, V, W and Y */
			/* T : */ 0xc35679b1, /* ( |M|E) - A, E, F, H, I, L, M, N, O, R, S, U, W, Y and Z */
			/* U : */ 0xe00e797f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, O, R, S and T */
			/* V : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* W : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* X : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Y : */ 0xe0041801, /* (B|M|E) - A, L, M and S */
			/* Z : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "O" */
			/* A : */ 0xe00e3c0c, /* (B|M|E) - C, D, K, L, M, N, R, S and T */
			/* B : */ 0xe11e4b13, /* (B|M|E) - A, B, E, I, J, L, O, R, S, T, U and Y */
			/* C : */ 0xe11a4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, R, T, U and Y */
			/* D : */ 0xe15659dd, /* (B|M|E) - A, C, D, E, G, H, I, L, M, O, R, S, U, W and Y */
			/* E : */ 0xe02e790f, /* (B|M|E) - A, B, C, D, I, L, M, N, O, R, S, T and V */
			/* F : */ 0xe0084931, /* (B|M|E) - A, E, F, I, L, O and T */
			/* G : */ 0xe11679d1, /* (B|M|E) - A, E, G, H, I, L, M, N, O, R, S, U and Y */
			/* H : */ 0x40006111, /* ( |M| ) - A, E, I, N and O */
			/* I : */ 0xe00e280c, /* (B|M|E) - C, D, L, N, R, S and T */
			/* J : */ 0x40004010, /* ( |M| ) - E and O */
			/* K : */ 0xe0044911, /* (B|M|E) - A, E, I, L, O and S */
			/* L : */ 0xe13cdd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, O, P, S, T, U, V and Y */
			/* M : */ 0xe114f933, /* (B|M|E) - A, B, E, F, I, L, M, N, O, P, S, U and Y */
			/* N : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe22ebc68, /* (B|M|E) - D, F, G, K, L, M, N, P, R, S, T, V and Z */
			/* P : */ 0xe11ec991, /* (B|M|E) - A, E, H, I, L, O, P, R, S, T, U and Y */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe17efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* S : */ 0xe15cd997, /* (B|M|E) - A, B, C, E, H, I, L, M, O, P, S, T, U, W and Y */
			/* T : */ 0xe11ec997, /* (B|M|E) - A, B, C, E, H, I, L, O, P, R, S, T, U and Y */
			/* U : */ 0xe02ea95f, /* (B|M|E) - A, B, C, D, E, G, I, L, N, P, R, S, T and V */
			/* V : */ 0xe0004111, /* (B|M|E) - A, E, I and O */
			/* W : */ 0xe10cb9bb, /* (B|M|E) - A, B, D, E, F, H, I, L, M, N, P, S, T and Y */
			/* X : */ 0xe1000111, /* (B|M|E) - A, E, I and Y */
			/* Y : */ 0xc004481d, /* ( |M|E) - A, C, D, E, L, O and S */
			/* Z : */ 0x41004011, /* ( |M| ) - A, E, O and Y */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "P" */
			/* A : */ 0xe17eb9de, /* (B|M|E) - B, C, D, E, G, H, I, L, M, N, P, R, S, T, U, V, W and Y */
			/* B : */ 0x40004000, /* ( |M| ) - O,  */
			/* C : */ 0x40004000, /* ( |M| ) - O,  */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe14ee95d, /* (B|M|E) - A, C, D, E, G, I, L, N, O, P, R, S, T, W and Y */
			/* F : */ 0x40100000, /* ( |M| ) - U,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe11e4911, /* (B|M|E) - A, E, I, L, O, R, S, T, U and Y */
			/* I : */ 0xe00efc5d, /* (B|M|E) - A, C, D, E, G, K, L, M, N, O, P, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0x40004010, /* ( |M| ) - E and O */
			/* O : */ 0xe05efd5c, /* (B|M|E) - C, D, E, G, I, K, L, M, N, O, P, R, S, T, U and W */
			/* P : */ 0xc1124911, /* ( |M|E) - A, E, I, L, O, R, U and Y */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xe1184191, /* (B|M|E) - A, E, H, I, O, T, U and Y */
			/* T : */ 0xe0104111, /* (B|M|E) - A, E, I, O and U */
			/* U : */ 0x600eb86e, /* (B|M| ) - B, C, D, F, G, L, M, N, P, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0020000, /* (B|M|E) - R,  */
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
			/* A : */ 0xe3febffe, /* (B|M|E) - B, C, D, E, F, G, H, I, J, K, L, M, N, P, R, S, T, U, V, W, X, Y and Z */
			/* B : */ 0xc1164911, /* ( |M|E) - A, E, I, L, O, R, S, U and Y */
			/* C : */ 0x411a4991, /* ( |M| ) - A, E, H, I, L, O, R, T, U and Y */
			/* D : */ 0xc1566993, /* ( |M|E) - A, B, E, H, I, L, N, O, R, S, U, W and Y */
			/* E : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0xc0104911, /* ( |M|E) - A, E, I, L, O and U */
			/* G : */ 0xc1124991, /* ( |M|E) - A, E, H, I, L, O, R, U and Y */
			/* H : */ 0x61084111, /* (B|M| ) - A, E, I, O, T and Y */
			/* I : */ 0xe2bdfc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, Q, S, T, U, V, X and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc1040991, /* ( |M|E) - A, E, H, I, L, S and Y */
			/* L : */ 0xc1044119, /* ( |M|E) - A, D, E, I, O, S and Y */
			/* M : */ 0xc1144911, /* ( |M|E) - A, E, I, L, O, S, U and Y */
			/* N : */ 0xc11c59b3, /* ( |M|E) - A, B, E, F, H, I, L, M, O, S, T, U and Y */
			/* O : */ 0xe3feffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0xc01e4991, /* ( |M|E) - A, E, H, I, L, O, R, S, T and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xc1104191, /* ( |M|E) - A, E, H, I, O, U and Y */
			/* S : */ 0xc018d595, /* ( |M|E) - A, C, E, H, I, K, M, O, P, T and U */
			/* T : */ 0xc35679b3, /* ( |M|E) - A, B, E, F, H, I, L, M, N, O, R, S, U, W, Y and Z */
			/* U : */ 0xe00cb97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, P, S and T */
			/* V : */ 0xc1004111, /* ( |M|E) - A, E, I, O and Y */
			/* W : */ 0x41004111, /* ( |M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe04cf91f, /* (B|M|E) - A, B, C, D, E, I, L, M, N, O, P, S, T and W */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "S" */
			/* A : */ 0xe1febd7e, /* (B|M|E) - B, C, D, E, F, G, I, K, L, M, N, P, R, S, T, U, V, W, X and Y */
			/* B : */ 0x41124011, /* ( |M| ) - A, E, O, R, U and Y */
			/* C : */ 0xe0124991, /* (B|M|E) - A, E, H, I, L, O, R and U */
			/* D : */ 0x40000011, /* ( |M| ) - A and E */
			/* E : */ 0xe1fff9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, Q, R, S, T, U, V, W, X and Y */
			/* F : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* G : */ 0x40120000, /* ( |M| ) - R and U */
			/* H : */ 0xe15a7917, /* (B|M|E) - A, B, C, E, I, L, M, N, O, R, T, U, W and Y */
			/* I : */ 0xe2bef87f, /* (B|M|E) - A, B, C, D, E, F, G, L, M, N, O, P, R, S, T, U, V, X and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xe1100111, /* (B|M|E) - A, E, I, U and Y */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0xe0104111, /* (B|M|E) - A, E, I, O and U */
			/* N : */ 0x60104111, /* (B|M| ) - A, E, I, O and U */
			/* O : */ 0xe05ef97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, O, P, R, S, T, U and W */
			/* P : */ 0xe0124991, /* (B|M|E) - A, E, H, I, L, O, R and U */
			/* Q : */ 0x60100000, /* (B|M| ) - U,  */
			/* R : */ 0x40000011, /* ( |M| ) - A and E */
			/* S : */ 0xc158f9b3, /* ( |M|E) - A, B, E, F, H, I, L, M, N, O, P, T, U, W and Y */
			/* T : */ 0xe156f9b7, /* (B|M|E) - A, B, C, E, F, H, I, L, M, N, O, P, R, S, U, W and Y */
			/* U : */ 0xe20eb97f, /* (B|M|E) - A, B, C, D, E, F, G, I, L, M, N, P, R, S, T and Z */
			/* V : */ 0x40000110, /* ( |M| ) - E and I */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0063806, /* (B|M|E) - B, C, L, M, N, R and S */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "T" */
			/* A : */ 0xe1febdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, X and Y */
			/* B : */ 0x40024101, /* ( |M| ) - A, I, O and R */
			/* C : */ 0x40004881, /* ( |M| ) - A, H, L and O */
			/* D : */ 0x40004000, /* ( |M| ) - O,  */
			/* E : */ 0xe0fef9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, R, S, T, U, V, W and X */
			/* F : */ 0x40104101, /* ( |M| ) - A, I, O and U */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe1567937, /* (B|M|E) - A, B, C, E, F, I, L, M, N, O, R, S, U, W and Y */
			/* I : */ 0xe23ff87f, /* (B|M|E) - A, B, C, D, E, F, G, L, M, N, O, P, Q, R, S, T, U, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000100, /* ( |M| ) - I,  */
			/* L : */ 0xc1004111, /* ( |M|E) - A, E, I, O and Y */
			/* M : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* N : */ 0x40000111, /* ( |M| ) - A, E and I */
			/* O : */ 0xe1fefd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V, W, X and Y */
			/* P : */ 0x40024001, /* ( |M| ) - A, O and R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xe158d595, /* (B|M|E) - A, C, E, H, I, K, M, O, P, T, U, W and Y */
			/* T : */ 0xc1166991, /* ( |M|E) - A, E, H, I, L, N, O, R, S, U and Y */
			/* U : */ 0xe00ef93f, /* (B|M|E) - A, B, C, D, E, F, I, L, M, N, O, P, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe006b800, /* (B|M|E) - L, M, N, P, R and S */
			/* Z : */ 0xc0000011, /* ( |M|E) - A and E */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "U" */
			/* A : */ 0xc12e294e, /* ( |M|E) - B, C, D, G, I, L, N, R, S, T, V and Y */
			/* B : */ 0xc01edb1f, /* ( |M|E) - A, B, C, D, E, I, J, L, M, O, P, R, S, T and U */
			/* C : */ 0x401a4d95, /* ( |M| ) - A, C, E, H, I, K, L, O, R, T and U */
			/* D : */ 0xe1064959, /* (B|M|E) - A, D, E, G, I, L, O, R, S and Y */
			/* E : */ 0xc21e293b, /* ( |M|E) - A, B, D, E, F, I, L, N, R, S, T, U and Z */
			/* F : */ 0x40000020, /* ( |M| ) - F,  */
			/* G : */ 0xe01049d1, /* (B|M|E) - A, E, G, H, I, L, O and U */
			/* H : */ 0x40000001, /* ( |M| ) - A,  */
			/* I : */ 0xc22ea81d, /* ( |M|E) - A, C, D, E, L, N, P, R, S, T, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc0000111, /* ( |M|E) - A, E and I */
			/* L : */ 0xe13efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V and Y */
			/* M : */ 0xe014f133, /* (B|M|E) - A, B, E, F, I, M, N, O, P, S and U */
			/* N : */ 0xe05efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U and W */
			/* O : */ 0x401a0000, /* ( |M| ) - R, T and U */
			/* P : */ 0xe01ec995, /* (B|M|E) - A, C, E, H, I, L, O, P, R, S, T and U */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xe13efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V and Y */
			/* S : */ 0xe11ced97, /* (B|M|E) - A, B, C, E, H, I, K, L, N, O, P, S, T, U and Y */
			/* T : */ 0xe25ef9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, R, S, T, U, W and Z */
			/* U : */ 0x00000000, /* ( | | ) - None are allowed */
			/* V : */ 0x40020110, /* ( |M| ) - E, I and R */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0xc0100110, /* ( |M|E) - E, I and U */
			/* Y : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Z : */ 0xc2000000, /* ( |M|E) - Z,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "V" */
			/* A : */ 0xe01ea94e, /* (B|M|E) - B, C, D, G, I, L, N, P, R, S, T and U */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe18e39dd, /* (B|M|E) - A, C, D, E, G, H, I, L, M, N, R, S, T, X and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0x602e6c5f, /* (B|M| ) - A, B, C, D, E, G, K, L, N, O, R, S, T and V */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe15a2d04, /* (B|M|E) - C, I, K, L, N, R, T, U, W and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x40000010, /* ( |M| ) - E,  */
			/* S : */ 0x40000400, /* ( |M| ) - K,  */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x60000800, /* (B|M| ) - L,  */
			/* V : */ 0x41000000, /* ( |M| ) - Y,  */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xc0002000, /* ( |M|E) - N,  */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "W" */
			/* A : */ 0xe1be3d4e, /* (B|M|E) - B, C, D, G, I, K, L, M, N, R, S, T, U, V, X and Y */
			/* B : */ 0x40004001, /* ( |M| ) - A and O */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x40000010, /* ( |M| ) - E,  */
			/* E : */ 0xe00e291b, /* (B|M|E) - A, B, D, E, I, L, N, R, S and T */
			/* F : */ 0x40100000, /* ( |M| ) - U,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* I : */ 0x600e387c, /* (B|M| ) - C, D, E, F, G, L, M, N, R, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0xc1040110, /* ( |M|E) - E, I, S and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0xc0040110, /* ( |M|E) - E, I and S */
			/* O : */ 0x60127808, /* (B|M| ) - D, L, M, N, O, R and U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* S : */ 0xc0000090, /* ( |M|E) - E and H */
			/* T : */ 0x40000080, /* ( |M| ) - H,  */
			/* U : */ 0x60000800, /* (B|M| ) - L,  */
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
			/* A : */ 0x60083844, /* (B|M| ) - C, G, L, M, N and T */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x40120910, /* ( |M| ) - E, I, L, R and U */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0x6006300c, /* (B|M| ) - C, D, M, N, R and S */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40000101, /* ( |M| ) - A and I */
			/* I : */ 0xc00c781d, /* ( |M|E) - A, C, D, E, L, M, N, O, S and T */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x00000000, /* ( | | ) - None are allowed */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0x400a2000, /* ( |M| ) - N, R and T */
			/* P : */ 0x40024911, /* ( |M| ) - A, E, I, L, O and R */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x40124110, /* ( |M| ) - E, I, O, R and U */
			/* U : */ 0x40020001, /* ( |M| ) - A and R */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Z : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Y" */
			/* A : */ 0xe00e3c86, /* (B|M|E) - B, C, H, K, L, M, N, R, S and T */
			/* B : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* C : */ 0x40004991, /* ( |M| ) - A, E, H, I, L and O */
			/* D : */ 0xc0024111, /* ( |M|E) - A, E, I, O and R */
			/* E : */ 0xe00e2809, /* (B|M|E) - A, D, L, N, R, S and T */
			/* F : */ 0x40100000, /* ( |M| ) - U,  */
			/* G : */ 0x40004110, /* ( |M| ) - E, I and O */
			/* H : */ 0x40004010, /* ( |M| ) - E and O */
			/* I : */ 0x60042000, /* (B|M| ) - N and S */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x80000000, /* ( | |E) - None are allowed */
			/* L : */ 0xc1304911, /* ( |M|E) - A, E, I, L, O, U, V and Y */
			/* M : */ 0xc000f113, /* ( |M|E) - A, B, E, I, M, N, O and P */
			/* N : */ 0xc008615d, /* ( |M|E) - A, C, D, E, G, I, N, O and T */
			/* O : */ 0x60162c40, /* (B|M| ) - G, K, L, N, R, S and U */
			/* P : */ 0x400c6191, /* ( |M| ) - A, E, H, I, N, O, S and T */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0xc0124111, /* ( |M|E) - A, E, I, O, R and U */
			/* S : */ 0xc00c4191, /* ( |M|E) - A, E, H, I, O, S and T */
			/* T : */ 0x40004191, /* ( |M| ) - A, E, H, I and O */
			/* U : */ 0x20000000, /* (B| | ) - None are allowed */
			/* V : */ 0x40000010, /* ( |M| ) - E,  */
			/* W : */ 0x40004001, /* ( |M| ) - A and O */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Z : */ 0x40000010, /* ( |M| ) - E,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "Z" */
			/* A : */ 0xe00a3806, /* (B|M|E) - B, C, L, M, N, R and T */
			/* B : */ 0x00000000, /* ( | | ) - None are allowed */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe00e280b, /* (B|M|E) - A, B, D, L, N, R, S and T */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x00000000, /* ( | | ) - None are allowed */
			/* I : */ 0xe0006851, /* (B|M|E) - A, E, G, L, N and O */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x00000000, /* ( | | ) - None are allowed */
			/* L : */ 0x40000010, /* ( |M| ) - E,  */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x00000000, /* ( | | ) - None are allowed */
			/* O : */ 0xe0026100, /* (B|M|E) - I, N, O and R */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x00000000, /* ( | | ) - None are allowed */
			/* S : */ 0x00000000, /* ( | | ) - None are allowed */
			/* T : */ 0x00000000, /* ( | | ) - None are allowed */
			/* U : */ 0x20000000, /* (B| | ) - None are allowed */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0x80000000, /* ( | |E) - None are allowed */
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
			/* B : */ 0xe35efdfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W, Y and Z */
			/* C : */ 0xe11f4d95, /* (B|M|E) - A, C, E, H, I, K, L, O, Q, R, S, T, U and Y */
			/* D : */ 0xe37efbff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* E : */ 0xe01e784d, /* (B|M|E) - A, C, D, G, L, M, N, O, R, S, T and U */
			/* F : */ 0xe00e4971, /* (B|M|E) - A, E, F, G, I, L, O, R, S and T */
			/* G : */ 0xe25ef9fb, /* (B|M|E) - A, B, D, E, F, G, H, I, L, M, N, O, P, R, S, T, U, W and Z */
			/* H : */ 0xe03e7d1b, /* (B|M|E) - A, B, D, E, I, K, L, M, N, O, R, S, T, U and V */
			/* I : */ 0xe26efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, V, W and Z */
			/* J : */ 0xc0004119, /* ( |M|E) - A, D, E, I and O */
			/* K : */ 0xe31e4db1, /* (B|M|E) - A, E, F, H, I, K, L, O, R, S, T, U, Y and Z */
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
			/* W : */ 0xe1066d3b, /* (B|M|E) - A, B, D, E, F, I, K, L, N, O, R, S and Y */
			/* X : */ 0xe0484111, /* (B|M|E) - A, E, I, O, T and W */
			/* Y : */ 0xe05e79bf, /* (B|M|E) - A, B, C, D, E, F, H, I, L, M, N, O, R, S, T, U and W */
			/* Z : */ 0xe3585911, /* (B|M|E) - A, E, I, L, M, O, T, U, W, Y and Z */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "B" */
			/* A : */ 0xe31ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, Y and Z */
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
			/* T : */ 0xc0064191, /* ( |M|E) - A, E, H, I, O, R and S */
			/* U : */ 0xe30e3dfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, R, S, T, Y and Z */
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
			/* E : */ 0xe16efd3f, /* (B|M|E) - A, B, C, D, E, F, I, K, L, M, N, O, P, R, S, T, V, W and Y */
			/* F : */ 0x00000000, /* ( | | ) - None are allowed */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* I : */ 0xe23ef8ff, /* (B|M|E) - A, B, C, D, E, F, G, H, L, M, N, O, P, R, S, T, U, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0xc37efdff, /* ( |M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x00000000, /* ( | | ) - None are allowed */
			/* N : */ 0x40000010, /* ( |M| ) - E,  */
			/* O : */ 0xe17ef9ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0x80000000, /* ( | |E) - None are allowed */
			/* T : */ 0xc0166911, /* ( |M|E) - A, E, I, L, N, O, R, S and U */
			/* U : */ 0x600ef93b, /* (B|M| ) - A, B, D, E, F, I, L, M, N, O, P, R, S and T */
			/* V : */ 0x00000000, /* ( | | ) - None are allowed */
			/* W : */ 0x00000000, /* ( | | ) - None are allowed */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00ea807, /* (B|M|E) - A, B, C, L, N, P, R, S and T */
			/* Z : */ 0x60000010, /* (B|M| ) - E,  */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "D" */
			/* A : */ 0xe37ebdfe, /* (B|M|E) - B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x41124911, /* ( |M| ) - A, E, I, L, O, R, U and Y */
			/* C : */ 0x40004081, /* ( |M| ) - A, H and O */
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
			/* S : */ 0xc07cddf7, /* ( |M|E) - A, B, C, E, F, G, H, I, K, L, M, O, P, S, T, U, V and W */
			/* T : */ 0xc0160191, /* ( |M|E) - A, E, H, I, R, S and U */
			/* U : */ 0xe20efd3f, /* (B|M|E) - A, B, C, D, E, F, I, K, L, M, N, O, P, R, S, T and Z */
			/* V : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* W : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe0043913, /* (B|M|E) - A, B, E, I, L, M, N and S */
			/* Z : */ 0x40500111, /* ( |M| ) - A, E, I, U and W */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "E" */
			/* A : */ 0xe07ebcee, /* (B|M|E) - B, C, D, F, G, H, K, L, M, N, P, R, S, T, U, V and W */
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
			/* U : */ 0xe2febdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, P, R, S, T, U, V, W, X and Z */
			/* V : */ 0xe1164911, /* (B|M|E) - A, E, I, L, O, R, S, U and Y */
			/* W : */ 0xe01e6991, /* (B|M|E) - A, E, H, I, L, N, O, R, S, T and U */
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
			/* I : */ 0xe28e6c7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, N, O, R, S, T, X and Z */
			/* J : */ 0x60004000, /* (B|M| ) - O,  */
			/* K : */ 0x40024801, /* ( |M| ) - A, L, O and R */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* N : */ 0x40100111, /* ( |M| ) - A, E, I and U */
			/* O : */ 0xe0deedd5, /* (B|M|E) - A, C, E, G, H, I, K, L, N, O, P, R, S, T, U, W and X */
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
			/* H : */ 0xe01c4913, /* (B|M|E) - A, B, E, I, L, O, S, T and U */
			/* I : */ 0xe23efc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, V and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40024011, /* ( |M| ) - A, E, O and R */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* N : */ 0xe0104111, /* (B|M|E) - A, E, I, O and U */
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
			/* Y : */ 0xe002b000, /* (B|M|E) - M, N, P and R */
			/* Z : */ 0x40100010, /* ( |M| ) - E and U */
			/* ä : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ö : */ 0x00000000, /* ( | | ) - None are allowed */
			/* ü : */ 0x00000000  /* ( | | ) - None are allowed */
		},
		{ /* Rules following letter "H" */
			/* A : */ 0xe37effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* B : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* C : */ 0x40000880, /* ( |M| ) - H and L */
			/* D : */ 0x40024111, /* ( |M| ) - A, E, I, O and R */
			/* E : */ 0xe3fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* G : */ 0x40020911, /* ( |M| ) - A, E, I, L and R */
			/* H : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* I : */ 0xe23efc7f, /* (B|M|E) - A, B, C, D, E, F, G, K, L, M, N, O, P, R, S, T, U, V and Z */
			/* J : */ 0x40000001, /* ( |M| ) - A,  */
			/* K : */ 0x40126911, /* ( |M| ) - A, E, I, L, N, O, R and U */
			/* L : */ 0xc37eddfb, /* ( |M|E) - A, B, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U, V, W, Y and Z */
			/* M : */ 0xc01c4913, /* ( |M|E) - A, B, E, I, L, O, S, T and U */
			/* N : */ 0xc05c5ff7, /* ( |M|E) - A, B, C, E, F, G, H, I, J, K, L, M, O, S, T, U and W */
			/* O : */ 0xe17efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* P : */ 0x40124821, /* ( |M| ) - A, F, L, O, R and U */
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
			/* O : */ 0xe2bebc4f, /* (B|M|E) - A, B, C, D, G, K, L, M, N, P, R, S, T, U, V, X and Z */
			/* P : */ 0xe01ed9b3, /* (B|M|E) - A, B, E, F, H, I, L, M, O, P, R, S, T and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe17efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* S : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* T : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* U : */ 0x40063800, /* ( |M| ) - L, M, N, R and S */
			/* V : */ 0xe17ecd73, /* (B|M|E) - A, B, E, F, G, I, K, L, O, P, R, S, T, U, V, W and Y */
			/* W : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* X : */ 0xc00c4111, /* ( |M|E) - A, E, I, O, S and T */
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
			/* O : */ 0xe1362ddf, /* (B|M|E) - A, B, C, D, E, G, H, I, K, L, N, R, S, U, V and Y */
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
			/* Y : */ 0xe0020800, /* (B|M|E) - L and R */
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
			/* F : */ 0xc01e59b1, /* ( |M|E) - A, E, F, H, I, L, M, O, R, S, T and U */
			/* G : */ 0xc01e4911, /* ( |M|E) - A, E, I, L, O, R, S, T and U */
			/* H : */ 0x40004111, /* ( |M| ) - A, E, I and O */
			/* I : */ 0xe3fffeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* J : */ 0x60100001, /* (B|M| ) - A and U */
			/* K : */ 0xc11e7991, /* ( |M|E) - A, E, H, I, L, M, N, O, R, S, T, U and Y */
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
			/* O : */ 0xe2fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X and Z */
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
			/* W : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* X : */ 0x80000000, /* ( | |E) - None are allowed */
			/* Y : */ 0xe0045811, /* (B|M|E) - A, E, L, M, O and S */
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
			/* E : */ 0xe0be792f, /* (B|M|E) - A, B, C, D, F, I, L, M, N, O, R, S, T, U, V and X */
			/* F : */ 0xe01e4931, /* (B|M|E) - A, E, F, I, L, O, R, S, T and U */
			/* G : */ 0xe11e79db, /* (B|M|E) - A, B, D, E, G, H, I, L, M, N, O, R, S, T, U and Y */
			/* H : */ 0xe11e7911, /* (B|M|E) - A, E, I, L, M, N, O, R, S, T, U and Y */
			/* I : */ 0xe00e2c4d, /* (B|M|E) - A, C, D, G, K, L, N, R, S and T */
			/* J : */ 0x40004011, /* ( |M| ) - A, E and O */
			/* K : */ 0xe31e5d13, /* (B|M|E) - A, B, E, I, K, L, M, O, R, S, T, U, Y and Z */
			/* L : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* M : */ 0xe17efd33, /* (B|M|E) - A, B, E, F, I, K, L, M, N, O, P, R, S, T, U, V, W and Y */
			/* N : */ 0xe37fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* O : */ 0xe22ebc68, /* (B|M|E) - D, F, G, K, L, M, N, P, R, S, T, V and Z */
			/* P : */ 0xe11eddb1, /* (B|M|E) - A, E, F, H, I, K, L, M, O, P, R, S, T, U and Y */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe37ffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* S : */ 0xe37efdf7, /* (B|M|E) - A, B, C, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* T : */ 0xe35eddf7, /* (B|M|E) - A, B, C, E, F, G, H, I, K, L, M, O, P, R, S, T, U, W, Y and Z */
			/* U : */ 0xe22ebd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, P, R, S, T, V and Z */
			/* V : */ 0xe0104111, /* (B|M|E) - A, E, I, O and U */
			/* W : */ 0xe10cfbbf, /* (B|M|E) - A, B, C, D, E, F, H, I, J, L, M, N, O, P, S, T and Y */
			/* X : */ 0xe1084111, /* (B|M|E) - A, E, I, O, T and Y */
			/* Y : */ 0xc004491d, /* ( |M|E) - A, C, D, E, I, L, O and S */
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
			/* I : */ 0xe21efc5d, /* (B|M|E) - A, C, D, E, G, K, L, M, N, O, P, R, S, T, U and Z */
			/* J : */ 0x00000000, /* ( | | ) - None are allowed */
			/* K : */ 0x40000011, /* ( |M| ) - A and E */
			/* L : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0x60004010, /* (B|M| ) - E and O */
			/* O : */ 0xe2fefd5c, /* (B|M|E) - C, D, E, G, I, K, L, M, N, O, P, R, S, T, U, V, W, X and Z */
			/* P : */ 0xc11e4d95, /* ( |M|E) - A, C, E, H, I, K, L, O, R, S, T, U and Y */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xe118c195, /* (B|M|E) - A, C, E, H, I, O, P, T, U and Y */
			/* T : */ 0xe1165993, /* (B|M|E) - A, B, E, H, I, L, M, O, R, S, U and Y */
			/* U : */ 0x620ebc7e, /* (B|M| ) - B, C, D, E, F, G, K, L, M, N, P, R, S, T and Z */
			/* V : */ 0x40000010, /* ( |M| ) - E,  */
			/* W : */ 0x40000001, /* ( |M| ) - A,  */
			/* X : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Y : */ 0xe00a0800, /* (B|M|E) - L, R and T */
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
			/* I : */ 0xe3fdfeff, /* (B|M|E) - A, B, C, D, E, F, G, H, J, K, L, M, N, O, P, Q, S, T, U, V, W, X, Y and Z */
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
			/* W : */ 0x41104191, /* ( |M| ) - A, E, H, I, O, U and Y */
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
			/* C : */ 0xe0124991, /* (B|M|E) - A, E, H, I, L, O, R and U */
			/* D : */ 0x40124111, /* ( |M| ) - A, E, I, O, R and U */
			/* E : */ 0xe3ffffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y and Z */
			/* F : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* G : */ 0x40124911, /* ( |M| ) - A, E, I, L, O, R and U */
			/* H : */ 0xe15a7d37, /* (B|M|E) - A, B, C, E, F, I, K, L, M, N, O, R, T, U, W and Y */
			/* I : */ 0xe2befcff, /* (B|M|E) - A, B, C, D, E, F, G, H, K, L, M, N, O, P, R, S, T, U, V, X and Z */
			/* J : */ 0x40104001, /* ( |M| ) - A, O and U */
			/* K : */ 0xe1166911, /* (B|M|E) - A, E, I, L, N, O, R, S, U and Y */
			/* L : */ 0xe1104111, /* (B|M|E) - A, E, I, O, U and Y */
			/* M : */ 0xe1104111, /* (B|M|E) - A, E, I, O, U and Y */
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
			/* H : */ 0xe1567937, /* (B|M|E) - A, B, C, E, F, I, L, M, N, O, R, S, U, W and Y */
			/* I : */ 0xe23fffff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V and Z */
			/* J : */ 0x60100011, /* (B|M| ) - A, E and U */
			/* K : */ 0x40126911, /* ( |M| ) - A, E, I, L, N, O, R and U */
			/* L : */ 0xc1104111, /* ( |M|E) - A, E, I, O, U and Y */
			/* M : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* N : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* O : */ 0xe3fefdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, X, Y and Z */
			/* P : */ 0x401249b1, /* ( |M| ) - A, E, F, H, I, L, O, R and U */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0x61104111, /* (B|M| ) - A, E, I, O, U and Y */
			/* S : */ 0xe17eddff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, O, P, R, S, T, U, V, W and Y */
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
			/* C : */ 0x601a4d95, /* (B|M| ) - A, C, E, H, I, K, L, O, R, T and U */
			/* D : */ 0xe15649d9, /* (B|M|E) - A, D, E, G, H, I, L, O, R, S, U, W and Y */
			/* E : */ 0xe21e393f, /* (B|M|E) - A, B, C, D, E, F, I, L, M, N, R, S, T, U and Z */
			/* F : */ 0xe25efdfb, /* (B|M|E) - A, B, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, W and Z */
			/* G : */ 0xe21e79f3, /* (B|M|E) - A, B, E, F, G, H, I, L, M, N, O, R, S, T, U and Z */
			/* H : */ 0xe0167911, /* (B|M|E) - A, E, I, L, M, N, O, R, S and U */
			/* I : */ 0xc22ea85d, /* ( |M|E) - A, C, D, E, G, L, N, P, R, S, T, V and Z */
			/* J : */ 0x40100101, /* ( |M| ) - A, I and U */
			/* K : */ 0xe01a6d11, /* (B|M|E) - A, E, I, K, L, N, O, R, T and U */
			/* L : */ 0xe37efd7f, /* (B|M|E) - A, B, C, D, E, F, G, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* M : */ 0xe27efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W and Z */
			/* N : */ 0xe27effff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, R, S, T, U, V, W and Z */
			/* O : */ 0xc01e0808, /* ( |M|E) - D, L, R, S, T and U */
			/* P : */ 0xe05ec9bd, /* (B|M|E) - A, C, D, E, F, H, I, L, O, P, R, S, T, U and W */
			/* Q : */ 0x40100000, /* ( |M| ) - U,  */
			/* R : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* S : */ 0xe37ffdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, Q, R, S, T, U, V, W, Y and Z */
			/* T : */ 0xe37efdff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, K, L, M, N, O, P, R, S, T, U, V, W, Y and Z */
			/* U : */ 0x40003000, /* ( |M| ) - M and N */
			/* V : */ 0x40024111, /* ( |M| ) - A, E, I, O and R */
			/* W : */ 0x40104111, /* ( |M| ) - A, E, I, O and U */
			/* X : */ 0xc0100110, /* ( |M|E) - E, I and U */
			/* Y : */ 0xc0000001, /* ( |M|E) - A,  */
			/* Z : */ 0xc2504193, /* ( |M|E) - A, B, E, H, I, O, U, W and Z */
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
			/* O : */ 0xe15e2d44, /* (B|M|E) - C, G, I, K, L, N, R, S, T, U, W and Y */
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
			/* B : */ 0x40004001, /* ( |M| ) - A and O */
			/* C : */ 0x00000000, /* ( | | ) - None are allowed */
			/* D : */ 0x40000010, /* ( |M| ) - E,  */
			/* E : */ 0xe10e39ff, /* (B|M|E) - A, B, C, D, E, F, G, H, I, L, M, N, R, S, T and Y */
			/* F : */ 0x40104000, /* ( |M| ) - O and U */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x60004111, /* (B|M| ) - A, E, I and O */
			/* I : */ 0xe00ebc7c, /* (B|M|E) - C, D, E, F, G, K, L, M, N, P, R, S and T */
			/* J : */ 0x40000010, /* ( |M| ) - E,  */
			/* K : */ 0x40000100, /* ( |M| ) - I,  */
			/* L : */ 0xe1040111, /* (B|M|E) - A, E, I, S and Y */
			/* M : */ 0x40000011, /* ( |M| ) - A and E */
			/* N : */ 0xc0040910, /* ( |M|E) - E, I, L and S */
			/* O : */ 0xe01278ce, /* (B|M|E) - B, C, D, G, H, L, M, N, O, R and U */
			/* P : */ 0x00000000, /* ( | | ) - None are allowed */
			/* Q : */ 0x00000000, /* ( | | ) - None are allowed */
			/* R : */ 0x61004111, /* (B|M| ) - A, E, I, O and Y */
			/* S : */ 0xc0080490, /* ( |M|E) - E, H, K and T */
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
			/* C : */ 0x40120991, /* ( |M| ) - A, E, H, I, L, R and U */
			/* D : */ 0x00000000, /* ( | | ) - None are allowed */
			/* E : */ 0xe0063c4c, /* (B|M|E) - C, D, G, K, L, M, N, R and S */
			/* F : */ 0x40004000, /* ( |M| ) - O,  */
			/* G : */ 0x00000000, /* ( | | ) - None are allowed */
			/* H : */ 0x40000101, /* ( |M| ) - A and I */
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
			/* T : */ 0xc0124111, /* ( |M|E) - A, E, I, O, R and U */
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
			/* T : */ 0xc0004191, /* ( |M|E) - A, E, H, I and O */
			/* U : */ 0x60062400, /* (B|M| ) - K, N, R and S */
			/* V : */ 0x60000010, /* (B|M| ) - E,  */
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
			/* L : */ 0x60004111, /* (B|M| ) - A, E, I and O */
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
  * Start   : 06.06.2013 16:14:05                     *
  * Stop    : 06.06.2013 16:15:51                     *
  * Duration:    0 h,  1 m, 46 s                      *
  * ------------------------------------------------- *
  * textAnalyzer (c) Sven Eden, PrydeWorX 2010 - 2013 *
  @endverbatim
**/

#endif // PWX_LIBPWX_PWX_INTERNAL_CRANDOMWORDCONSTANTS_H_INCLUDED

