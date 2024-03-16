//
// Created by clovis on 15/11/23.
//

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>

#include "main.h"
#include "vfx.h"

static void renderAAFilledCircle(SDL_Renderer* ren, int x, int y, int radius, int r, int g, int b, int a);

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
    int alpha = 128;
    Uint32 currentTicks = SDL_GetTicks();
    for (int i = 0; i < playerCount; i++)
    {
        // If it has been less than 1000ms since tank[i] got hit
        if (currentTicks - textures->tank[i].ticksAtLastHit < 1000 && textures->tank[i].isLatestHit)
        {
            alpha = (int)(128.0f - (float)(currentTicks - textures->tank[i].ticksAtLastHit) / 1000.0f * 128.0f);
        }
    }

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, alpha);
    SDL_RenderFillRect(ren, NULL);
}

int isBulletGroundImpactEffectActive(bullet_s* bullet)
{
    Uint32 currentTicks = SDL_GetTicks();

    // If it has been less than 1000ms since a crater was made
    if (currentTicks - bullet->ticksAtLastCrater < 1000)
    {
        return 1;
    }

    return 0;
}

void updateAndRenderBulletGroundImpactEffect(SDL_Renderer* ren, textures_s* textures)
{
    float alpha = 128;
    Uint32 currentTicks = SDL_GetTicks();
    for (int i = 0; i < playerCount; i++)
    {
        // If it has been less than 1000ms since tank[i] got hit
        if (currentTicks - textures->bullet.ticksAtLastCrater < 1000)
        {
            alpha = 255 - (float)(currentTicks - textures->bullet.ticksAtLastCrater) / 1000 * 255;
        }
    }

    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    renderAAFilledCircle(ren, textures->bullet.lastCraterPos.x, textures->bullet.lastCraterPos.y, (int)(20 * resolutionScale), 255, 0, 0, (int)alpha);
}

static void renderAAFilledCircle(SDL_Renderer* ren, int x, int y, int radius, int r, int g, int b, int a)
{
    SDL_Rect circleRect = {x - radius, y - radius, radius * 2, radius * 2};
    SDL_Texture* circle = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, circleRect.w, circleRect.h);
    SDL_SetTextureBlendMode(circle, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(circle, a);

    SDL_SetRenderTarget(ren, circle);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0); // set transparent background
    SDL_RenderClear(ren);

    aacircleRGBA(ren, (Sint16)radius, (Sint16)radius, (Sint16)(radius - 1), (Uint8)r, (Uint8)g, (Uint8)b, 255);
    aacircleRGBA(ren, (Sint16)radius, (Sint16)radius, (Sint16)(radius - 2), (Uint8)r, (Uint8)g, (Uint8)b, 255);
    filledCircleRGBA(ren, (Sint16)radius, (Sint16)radius, (Sint16)(radius - 2), (Uint8)r, (Uint8)g, (Uint8)b, 255);

    SDL_SetRenderTarget(ren, NULL);

    SDL_RenderCopy(ren, circle, NULL, &circleRect);
    SDL_DestroyTexture(circle);
}