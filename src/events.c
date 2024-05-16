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
#include "ui.h"

int showDebug;
int debugMode;

static SDL_Event event;

const Uint8 *keyStates;

void handleGlobalEvents(int *quit)
{
    keyStates = SDL_GetKeyboardState(NULL);

    // For some reason the keyboard keystates "stick", meaning the tank sometimes keeps moving
    // for a second after the key is released, and for some reason, calling SDL_PumpEvents() fixes this
    SDL_PumpEvents();
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT || (keyStates[SDL_SCANCODE_LALT] && keyStates[SDL_SCANCODE_F4]))
    {
        *quit = 1;
    }
}

void handleGameEvents(terrain_s* terrain, textures_s *textures)
{
    // tank
    if (!textures->bullet.active)
    {
        if (keyStates[SDL_SCANCODE_D])
        {
            moveTank(&textures->tank[turn], 0.075f * resolutionScale, terrain); // move right
        }
        if (keyStates[SDL_SCANCODE_A])
        {
            moveTank(&textures->tank[turn], -0.075f * resolutionScale, terrain); // move left
        }
        if (keyStates[SDL_SCANCODE_W])
        {
            rotateGunCounterClockwise(&textures->tank[turn].gun);
        }
        if (keyStates[SDL_SCANCODE_S])
        {
            rotateGunClockwise(&textures->tank[turn].gun);
        }
    }

    // bullet and turns
    static int fire = 0;

    if (keyStates[SDL_SCANCODE_SPACE] && !textures->bullet.active)
    {
        fire = 1;
    }
    else if (fire)
    {
        fireBullet(&textures->bullet, &textures->tank[turn]);
        fire = 0;

        turn++;
        turn %= playerCount;
    }

    // At any time, if it's a dead player's turn, skip before any other events are checked. This is necessary since the
    // turns are otherwise only updated when a bullet is shot, not when damage is done.
    while (!textures->tank[turn].isAlive)
    {
        turn++;
        turn %= playerCount;
    }

    // toggle debug overlay
    static int lockF3 = 0;

    if (keyStates[SDL_SCANCODE_F3] && !lockF3)
    {
        showDebug++;
        showDebug %= 2;
        lockF3 = 1;
    }
    else if (!keyStates[SDL_SCANCODE_F3])
    {
        // Once the key is released, unlock it so debug mode can be toggled again
        lockF3 = 0;
    }

    // debugMode can be used to change the game's behavior in various ways
    static int lockF4 = 0;

    if (keyStates[SDL_SCANCODE_F4] && !lockF4)
    {
        debugMode++;
        debugMode %= 10;
        printf("debugMode: %d\n", debugMode);
        lockF4 = 1;
    }
    else if (!keyStates[SDL_SCANCODE_F4])
    {
        // Once the key is released, unlock it so debug mode can be toggled again
        lockF4 = 0;
    }

    static int lockEsc = 0;

    if (keyStates[SDL_SCANCODE_ESCAPE] && !lockEsc)
    {
        gameState = 0;
        lockEsc = 1;
    }
    else if (!keyStates[SDL_SCANCODE_ESCAPE])
    {
        lockEsc = 0;
    }
}

void handleMenuEvents(menutextures_s* menuTextures)
{
    updateButton(menuTextures->startButton);

    static int lockEnter = 0;

    if (keyStates[SDL_SCANCODE_RETURN] && !lockEnter)
    {
        gameState = 1;
        lockEnter = 1;
    }
    else if (!keyStates[SDL_SCANCODE_RETURN])
    {
        lockEnter = 0;
    }
}