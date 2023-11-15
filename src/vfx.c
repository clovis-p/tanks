//
// Created by clovis on 15/11/23.
//

#include <stdio.h>

#include <SDL2/SDL.h>

#include "main.h"
#include "vfx.h"

int isTankHitEffectActive(textures_s* textures)
{
    Uint32 currentTicks = SDL_GetTicks();
    for (int i = 0; i < playerCount; i++)
    {
        // If it has been less than 1000ms since tank[i] got hit and if it ever got hit
        if (currentTicks - textures->tank[i].ticksAtLastHit < 1000 && textures->tank[i].ticksAtLastHit > 0)
        {
            // Return the id of the tank that got hit during the last 1000ms
            return i;
        }
    }

    // Return -1 if we shouldn't render a hit effect
    return -1;
}

void updateAndRenderTankHitEffect(SDL_Renderer* ren, textures_s* textures)
{
    SDL_Rect hitEffectRect = {0, 0, RESOLUTION_X, RESOLUTION_Y};

    float alpha = 128;
    Uint32 currentTicks = SDL_GetTicks();
    for (int i = 0; i < playerCount; i++)
    {
        // If it has been less than 1000ms since tank[i] got hit
        if (currentTicks - textures->tank[i].ticksAtLastHit < 1000)
        {
            alpha = 128 - (float)(currentTicks - textures->tank[i].ticksAtLastHit) / 1000 * 128;
        }
    }

    SDL_SetRenderDrawColor(ren, 255, 255, 255, (int)alpha);
    SDL_RenderFillRect(ren, &hitEffectRect);
}