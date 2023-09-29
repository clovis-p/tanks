//
// Created by clovis on 28/09/23.
//

#include <stdio.h>
#include <SDL2/SDL.h>

#include "render.h"

void render(SDL_Window** win, SDL_Renderer** ren)
{
    SDL_SetRenderDrawColor(*ren, 0, 0, 0, 255);
    SDL_RenderClear(*ren);

    SDL_SetRenderDrawColor(*ren, 255, 255, 255, 255);
    SDL_RenderDrawLine(*ren, 0, 0, 1280, 720);
    SDL_RenderDrawLine(*ren, 1280, 0, 0, 720);

    SDL_RenderPresent(*ren);
}