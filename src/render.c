//
// Created by clovis on 28/09/23.
//

#include <stdio.h>
#include <SDL2/SDL.h>

#include "main.h"
#include "render.h"
#include "terrain/terrain.h"
#include "events.h"

void render(SDL_Window** win, SDL_Renderer** ren, terrain_s* terrain, textures_s* textures) {
    // sky
    SDL_SetRenderDrawColor(*ren, 135, 206, 235, 255);
    SDL_RenderClear(*ren);

    // ground
    SDL_SetRenderDrawColor(*ren, 34, 139, 34, 255);
    SDL_RenderDrawPoints(*ren, terrain->sdlGroundPoints, terrain->groundPointsCount);

    // debug points
    if (showDebug)
    {
        SDL_SetRenderDrawColor(*ren, 255, 0, 0, 255);
        SDL_RenderDrawPoints(*ren, terrain->debugPoints, terrain->debugPointsCount);
    }

    // tank1 gun
    SDL_RenderCopyEx(*ren, textures->tank1.gun.texture, NULL, &textures->tank1.gun.rect, textures->tank1.gun.angle, NULL, SDL_FLIP_NONE);

    // tank 1
    SDL_RenderCopyEx(*ren, textures->tank1.texture, NULL, &textures->tank1.rect, textures->tank1.angle, NULL, SDL_FLIP_NONE);

    // tank2 gun
    SDL_RenderCopyEx(*ren, textures->tank2.gun.texture, NULL, &textures->tank2.gun.rect, textures->tank2.gun.angle, NULL, SDL_FLIP_NONE);

    // tank 2
    SDL_RenderCopyEx(*ren, textures->tank2.texture, NULL, &textures->tank2.rect, textures->tank2.angle, NULL, SDL_FLIP_NONE);

    // bullet
    if (textures->bullet.active)
    {
        SDL_SetRenderDrawColor(*ren, 0, 0, 0, 255);
        SDL_RenderFillRect(*ren, &textures->bullet.rect);
    }

    SDL_RenderPresent(*ren);
}

void renderArray(SDL_Window** win, SDL_Renderer** ren, int** array, int x, int y, int width, int height, SDL_Color fg, SDL_Color bg)
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