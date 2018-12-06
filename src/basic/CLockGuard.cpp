/**
  * This file is part of the PrydeWorX Library (pwxLib).
  *
  * (c) 2007 - 2018 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://prydeworx.com/pwxlib/
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
  * History and change log are maintained in pwx.h
**/


#include <thread>

#include "CLockGuard.h"


namespace pwx {


/** @brief CLockGuard
  *
  * @todo: document this function
  */
CLockGuard::CLockGuard(const CLockable* objA) noexcept
{
	reset(objA, nullptr, nullptr);
}

/** @brief CLockGuard
  *
  * @todo: document this function
  */
CLockGuard::CLockGuard(const CLockable* objA, const CLockable* objB) noexcept
{
	reset(objA, objB, nullptr);
}

/** @brief CLockGuard
  *
  * @todo: document this function
  */
CLockGuard::CLockGuard(const CLockable* objA, const CLockable* objB, const CLockable* objC) noexcept
{
	reset(objA, objB, objC);
}

/** @brief CLockGuard
  *
  * @todo: document this function
  */
CLockGuard::CLockGuard(const CLockGuard& src) noexcept
{
	reset(src.l_a, src.l_b, src.l_c);
}

/** @brief ~CLockGuard
  *
  * @todo: document this function
  */
CLockGuard::~CLockGuard() noexcept
{
	reset(nullptr, nullptr, nullptr);
}


/** @brief operator=
  *
  * @todo: document this function
  */
CLockGuard &CLockGuard::operator=(const CLockGuard& src) noexcept
{
	reset(src.l_a, src.l_b, src.l_c);
	return *this;
}

/** @brief reset
  *
  * @todo: document this function
  */
void CLockGuard::reset(const CLockable* objA) noexcept
{
	reset(objA, l_b, l_c);
}

/** @brief reset
  *
  * @todo: document this function
  */
void CLockGuard::reset(const CLockable* objA, const CLockable* objB) noexcept
{
	reset(objA, objB, l_c);
}

/** @brief reset
  *
  * @todo: document this function
  */
void CLockGuard::reset(const CLockable* objA, const CLockable* objB, const CLockable* objC) noexcept
{
	if (l_a) l_a->unlock();
	if (l_b) l_b->unlock();
	if (l_c) l_c->unlock();

	l_a = const_cast<CLockable*>(objA);
	l_b = const_cast<CLockable*>(objB);
	l_c = const_cast<CLockable*>(objC);

	while (!try_locks(l_a, l_b, l_c)) {
		std::this_thread::yield();
		if (l_a && l_a->destroyed()) l_a = nullptr;
		if (l_b && l_b->destroyed()) l_b = nullptr;
		if (l_c && l_c->destroyed()) l_c = nullptr;
	}
}


} // namespace pwx
