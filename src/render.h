//
// Created by clovis on 28/09/23.
//

#ifndef SDLTEST_RENDER_H
#define SDLTEST_RENDER_H

void render(SDL_Window** win, SDL_Renderer** ren, int** terrain);
void renderArray(SDL_Window** win, SDL_Renderer** ren, int** array, int x, int y, int width, int height, SDL_Color fg, SDL_Color bg);

#endif //SDLTEST_RENDER_H
