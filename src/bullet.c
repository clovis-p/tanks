//
// Created by clovis on 11/10/23.
//

#include <stdio.h>
#include <math.h>

#include "bullet.h"
#include "main.h"
#include "terrain/terrain.h"

void calculateBulletXYSpeed(bullet_s *bullet, tank_s *tank, float speed);
double degToRad(int deg);

void fireBullet(bullet_s* bullet, tank_s* tank)
{
    bullet->fPoint.x = (float)(tank->rect.x + tank->rect.w / 2.0);
    bullet->fPoint.y = (float)(tank->rect.y + tank->rect.h / 2.0);
    bullet->rect.x = (int)bullet->fPoint.x;
    bullet->rect.y = (int)bullet->fPoint.y;
    bullet->active = 1;
    calculateBulletXYSpeed(bullet, tank, 5);
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

void updateBulletPos(bullet_s* bullet)
{
    bullet->fPoint.x += bullet->speedX;
    bullet->fPoint.y += bullet->speedY;

    bullet->rect.x = (int)bullet->fPoint.x;
    bullet->rect.y = (int)bullet->fPoint.y;

    bullet->speedY += 0.05;
}

int bulletIsOutOfBounds(bullet_s* bullet, terrain_s* terrain)
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

void calculateBulletXYSpeed(bullet_s *bullet, tank_s *tank, float speed)
{
    if (tank->gun.angle == 0)
    {
        bullet->speedX = 0;
        bullet->speedY = -speed;
    }
    else
    {
        int vectorAngle = 0;

        if (tank->gun.angle > 0 && tank->gun.angle < 90)
        {
            vectorAngle = 90 - tank->gun.angle;
        }
        else if (tank->gun.angle < 0 && tank->gun.angle > -90)
        {
            vectorAngle = 90 + -(tank->gun.angle);
        }
        else
        {
            printf("Tank gun angle out of bounds: %d\n", tank->gun.angle);
            vectorAngle = 90;
        }

        // Converting polar vector to cartesian form
        bullet->speedX = speed * (float)cos(degToRad(vectorAngle - tank->angle));
        bullet->speedY = -speed * (float)sin(degToRad(vectorAngle - tank->angle));
        printf("angle: %d, tank angle: %d, speedX: %f, speedY: %f\n", tank->gun.angle, tank->angle, bullet->speedX, bullet->speedY);
    }
}

double degToRad(int deg)
{
    return (double)deg * M_PI / 180;
}