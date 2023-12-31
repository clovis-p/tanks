//
// Created by clovis on 11/10/23.
//

#ifndef TANKS_BULLET_H
#define TANKS_BULLET_H

#include <SDL2/SDL.h>

#include "main.h"
#include "terrain/terrain.h"

void initBullet(bullet_s* bullet);
void fireBullet(bullet_s* bullet, tank_s* tank);
void updateBullet(textures_s* textures, terrain_s* terrain);
double degToRad(float deg);

#endif //TANKS_BULLET_H
