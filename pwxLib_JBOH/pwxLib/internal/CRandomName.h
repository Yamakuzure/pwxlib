#pragma once
#ifndef PWX_RANDOM_NAME_H
#  define PWX_RANDOM_NAME_H 1

#ifndef PWX_RANDOM_H_INCLUDED
# error "Don't include CRandomName.h directly! Include pwxLib/CRandom.h instead!"
#endif

/* =================================== *
 * === Implementations for CRandom === *
 * =                                 = *
 * ===    Random Name Generation   === *
 * =================================== **/

/** @internal
  * @file CRandomName.h
	*
	* @brief internal header that implements random name functions
	*
	@verbatim
  * Statistics: (Number of unique names out of 10,000,000 generated)
  * ----------------------------------------------------------------
  * Nr | short single |  long single |  short multi |   long multi | min/max rate
  * ---+--------------+--------------+--------------+--------------+--------------
  *  1 |    8,290,719 |    9,833,814 |    9,919,707 |    9,927,408 | 82.90% - 99.27%
  * Unfortunately a lot of the names have more then two vowels or consonants in a row.
  * Somewhere the rules are not checked and/or applied correctly.
  *  2 |    6,309,742 |    8,090,551 |    7,887,759 |    8,466,236 | 63.10% - 84.66%
  * Now the rules are applioed correctly, but the rate dropped from min 83% to 63%, which is abyssmal.
  * An addition to check against noise chance for double vowels and consonants might raise that.
  *  3 |    6,594,949 |    8,331,773 |    8,075,722 |    8,645,370 | 65.95% - 86.45%
  * Although this is better, alot of names clearly show that the current ruleset is not enough.
  * As the current "state" member can not hold more rules, the name generation should be rewritten
  * with a complete ruleset and state members for each step, namingly name, part, syllable and rule.
  *  4 |    7,032,458 |    8,649,546 |    8,419,848 |    8,887,568 | 70.32% - 88.88%
  * After the rewrite of genSyllable(), rates did not only rise, but the generated name have quality
  * far beyond what the previous implementation was able to produce. However, I think the test tool
  * is not testing sufficiently (two dimensions instead of four) and the generation of the index
  * stepping is still the old version.
  *  5 |    9,366,494 |    9,955,661 |    9,999,096 |    9,999,583 | 93.66% - 99.99%
  * This is it. Finally. And the names are of very good quality, too. I am really happy now!
  * ...
  * Unfortunately not. I found a bug that could cause genSyllable to return an illegal fifth char.
  * After fixing that, the stats are as follows:
  *  6 |    9,257,140 |    9,992,629 |    9,998,950 |    9,999,284 | 92.57% - 99.98%
  * short-multi and long-multi are a bit weaker, long-single is (a lot) stronger, and short single has
  * a definite drop. But I daresay that 92.6% with the short-single settings is still great!
  * ...
  * Well, I found out, that the names look rather weird most of the time. They have strange consonant
  * combinations at word start and end, and there are more oddities. The rules are pumped up to use
  * newly generated rules which state whether a 2-char-combination is allowed at the start, end or in
  * the middle of a word, and the result is as follows:
  *  7 |    8,512,064 |    9,853,612 |    9,999,115 |    9,999,422 | 85.12% - 99.99%
  * So while the single versions (as expected) have a drop due to less variability, the now more
  * variable word start and ending creates the best multi results ever.
  * The bad result with short-single is the result of filtering out a lot of useless Word-ending-sequences,
  * which means that combinations not fit for further processing might end a name early.
  * However, I will go and search for more name lists to add to the text-file repository used to generate
  * the rules matrix.
  * ... I have found some, adding up to nearly 100K individual names. The result:
  *  8 |    9,391,808 |    9,861,538 |    9,998,942 |    9,999,253| 93.91% - 99.99%
  * wow... that's it!
  * Of course there still could be missing filters.
  @endverbatim
**/

