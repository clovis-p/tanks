//
// Created by clovis on 28/09/23.
//

#ifndef SDLTEST_RENDER_H
#define SDLTEST_RENDER_H

#include "terrain/terrain.h"
#include "ui.h"

void render(SDL_Window** win, SDL_Renderer** ren, terrain_s* terrain, textures_s* textures);
void renderMenu(SDL_Window** win, SDL_Renderer** ren, menutextures_s* menuTextures);
void renderButton(SDL_Renderer* ren, button_s* button);

#endif //SDLTEST_RENDER_H
