//
// Created by clovis on 11/10/23.
//

#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "main.h"

void moveTank(tank_s* tank, float amount, terrain_s* terrain);
void updateTankGunAngle(tank_s* tank);
void rotateGunClockwise(gun_s* gun);
void rotateGunCounterClockwise(gun_s* gun);
void teleportTank(tank_s* tank, int x, terrain_s* terrain);
void checkTankCollisionWithBullet(tank_s* tank, bullet_s* bullet);
void resetAllTanksHitboxStates(tank_s tanks[]);

#endif //TANKS_TANK_H
