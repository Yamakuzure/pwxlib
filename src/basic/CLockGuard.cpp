/***
  This file is part of the PrydeWorX Library (pwxLib).

  Copyright 2007 - 2017 Sven Eden

  The PrydeWorX Library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  The PrydeWorX Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with pwxLib; If not, see <http://www.gnu.org/licenses/>.

  History and Changelog are maintained in pwx.h
***/


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
