//
// Created by clovis on 28/09/23.
//

#include <stdio.h>
#include <SDL2/SDL.h>

#include "main.h"
#include "render.h"
#include "terrain/terrain.h"
#include "events.h"

static void renderArray(SDL_Window** win, SDL_Renderer** ren, int** array, int x, int y, int width, int height, SDL_Color fg, SDL_Color bg);

void render(SDL_Window** win, SDL_Renderer** ren, terrain_s* terrain, textures_s* textures) {
    // sky
    SDL_SetRenderDrawColor(*ren, 135, 206, 235, 255);
    SDL_RenderClear(*ren);

    // bullet
    if (textures->bullet.active)
    {
        SDL_SetRenderDrawColor(*ren, 0, 0, 0, 255);
        SDL_RenderFillRect(*ren, &textures->bullet.rect);
    }

    // ground
    SDL_SetRenderDrawColor(*ren, 34, 139, 34, 255);
    SDL_RenderDrawPoints(*ren, terrain->sdlGroundPoints, terrain->groundPointsCount);

    // debug, showDebug = 1 while left shift and left alt are held down, otherwise showDebug = 0
    if (showDebug)
    {
        SDL_SetRenderDrawColor(*ren, 255, 0, 0, 255);
        SDL_RenderDrawPoints(*ren, terrain->debugPoints, terrain->debugPointsCount);

        SDL_RenderDrawRect(*ren, &textures->tank1.rect);
        SDL_RenderDrawRect(*ren, &textures->tank2.rect);
    }

    // Set render target to tank1 texture
    SDL_SetRenderTarget(*ren, textures->tank1.combinedTexture);

    // Clear tank1 texture, then render tank1 base and tank1 gun to tank1 texture
    SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0); // set transparent background
    SDL_RenderClear(*ren);
    SDL_RenderCopyEx(*ren,
                     textures->tank1.gun.texture,
                     NULL,
                     &textures->tank1.gun.rect,
                     textures->tank1.gun.angle,
                     NULL,
                     SDL_FLIP_NONE);
    SDL_RenderCopy(*ren, textures->tank1.baseTexture, NULL, NULL);

    // Set render target to tank2 texture
    SDL_SetRenderTarget(*ren, textures->tank2.combinedTexture);

    // Clear tank2 texture, then render tank2 base and tank2 gun to tank2 texture
    SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0); // set transparent background
    SDL_RenderClear(*ren);
    SDL_RenderCopyEx(*ren,
                     textures->tank2.gun.texture,
                     NULL,
                     &textures->tank2.gun.rect,
                     textures->tank2.gun.angle,
                     NULL,
                     SDL_FLIP_NONE);
    SDL_RenderCopy(*ren, textures->tank2.baseTexture, NULL, NULL);

    // Reset render target
    SDL_SetRenderTarget(*ren, NULL);

    // set tank center point to the bottom of the tank so it stays level with the ground when on a hill
    SDL_Point tankCenterPoint = {textures->tank1.rect.w / 2, textures->tank1.rect.h};

    // Render tank1
    SDL_RenderCopyEx(*ren,
                     textures->tank1.combinedTexture,
                     NULL,
                     &textures->tank1.rect,
                     textures->tank1.angle,
                     &tankCenterPoint,
                     SDL_FLIP_NONE);

    // Render tank2
    SDL_RenderCopyEx(*ren,
                     textures->tank2.combinedTexture,
                     NULL,
                     &textures->tank2.rect,
                     textures->tank2.angle,
                     &tankCenterPoint,
                     SDL_FLIP_NONE);

    SDL_RenderPresent(*ren);
}

static void renderArray(SDL_Window** win, SDL_Renderer** ren, int** array, int x, int y, int width, int height, SDL_Color fg, SDL_Color bg)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (array[i][j] == 1)
            {
                SDL_SetRenderDrawColor(*ren, fg.r, fg.g, fg.b, fg.a);
                SDL_RenderDrawPoint(*ren, i + x, j + y);
            }
            else if (bg.a != 0)
            {
                SDL_SetRenderDrawColor(*ren, bg.r, bg.g, bg.b, bg.a);
                SDL_RenderDrawPoint(*ren, i + x, j + y);
            }
        }
    }
}