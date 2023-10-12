//
// Created by clovis on 11/10/23.
//

#include "bullet.h"
#include "main.h"

void fireBullet(bullet_s* bullet, tank_s* tank)
{
    bullet->rect.x = tank->rect.x + tank->rect.w / 2;
    bullet->rect.y = tank->rect.y + tank->rect.h / 2;
    bullet->active = 1;
    bullet->speedX = (int)(10 * cos(tank->angle * M_PI / 180));
    bullet->speedY = (int)(10 * sin(tank->angle * M_PI / 180));
}

void updateBullet(bullet_s* bullet)
{
    if (bulletIsOutOfBounds(bullet))
    {
        bullet->active = 0;
    }

    if (bullet->active)
    {
        updateBulletPos(bullet);
    }
}

void updateBulletPos(bullet_s* bullet)
{
    bullet->rect.x += bullet->speedX;
    bullet->rect.y += bullet->speedY;
}

int bulletIsOutOfBounds(bullet_s* bullet)
{
    if (bullet->rect.x < 0 || bullet->rect.x > RESOLUTION_X || bullet->rect.y < 0 || bullet->rect.y > RESOLUTION_Y)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}