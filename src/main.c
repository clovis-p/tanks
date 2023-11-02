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
int playerCount = 2;

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);
    SDL_Window *win;
    SDL_Renderer *ren;
    terrain_s *terrain;

    textures_s textures;
    Uint32 ticks = 0;

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

        //printf("deltaTime: %d\n%f fps\n", deltaTime, 1000 / (float)deltaTime);

        SDL_Delay(5);

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
    *terrain = generateTerrain(RESOLUTION_X, RESOLUTION_Y, TERRAIN_TYPE_MIDPOINT);

    // init tank[0] base
    SDL_Surface* buffer = SDL_LoadBMP("../resources/images/red_tank.bmp");
    if (buffer == NULL)
    {
        printf("Failed to load red_tank.bmp: %s\n", SDL_GetError());
    }
    textures->tank[0].baseTexture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    SDL_QueryTexture(textures->tank[0].baseTexture, NULL, NULL, &textures->tank[0].rect.w, &textures->tank[0].rect.h);
    textures->tank[0].rect.x = 0;
    textures->tank[0].rect.y = 0;
    textures->tank[0].angle = 0;
    textures->tank[0].fPoint.x = 0;
    textures->tank[0].fPoint.y = 0;
    textures->tank[0].health = 100;
    textures->tank[0].isInvincible = 0;
    textures->tank[0].id = 0;


    // init tank[0] combined texture
    textures->tank[0].combinedTexture = SDL_CreateTexture(*ren,
                                                        SDL_PIXELFORMAT_RGBA8888,
                                                        SDL_TEXTUREACCESS_TARGET,
                                                        textures->tank[0].rect.w,
                                                        textures->tank[0].rect.h);
    SDL_SetTextureBlendMode(textures->tank[0].combinedTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(textures->tank[0].combinedTexture, 255);

    // tank[0] gun
    buffer = SDL_LoadBMP("../resources/images/gun.bmp");
    if (buffer == NULL)
    {
        printf("Failed to load gun.bmp: %s\n", SDL_GetError());
    }
    textures->tank[0].gun.texture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    SDL_QueryTexture(textures->tank[0].gun.texture, NULL, NULL, &textures->tank[0].gun.rect.w, &textures->tank[0].gun.rect.h);
    textures->tank[0].gun.rect.x = 0;
    textures->tank[0].gun.rect.y = 6;
    textures->tank[0].gun.angle = 80;
    textures->tank[0].gun.fAngle = (float)textures->tank[0].gun.angle;

    // init tank[1]
    buffer = SDL_LoadBMP("../resources/images/blue_tank.bmp");
    if (buffer == NULL)
    {
        printf("Failed to load blue_tank.bmp: %s\n", SDL_GetError());
    }
    textures->tank[1].baseTexture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    SDL_QueryTexture(textures->tank[1].baseTexture, NULL, NULL, &textures->tank[1].rect.w, &textures->tank[1].rect.h);
    textures->tank[1].rect.x = 0;
    textures->tank[1].rect.y = 0;
    textures->tank[1].angle = 0;
    textures->tank[1].fPoint.x = 0;
    textures->tank[1].fPoint.y = 0;
    textures->tank[1].health = 100;
    textures->tank[1].isInvincible = 0;
    textures->tank[1].id = 1;

    // init tank[1] combined texture
    textures->tank[1].combinedTexture = SDL_CreateTexture(*ren,
                                                        SDL_PIXELFORMAT_RGBA8888,
                                                        SDL_TEXTUREACCESS_TARGET,
                                                        textures->tank[1].rect.w,
                                                        textures->tank[1].rect.h);
    SDL_SetTextureBlendMode(textures->tank[1].combinedTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(textures->tank[1].combinedTexture, 255);

    // tank[1] gun
    buffer = SDL_LoadBMP("../resources/images/gun.bmp");
    if (buffer == NULL)
    {
        printf("Failed to load gun.bmp: %s\n", SDL_GetError());
    }
    textures->tank[1].gun.texture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    SDL_QueryTexture(textures->tank[0].gun.texture, NULL, NULL, &textures->tank[1].gun.rect.w, &textures->tank[1].gun.rect.h);
    textures->tank[1].gun.rect.x = 0;
    textures->tank[1].gun.rect.y = 6;
    textures->tank[1].gun.angle = -80;
    textures->tank[1].gun.fAngle = (float)textures->tank[1].gun.angle;
    textures->tank[1].gun.relativePosY = textures->tank[0].gun.relativePosY;

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
    textures->bullet.harmlessToShooter = 1;

    // init hitboxes
    resetAllTanksHitboxStates(textures->tank);

    // init health bars
    textures->tank[0].healthBar.fRect.w = 30;
    textures->tank[0].healthBar.fRect.h = 8;
    textures->tank[0].healthBar.rect.w = 30;
    textures->tank[0].healthBar.rect.h = 8;

    textures->tank[1].healthBar.fRect.w = 30;
    textures->tank[1].healthBar.fRect.h = 8;
    textures->tank[1].healthBar.rect.w = 30;
    textures->tank[1].healthBar.rect.h = 8;

    // Set tank spawn points
    // tank[0] spawn point
    teleportTank(&textures->tank[0], 150, *terrain);

    // tank[1] spawn point
    teleportTank(&textures->tank[1], 1280 - 150, *terrain);
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

static void quitSDL(SDL_Window** win, SDL_Renderer** ren)
{
    SDL_DestroyRenderer(*ren);
    SDL_DestroyWindow(*win);
    SDL_Quit();
}