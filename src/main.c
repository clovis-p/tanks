#include <stdio.h>
#include <SDL2/SDL.h>

#include "render.h"

int init(SDL_Window** win, SDL_Renderer** ren);

int main(int argc, char *argv[])
{
    SDL_Window* win;
    SDL_Renderer* ren;

    int quit = 0;

    if (init(&win, &ren) < 0)
    {
        return 1;
    }

    while (quit < 200)
    {
        SDL_Delay(10);
        render(&win, &ren);
        quit++;
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
    return 0;
}

int init(SDL_Window** win, SDL_Renderer** ren)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to init SDL: %s\n", SDL_GetError());
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

