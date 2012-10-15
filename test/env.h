#pragma once
#ifndef PWX_LIBPWX_TEST_ENV_H_INCLUDED
#define PWX_LIBPWX_TEST_ENV_H_INCLUDED 1

/// @brief simple struct to hold test statistics together
struct sEnv {
	int32_t testCount   = 0;
	int32_t testSuccess = 0;
	int32_t testFail    = 0;
};

#endif // PWX_LIBPWX_TEST_ENV_H_INCLUDED
