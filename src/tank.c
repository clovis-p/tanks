//
// Created by clovis on 11/10/23.
//

#include <stdio.h>
#include <math.h>

#include "main.h"
#include "terrain/terrain.h"
#include "tank.h"

void calculateTankAngle(tank_s* tank, terrain_s* terrain)
{
    int x1, x2, y1, y2;
    double angle;

    x1 = tank->rect.x + (int)(0.35 * tank->rect.w);
    x2 = tank->rect.x + (int)(0.65 * tank->rect.w);
    y1 = terrain->groundLevel[x1];
    y2 = terrain->groundLevel[x2];

    if (y1 == y2)
    {
        tank->angle = 0;
    }
    else
    {
        angle = (int)(atan((double)(y1 - y2) / (double)(x2 - x1)) * 180 / M_PI);
        if (y1 > y2) // Going uphill
        {
            tank->angle = -(int)angle;
        }
        else // Going downhill
        {
            tank->angle = 360 - (int)angle;
        }

        tank->angle %= 360;
    }
}

void moveTank(tank_s* tank, int amount, terrain_s* terrain)
{
    // tank angle according to terrain
    calculateTankAngle(tank, terrain);

    // move according to angle
    float speed = (float)amount * (float)cos(tank->angle * M_PI / 180);
    tank->fPoint.x += speed;
    tank->rect.x = (int)tank->fPoint.x;

    // update y position
    tank->fPoint.y = (float)(terrain->groundLevel[tank->rect.x + tank->rect.w / 2] - tank->rect.h);
    tank->rect.y = (int)tank->fPoint.y;
}

void rotateGunClockwise(gun_s* gun)
{
    if (gun->angle < 90)
    {
        gun->angle += 1;
    }
}

void rotateGunCounterClockwise(gun_s* gun)
{
    if (gun->angle > -90)
    {
        gun->angle -= 1;
    }
}