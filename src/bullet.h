//
// Created by clovis on 11/10/23.
//

#ifndef TANKS_BULLET_H
#define TANKS_BULLET_H

#include <SDL2/SDL.h>

#include "main.h"

void fireBullet(bullet_s* bullet, tank_s* tank);
void updateBullet(bullet_s* bullet);
void updateBulletPos(bullet_s* bullet);
int bulletIsOutOfBounds(bullet_s* bullet);

#endif //TANKS_BULLET_H
