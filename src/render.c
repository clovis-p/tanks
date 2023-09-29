//
// Created by clovis on 28/09/23.
//

#include <stdio.h>
#include <SDL2/SDL.h>

#include "render.h"

void render(SDL_Window** win, SDL_Renderer** ren, int** terrain)
{
    SDL_SetRenderDrawColor(*ren, 0, 0, 0, 255);
    SDL_RenderClear(*ren);
    SDL_SetRenderDrawColor(*ren, 255, 255, 255, 255);

    for (int i = 0; i < 1280; i++)
    {
        int isBelowGround = 0;
        for (int j = 0; j < 720; j++)
        {
            if (terrain[i][j] == 1 || isBelowGround)
            {
                SDL_RenderDrawPoint(*ren, i, j);
                isBelowGround = 1;
            }
        }
    }

    SDL_RenderPresent(*ren);
}