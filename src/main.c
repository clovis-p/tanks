#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "main.h"
#include "render.h"
#include "events.h"
#include "terrain/terrain.h"
#include "bullet.h"
#include "tank.h"

static int initSDL(SDL_Window** win, SDL_Renderer** ren);
static void quitSDL(SDL_Window** win, SDL_Renderer** ren);
static void quitGame(textures_s* textures, terrain_s** terrain);
static void initGame(SDL_Renderer** ren, terrain_s** terrain, textures_s* textures);

Uint32 deltaTime = 0;

int turn = 0;
int playerCount = 4;

float resolutionScale;

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    SDL_Window *win;
    SDL_Renderer *ren;
    terrain_s *terrain;

    textures_s textures;
    Uint32 ticks = 0;

    resolutionScale = (float)RESOLUTION_X / 1280;
    if (resolutionScale != (float)RESOLUTION_Y / 720)
    {
        printf("Warning: resolution not 16:9\n");
    }

    printf("resolutionScale: %f\n", resolutionScale);

    int quit = 0;

    if (initSDL(&win, &ren) < 0)
    {
        return 1;
    }

    initGame(&ren, &terrain, &textures);

    while (!quit)
    {
        deltaTime = SDL_GetTicks() - ticks;
        ticks = SDL_GetTicks();

        printf("deltaTime: %d, %d fps         \r", deltaTime, (int)(1000 / (float)deltaTime));

        // Cap framerate to 250fps
        int delay = 5 - (int)deltaTime;
        if (delay > 0)
        {
            SDL_Delay(delay);
        }

        updateBullet(&textures, terrain);

        render(&win, &ren, terrain, &textures);

        handleEvents(&quit, terrain, &textures);
    }

    quitGame(&textures, &terrain);
    quitSDL(&win, &ren);
    return 0;
}

static void initGame(SDL_Renderer** ren, terrain_s** terrain, textures_s* textures)
{
    *terrain = generateTerrain(*ren, RESOLUTION_X, RESOLUTION_Y, TERRAIN_TYPE_MIDPOINT);

    for (int i = 0; i < playerCount; i++)
    {
        initTank(ren, textures, i);
    }

    // init bullet
    initBullet(&textures->bullet);

    // init hitboxes
    initAllTanksHitboxes(textures->tank);

    // init health bars
    for (int i = 0; i < playerCount; i++)
    {
        initHealthBars(&textures->tank[i].healthBar);
    }

    // Set tank spawn points
    if (playerCount >= 1)
    {
        teleportTank(&textures->tank[0], (float)RESOLUTION_X / 5, *terrain);
    }
    if (playerCount >= 2)
    {
        teleportTank(&textures->tank[1], (float)RESOLUTION_X / 5 * 4, *terrain);
    }
    if (playerCount >= 3)
    {
        teleportTank(&textures->tank[2], (float)RESOLUTION_X / 5 * 2, *terrain);
    }
    if (playerCount >= 4)
    {
        teleportTank(&textures->tank[3], (float)RESOLUTION_X / 5 * 3, *terrain);
    }
}

static void quitGame(textures_s* textures, terrain_s** terrain)
{
    SDL_DestroyTexture(textures->tank[0].baseTexture);
    SDL_DestroyTexture(textures->tank[1].baseTexture);
    SDL_DestroyTexture(textures->tank[0].combinedTexture);
    SDL_DestroyTexture(textures->tank[1].combinedTexture);
    SDL_DestroyTexture(textures->tank[0].gun.texture);
    SDL_DestroyTexture(textures->tank[1].gun.texture);
    free(*terrain);
}

static int initSDL(SDL_Window** win, SDL_Renderer** ren)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to init SDL SDL: %s\n", SDL_GetError());
        return -1;
    }

    *win = SDL_CreateWindow("tanks",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            RESOLUTION_X, RESOLUTION_Y,
                            SDL_WINDOW_SHOWN);

    if (!*win)
    {
        printf("Failed to create window: %s\n", SDL_GetError());
        return -1;
    }

    *ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED);

    if (!*ren)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return -1;
    }

    // Linear filtering
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    return 0;
}

static void quitSDL(SDL_Window** win, SDL_Renderer** ren)
{
    SDL_DestroyRenderer(*ren);
    SDL_DestroyWindow(*win);
    SDL_Quit();
}