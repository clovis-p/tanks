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
static void updateBulletPos(textures_s* textures);
static int bulletIsOutOfBounds(bullet_s* bullet, terrain_s* terrain);
static int checkBulletCollisions(textures_s* textures);

void fireBullet(bullet_s* bullet, tank_s* tank)
{
    calculateBulletOriginPoint(bullet, tank);
    bullet->active = 1;
    calculateBulletXYSpeed(bullet, tank, 0.4f);
}

void updateBullet(textures_s* textures, terrain_s* terrain)
{
    if (bulletIsOutOfBounds(&textures->bullet, terrain))
    {
        textures->bullet.active = 0;
        textures->bullet.rect.x = -1;
        textures->bullet.rect.y = -1;
        textures->bullet.fPoint.x = -1;
        textures->bullet.fPoint.y = -1;
    }

    if (textures->bullet.active)
    {
        updateBulletPos(textures);
    }
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

static void updateBulletPos(textures_s* textures)
{
    for (int i = 0; i < deltaTime; i++)
    {
        textures->bullet.fPoint.x += textures->bullet.speedX;
        textures->bullet.fPoint.y += textures->bullet.speedY;
        textures->bullet.speedY += 0.0003f;

        checkBulletCollisions(textures);
    }

    textures->bullet.rect.x = (int)textures->bullet.fPoint.x;
    textures->bullet.rect.y = (int)textures->bullet.fPoint.y;
}

static int bulletIsOutOfBounds(bullet_s* bullet, terrain_s* terrain)
{
    if (bullet->rect.x < 0 || bullet->rect.x > RESOLUTION_X ||
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
    //printf("angle: %d, tank angle: %d, speedX: %f, speedY: %f\n", tank->gun.angle, tank->angle, bullet->speedX, bullet->speedY);
}

static double degToRad(int deg)
{
    return (double)deg * M_PI / 180;
}

static int checkBulletCollisions(textures_s* textures)
{
    if (textures->bullet.fPoint.x + (float)textures->bullet.rect.w >= textures->tank[0].fPoint.x && textures->bullet.fPoint.x < textures->tank[0].fPoint.x + (float)textures->tank[0].rect.w && // Check X axis
        textures->bullet.fPoint.y + (float)textures->bullet.rect.w >= textures->tank[0].fPoint.y - (float)textures->tank[0].rect.h && textures->bullet.fPoint.y < textures->tank[0].fPoint.y + (float)textures->tank[0].rect.h)
    {
        printf("Collision!! ");
        textures->tank[0].collidesWithBullet = 1;
    }
    else
    {
        textures->tank[0].collidesWithBullet = 0;
    }

    if (textures->bullet.fPoint.x + (float)textures->bullet.rect.w >= textures->tank[1].fPoint.x && textures->bullet.fPoint.x < textures->tank[1].fPoint.x + (float)textures->tank[1].rect.w && // Check X axis
        textures->bullet.fPoint.y + (float)textures->bullet.rect.w >= textures->tank[1].fPoint.y - (float)textures->tank[1].rect.h && textures->bullet.fPoint.y < textures->tank[1].fPoint.y + (float)textures->tank[1].rect.h)
    {
        printf("Collision!! ");
        textures->tank[1].collidesWithBullet = 1;
    }
    else
    {
        textures->tank[1].collidesWithBullet = 0;
    }
}