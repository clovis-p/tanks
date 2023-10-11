//
// Created by clovis on 28/09/23.
//

#ifndef TANKS_MAIN_H
#define TANKS_MAIN_H

#include <SDL2/SDL.h>

#define RESOLUTION_X 1280
#define RESOLUTION_Y 720

typedef struct bullet_s
{
    SDL_Rect rect;
    int active;
    int speedX;
    int speedY;
} bullet_s;

typedef struct textureAndRect_s
{
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_FPoint fPoint;
    int angle;
} textureAndRect_s;

typedef struct textures_s
{
    textureAndRect_s tank1;
    textureAndRect_s tank2;
    bullet_s bullet;
} textures_s;

#endif //TANKS_MAIN_H