namespace pwx {
namespace rng {
// Note: The namespaces are documented in CRandom.h

/* -------------------------------------- *
 * --- Public Methods Implementations --- *
 * -------------------------------------- **/

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
char * CRandom::rndName(double x, bool lN, bool mW)
{
  return (rndName(x, lN ? 20 : 12, lN ?  6 :  4, mW ?  3 :  1));
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
char * CRandom::rndName(double x, int32_t chars, int32_t sylls, int32_t parts)
{
  double newY = x * noiseD(x) * (abs(x) < 1.0 ? 1000.0 : abs(x) < 10.0 ? 100.0 : abs(x) < 100 ? 10.0 : 1.0);
  return (rndName(x, newY, chars, sylls, parts));
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
char * CRandom::rndName(double x, double y, bool lN, bool mW)
{
  return (rndName(x, y, lN ? 20 : 12, lN ?  6 :  4, mW ?  3 :  1));
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
char * CRandom::rndName(double x, double y, int32_t chars, int32_t sylls, int32_t parts)
{
  double newZ = (x * noiseD(y)) + (y * noiseD(x));
  newZ *= abs(newZ) < 1 ? 1000.0 : abs(newZ) < 10.0 ? 100.0 : abs(newZ) < 100 ? 10.0 : 1.0;
  return (rndName(x, y, newZ, chars, sylls, parts));
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
char * CRandom::rndName(double x, double y, double z, bool lN, bool mW)
{
  return (rndName(x, y, z,  lN ? 20 : 12, lN ?  6 :  4, mW ?  3 :  1));
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
char * CRandom::rndName(double x, double y, double z, int32_t chars, int32_t sylls, int32_t parts)
{
  double newW = (x * noiseD(y + z)) + (y * noiseD(x + z)) + (z * noiseD(x + y));
  newW *= abs(newW) < 1 ? 1000.0 : abs(newW) < 10.0 ? 100.0 : abs(newW) < 100 ? 10.0 : 1.0;
  return (rndName(x, y, z, newW, chars, sylls, parts));
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
char * CRandom::rndName(double x, double y, double z, double w, bool lN, bool mW)
{
  return (rndName(x, y, z, w, lN ? 20 : 12, lN ?  6 :  4, mW ?  3 :  1));
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
char * CRandom::rndName(double x, double y, double z, double w, int32_t chars, int32_t sylls, int32_t parts)
{
  ::std::string name = "";
  char    syll[5]    = { 0x0, 0x0, 0x0, 0x0, 0x0 };
  int32_t partsLeft  = ::std::max(1                  , parts);
  int32_t syllsLeft  = ::std::max(1 +      partsLeft , sylls);
  int32_t charsLeft  = ::std::max(2 + (3 * syllsLeft), chars);
  int32_t genState   = NameConstants::genPartStart;
  char    lastChrs[2]= { 0x0, 0x0 }; // This is an explicit array, no C-String, so no \0 ending
  int32_t syllsDone  = 0;
  double  index      = (x * simplex3D(   y, z, w))
                     + (y * simplex3D(x,    z, w))
                     + (z * simplex3D(x, y,    w))
                     + (w * simplex3D(x, y, z   ))
                     + seed;
  double stepping    = getStepping(index, x, y, z, w, charsLeft, syllsLeft, partsLeft);
  double endChance   = 0.0;

  // Do - while genState doesn't equal NameConstants::genFinished
  do
    {
      /// 1) Determine whether the next syllable ends a part, genSyllable() needs to know.
      endChance = static_cast<double>((syllsLeft * 2) - (partsLeft * 2)) / 10.0;
      /* maximum : 12 - 2 = 10 => / 10 = 1.0 (after first syll, !mW &&  lN) =>  0%
       * minimum :  8 - 6 =  2 => / 10 = 0.2 (after first syll,  mW && !lN) => 40%
       */

      // Nevertheless we reduce the endchance if this is the first syllable and no multiword selected:
      if (!syllsDone && (1 == partsLeft))
        endChance += static_cast<double>(syllsLeft) / 20.0;
        /* The initial chance is (8-2)/10 = 0.6 = 20%.
         * After this modification it is 0.6 + 0.2 = 10%
         * This, however, does not cover weird arguments set by the user!
         */
      // However, we need to raise the chance if we have too few sylls left:
      if (syllsLeft < (partsLeft * 2))
        endChance -= static_cast<double>(syllsLeft) / static_cast<double>(partsLeft * 2);
        /* So if we have three sylls left and two parts, the chance is raised by 0.75
         * If we have 5 sylls left and 3 parts, it would be 0,83
         */

      // If this is the very first syllable, the chance is halved:
      if (0 == syllsDone)
        endChance += (endChance + 1.0) / 2.0;

      // Now test the chance:
      if (simplex3D(index, charsLeft, partsLeft) > endChance)
        genState |= NameConstants::genPartEnd;

      /// 2) generate syllable:
      charsLeft -= genSyllable(index, stepping, syll, genState, lastChrs);

      /// 3) if we have a syllable (genSyllable produces an empty string on error) it can be added:
      if (strlen(syll) > 1)
        {
          name += syll;
          syllsDone++;
          syllsLeft--;

          // If this is a partEnd, react
          if (genState & NameConstants::genPartEnd)
            {
              genState = NameConstants::genPartStart;
              if ((charsLeft >= 4) && --partsLeft && syllsLeft)
                name += " "; // add a space, as we will start a new part
              memset(lastChrs, 0, sizeof(char)); // Needs to be resetted...
            }
        }
      // 4) If we have work to do, generate a new stepping and index
      if ((charsLeft >= 4) && partsLeft && syllsLeft)
        {
          stepping = getStepping(index, x, y, z, w, charsLeft, syllsLeft, partsLeft);
          index   += stepping;
        }
      else
        genState = NameConstants::genFinished;
    }
  while (genState != NameConstants::genFinished);

  return (pwx_strdup(name.c_str()));
}


/* --------------------------------------- *
 * --- Private Methods Implementations --- *
 * --------------------------------------- */

/** @brief checkRule - check state and character against followup matrix rules from namecon
**/
void CRandom::checkRule(int32_t &state, const char first, const char second, const char third)
{
  int32_t one   = first  - NameConstants::chrOffsetLower;
  int32_t two   = second - NameConstants::chrOffsetLower;
  int32_t three = third  - NameConstants::chrOffsetLower;

  assert ((first > -1) && (second > -1) && (third > -1));

  if ( ((one == two) && (two == three)) // eleminate tripple threats
    || (0 == ( NameConstants::nameFUM[one][two]
             & static_cast<int32_t>(pwx_pow(2, three)) )
     ) )
    {
      // The desired character is not allowed to follow the set two chars
      if (state & NameConstants::genNextIsCon)
        state ^= NameConstants::genNextIsCon;
      if (state & NameConstants::genNextIsVow)
        state ^= NameConstants::genNextIsVow;
    }
}


/** @brief generate a syllable out of various rules
**/
int32_t CRandom::genSyllable(double &idx, double step, char * syll, int32_t &state, char * lastChrs)
{
  int32_t charCount = 0;
  int32_t charIndex = 0;
  char    nextCon   = 0;     // Shortcut to the next consonant
  char    nextVow   = 0;     // Shortcut to the next vowel
  int32_t oldState  = state; // save state to revert if generation failed
  char    oldLstChrs[2];     // Save lastChar to revert back if generation fails
  int32_t oldWasLast= 0;     // Saves genLastIsVow(Con) to revert if the neding is illegal
  int32_t conCount  = 0;     // Count generated consonants
  int32_t vowCount  = 0;     // Count generated vowels
  int32_t genTries  = 8;     // Eight tries to generate a syllable. That should be enough!
  double  endChance = 0.0;   // propability to end (early)
  double  vowChance = 0.0;   // 50% if last was consonant, 25% if last was vowel

  // Initialize syll and state
  memset(syll, 0, 5);
  state |= NameConstants::genRoundA;

  // save lastChrs
  assert (lastChrs);
  oldLstChrs[0] = lastChrs[0];
  oldLstChrs[1] = lastChrs[1];

  // do - while state has no NameConstants::genSyllEnd
  do
    {
      /*********************************************************************
       * 1) Determine charIndex and set shortcuts                          *
       *********************************************************************/
      charIndex = static_cast<int32_t>(pwx_round(abs(idx)));
      nextCon   = NameConstants::conList[charIndex % NameConstants::conListLen];
      nextVow   = NameConstants::vowList[charIndex % NameConstants::vowListLen];
      assert ( (nextCon > 0x60) && (nextCon < 0x7b)
            && (nextVow > 0x60) && (nextVow < 0x7b));

      /*********************************************************************
       * 2) Set chance to select a vowel next                              *
       *********************************************************************/
      if (state & NameConstants::genLastIsVow)
        {
          state     ^= NameConstants::genLastIsVow;
          oldWasLast = NameConstants::genLastIsVow;
          vowChance  = 0.5; // 25% chance of a double vowel
        }
      else if (state & NameConstants::genLastIsCon)
        {
          state     ^= NameConstants::genLastIsCon;
          oldWasLast = NameConstants::genLastIsCon;
          vowChance  = 0.0; // 50% chance of a vowel to be next
        }
      else
        {
          vowChance  = 0.33; // 33% chance of a vowel to be next
          oldWasLast = 0;
        }

      /*********************************************************************
       * 3) Decide whether to go for a consonant or a vowel                *
       *********************************************************************/
      if (simplex3D(idx, step, charCount) >= vowChance)
        // Next shall be a vowel
        state |= NameConstants::genNextIsVow;
      else
        // Next shall be consonant
        state |= NameConstants::genNextIsCon;

      /*********************************************************************
       * 4) Check lastChars + firstChar against nameFUM for Round A        *
       *********************************************************************/
      if ( (state & NameConstants::genRoundA) && (0 == (state & NameConstants::genPartStart)) )
        {
          if (     state & NameConstants::genNextIsCon)
            checkRule(state, lastChrs[0], lastChrs[1], nextCon);
          else if (state & NameConstants::genNextIsVow)
            checkRule(state, lastChrs[0], lastChrs[1], nextVow);
        }

      /*********************************************************************
       * 5) call checkRule() for Round B with lastChrs or check position   *
       *********************************************************************/
      if (state & NameConstants::genRoundB)
        {
          // First, handle part start rules:
          if (state & NameConstants::genPartStart)
            {
              // On a part start, we need to check against the position:
              if ( (state & NameConstants::genNextIsCon)
                && (0 == ( NameConstants::genStartAllow
                         & NameConstants::nameFUM[syll[0] - NameConstants::chrOffsetLower]
                                                    [nextCon - NameConstants::chrOffsetLower])) )
                // What a pity, this combination is illegal on a part start
                state ^= NameConstants::genNextIsCon;
              else if ( (state & NameConstants::genNextIsVow)
                     && (0 == ( NameConstants::genStartAllow
                              & NameConstants::nameFUM[syll[0] - NameConstants::chrOffsetLower]
                                                      [nextVow - NameConstants::chrOffsetLower])) )
                // Nope, this vowel isn't  creating a legal part start
                state ^= NameConstants::genNextIsVow;
            }
          else
            {
              // Somewhere else this is a normal checkrule
              if ( (state & NameConstants::genNextIsCon)
                && ( NameConstants::genMiddleAllow
                   & NameConstants::nameFUM[syll[0] - NameConstants::chrOffsetLower]
                                           [nextCon - NameConstants::chrOffsetLower]))
                checkRule(state, lastChrs[1], syll[0], nextCon);
              else
                // What a pity, this combination is illegal in the middle of a part
                state ^= NameConstants::genNextIsCon;
              if ( (state & NameConstants::genNextIsVow)
                && ( NameConstants::genMiddleAllow
                   & NameConstants::nameFUM[syll[0] - NameConstants::chrOffsetLower]
                                           [nextVow - NameConstants::chrOffsetLower]))
                checkRule(state, lastChrs[1], syll[0], nextVow);
              else
                // What a pity, this combination is illegal in the middle of a part
                state ^= NameConstants::genNextIsVow;
            }
        }

      /*********************************************************************
       * 6) call checkRule() for Round C and D                             *
       *********************************************************************/
      if (state & (NameConstants::genRoundC | NameConstants::genRoundD))
        {
          if (state & NameConstants::genNextIsCon)
            checkRule(state, syll[charCount - 2], syll[charCount - 1], nextCon);
          if (state & NameConstants::genNextIsVow)
            checkRule(state, syll[charCount - 2], syll[charCount - 1], nextVow);
        }

      /*********************************************************************
       * 7) Add the selected char if we can or decrease genTries otherwise *
       *********************************************************************/
      if (state & NameConstants::genNextIsCon)
        {
          syll[charCount++] = nextCon;
          state ^= NameConstants::genNextIsCon;
          state |= NameConstants::genLastIsCon | NameConstants::genHasNextChar;
          conCount++;
        }
      else if (state & NameConstants::genNextIsVow)
        {
          syll[charCount++] = nextVow;
          state ^= NameConstants::genNextIsVow;
          state |= NameConstants::genLastIsVow | NameConstants::genHasNextChar;
          vowCount++;
        }
      else
        // We have nothing, so reduce our tries:
        genTries--;

      /*********************************************************************
       * 8) If we added a char, advance the round                          *
       **********************************************************************/
      if (state & NameConstants::genHasNextChar)
        {
          state ^= NameConstants::genHasNextChar;
          if (state & NameConstants::genRoundD)
            {
              // We have to End here:
              state ^= NameConstants::genRoundD;
              endChance = 2.0; // 100% chance to stop here
            }
          else if (state & NameConstants::genRoundC)
            {
              // Simple Advance
              state ^= NameConstants::genRoundC;
              state |= NameConstants::genRoundD;
              endChance = 0.20; // 60% chance to stop
            }
          else if (state & NameConstants::genRoundB)
            {
              // Simple advance
              state ^= NameConstants::genRoundB;
              state |= NameConstants::genRoundC;
              endChance = -0.50; // 25% chance to stop
            }
          else if (state & NameConstants::genRoundA)
            {
              // Simple advance
              state ^= NameConstants::genRoundA;
              state |= NameConstants::genRoundB;
              endChance = -2.0; // 0% chance to stop
            }

          // If this is a single syll (both part start and end), the endChance has to be reduced:
          if ((state & NameConstants::genPartStart) && (state & NameConstants::genPartEnd))
            endChance -= 0.3;
            // so 45% (RoundC) or 10% (RoundB) chance. D is too high to be affected.

          /*********************************************************************
           * 9) Check against chance to end this syllable                      *
           *********************************************************************/
          if (simplex3D(idx, charIndex, (charCount * conCount) + (genTries * vowCount)) <= endChance)
            {
              // We shall stop! But are we allowed to?
              if ( ( (state & NameConstants::genPartEnd)    // This part ends, so:
                  && ( NameConstants::genEndAllow // Is this combination allowed at a Parts end?
                       & NameConstants::nameFUM[syll[charCount - 2] - NameConstants::chrOffsetLower]
                                               [syll[charCount - 1] - NameConstants::chrOffsetLower]))
                || ( (0 == (state & NameConstants::genPartEnd)) // This part shall go on, so:
                  && ( NameConstants::genMiddleAllow  // Is this combination allowed in the middle?
                       & NameConstants::nameFUM[syll[charCount - 2] - NameConstants::chrOffsetLower]
                                               [syll[charCount - 1] - NameConstants::chrOffsetLower])) )
                // Yeeees!
                state |= NameConstants::genSyllEnd;
              else if ( ( ( ( state & NameConstants::genRoundC  // In round C or D, which are next, we go for a chance
                           || state & NameConstants::genRoundD) // via noise, because we could simply go on as well.
                         && (noise(hash(static_cast<int32_t>(step * (idx + charIndex + charCount
                                                             + genTries + vowCount)))) > 0)
                          ) // But with a fourth char already, we _have_ to revert:
                       || (0 == (state & (NameConstants::genRoundC | NameConstants::genRoundD)))
                        ) // Finally we need tries left:
                     && --genTries)
                {
                  // We simply search for a new char:
                  syll[charCount--] = 0x0;
                  if (state & NameConstants::genRoundC)
                    {
                      state ^= NameConstants::genRoundC;
                      state |= NameConstants::genRoundB;
                    }
                  else if (state & NameConstants::genRoundD)
                    {
                      state ^= NameConstants::genRoundD;
                      state |= NameConstants::genRoundC;
                    }
                  else
                    state |= NameConstants::genRoundD;

                  // Revert the counts:
                  if (state & NameConstants::genLastIsCon)
                    {
                      state ^= NameConstants::genLastIsCon;
                      conCount--;
                    }
                  else if (state & NameConstants::genLastIsVow)
                    {
                      syll[charCount++] = nextVow;
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
    }
  while ((genTries > 0) && (0 == (state & NameConstants::genSyllEnd)));

  /*********************************************************************
   * 11) if genTries reached zero or we have 0 vow/conCount, we fail   *
   *********************************************************************/

  if (genTries && vowCount && conCount)
    {
      // great!
      state ^= NameConstants::genSyllEnd;
      if (state & NameConstants::genRoundC)
        state ^= NameConstants::genRoundC; // might have advanced from B, then ended
      else if (state & NameConstants::genRoundD)
        state ^= NameConstants::genRoundD; // might have advanced from C, then ended
      // We don't end from A (advanced to B)

      // We need to record the last two chars:
      lastChrs[0] = syll[charCount - 2];
      lastChrs[1] = syll[charCount - 1];

      // If this is a part start, we have to change the first character to upper case
      if (state & NameConstants::genPartStart)
        {
          state ^= NameConstants::genPartStart;
          syll[0] -= NameConstants::chrOffsetDown;
        }

      // If this is not a part end, but the last chars do not allow
      // follow up characters, we have to force an ending:
      if ( (0 == (state & NameConstants::genPartEnd))
        && (0 == ( NameConstants::genCharMask
                   & NameConstants::nameFUM[lastChrs[0] - NameConstants::chrOffsetLower]
                                           [lastChrs[1] - NameConstants::chrOffsetLower]) ) )
        // Yep, we have to
        state |= NameConstants::genPartEnd;
      // We keep genLastIsCon/Vow for the next round
    }
  else
    {
      // what a pity...
      state = oldState;
      lastChrs[0] = oldLstChrs[0];
      lastChrs[1] = oldLstChrs[1];
      memset(syll, 0, 5);
      charCount = 0;
    }
  return (charCount);
}


/** @brief generate a stepping for rndName() - result <= -1.0 || 1.0 <= result
  *
  * i = index, cl = charsLeft, sl = syllsLeft, pl = partsLeft
**/
double CRandom::getStepping(double i, double x, double y, double z, double w, int32_t cl, int32_t sl, int32_t pl)
{
  double result = (i * noise(cl) * noiseD(x,    z   ))
                + (i * noise(sl) * noiseD(   y,    w))
                + (i * noise(pl) * noiseD(x, y, z, w));

  if ((result <  0) && (result > -1.0)) result = -2.0 + noiseD(i);
  if ((result >= 0) && (result <  1.0)) result =  2.0 + noiseD(i);
  while (abs(result) >= NameConstants::conListLen) result /= 7.3673L;
  while (abs(result) <= NameConstants::vowListLen) result *= 1.7667L;
  return (result);
}

} // End of namespace rng

} // End of namespace pwx

#endif // PWX_RANDOM_NAME_H
