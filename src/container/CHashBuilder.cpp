/** @file
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
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


#include "basic/compiler.h"
#include "basic/macros.h"
#include "basic/debug.h"

#include "container/CHashBuilder.h"
#include "random/CRandom.h"


namespace pwx {


/// From pwx.cpp
extern CRandom RNG; // [R]andom [N]-Value [G]enerator


uint32_t CHashBuilder::getKeyLen() const noexcept {
	return keyLen;
}

void CHashBuilder::setKeyLen( uint32_t keyLen_ ) noexcept {
	keyLen = keyLen_;
}

uint32_t CHashBuilder::hash_rng( const int16_t* key )      const noexcept { return RNG.hash( *key ); }
uint32_t CHashBuilder::hash_rng( const uint16_t* key )     const noexcept { return RNG.hash( *key ); }
uint32_t CHashBuilder::hash_rng( const int32_t* key )      const noexcept { return RNG.hash( *key ); }
uint32_t CHashBuilder::hash_rng( const uint32_t* key )     const noexcept { return RNG.hash( *key ); }
uint32_t CHashBuilder::hash_rng( const int64_t* key )      const noexcept { return RNG.hash( *key ); }
uint32_t CHashBuilder::hash_rng( const uint64_t* key )     const noexcept { return RNG.hash( *key ); }
uint32_t CHashBuilder::hash_rng( const float* key )        const noexcept { return RNG.hash( *key ); }
uint32_t CHashBuilder::hash_rng( const double* key )       const noexcept { return RNG.hash( *key ); }
uint32_t CHashBuilder::hash_rng( const long double* key )  const noexcept { return RNG.hash( *key ); }

uint32_t CHashBuilder::hash_rng( char const* key )  const noexcept {
	if ( keyLen )
		return RNG.hash( key, keyLen );
	else
		return RNG.hash( key, strlen( key ) );
}

uint32_t CHashBuilder::hash_rng( const std::string* key ) const noexcept {
	if ( keyLen && ( keyLen > key->size() ) ) {
		std::string subKey = key->substr( 0, keyLen );
		return RNG.hash( subKey );
	} else
		return RNG.hash( *( const_cast<std::string*>( key ) ) );
	return 0;
}

} // namespace pwx
