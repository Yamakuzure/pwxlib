#pragma once
#ifndef PWX_RANDOM_HASH_H
#define PWX_RANDOM_HASH_H

#ifndef PWX_RANDOM_H_INCLUDED
# error "Don't include CRandomHash.h directly! Include pwxLib/CRandom.h instead!"
#endif

/** @internal
  * @file CRandomHash.h
  *
  * @brief Implementations for CRandom - Number Hash Generation
  *
  * @see CRandom.h for documentsation and history
  *
**/

namespace pwx {
namespace rng {
// Note: The namespaces are documented in CRandom.h

/* -------------------------------------- *
 * --- Public Methods Implementations --- *
 * -------------------------------------- */


/** @brief hash32shift with signed key
  *
  * This is hash32shift() like described by Thomas Wang, 01/2007
  *
  * @param[in] key the value that is hashed
  * @return uint32_t with the resulting hash
**/
uint32_t CRandom::hash(int32_t key) const
{
  key = (~key) + (key << 15);
  key ^= (key & constants::fullMaxInt) >> 12; // key >>> 12
  key += key << 2;
  key ^= (key & constants::fullMaxInt) >> 4; // key >>> 4
  key *= 2057;
  key ^= (key & constants::fullMaxInt) >> 16; // key >>> 16
  return static_cast<uint32_t>(key);
}


/** @brief hash32shift with unsigned key
  *
  * This is hash() like described by Robert Jenkins, 6-shift version
  *
  * @param[in] key the value that is seeded
  * @return uint32_t with the resulting hash
**/
uint32_t CRandom::hash(uint32_t key) const
{
  key = (key  + 0x7ed55d16) + (key << 12);
  key = (key  ^ 0xc761c23c) ^ (key >> 19);
  key = (key  + 0x165667b1) + (key << 5);
  key = (key  + 0xd3a2646c) ^ (key << 9);
  key = (key  + 0xfd7046c5) + (key << 3);
  return (key ^ 0xb55a4f09) ^ (key >> 16);
}


/** @brief hash64shift with signed key
  *
  * This is hash64shift() like described by Thomas Wang, 01/2007
  *
  * @param[in] key the value that is seeded
  * @return uint64_t with the resulting hash
**/
uint64_t CRandom::hash(int64_t key) const
{
  key  = (~key) + (key << 21);
  key ^= (key & constants::fullMaxLong) >> 24; // key >>> 24
  key += (key << 3) + (key << 8);
  key ^= (key & constants::fullMaxLong) >> 14; // key >>> 14
  key += (key << 2) + (key << 4);
  key ^= (key & constants::fullMaxLong) >> 28; // key >>> 28
  return static_cast<uint64_t>(key + (key << 31));
}

/** @brief hash64 to 32 bit shift with unsigned key
  *
  * This is a 64 (or 2 x 32) to 32 bit shift hash function
  * like described by Thomas Wang, 01/2007
  *
  * @param[in] key the value that is seeded
  * @return uint64_t with the resulting hash
**/
uint32_t CRandom::hash(uint64_t key) const
{
  // Note: We still use the unsigned shift >>> to simply eliminate the left most bit
  key  = (~key) + (key << 18);
  key ^= (key & constants::fullMaxLong) >> 31; // key >>> 31
  key *= 21;
  key ^= (key & constants::fullMaxLong) >> 11; // key >>> 11
  key += key << 6;
  key ^= (key & constants::fullMaxLong) >> 22; // key >>> 22
  return static_cast<uint32_t>(key);
}

/* --------------------------------------- *
 * --- Private Methods Implementations --- *
 * --------------------------------------- */

} // End of namespace rng

} // End of namespace pwx

#endif // PWX_RANDOM_HASH_H
