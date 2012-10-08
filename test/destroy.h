#pragma once
#ifndef PWX_TEST_DESTROY_H_INCLUDED
#define PWX_TEST_DESTROY_H_INCLUDED 1

/* This file just adds some destroy functions to use with the
 * various containers that get tested.
*/

template<typename T>
void do_not_destroy(T*)
  { /* Don't you dare! */ }


#endif // PWX_TEST_DESTROY_H_INCLUDED

