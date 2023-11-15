//
// Created by clovis on 28/09/23.
//

#ifndef TANKS_MAIN_H
#define TANKS_MAIN_H

#include <SDL2/SDL.h>

#define RESOLUTION_X 1280
#define RESOLUTION_Y 720

extern Uint32 deltaTime;

extern int turn;
extern int playerCount;

extern float resolutionScale;

typedef struct gun_s
{
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_FRect fRect;
    double angle;
    int relativePosY; // Y position relative to the tank
    // (relative X position is always 0)
} gun_s;

typedef struct bullet_s
{
    SDL_Rect rect;
    SDL_FRect fRect;
    int active;
    float speedX;
    float speedY;
    int harmlessToShooter;
    int shooterId;
} bullet_s;

typedef struct tankHitBox_s
{
    SDL_FPoint bottomCenter;
    SDL_FPoint topLeft;
    SDL_FPoint topRight;
    SDL_FPoint bottomLeft;
    SDL_FPoint bottomRight;

    float topSlope;
    float bottomSlope;
    float leftSlope;
    float rightSlope;

    float topOffset;
    float bottomOffset;
    float leftOffset;
    float rightOffset;

    int topColConditionMet;
    int bottomColConditionMet;
    int leftColConditionMet;
    int rightColConditionMet;
} tankHitBox_s;

typedef struct healthBar_s
{
    // Rect of the whole health bar
    SDL_FRect fRect;
    SDL_Rect rect;

    // Rect of the filled (green) part of the health bar
    SDL_FRect filledFRect;
    SDL_Rect filledRect;
} healthBar_s;

typedef struct tank_s
{
    SDL_Texture* baseTexture; // Texture for the tank without the gun
    SDL_Texture* combinedTexture; // Texture for the tank and gun combined
    SDL_Rect rect;
    SDL_FRect fRect;
    SDL_FPoint bottomCenter;
    float angle;
    gun_s gun;
    int health;
    int collidesWithBullet;
    tankHitBox_s hitBox;
    int isInvincible;
    int id;
    healthBar_s healthBar;
    Uint32 ticksAtLastHit; // The number of ticks at the time damage was taken
} tank_s;

typedef struct textures_s
{
    tank_s tank[4];
    bullet_s bullet;
} textures_s;

#endif //TANKS_MAIN_H
