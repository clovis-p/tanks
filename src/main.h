//
// Created by clovis on 28/09/23.
//

#ifndef TANKS_MAIN_H
#define TANKS_MAIN_H

#include <SDL2/SDL.h>

typedef struct textureAndRect_s
{
    SDL_Texture* texture;
    SDL_Rect rect;
    int angle;
} textureAndRect_s;

typedef struct textures_s
{
    textureAndRect_s tank1;
} textures_s;

#endif //TANKS_MAIN_H
