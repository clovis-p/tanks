//
// Created by clovis on 28/09/23.
//

#include <SDL2/SDL.h>

void handleEvents(int *quit) {
    static SDL_Event event;

    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
        *quit = 1;
    }

}