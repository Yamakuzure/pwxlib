/** @file
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2019 PrydeWorX
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


#include <thread>

#include "basic/pwx_compiler.h"
#include "basic/pwx_macros.h"
#include "basic/pwx_debug.h"

#include "basic/CLockGuard.h"


/// @namespace pwx
namespace pwx {


CLockGuard::CLockGuard( CLockable* objA ) noexcept {
	reset( objA, nullptr, nullptr );
}

CLockGuard::CLockGuard( CLockable* objA, CLockable* objB ) noexcept {
	reset( objA, objB, nullptr );
}

CLockGuard::CLockGuard( CLockable* objA, CLockable* objB, CLockable* objC ) noexcept {
	reset( objA, objB, objC );
}

CLockGuard::CLockGuard( CLockGuard& src ) noexcept {
	l_a = src.l_a;
	l_b = src.l_b;
	l_c = src.l_c;
	src.reset( NULL_LOCK, NULL_LOCK, NULL_LOCK );  // Causes it to unlock
	reset( l_a, l_b, l_c );
}

CLockGuard::~CLockGuard() noexcept {
	reset( NULL_LOCK, NULL_LOCK, NULL_LOCK );
}

CLockGuard& CLockGuard::operator=( CLockGuard& src ) noexcept {
	if ( &src != this ) {
		l_a = src.l_a;
		l_b = src.l_b;
		l_c = src.l_c;
		src.reset( NULL_LOCK, NULL_LOCK, NULL_LOCK );  // Causes it to unlock
		reset( l_a, l_b, l_c );
	}
	return *this;
}

void CLockGuard::reset( CLockable* objA ) noexcept {
	reset( objA, l_b, l_c );
}

void CLockGuard::reset( CLockable* objA, CLockable* objB ) noexcept {
	reset( objA, objB, l_c );
}

void CLockGuard::reset( CLockable* objA, CLockable* objB, CLockable* objC ) noexcept {
	if ( l_a ) l_a->unlock();
	if ( l_b ) l_b->unlock();
	if ( l_c ) l_c->unlock();

	l_a = objA;
	l_b = objB;
	l_c = objC;

	while ( !try_locks( l_a, l_b, l_c ) ) {
		std::this_thread::yield();
		if ( l_a && l_a->destroyed() ) l_a = nullptr;
		if ( l_b && l_b->destroyed() ) l_b = nullptr;
		if ( l_c && l_c->destroyed() ) l_c = nullptr;
	}
}


} // namespace pwx
