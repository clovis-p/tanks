//
// Created by clovis on 11/10/23.
//

#include <math.h>
#include <stdio.h>

#include "main.h"
#include "terrain/terrain.h"
#include "tank.h"
#include "bullet.h"

static void calculateTankAngle(tank_s* tank, terrain_s* terrain);
static void updateTankHitbox(tank_s* tank);
static float calculateLinearSlope(float x1, float y1, float x2, float y2);
static void updateTankHealthBar(tank_s* tank);

void initTank(SDL_Renderer** ren, textures_s* textures, int id)
{
    // init tank[id] base
    SDL_Surface* buffer;
    if (id == 0)
    {
        buffer = SDL_LoadBMP("../resources/images/red_tank_4x.bmp");
        if (buffer == NULL)
        {
            printf("Failed to load red_tank.bmp: %s\n", SDL_GetError());
        }
    }
    else if (id == 1)
    {
        buffer = SDL_LoadBMP("../resources/images/blue_tank_4x.bmp");
        if (buffer == NULL)
        {
            printf("Failed to load blue_tank.bmp: %s\n", SDL_GetError());
        }
    }
    else if (id == 2)
    {
        buffer = SDL_LoadBMP("../resources/images/green_tank_4x.bmp");
        if (buffer == NULL)
        {
            printf("Failed to load green_tank.bmp: %s\n", SDL_GetError());
        }
    }
    else if (id == 3)
    {
        buffer = SDL_LoadBMP("../resources/images/yellow_tank_4x.bmp");
        if (buffer == NULL)
        {
            printf("Failed to load yellow_tank.bmp: %s\n", SDL_GetError());
        }
    }
    else
    {
        printf("Invalid tank id: %d\n", id);
        return;
    }

    textures->tank[id].baseTexture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    //SDL_QueryTexture(textures->tank[id].baseTexture, NULL, NULL, &textures->tank[id].rect.w, &textures->tank[id].rect.h);
    textures->tank[id].rect.x = 0;
    textures->tank[id].rect.y = 0;
    textures->tank[id].angle = 0;
    textures->tank[id].fRect.w = 45 * resolutionScale;
    textures->tank[id].fRect.h = 45 * resolutionScale;
    textures->tank[id].fRect.x = 0;
    textures->tank[id].fRect.y = 0;
    textures->tank[id].health = 100;
    textures->tank[id].isInvincible = 0;
    textures->tank[id].id = id;
    textures->tank[id].ticksAtLastHit = 0;


    // init tank[id] combined texture
    textures->tank[id].combinedTexture = SDL_CreateTexture(*ren,
                                                          SDL_PIXELFORMAT_RGBA8888,
                                                          SDL_TEXTUREACCESS_TARGET,
                                                           (int)textures->tank[id].fRect.w,
                                                           (int)textures->tank[id].fRect.h);
    SDL_SetTextureBlendMode(textures->tank[id].combinedTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(textures->tank[id].combinedTexture, 255);

    // tank[id] gun
    buffer = SDL_LoadBMP("../resources/images/gun.bmp");
    if (buffer == NULL)
    {
        printf("Failed to load gun.bmp: %s\n", SDL_GetError());
    }
    textures->tank[id].gun.texture = SDL_CreateTextureFromSurface(*ren, buffer);
    SDL_FreeSurface(buffer);
    SDL_QueryTexture(textures->tank[id].gun.texture, NULL, NULL, &textures->tank[id].gun.rect.w, &textures->tank[id].gun.rect.h);
    textures->tank[id].gun.fRect.w = (float)textures->tank[id].gun.rect.w * resolutionScale;
    textures->tank[id].gun.fRect.h = (float)textures->tank[id].gun.rect.h * resolutionScale;
    textures->tank[id].gun.rect.x = 0;
    textures->tank[id].gun.rect.y = 6;
    textures->tank[id].gun.fRect.x = 0;
    textures->tank[id].gun.fRect.y = (float)textures->tank[id].gun.rect.y * resolutionScale;
    textures->tank[id].gun.angle = 80;
}

void initHealthBars(healthBar_s* healthBar)
{
    healthBar->rect.w = 30;
    healthBar->rect.h = 8;
    healthBar->fRect.w = (float)healthBar->rect.w * resolutionScale;
    healthBar->fRect.h = (float)healthBar->rect.h * resolutionScale;
}

void initAllTanksHitboxes(tank_s tanks[])
{
    resetAllTanksHitboxStates(tanks);
}

void teleportTank(tank_s* tank, float x, terrain_s* terrain)
{
    // set x coordinate
    tank->fRect.x = x - tank->fRect.w / 2.0f;

    // update y position
    tank->fRect.y = (float)terrain->groundLevel[(int)(tank->fRect.x + tank->fRect.w / 2)] - tank->fRect.h;

    // tank angle according to terrain
    calculateTankAngle(tank, terrain);

    // update hitbox
    updateTankHitbox(tank);

    // update health bar
    updateTankHealthBar(tank);
}

void moveTank(tank_s* tank, float amount, terrain_s* terrain)
{
    if (tank->fRect.x + amount < tank->fRect.w / 2 && amount < 0 || // tank is too far left and going left or
        tank->fRect.x + tank->fRect.w + amount > RESOLUTION_X - (tank->fRect.w / 2) && amount > 0) // tank is too far right and going right
    {
        return;
    }

    // move according to angle
    float speed = amount * (float)cos(tank->angle * M_PI / 180);
    tank->fRect.x += speed * (float)deltaTime;

    // update y position
    tank->fRect.y = (float)(terrain->groundLevel[(int)(tank->fRect.x + tank->fRect.w / 2)] - tank->rect.h);

    // tank angle according to terrain
    calculateTankAngle(tank, terrain);

    // update hitbox
    updateTankHitbox(tank);

    // update health bar
    updateTankHealthBar(tank);
}

void applyDamageToTank(tank_s* tank, int damage)
{
    if (!tank->isInvincible && tank->health > 0)
    {
        tank->health -= damage;
        tank->ticksAtLastHit = SDL_GetTicks();
        updateTankHealthBar(tank);
    }
}

void rotateGunClockwise(gun_s* gun)
{
    if (gun->angle < 90)
    {
        gun->angle += 0.1f * (float)deltaTime;

        if (gun->angle > 90)
        {
            gun->angle = 90;
        }
    }
}

void rotateGunCounterClockwise(gun_s* gun)
{
    if (gun->angle > -90)
    {
        gun->angle -= 0.1f * (float)deltaTime;

        if (gun->angle < -90)
        {
            gun->angle = -90;
        }
    }
}

void checkTankCollisionWithBullet(tank_s* tank, bullet_s* bullet)
{
    if (tank->angle == 0)
    {
        if (bullet->fRect.x + bullet->fRect.w > tank->hitBox.topLeft.x)
        {
            tank->hitBox.leftColConditionMet = 1;
        }
        else
        {
            tank->hitBox.leftColConditionMet = 0;
        }

        if (bullet->fRect.x < tank->hitBox.topRight.x)
        {
            tank->hitBox.rightColConditionMet = 1;
        }
        else
        {
            tank->hitBox.rightColConditionMet = 0;
        }

        if (bullet->fRect.y + bullet->fRect.h > tank->hitBox.topLeft.y)
        {
            tank->hitBox.topColConditionMet = 1;
        }
        else
        {
            tank->hitBox.topColConditionMet = 0;
        }

        if (bullet->fRect.y < tank->hitBox.bottomLeft.y)
        {
            tank->hitBox.bottomColConditionMet = 1;
        }
        else
        {
            tank->hitBox.bottomColConditionMet = 0;
        }
    }
    else if (tank->angle > 0)
    {
        if (bullet->fRect.y >= tank->hitBox.leftSlope * bullet->fRect.x + tank->hitBox.leftOffset)
        {
            tank->hitBox.leftColConditionMet = 1;
        }
        else
        {
            tank->hitBox.leftColConditionMet = 0;
        }

        if (bullet->fRect.y + bullet->fRect.h <= tank->hitBox.rightSlope * bullet->fRect.x + tank->hitBox.rightOffset)
        {
            tank->hitBox.rightColConditionMet = 1;
        }
        else
        {
            tank->hitBox.rightColConditionMet = 0;
        }

        if (bullet->fRect.x <= (bullet->fRect.y - tank->hitBox.topOffset) / tank->hitBox.topSlope)
        {
            tank->hitBox.topColConditionMet = 1;
        }
        else
        {
            tank->hitBox.topColConditionMet = 0;
        }

        if (bullet->fRect.x + bullet->fRect.w >= (bullet->fRect.y - tank->hitBox.bottomOffset) / tank->hitBox.bottomSlope)
        {
            tank->hitBox.bottomColConditionMet = 1;
        }
        else
        {
            tank->hitBox.bottomColConditionMet = 0;
        }
    }
    else if (tank->angle < 0)
    {
        if (bullet->fRect.y + bullet->fRect.h <= tank->hitBox.leftSlope * bullet->fRect.x + tank->hitBox.leftOffset)
        {
            tank->hitBox.leftColConditionMet = 1;
        }
        else
        {
            tank->hitBox.leftColConditionMet = 0;
        }

        if (bullet->fRect.y >= tank->hitBox.rightSlope * bullet->fRect.x + tank->hitBox.rightOffset)
        {
            tank->hitBox.rightColConditionMet = 1;
        }
        else
        {
            tank->hitBox.rightColConditionMet = 0;
        }

        if (bullet->fRect.x + bullet->fRect.w >= (bullet->fRect.y - tank->hitBox.topOffset) / tank->hitBox.topSlope)
        {
            tank->hitBox.topColConditionMet = 1;
        }
        else
        {
            tank->hitBox.topColConditionMet = 0;
        }

        if (bullet->fRect.x <= (bullet->fRect.y - tank->hitBox.bottomOffset) / tank->hitBox.bottomSlope)
        {
            tank->hitBox.bottomColConditionMet = 1;
        }
        else
        {
            tank->hitBox.bottomColConditionMet = 0;
        }
    }

    if (tank->hitBox.topColConditionMet && tank->hitBox.bottomColConditionMet &&
        tank->hitBox.leftColConditionMet && tank->hitBox.rightColConditionMet)
    {
        tank->collidesWithBullet = 1;
    }
    else
    {
        tank->collidesWithBullet = 0;
    }
}

void resetAllTanksHitboxStates(tank_s tanks[])
{
    for (int i = 0; i < playerCount; i++)
    {
        tanks[i].hitBox.topColConditionMet = 0;
        tanks[i].hitBox.bottomColConditionMet = 0;
        tanks[i].hitBox.leftColConditionMet = 0;
        tanks[i].hitBox.rightColConditionMet = 0;

        tanks[i].collidesWithBullet = 0;
    }
}

static void updateTankHealthBar(tank_s* tank)
{
    tank->healthBar.fRect.x = tank->bottomCenter.x + 9.0f * (float)sin(degToRad(tank->angle)) - tank->healthBar.fRect.w / 2;
    tank->healthBar.fRect.y = tank->fRect.y;

    tank->healthBar.filledFRect = tank->healthBar.fRect;
    tank->healthBar.filledFRect.w = (float)tank->health / 100.0f * tank->healthBar.fRect.w;
}

static void calculateTankAngle(tank_s* tank, terrain_s* terrain)
{
    float x1, x2, y1, y2;
    double angle;

    x1 = tank->fRect.x + (0.35f * tank->fRect.w);
    x2 = tank->fRect.x + (0.65f * tank->fRect.w);
    y1 = (float)terrain->groundLevel[(int)x1];
    y2 = (float)terrain->groundLevel[(int)x2];

    if (y1 == y2)
    {
        tank->angle = 0;
    }
    else
    {
        angle = atan((double)((y1 - y2) / (x2 - x1))) * 180 / M_PI;
        if (y1 > y2) // Going uphill
        {
            tank->angle = -(float)angle;
        }
        else // Going downhill
        {
            tank->angle = 360 - (float)angle;
        }

        //tank->angle %= 360;
    }

    tank->bottomCenter.x = tank->fRect.x + tank->fRect.w / 2;
    tank->bottomCenter.y = tank->fRect.y + tank->fRect.h;
}

static void updateTankHitbox(tank_s* tank)
{
    const float HITBOX_HEIGHT = 20 * resolutionScale;

    tankHitBox_s untranslatedHitBox;

    // Set initial values centered on bottom center at (0, 0)
    untranslatedHitBox.bottomCenter.x = 0;
    untranslatedHitBox.bottomCenter.y = 0;

    // top left
    untranslatedHitBox.topLeft.x = -tank->fRect.w / 2;
    untranslatedHitBox.topLeft.y = -HITBOX_HEIGHT;

    // top right
    untranslatedHitBox.topRight.x = tank->fRect.w / 2;
    untranslatedHitBox.topRight.y = -HITBOX_HEIGHT;

    // bottom left
    untranslatedHitBox.bottomLeft.x = -tank->fRect.w / 2;
    untranslatedHitBox.bottomLeft.y = 0;

    // bottom right
    untranslatedHitBox.bottomRight.x = tank->fRect.w / 2;
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

    // Calculate slopes
    tank->hitBox.topSlope = calculateLinearSlope(tank->hitBox.topLeft.x, tank->hitBox.topLeft.y, tank->hitBox.topRight.x, tank->hitBox.topRight.y);
    tank->hitBox.leftSlope = calculateLinearSlope(tank->hitBox.topLeft.x, tank->hitBox.topLeft.y, tank->hitBox.bottomLeft.x, tank->hitBox.bottomLeft.y);
    tank->hitBox.rightSlope = calculateLinearSlope(tank->hitBox.topRight.x, tank->hitBox.topRight.y, tank->hitBox.bottomRight.x, tank->hitBox.bottomRight.y);
    tank->hitBox.bottomSlope = calculateLinearSlope(tank->hitBox.bottomLeft.x, tank->hitBox.bottomLeft.y, tank->hitBox.bottomRight.x, tank->hitBox.bottomRight.y);

    // Calculate slope offsets
    tank->hitBox.topOffset = -(tank->hitBox.topSlope) * tank->hitBox.topLeft.x + tank->hitBox.topLeft.y;
    tank->hitBox.leftOffset = -(tank->hitBox.leftSlope) * tank->hitBox.topLeft.x + tank->hitBox.topLeft.y;
    tank->hitBox.rightOffset = -(tank->hitBox.rightSlope) * tank->hitBox.topRight.x + tank->hitBox.topRight.y;
    tank->hitBox.bottomOffset = -(tank->hitBox.bottomSlope) * tank->hitBox.bottomLeft.x + tank->hitBox.bottomLeft.y;
}

static float calculateLinearSlope(float x1, float y1, float x2, float y2)
{
    return (y2 - y1) / (x2 - x1);
}