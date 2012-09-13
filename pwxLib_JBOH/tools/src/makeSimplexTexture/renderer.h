#ifndef PWX_MAKESIMPLEXTEXTURE_RENDERER_H_INCLUDED
#define PWX_MAKESIMPLEXTEXTURE_RENDERER_H_INCLUDED 1
#pragma once

#include "main.h"
#include "sfmlui.h"

int32_t            loadNewImage(ENVIRONMENT * env);
int32_t            swapImage   (ENVIRONMENT * env);
void               thrdDraw    (void *xEnv);

#endif // PWX_MAKESIMPLEXTEXTURE_RENDERER_H_INCLUDED
