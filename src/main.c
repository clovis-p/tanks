#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#include "main.h"
#include "render.h"
#include "events.h"
#include "terrain/terrain.h"
#include "bullet.h"
#include "tank.h"
#include "ui.h"

static int initSDL(SDL_Window** win, SDL_Renderer** ren);
static void quitSDL(SDL_Window** win, SDL_Renderer** ren);
static void quitGame(textures_s* textures, menutextures_s* menuTextures, terrain_s** terrain);
static void initMenu(SDL_Renderer* ren, menutextures_s* menuTextures);
static void initGame(SDL_Renderer** ren, terrain_s** terrain, textures_s* textures);

Uint32 deltaTime = 0;

int turn = 0;
int playerCount = 4;

int gameState = 0; // 0 = main menu, 1 = game initializing, 2 = game in progress

float resolutionScale;

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    SDL_Window *win;
    SDL_Renderer *ren;
    terrain_s *terrain;

    textures_s textures;
    menutextures_s menuTextures;
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

    initMenu(ren, &menuTextures);
    initGame(&ren, &terrain, &textures);
    gameState = 0;

    while (!quit)
    {
        deltaTime = SDL_GetTicks() - ticks;
        ticks = SDL_GetTicks();

        // Cap framerate to 250fps
        int delay = 5 - (int)deltaTime;
        if (delay > 0)
        {
            SDL_Delay(delay);
        }

        handleEvents(&quit, terrain, &textures, &menuTextures);

        if (gameState == 0)
        {
            renderMenu(&win, &ren, &menuTextures);
        }
        else if (gameState == 1)
        {
            initGame(&ren, &terrain, &textures);
        }
        else if (gameState == 2)
        {
            updateBullet(&textures, terrain);
            render(&win, &ren, terrain, &textures);
        }
        else
        {
            printf("Something went horribly wrong! (invalid game state)");
        }
    }

    quitGame(&textures, &menuTextures, &terrain);
    quitSDL(&win, &ren);
    return 0;
}

static void initGame(SDL_Renderer** ren, terrain_s** terrain, textures_s* textures)
{
    turn = 0;

    *terrain = generateTerrain(*ren, RESOLUTION_X, RESOLUTION_Y, TERRAIN_TYPE_MIDPOINT, (int)time(NULL));

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

    gameState = 2;
}

static void initMenu(SDL_Renderer* ren, menutextures_s* menuTextures)
{
    menuTextures->uiBg.r = 0;
    menuTextures->uiBg.g = 0;
    menuTextures->uiBg.b = 0;
    menuTextures->uiBg.a = 255;

    menuTextures->uiFg.r = 255;
    menuTextures->uiFg.g = 255;
    menuTextures->uiFg.b = 255;
    menuTextures->uiFg.a = 255;

    menuTextures->titleFont = TTF_OpenFont("../resources/fonts/DeltaBlock/DeltaBlock-Regular.ttf", RESOLUTION_Y / 4);
    menuTextures->freeSans = TTF_OpenFont("../resources/fonts/freefont/FreeSans.ttf", RESOLUTION_Y / 10);
    menuTextures->title = createTextTexture(&menuTextures->titleRect, ren, "TANKS",
                                            menuTextures->titleFont, menuTextures->uiFg);
    menuTextures->titleRect.x = RESOLUTION_X / 2 - menuTextures->titleRect.w / 2;
    menuTextures->titleRect.y = RESOLUTION_Y / 4 - menuTextures->titleRect.h / 2;

    menuTextures->startButton = createArrowButton(ren, menuTextures->uiFg, menuTextures->uiBg, RESOLUTION_X / 2, RESOLUTION_Y / 8 * 5,
                                                  RESOLUTION_X / 7, RESOLUTION_X / 7, ARROW_DIRECTION_RIGHT);

    char buf[2];
    if (playerCount <= 4)
    {
        sprintf(buf, "%d", playerCount);
    }
    menuTextures->playerCountDisplay = createTextTexture(&menuTextures->playerCountDisplayRect,
                                                         ren, buf, menuTextures->freeSans, menuTextures->uiFg);
    menuTextures->playerCountDisplayRect.x = RESOLUTION_X / 4 - menuTextures->playerCountDisplayRect.w / 2;
    menuTextures->playerCountDisplayRect.y = RESOLUTION_Y / 2 - menuTextures->playerCountDisplayRect.h / 2;

    menuTextures->increasePlayerCountButton = createArrowButton(ren, menuTextures->uiFg, menuTextures->uiBg,
                                                                RESOLUTION_X / 4,
                                                                RESOLUTION_Y / 2 - RESOLUTION_X / 14,
                                                                RESOLUTION_X / 14, RESOLUTION_X / 14,
                                                                ARROW_DIRECTION_UP);
    menuTextures->decreasePlayerCountButton = createArrowButton(ren, menuTextures->uiFg, menuTextures->uiBg,
                                                                RESOLUTION_X / 4,
                                                                RESOLUTION_Y / 2 + RESOLUTION_X / 14,
                                                                RESOLUTION_X / 14, RESOLUTION_X / 14,
                                                                ARROW_DIRECTION_DOWN);
    menuTextures->updatePlayerCount = 0;
}

static void quitGame(textures_s* textures, menutextures_s* menuTextures, terrain_s** terrain)
{
    SDL_DestroyTexture(menuTextures->title);
    SDL_DestroyTexture(menuTextures->playerCountDisplay);
    destroyButton(menuTextures->startButton);
    destroyButton(menuTextures->increasePlayerCountButton);
    destroyButton(menuTextures->decreasePlayerCountButton);

    for (int i = 0; i < 4; i++)
    {
        SDL_DestroyTexture(textures->tank[i].baseTexture);
        SDL_DestroyTexture(textures->tank[i].combinedTexture);
        SDL_DestroyTexture(textures->tank[i].gun.texture);
    }
    free(*terrain);
}

static int initSDL(SDL_Window** win, SDL_Renderer** ren)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to init SDL: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() < 0)
    {
        printf("Failed to init SDL_ttf: %s\n", TTF_GetError());
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

    TTF_Quit();
    SDL_Quit();
}