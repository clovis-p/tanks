//
// Created by clovis on 28/09/23.
//

#ifndef TANKS_MAIN_H
#define TANKS_MAIN_H

#include <SDL2/SDL.h>

#define RESOLUTION_X 1280
#define RESOLUTION_Y 720

typedef struct gun_s
{
    SDL_Texture* texture;
    SDL_Rect rect;
    int angle; // rendered angle
    int relativeAngle; // angle relative to the tank assuming the tank is straight
    int relativePosY; // Y position relative to the tank
    // (relative X position is always 0)
} gun_s;

typedef struct bullet_s
{
    SDL_Rect rect;
    int active;
    int speedX;
    int speedY;
} bullet_s;

typedef struct tank_s
{
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_FPoint fPoint;
    int angle;
    gun_s gun;
} tank_s;

typedef struct textures_s
{
    tank_s tank1;
    tank_s tank2;
    bullet_s bullet;
} textures_s;

#endif //TANKS_MAIN_H
