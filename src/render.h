//
// Created by clovis on 28/09/23.
//

#ifndef SDLTEST_RENDER_H
#define SDLTEST_RENDER_H

#include "terrain/terrain.h"

void render(SDL_Window** win, SDL_Renderer** ren, terrain_s* terrain, textures_s* textures);
void renderArray(SDL_Window** win, SDL_Renderer** ren, int** array, int x, int y, int width, int height, SDL_Color fg, SDL_Color bg);

#endif //SDLTEST_RENDER_H
