//
// Created by clovis on 11/10/23.
//

#ifndef TANKS_TANK_H
#define TANKS_TANK_H

#include "main.h"

void initTank(SDL_Renderer** ren, textures_s* textures, int id);
void initHealthBars(healthBar_s* healthBar);
void initAllTanksHitboxes(tank_s tanks[]);
void moveTank(tank_s* tank, float amount, terrain_s* terrain);
void rotateGunClockwise(gun_s* gun);
void rotateGunCounterClockwise(gun_s* gun);
void teleportTank(tank_s* tank, float x, terrain_s* terrain);
void checkTankCollisionWithBullet(tank_s* tank, bullet_s* bullet);
void resetAllTanksHitboxStates(tank_s tanks[]);
void applyDamageToTank(tank_s tanks[], int tankIndex, int damage);

#endif //TANKS_TANK_H
