#include <stdio.h>
#include <SDL2/SDL.h>

#include "main.h"
#include "render.h"
#include "events.h"
#include "terraingen.h"

const int RESOLUTION_X = 1280;
const int RESOLUTION_Y = 720;

int initSDL(SDL_Window** win, SDL_Renderer** ren);
void quitSDL(SDL_Window** win, SDL_Renderer** ren);
void initGame(int*** terrain);

int main(int argc, char *argv[])
{
    SDL_Window *win;
    SDL_Renderer *ren;
    int **terrain;

    int quit = 0;

    if (initSDL(&win, &ren) < 0)
    {
        return 1;
    }

    initGame(&terrain);

    while (!quit)
    {
        SDL_Delay(10);
        render(&win, &ren, terrain);
        handleEvents(&quit);
    }

    freeTerrain(terrain, RESOLUTION_X);
    quitSDL(&win, &ren);
    return 0;
}

void initGame(int*** terrain)
{
    *terrain = generateTerrain(RESOLUTION_X, RESOLUTION_Y);
}

int initSDL(SDL_Window** win, SDL_Renderer** ren)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initSDL SDL: %s\n", SDL_GetError());
        return -1;
    }

    *win = SDL_CreateWindow("test",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            1280, 720,
                            SDL_WINDOW_SHOWN);

    if (!*win)
    {
        printf("Failed to create window: %s\n", SDL_GetError());
        return -1;
    }

    *ren = SDL_CreateRenderer(*win, -1, 0);

    if (!*ren)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}

void quitSDL(SDL_Window** win, SDL_Renderer** ren)
{
    SDL_DestroyRenderer(*ren);
    SDL_DestroyWindow(*win);
    SDL_Quit();
}