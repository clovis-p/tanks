//
// Created by clovis on 28/09/23.
//

#include <SDL2/SDL.h>

#include "main.h"

void handleEvents(int *quit, int **terrain, textures_s *textures)
{
    static SDL_Event event;

    const Uint8 *keyStates = SDL_GetKeyboardState(NULL);

    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT || (keyStates[SDL_SCANCODE_LALT] && keyStates[SDL_SCANCODE_F4]))
    {
        *quit = 1;
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