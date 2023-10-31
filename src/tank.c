//
// Created by clovis on 11/10/23.
//

#include <math.h>

#include "main.h"
#include "terrain/terrain.h"
#include "tank.h"

static void calculateTankAngle(tank_s* tank, terrain_s* terrain);
static void updateTankHitbox(tank_s* tank);
static float calculateLinearSlope(float x1, float y1, float x2, float y2);

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

    // move according to angle
    float speed = (float)amount * (float)cos(tank->angle * M_PI / 180);
    tank->fPoint.x += speed * (float)deltaTime;
    tank->rect.x = (int)tank->fPoint.x;

    // update y position
    tank->fPoint.y = (float)(terrain->groundLevel[tank->rect.x + tank->rect.w / 2] - tank->rect.h);
    tank->rect.y = (int)tank->fPoint.y;

    // tank angle according to terrain
    calculateTankAngle(tank, terrain);

    // update hitbox
    updateTankHitbox(tank);
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

    tank->bottomCenter.x = (float)(tank->rect.x + tank->rect.w / 2.0);
    tank->bottomCenter.y = (float)(tank->rect.y + tank->rect.h);
}

static void updateTankHitbox(tank_s* tank)
{
    const float HITBOX_TOP_OFFSET = 30.0f;

    const float HITBOX_HEIGHT = (float)tank->rect.h - HITBOX_TOP_OFFSET;

    tankHitBox_s untranslatedHitBox;

    // Set initial values centered on bottom center at (0, 0)
    untranslatedHitBox.bottomCenter.x = 0;
    untranslatedHitBox.bottomCenter.y = 0;

    // top left
    untranslatedHitBox.topLeft.x = -(float)tank->rect.w / 2;
    untranslatedHitBox.topLeft.y = -HITBOX_HEIGHT;

    // top right
    untranslatedHitBox.topRight.x = (float)tank->rect.w / 2;
    untranslatedHitBox.topRight.y = -HITBOX_HEIGHT;

    // bottom left
    untranslatedHitBox.bottomLeft.x = -(float)tank->rect.w / 2;
    untranslatedHitBox.bottomLeft.y = 0;

    // bottom right
    untranslatedHitBox.bottomRight.x = (float)tank->rect.w / 2;
    untranslatedHitBox.bottomRight.y = 0;

    // Convert tank angle to radians
    float tankAngleRad = (float)(tank->angle * M_PI / 180);

    // Rotate hitbox
    float bottomRightVectorLength = untranslatedHitBox.bottomRight.x; // length from bottom center to bottom right
    float topRightVectorLength = sqrtf(powf(untranslatedHitBox.topRight.x, 2) + powf(untranslatedHitBox.topRight.y, 2)); // length from bottom center to top right
    float topLeftVectorLength = topRightVectorLength; // length from bottom center to top left - symmetrical to top right
    float bottomLeftVectorLength = bottomRightVectorLength; // length from bottom center to bottom left - symmetrical to bottom right

    float bottomRightVectorAngle = tankAngleRad; // angle of bottom center to bottom right vector
    float topRightVectorAngle = atanf(untranslatedHitBox.topRight.y / untranslatedHitBox.topRight.x); // angle of bottom center to top right vector
    float topLeftVectorAngle = -(float)M_PI - topRightVectorAngle; // angle of bottom center to top left vector
    topRightVectorAngle += tankAngleRad;
    topLeftVectorAngle += tankAngleRad;
    float bottomLeftVectorAngle = -(float)M_PI + tankAngleRad; // angle of bottom center to bottom left vector


    untranslatedHitBox.bottomLeft.x = bottomLeftVectorLength * cosf(bottomLeftVectorAngle);
    untranslatedHitBox.bottomLeft.y = bottomLeftVectorLength * sinf(bottomLeftVectorAngle);

    untranslatedHitBox.topLeft.x = topLeftVectorLength * cosf(topLeftVectorAngle);
    untranslatedHitBox.topLeft.y = topLeftVectorLength * sinf(topLeftVectorAngle);

    untranslatedHitBox.topRight.x = topRightVectorLength * cosf(topRightVectorAngle);
    untranslatedHitBox.topRight.y = topRightVectorLength * sinf(topRightVectorAngle);

    untranslatedHitBox.bottomRight.x = bottomRightVectorLength * cosf(bottomRightVectorAngle);
    untranslatedHitBox.bottomRight.y = bottomRightVectorLength * sinf(bottomRightVectorAngle);

    // Translate hitbox to tank position
    tank->hitBox.bottomCenter = tank->bottomCenter;
    tank->hitBox.topLeft.x = tank->bottomCenter.x + untranslatedHitBox.topLeft.x;
    tank->hitBox.topLeft.y = tank->bottomCenter.y + untranslatedHitBox.topLeft.y;
    tank->hitBox.topRight.x = tank->bottomCenter.x + untranslatedHitBox.topRight.x;
    tank->hitBox.topRight.y = tank->bottomCenter.y + untranslatedHitBox.topRight.y;
    tank->hitBox.bottomLeft.x = tank->bottomCenter.x + untranslatedHitBox.bottomLeft.x;
    tank->hitBox.bottomLeft.y = tank->bottomCenter.y + untranslatedHitBox.bottomLeft.y;
    tank->hitBox.bottomRight.x = tank->bottomCenter.x + untranslatedHitBox.bottomRight.x;
    tank->hitBox.bottomRight.y = tank->bottomCenter.y + untranslatedHitBox.bottomRight.y;
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

    // move according to angle
    float speed = (float)amount * (float)cos(tank->angle * M_PI / 180);
    tank->fPoint.x += speed * (float)deltaTime;
    tank->rect.x = (int)tank->fPoint.x;

    // update y position
    tank->fPoint.y = (float)(terrain->groundLevel[tank->rect.x + tank->rect.w / 2] - tank->rect.h);
    tank->rect.y = (int)tank->fPoint.y;

    // tank angle according to terrain
    calculateTankAngle(tank, terrain);

    // update hitbox
    updateTankHitbox(tank);
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