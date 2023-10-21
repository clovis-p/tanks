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
        SDL_Delay(10);

        updateBullet(&textures.bullet, terrain);

        render(&win, &ren, terrain, &textures);

        handleEvents(&quit, terrain, &textures);
    }

    quitGame(&textures, &terrain);
    quitSDL(&win, &ren);
    return 0;
}

void initGame(SDL_Renderer** ren, terrain_s** terrain, textures_s* textures)
{
    *terrain = generateTerrain(RESOLUTION_X, RESOLUTION_Y, TERRAIN_TYPE_MIDPOINT);

    // init tank1 base
    SDL_Surface* buffer = SDL_LoadBMP("../resources/images/red_tank.bmp");
    if (buffer == NULL)
    {
        printf("Failed to load red_tank.bmp: %s\n", SDL_GetError());
    }
    textures->tank1.baseTexture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    SDL_QueryTexture(textures->tank1.baseTexture, NULL, NULL, &textures->tank1.rect.w, &textures->tank1.rect.h);
    textures->tank1.rect.x = 0;
    textures->tank1.rect.y = 0;
    textures->tank1.angle = 0;
    textures->tank1.fPoint.x = 0;
    textures->tank1.fPoint.y = 0;


    // init tank1 combined texture
    textures->tank1.combinedTexture = SDL_CreateTexture(*ren,
                                                        SDL_PIXELFORMAT_RGBA8888,
                                                        SDL_TEXTUREACCESS_TARGET,
                                                        textures->tank1.rect.w,
                                                        textures->tank1.rect.h);
    SDL_SetTextureBlendMode(textures->tank1.combinedTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(textures->tank1.combinedTexture, 255);

    // tank1 gun
    buffer = SDL_LoadBMP("../resources/images/gun.bmp");
    if (buffer == NULL)
    {
        printf("Failed to load gun.bmp: %s\n", SDL_GetError());
    }
    textures->tank1.gun.texture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    SDL_QueryTexture(textures->tank1.gun.texture, NULL, NULL, &textures->tank1.gun.rect.w, &textures->tank1.gun.rect.h);
    textures->tank1.gun.rect.x = 0;
    textures->tank1.gun.rect.y = 6;
    textures->tank1.gun.angle = 0;

    // init tank2
    buffer = SDL_LoadBMP("../resources/images/blue_tank.bmp");
    if (buffer == NULL)
    {
        printf("Failed to load blue_tank.bmp: %s\n", SDL_GetError());
    }
    textures->tank2.baseTexture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    SDL_QueryTexture(textures->tank2.baseTexture, NULL, NULL, &textures->tank2.rect.w, &textures->tank2.rect.h);
    textures->tank2.rect.x = 0;
    textures->tank2.rect.y = 0;
    textures->tank2.angle = 0;
    textures->tank2.fPoint.x = 0;
    textures->tank2.fPoint.y = 0;

    // tank2 gun
    buffer = SDL_LoadBMP("../resources/images/gun.bmp");
    if (buffer == NULL)
    {
        printf("Failed to load gun.bmp: %s\n", SDL_GetError());
    }
    textures->tank2.gun.texture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    SDL_QueryTexture(textures->tank1.gun.texture, NULL, NULL, &textures->tank2.gun.rect.w, &textures->tank2.gun.rect.h);
    textures->tank2.gun.rect.x = 0;
    textures->tank2.gun.rect.y = 0;
    textures->tank2.gun.angle = 0;
    textures->tank2.gun.angle = 45;
    textures->tank2.gun.relativePosY = textures->tank1.gun.relativePosY;

    // init bullet
    textures->bullet.rect.w = 3;
    textures->bullet.rect.h = 3;
    textures->bullet.rect.x = 0;
    textures->bullet.rect.y = 0;
    textures->bullet.active = 0;
    textures->bullet.speedX = 1;
    textures->bullet.speedY = 1;
    textures->bullet.fPoint.x = 0;
    textures->bullet.fPoint.y = 0;
}

void quitGame(textures_s* textures, terrain_s** terrain)
{
    SDL_DestroyTexture(textures->tank1.baseTexture);
    SDL_DestroyTexture(textures->tank2.baseTexture);
    SDL_DestroyTexture(textures->tank1.combinedTexture);
    SDL_DestroyTexture(textures->tank2.combinedTexture);
    SDL_DestroyTexture(textures->tank1.gun.texture);
    SDL_DestroyTexture(textures->tank2.gun.texture);
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