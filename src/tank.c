//
// Created by clovis on 11/10/23.
//

#include "main.h"
#include "terrain/terrain.h"

void calculateTankAngle(textureAndRect_s* tank, terrain_s* terrain)
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
    }
}

void moveTankX(textureAndRect_s* tank, int amount, terrain_s* terrain)
{
    calculateTankAngle(tank, terrain);
    tank->rect.x += amount;
    /*
    if (tank->angle < 45 || tank->angle > 315)
    {
        tank->rect.x += amount;
    }
    */
}