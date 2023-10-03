//
// Created by clovis on 28/09/23.
//

#include <stdio.h>
#include <SDL2/SDL.h>

#include "main.h"
#include "render.h"

int angle = 0;

void render(SDL_Window** win, SDL_Renderer** ren, terrain_s* terrain, textures_s* textures) {
    SDL_SetRenderDrawColor(*ren, 135, 206, 235, 255);
    SDL_RenderClear(*ren);
    SDL_SetRenderDrawColor(*ren, 34, 139, 34, 255);

    SDL_RenderDrawPoints(*ren, terrain->sdlGroundPoints, terrain->groundPointsCount);

    angle++;
    SDL_RenderCopyEx(*ren, textures->tank1.texture, NULL, &textures->tank1.rect, angle, NULL, SDL_FLIP_NONE);

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