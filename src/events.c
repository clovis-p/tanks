//
// Created by clovis on 28/09/23.
//

#include <SDL2/SDL.h>

#include "main.h"

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

    //printf("tank: %d %d\nterrain: %d %d\n", (textures->tank1.rect.x + textures->tank1.rect.w / 2), (textures->tank1.rect.y + textures->tank1.rect.h), terrain->groundLevel[textures->tank1.rect.x + textures->tank1.rect.w / 2], terrain->groundLevel[textures->tank1.rect.y + textures->tank1.rect.h]);
    if (textures->tank1.rect.y + textures->tank1.rect.h < terrain->groundLevel[textures->tank1.rect.x + textures->tank1.rect.w / 2])
    {
        textures->tank1.rect.y += 2;
    }

    if (keyStates[SDL_SCANCODE_D])
    {
        textures->tank1.rect.x += 1;
    }
    if (keyStates[SDL_SCANCODE_A])
    {
        textures->tank1.rect.x -= 1;
    }

}