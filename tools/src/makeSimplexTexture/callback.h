#pragma once
#ifndef PWX_MAKESIMPLEXTEXTURE_CALLBACK_H_INCLUDED
#define PWX_MAKESIMPLEXTEXTURE_CALLBACK_H_INCLUDED 1

#include <cstring> // for strlen and strcmp
#include <pwxLib/tools/DefaultDefines.h>

// These are callback functions to work with pwxArg
void cbDimension  (const char *arg, void *env);
void cbColorHi    (const char *arg, void *env);
void cbColorMid   (const char *arg, void *env);
void cbColorLow   (const char *arg, void *env);
void cbHelpVersion(const char *arg, void *env);
void cbSetSequence(const char *arg, void *env);

#endif // PWX_MAKESIMPLEXTEXTURE_CALLBACK_H_INCLUDED

