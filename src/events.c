//
// Created by clovis on 28/09/23.
//

#include <stdio.h>

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
        //printf("tank1 angle: %d\n", textures->tank1.angle);
    }

    // tank1
    if (keyStates[SDL_SCANCODE_D])
    {
        moveTank(&textures->tank1, 0.075f, terrain); // move right
    }
    if (keyStates[SDL_SCANCODE_A])
    {
        moveTank(&textures->tank1, -0.075f, terrain); // move left
    }
    if (keyStates[SDL_SCANCODE_W])
    {
        rotateGunCounterClockwise(&textures->tank1.gun);
    }
    if (keyStates[SDL_SCANCODE_S])
    {
        rotateGunClockwise(&textures->tank1.gun);
    }

    // tank2
    if (keyStates[SDL_SCANCODE_RIGHT])
    {
        moveTank(&textures->tank2, 0.075f, terrain);
    }
    if (keyStates[SDL_SCANCODE_LEFT])
    {
        moveTank(&textures->tank2, -0.075f, terrain);
    }
    if (keyStates[SDL_SCANCODE_UP])
    {
        rotateGunCounterClockwise(&textures->tank2.gun);
    }
    if (keyStates[SDL_SCANCODE_DOWN])
    {
        rotateGunClockwise(&textures->tank2.gun);
    }

    // bullet
    static int fire = 0;

    if (keyStates[SDL_SCANCODE_SPACE])
    {
        fire = 1;
    }
    else if (fire)
    {
        if (!textures->bullet.active)
        {
            if (turn == 0)
            {
                fireBullet(&textures->bullet, &textures->tank1);
            }
            else if (turn == 1)
            {
                fireBullet(&textures->bullet, &textures->tank2);
            }
        }
        fire = 0;
        turn++;
        turn %= playerCount;
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

    /*
    if (keyStates[SDL_SCANCODE_T])
    {
        SDL_Delay(50);
    }
    else
    {
        SDL_Delay(10);
    }
    */
}