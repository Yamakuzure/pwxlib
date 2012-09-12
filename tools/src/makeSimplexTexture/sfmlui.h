#ifndef PWX_MAKESIMPLEXTEXTURE_SFMLUI_H_INCLUDED
#define PWX_MAKESIMPLEXTEXTURE_SFMLUI_H_INCLUDED 1
#pragma once

// This header adds some methods that are used by the workLoop only

#include "main.h"

int32_t saveTexture(ENVIRONMENT * env) PWX_WARNUNUSED;
void showMsg(ENVIRONMENT * env, const char * fmt, ...);
int32_t showOSHelp(ENVIRONMENT * env) PWX_WARNUNUSED;
int32_t showOSStat(ENVIRONMENT * env) PWX_WARNUNUSED;
void showSeqPat(ENVIRONMENT * env, char * seqZtgt = NULL, char * seqWtgt = NULL);
void showState(ENVIRONMENT * env);

#endif // PWX_MAKESIMPLEXTEXTURE_SFMLUI_H_INCLUDED
