#pragma once
#ifndef PWX_TEST_DESTROY_H_INCLUDED
#define PWX_TEST_DESTROY_H_INCLUDED 1

/** @file destroy.h
  * (c) 2007 - 2019 PrydeWorX
  * @author Sven Eden, PrydeWorX - Bardowick, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

/* This file just adds some destroy functions to use with the
 * various containers that get tested.
*/

template<typename T>
void do_not_destroy ( T * ) {
        /* Don't you dare! */
}


#endif // PWX_TEST_DESTROY_H_INCLUDED

