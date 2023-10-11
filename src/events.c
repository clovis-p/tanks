//
// Created by clovis on 28/09/23.
//

#include <SDL2/SDL.h>

#include "main.h"
#include "events.h"
#include "terrain/terrain.h"
#include "tank.h"
#include "bullet.h"

int showDebug;

void handleEvents(int *quit, terrain_s* terrain, textures_s *textures)
{
    static SDL_Event event;

    const Uint8 *keyStates = SDL_GetKeyboardState(NULL);

    // For some reason the keyboard keystates "stick", meaning the tank sometimes keeps moving
    // for a second after the key is released, and for some reason, calling SDL_PumpEvents() fixes this
    SDL_PumpEvents();
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT || (keyStates[SDL_SCANCODE_LALT] && keyStates[SDL_SCANCODE_F4]))
    {
        *quit = 1;
    }

    if (showDebug)
    {
        /*printf("tank: %d %d\nterrain: %d %d\n", (textures->tank1.rect.x + textures->tank1.rect.w / 2),
                                                       (textures->tank1.rect.y + textures->tank1.rect.h),
                                                       terrain->groundLevel[textures->tank1.rect.x + textures->tank1.rect.w / 2],
                                                       terrain->groundLevel[textures->tank1.rect.y + textures->tank1.rect.h]);*/
        printf("tank1 angle: %d\n", textures->tank1.angle);
    }

    // tank1
    if (keyStates[SDL_SCANCODE_D])
    {
        moveTank(&textures->tank1, 1, terrain);
    }
    if (keyStates[SDL_SCANCODE_A])
    {
        moveTank(&textures->tank1, -1, terrain);
    }

    // tank2
    if (keyStates[SDL_SCANCODE_RIGHT])
    {
        moveTank(&textures->tank2, 1, terrain);
    }
    if (keyStates[SDL_SCANCODE_LEFT])
    {
        moveTank(&textures->tank2, -1, terrain);
    }

    // bullet
    static int fire = 0;

    if (keyStates[SDL_SCANCODE_SPACE])
    {
        fire = 1;
    }
    else if (fire)
    {
        fireBullet(&textures->bullet, &textures->tank1);
        fire = 0;
    }

    // debug
    if (keyStates[SDL_SCANCODE_LALT] && keyStates[SDL_SCANCODE_LSHIFT])
    {
        showDebug = 1;
    }
    else
    {
        showDebug = 0;
    }

}