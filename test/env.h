#pragma once
#ifndef PWX_LIBPWX_TEST_ENV_H_INCLUDED
#define PWX_LIBPWX_TEST_ENV_H_INCLUDED 1

/// @brief simple struct to hold test statistics together
struct sEnv
{
	int32_t testCount   = 0;
	int32_t testSuccess = 0;
	int32_t testFail    = 0;
	bool    doCont      = false;
	bool    doSpeed     = false;
	bool    doRNG       = false;
	bool    doSCT       = false;
};

#endif // PWX_LIBPWX_TEST_ENV_H_INCLUDED

