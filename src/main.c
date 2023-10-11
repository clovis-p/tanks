#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "main.h"
#include "render.h"
#include "events.h"
#include "terrain/terrain.h"
#include "bullet.h"

int initSDL(SDL_Window** win, SDL_Renderer** ren);
void quitSDL(SDL_Window** win, SDL_Renderer** ren);
void quitGame(textures_s* textures, terrain_s** terrain);
void initGame(SDL_Renderer** ren, terrain_s** terrain, textures_s* textures);

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    printf("Hello World!\n");
    SDL_Window *win;
    SDL_Renderer *ren;
    terrain_s *terrain;

    textures_s textures;
    int ticks = 0;

    int quit = 0;

    if (initSDL(&win, &ren) < 0)
    {
        return 1;
    }

    initGame(&ren, &terrain, &textures);

    while (!quit)
    {
        //ticks = SDL_GetTicks();
        SDL_Delay(10);
        //printf("SDL_Delay: %d\n", SDL_GetTicks() - ticks);

        updateBullet(&textures.bullet);

        //ticks = SDL_GetTicks();
        render(&win, &ren, terrain, &textures);
        //printf("render: %d\n", SDL_GetTicks() - ticks);

        //ticks = SDL_GetTicks();
        handleEvents(&quit, terrain, &textures);
        //printf("handleEvents: %d\n", SDL_GetTicks() - ticks);
    }

    quitGame(&textures, &terrain);
    quitSDL(&win, &ren);
    return 0;
}

void initGame(SDL_Renderer** ren, terrain_s** terrain, textures_s* textures)
{
    *terrain = generateTerrain(RESOLUTION_X, RESOLUTION_Y, TERRAIN_TYPE_MIDPOINT);

    // init tank1
    SDL_Surface* buffer = SDL_LoadBMP("../resources/images/red_tank.bmp");
    if (buffer == NULL)
    {
        printf("Failed to load red_tank.bmp: %s\n", SDL_GetError());
    }
    textures->tank1.texture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    SDL_QueryTexture(textures->tank1.texture, NULL, NULL, &textures->tank1.rect.w, &textures->tank1.rect.h);
    textures->tank1.rect.x = 0;
    textures->tank1.rect.y = 0;
    textures->tank1.angle = 0;
    textures->tank1.fPoint.x = 0;
    textures->tank1.fPoint.y = 0;

    // init tank2
    buffer = SDL_LoadBMP("../resources/images/blue_tank.bmp");
    if (buffer == NULL)
    {
        printf("Failed to load blue_tank.bmp: %s\n", SDL_GetError());
    }
    textures->tank2.texture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    SDL_QueryTexture(textures->tank2.texture, NULL, NULL, &textures->tank2.rect.w, &textures->tank2.rect.h);
    textures->tank2.rect.x = 0;
    textures->tank2.rect.y = 0;
    textures->tank2.angle = 0;
    textures->tank2.fPoint.x = 0;
    textures->tank2.fPoint.y = 0;

    // init bullet
    textures->bullet.rect.w = 3;
    textures->bullet.rect.h = 3;
    textures->bullet.rect.x = 0;
    textures->bullet.rect.y = 0;
    textures->bullet.active = 0;
    textures->bullet.speedX = 1;
    textures->bullet.speedY = 1;
}

void quitGame(textures_s* textures, terrain_s** terrain)
{
    SDL_DestroyTexture(textures->tank1.texture);
    SDL_DestroyTexture(textures->tank2.texture);
    free(*terrain);
}

int initSDL(SDL_Window** win, SDL_Renderer** ren)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to init SDL SDL: %s\n", SDL_GetError());
        return -1;
    }

    *win = SDL_CreateWindow("tanks",
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