//
// Created by clovis on 11/10/23.
//

#include <stdio.h>
#include <math.h>

#include "bullet.h"
#include "main.h"
#include "terrain/terrain.h"

static void calculateBulletXYSpeed(bullet_s *bullet, tank_s *tank, float speed);
static void calculateBulletOriginPoint(bullet_s* bullet, tank_s* tank);
static double degToRad(int deg);
static void updateBulletPos(bullet_s* bullet);
static int bulletIsOutOfBounds(bullet_s* bullet, terrain_s* terrain);

void fireBullet(bullet_s* bullet, tank_s* tank)
{
    calculateBulletOriginPoint(bullet, tank);
    bullet->active = 1;
    calculateBulletXYSpeed(bullet, tank, 5);
}

static void calculateBulletOriginPoint(bullet_s* bullet, tank_s* tank)
{
    // Set to center point of tank rotation
    bullet->fPoint.x = (float)(tank->rect.x + tank->rect.w / 2.0);
    bullet->fPoint.y = (float)(tank->rect.y + tank->rect.h);

    // Adjust for tank angle
    bullet->fPoint.x += 18.0f * (float)sin(degToRad(tank->angle));
    bullet->fPoint.y += -18.0f * (float)cos(degToRad(tank->angle));

    // Adjust for the tip of the gun
    bullet->fPoint.x += -13.0f * (float)cos(degToRad(90 + tank->gun.angle + tank->angle));
    bullet->fPoint.y += -13.0f * (float)sin(degToRad(90 + tank->gun.angle + tank->angle));

    bullet->rect.x = (int)bullet->fPoint.x;
    bullet->rect.y = (int)bullet->fPoint.y;
}

void updateBullet(bullet_s* bullet, terrain_s* terrain)
{
    if (bulletIsOutOfBounds(bullet, terrain))
    {
        bullet->active = 0;
        bullet->rect.x = -1;
        bullet->rect.y = -1;
        bullet->fPoint.x = -1;
        bullet->fPoint.y = -1;
    }

    if (bullet->active)
    {
        updateBulletPos(bullet);
    }
}

static void updateBulletPos(bullet_s* bullet)
{
    bullet->fPoint.x += bullet->speedX;
    bullet->fPoint.y += bullet->speedY;

    bullet->rect.x = (int)bullet->fPoint.x;
    bullet->rect.y = (int)bullet->fPoint.y;

    bullet->speedY += 0.05f;
}

static int bulletIsOutOfBounds(bullet_s* bullet, terrain_s* terrain)
{
    if (bullet->rect.x < 0 || bullet->rect.x > RESOLUTION_X || bullet->rect.y < 0 || bullet->rect.y > RESOLUTION_Y ||
        bullet->rect.y + bullet->rect.h > terrain->groundLevel[bullet->rect.x + bullet->rect.w / 2])
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
    printf("angle: %d, tank angle: %d, speedX: %f, speedY: %f\n", tank->gun.angle, tank->angle, bullet->speedX, bullet->speedY);
}

static double degToRad(int deg)
{
    return (double)deg * M_PI / 180;
}