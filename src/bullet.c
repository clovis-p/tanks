//
// Created by clovis on 11/10/23.
//

#include <stdio.h>
#include <math.h>

#include "bullet.h"
#include "main.h"
#include "terrain/terrain.h"
#include "tank.h"

static void calculateBulletXYSpeed(bullet_s *bullet, tank_s *tank, float speed);
static void calculateBulletOriginPoint(bullet_s* bullet, tank_s* tank);
static void updateBulletPos(textures_s* textures);
static int bulletIsOutOfBounds(bullet_s* bullet, terrain_s* terrain);
static void deactivateBullet(textures_s* textures);

void initBullet(bullet_s* bullet)
{
    bullet->rect.w = 3;
    bullet->rect.h = 3;
    bullet->rect.x = 0;
    bullet->rect.y = 0;
    bullet->active = 0;
    bullet->speedX = 1;
    bullet->speedY = 1;
    bullet->fRect.x = 0;
    bullet->fRect.y = 0;
    bullet->fRect.w = 3 * resolutionScale;
    bullet->fRect.h = 3 * resolutionScale;
    bullet->harmlessToShooter = 1;
}

void fireBullet(bullet_s* bullet, tank_s* tank)
{
    calculateBulletOriginPoint(bullet, tank);
    bullet->active = 1;
    bullet->harmlessToShooter = 1;
    bullet->shooterId = tank->id;
    calculateBulletXYSpeed(bullet, tank, 0.4f);
}

void updateBullet(textures_s* textures, terrain_s* terrain)
{
    if (bulletIsOutOfBounds(&textures->bullet, terrain) && textures->bullet.active)
    {
        drawCrater(terrain, (int)textures->bullet.fRect.x, (int)textures->bullet.fRect.y, (int)(20 * resolutionScale));
        textures->bullet.ticksAtLastCrater = SDL_GetTicks();
        textures->bullet.lastCraterPos.x = (int)textures->bullet.fRect.x;
        textures->bullet.lastCraterPos.y = (int)textures->bullet.fRect.y;
        deactivateBullet(textures);
    }

    if (textures->bullet.active)
    {
        for (int i = 0; i < deltaTime; i++)
        {
            updateBulletPos(textures);
        }
    }
    else
    {
        resetAllTanksHitboxStates(textures->tank);
    }
}

static void deactivateBullet(textures_s* textures)
{
    // Runs once when bullet goes out of bounds or hits the ground

    textures->bullet.active = 0;
    textures->bullet.fRect.x = -1;
    textures->bullet.fRect.y = -1;

    resetAllTanksHitboxStates(textures->tank);
}

static void calculateBulletOriginPoint(bullet_s* bullet, tank_s* tank)
{
    // Set to center point of tank rotation
    bullet->fRect.x = tank->fRect.x + tank->fRect.w / 2;
    bullet->fRect.y = tank->fRect.y + tank->fRect.h;

    // Adjust for tank angle
    bullet->fRect.x += 18.0f * resolutionScale * (float)sin(degToRad(tank->angle));
    bullet->fRect.y += -18.0f * resolutionScale * (float)cos(degToRad(tank->angle));

    // Adjust for the tip of the gun
    bullet->fRect.x += -13.0f * (float)cos(degToRad(90 + tank->gun.angle + tank->angle));
    bullet->fRect.y += -13.0f * (float)sin(degToRad(90 + tank->gun.angle + tank->angle));
}

static void updateBulletPos(textures_s* textures)
{
    const float GRAVITY_CONST = 0.0003f;

    textures->bullet.fRect.x += textures->bullet.speedX * resolutionScale;
    textures->bullet.fRect.y += textures->bullet.speedY * resolutionScale;
    textures->bullet.speedY += GRAVITY_CONST;

    for (int i = 0; i < playerCount; i++)
    {
        checkTankCollisionWithBullet(&textures->tank[i], &textures->bullet);

        if (textures->bullet.harmlessToShooter && textures->tank[i].collidesWithBullet && i == textures->bullet.shooterId)
        {
            textures->tank[i].isInvincible = 1;
        }
        else if (!textures->tank[i].collidesWithBullet && i == textures->bullet.shooterId)
        {
            textures->tank[i].isInvincible = 0;
            textures->bullet.harmlessToShooter = 0;
        }

        if (textures->tank[i].collidesWithBullet && !textures->tank[i].isInvincible)
        {
            deactivateBullet(textures);
            applyDamageToTank(&textures->tank[i], 10);
        }
    }
}

static int bulletIsOutOfBounds(bullet_s* bullet, terrain_s* terrain)
{
    if (bullet->fRect.x < 0 || bullet->fRect.x > (float)RESOLUTION_X ||
        bullet->fRect.y + bullet->fRect.h > (float)terrain->groundLevel[(int)(bullet->fRect.x + bullet->fRect.w / 2)])
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void calculateBulletXYSpeed(bullet_s *bullet, tank_s *tank, float speed)
{
    int vectorAngle = 90 - tank->gun.angle;

    // Converting polar vector to cartesian form
    bullet->speedX = speed * (float)cos(degToRad(vectorAngle - tank->angle));
    bullet->speedY = -speed * (float)sin(degToRad(vectorAngle - tank->angle));
    //printf("angle: %d, tank angle: %d, speedX: %f, speedY: %f\n", tank->gun.angle, tank->angle, bullet->speedX, bullet->speedY);
}

double degToRad(float deg)
{
    return (double)deg * M_PI / 180;
}