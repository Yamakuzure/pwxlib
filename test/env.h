#pragma once
#ifndef PWX_LIBPWX_TEST_ENV_H_INCLUDED
#define PWX_LIBPWX_TEST_ENV_H_INCLUDED 1

/** @file env.h
  * (c)  2007 - 2021 PrydeWorX
  * @author Sven Eden, PrydeWorX - Adendorf, Germany
  *         sven.eden@prydeworx.com
  *         https://github.com/Yamakuzure/pwxlib ; https://pwxlib.prydeworx.com
  *
  * The PrydeWorX Library is free software under MIT License
**/

/// @brief simple struct to hold test statistics together
struct sEnv {
	int32_t testCount   = 0;
	int32_t testSuccess = 0;
	int32_t testFail    = 0;
	bool    doCont      = false;
	bool    doSpeed     = false;
	bool    doRNG       = false;
	bool    doSCT       = false;
	bool    doCWC       = false;
	bool    doPAH       = false;
};

#endif // PWX_LIBPWX_TEST_ENV_H_INCLUDED

