//
// Created by clovis on 15/11/23.
//

#ifndef TANKS_VFX_H
#define TANKS_VFX_H

#include <SDL2/SDL.h>

#include "main.h"
#include "vfx.h"

int isTankHitEffectActive(textures_s* textures);
void updateAndRenderTankHitEffect(SDL_Renderer* ren, textures_s* textures);

#endif //TANKS_VFX_H
