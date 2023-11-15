//
// Created by clovis on 28/09/23.
//

#include <SDL2/SDL.h>

#include "terrain.h"

terrain_s* generateTerrain(SDL_Renderer* ren, int width, int height, int type)
{
    terrain_s *terrain;

    if (type == TERRAIN_TYPE_MIDPOINT)
    {
        terrain = generateMidpointTerrain(ren, width, height, 7);
    }

    terrain->texture = SDL_CreateTexture(ren,
                                        SDL_PIXELFORMAT_RGBA8888,
                                        SDL_TEXTUREACCESS_TARGET,
                                        RESOLUTION_X,
                                        RESOLUTION_Y);
    SDL_SetTextureBlendMode(terrain->texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(terrain->texture, 255);

    SDL_SetRenderTarget(ren, terrain->texture);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);

    SDL_SetRenderDrawColor(ren, 34, 139, 34, 255);
    SDL_RenderDrawPoints(ren, terrain->sdlGroundPoints, terrain->groundPointsCount);

    SDL_SetRenderTarget(ren, NULL);

    return terrain;
}