//
// Created by clovis on 11/10/23.
//

#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "main.h"

void calculateTankAngle(tank_s* textures, terrain_s* terrain);
void moveTank(tank_s* tank, int amount, terrain_s* terrain);
void updateTankGunAngle(tank_s* tank);
void rotateGunClockwise(gun_s* gun);
void rotateGunCounterClockwise(gun_s* gun);

#endif //TANKS_TANK_H
