//
// Created by clovis on 11/10/23.
//

#include <math.h>

#include "main.h"
#include "terrain/terrain.h"
#include "tank.h"

static void calculateTankAngle(tank_s* tank, terrain_s* terrain);

static void calculateTankAngle(tank_s* tank, terrain_s* terrain)
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

static void updateTankHitbox(tank_s* tank)
{
    const float HITBOX_TOP_OFFSET = 25.0f;

    if (tank->angle == 0)
    {
        tank->hitBox.bottomCenter.x = (float)tank->rect.x + (float)tank->rect.w / 2;
        tank->hitBox.bottomCenter.y = (float)tank->rect.y + (float)tank->rect.h;

        tank->hitBox.topLeft.x = (float)tank->rect.x;
        tank->hitBox.topLeft.y = (float)tank->rect.y + HITBOX_TOP_OFFSET;

        tank->hitBox.topRight.x = (float)tank->rect.x + (float)tank->rect.w;
        tank->hitBox.topRight.y = (float)tank->rect.y + HITBOX_TOP_OFFSET;

        tank->hitBox.bottomLeft.x = (float)tank->rect.x;
        tank->hitBox.bottomLeft.y = (float)tank->rect.y + (float)tank->rect.h;

        tank->hitBox.bottomRight.x = (float)tank->rect.x + (float)tank->rect.w;
        tank->hitBox.bottomRight.y = (float)tank->rect.y + (float)tank->rect.h;
    }
    else
    {
        const float HITBOX_HEIGHT = (float)tank->rect.h - HITBOX_TOP_OFFSET;

        // Set initial values centered on bottom center at (0, 0)
        tank->hitBox.bottomCenter.x = 0;
        tank->hitBox.bottomCenter.y = HITBOX_HEIGHT;

        // top left
        tank->hitBox.topLeft.x = -(float)tank->rect.w / 2;
        tank->hitBox.topLeft.y = 0;

        // top right
        tank->hitBox.topRight.x = (float)tank->rect.w / 2;
        tank->hitBox.topRight.y = 0;

        // bottom left
        tank->hitBox.bottomLeft.x = -(float)tank->rect.w / 2;
        tank->hitBox.bottomLeft.y = HITBOX_HEIGHT;

        // bottom right
        tank->hitBox.bottomRight.x = (float)tank->rect.w / 2;
        tank->hitBox.bottomRight.y = HITBOX_HEIGHT;

        // Rotate hitbox
        float tankAngleRad = (float)tank->angle * M_PI / 180; // convert to radians
        float bottomLeftVectorLength = tank->hitBox.topLeft.x; // length from bottom center to bottom left
        float topLeftVectorLength = sqrtf(powf(tank->hitBox.topLeft.x, 2) + powf(tank->hitBox.topLeft.y + HITBOX_HEIGHT, 2)); // length from bottom center to top left
        float topRightVectorLength = sqrtf(powf(tank->hitBox.topRight.x, 2) + powf(tank->hitBox.topRight.y + HITBOX_HEIGHT, 2)); // length from bottom center to top right
        float bottomRightVectorLength = tank->hitBox.bottomRight.x; // length from bottom center to bottom right

        tank->hitBox.bottomLeft.x = bottomLeftVectorLength * sinf(-tankAngleRad);
        tank->hitBox.bottomLeft.y = bottomLeftVectorLength * cosf(-tankAngleRad);

        tank->hitBox.topLeft.x = topLeftVectorLength * sinf(-tankAngleRad);
        tank->hitBox.topLeft.y = topLeftVectorLength * cosf(-tankAngleRad) - HITBOX_HEIGHT;

        tank->hitBox.topRight.x = topRightVectorLength * sinf(-tankAngleRad);
        tank->hitBox.topRight.y = topRightVectorLength * cosf(-tankAngleRad) - HITBOX_HEIGHT;

        tank->hitBox.bottomRight.x = bottomRightVectorLength * sinf(-tankAngleRad);
        tank->hitBox.bottomRight.y = bottomRightVectorLength * cosf(-tankAngleRad);

        // Translate hitbox to tank position
        tank->hitBox.bottomCenter.x += (float)tank->rect.x + (float)tank->rect.w / 2;
        tank->hitBox.bottomCenter.y += (float)tank->rect.y + (float)tank->rect.h;

    }
}

void teleportTank(tank_s* tank, int x, terrain_s* terrain)
{
    // set x coordinate
    tank->fPoint.x = (float)x - tank->rect.w / 2;
    tank->rect.x = (int)tank->fPoint.x;

    // update y position
    tank->fPoint.y = (float)(terrain->groundLevel[tank->rect.x + tank->rect.w / 2] - tank->rect.h);
    tank->rect.y = (int)tank->fPoint.y;

    // tank angle according to terrain
    calculateTankAngle(tank, terrain);

    // update hitbox
    updateTankHitbox(tank);
}

void moveTank(tank_s* tank, float amount, terrain_s* terrain)
{
    if (tank->fPoint.x + (float)amount < (float)tank->rect.w / 2 && amount < 0 || // tank is too far left and going left or
        tank->fPoint.x + (float)tank->rect.w + (float)amount > RESOLUTION_X - ((float)tank->rect.w / 2) && amount > 0) // tank is too far right and going right
    {
        return;
    }

    // tank angle according to terrain
    calculateTankAngle(tank, terrain);

    // update hitbox
    updateTankHitbox(tank);

    // move according to angle
    float speed = (float)amount * (float)cos(tank->angle * M_PI / 180);
    tank->fPoint.x += speed * (float)deltaTime;
    tank->rect.x = (int)tank->fPoint.x;

    // update y position
    tank->fPoint.y = (float)(terrain->groundLevel[tank->rect.x + tank->rect.w / 2] - tank->rect.h);
    tank->rect.y = (int)tank->fPoint.y;
}

void rotateGunClockwise(gun_s* gun)
{
    if (gun->fAngle < 90)
    {
        gun->fAngle += 0.1f * (float)deltaTime;

        if (gun->fAngle > 90)
        {
            gun->fAngle = 90;
            gun->angle = (int)gun->fAngle;
        }
        else
        {
            gun->angle = (int)gun->fAngle;
        }
    }
}

void rotateGunCounterClockwise(gun_s* gun)
{
    if (gun->fAngle > -90)
    {
        gun->fAngle -= 0.1f * (float)deltaTime;

        if (gun->fAngle < -90)
        {
            gun->fAngle = -90;
            gun->angle = (int)gun->fAngle;
        }
        else
        {

            gun->angle = (int)gun->fAngle;
        }
    }
}