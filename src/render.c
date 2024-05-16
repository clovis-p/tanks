//
// Created by clovis on 28/09/23.
//

#include <stdio.h>
#include <SDL2/SDL.h>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "main.h"
#include "render.h"
#include "terrain/terrain.h"
#include "events.h"
#include "vfx.h"
#include "ui.h"

static void renderArray(SDL_Window** win, SDL_Renderer** ren, int** array, int x, int y, int width, int height, SDL_Color fg, SDL_Color bg);
static void renderTankHitboxes(SDL_Renderer** ren, tank_s* tank);
static void renderTankHealthBar(SDL_Renderer** ren, tank_s* tank);
static void renderTanks(SDL_Renderer** ren, textures_s* textures);
static void renderTank(SDL_Renderer** ren, tank_s* tank);
static void renderTerrain(SDL_Renderer** ren, terrain_s* terrain);
static void renderDebugInfo(SDL_Renderer** ren, terrain_s* terrain, textures_s* textures);
static void matchAllIntegerRectsToFloatRects(textures_s* textures);
static void matchIntegerRectToFloatRect(SDL_Rect* intRect, SDL_FRect* floatRect);
static void renderBottomBar(SDL_Renderer* ren, textures_s* textures);

void render(SDL_Window** win, SDL_Renderer** ren, terrain_s* terrain, textures_s* textures)
{
    // The game works with float coords, but SDL works with integer coords
    matchAllIntegerRectsToFloatRects(textures);

    // sky
    SDL_SetRenderDrawColor(*ren, 135, 206, 235, 255);
    SDL_RenderClear(*ren);

    // bullet
    if (textures->bullet.active)
    {
        SDL_SetRenderDrawColor(*ren, 0, 0, 0, 255);
        SDL_RenderFillRect(*ren, &textures->bullet.rect);
    }

    // ground
    if (terrain->refreshNeeded)
    {
        refreshTerrain(*ren, terrain);
    }
    renderTerrain(ren, terrain);

    // debug, showDebug = 1 while left shift and left alt are held down, otherwise showDebug = 0
    if (showDebug)
    {
        renderDebugInfo(ren, terrain, textures);
    }

    // Render tanks
    renderTanks(ren, textures);

    // Render bullet ground impact effect
    if (isBulletGroundImpactEffectActive(&textures->bullet))
    {
        updateAndRenderBulletGroundImpactEffect(*ren, textures);
    }

    // Render damage effect
    if (isTankHitEffectActive(textures) >= 0)
    {
        updateAndRenderTankHitEffect(*ren, textures);
    }

    renderBottomBar(*ren, textures);

    SDL_RenderPresent(*ren);
}

void renderMenu(SDL_Window** win, SDL_Renderer** ren, menutextures_s* menuTextures)
{
    SDL_SetRenderDrawColor(*ren, 0, 0, 0, 255);
    SDL_RenderClear(*ren);

    SDL_RenderCopy(*ren, menuTextures->title, NULL, &menuTextures->titleRect);
    renderButton(*ren, menuTextures->startButton);

    SDL_RenderPresent(*ren);
}

void renderButton(SDL_Renderer* ren, button_s* button)
{
    SDL_RenderCopy(ren, button->buttonTexture, NULL, &button->rect);

    if (button->textTexture != NULL)
    {
        SDL_RenderCopy(ren, button->textTexture, NULL, &button->textRect);
    }

    if (button->isHovered)
    {
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(ren, 255, 255, 255, 128);
        SDL_RenderFillRect(ren, &button->rect);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    }
}

static void renderDebugInfo(SDL_Renderer** ren, terrain_s* terrain, textures_s* textures)
{
    SDL_SetRenderDrawColor(*ren, 0, 0, 0, 255);
    SDL_RenderDrawPoints(*ren, terrain->debugPoints, terrain->debugPointsCount);

    for (int i = 0; i < playerCount; i++)
    {
        renderTankHitboxes(ren, &textures->tank[i]);

        SDL_SetRenderDrawColor(*ren, 0, 0, 0, 255);
        SDL_RenderDrawRect(*ren, &textures->tank[i].rect);
    }
}

static void renderTerrain(SDL_Renderer** ren, terrain_s* terrain)
{
    SDL_RenderCopy(*ren, terrain->antiAliasingTexture, NULL, NULL);
    SDL_RenderCopy(*ren, terrain->texture, NULL, NULL);
}

// Renders all living tanks and their health bars
static void renderTanks(SDL_Renderer** ren, textures_s* textures)
{
    for (int i = 0; i < playerCount; i++)
    {
        if (textures->tank[i].isAlive)
        {
            renderTank(ren, &textures->tank[i]);
            renderTankHealthBar(ren, &textures->tank[i]);
        }
    }
}

// Renders a tank (just the tank)
static void renderTank(SDL_Renderer** ren, tank_s* tank)
{
    // Set render target to tank texture
    SDL_SetRenderTarget(*ren, tank->combinedTexture);

    // Clear tank texture, then render tank base and tank gun to tank texture
    SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0); // set transparent background
    SDL_RenderClear(*ren);
    SDL_RenderCopyEx(*ren, tank->gun.texture, NULL, &tank->gun.rect, tank->gun.angle, NULL, SDL_FLIP_NONE);
    SDL_RenderCopy(*ren, tank->baseTexture, NULL, NULL);

    // Reset render target
    SDL_SetRenderTarget(*ren, NULL);

    // Set tank center point to the bottom of the tank so it stays level with the ground when on a hill
    // This is different from tank.bottomCenter!! tank.bottomCenter is relative to the entire window, this
    // is relative to the tank texture only
    SDL_Point tankCenterPoint = {tank->rect.w / 2, tank->rect.h};

    // Render whole tank to window
    SDL_RenderCopyEx(*ren, tank->combinedTexture, NULL, &tank->rect, tank->angle, &tankCenterPoint, SDL_FLIP_NONE);
}

static void renderTankHealthBar(SDL_Renderer** ren, tank_s* tank)
{
    SDL_SetRenderDrawColor(*ren, 255, 0, 0, 255);
    SDL_RenderFillRect(*ren, &tank->healthBar.rect);

    SDL_SetRenderDrawColor(*ren, 0, 255, 0, 255);
    SDL_RenderFillRect(*ren, &tank->healthBar.filledRect);

    // Draw outline
    SDL_SetRenderDrawColor(*ren, 0, 0, 0, 255);
    SDL_RenderDrawRect(*ren, &tank->healthBar.rect);
}

static void renderTankHitboxes(SDL_Renderer** ren, tank_s* tank) {
    // box around tank
    SDL_RenderDrawLineF(*ren, tank->hitBox.topLeft.x, tank->hitBox.topLeft.y, tank->hitBox.topRight.x,
                        tank->hitBox.topRight.y); // top line
    SDL_RenderDrawLineF(*ren, tank->hitBox.topLeft.x, tank->hitBox.topLeft.y, tank->hitBox.bottomLeft.x,
                        tank->hitBox.bottomLeft.y); // left line
    SDL_RenderDrawLineF(*ren, tank->hitBox.topRight.x, tank->hitBox.topRight.y, tank->hitBox.bottomRight.x,
                        tank->hitBox.bottomRight.y); // right line
    SDL_RenderDrawLineF(*ren, tank->hitBox.bottomLeft.x, tank->hitBox.bottomLeft.y, tank->hitBox.bottomRight.x,
                        tank->hitBox.bottomRight.y); // bottom line

    // top slope
    if (tank->hitBox.topColConditionMet)
    {
        SDL_SetRenderDrawColor(*ren, 255, 0, 0, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(*ren, 0, 0, 255, 255);
    }

    SDL_RenderDrawLineF(*ren,
                        tank->hitBox.topLeft.x - RESOLUTION_X,
                        tank->hitBox.topLeft.y - tank->hitBox.topSlope * RESOLUTION_X,
                        tank->hitBox.topLeft.x + RESOLUTION_X,
                        tank->hitBox.topLeft.y + tank->hitBox.topSlope * RESOLUTION_X);

    // bottom slope
    if (tank->hitBox.bottomColConditionMet)
    {
        SDL_SetRenderDrawColor(*ren, 255, 0, 0, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(*ren, 0, 0, 255, 255);
    }

    SDL_RenderDrawLineF(*ren,
                        tank->hitBox.bottomLeft.x - RESOLUTION_X,
                        tank->hitBox.bottomLeft.y - tank->hitBox.bottomSlope * RESOLUTION_X,
                        tank->hitBox.bottomLeft.x + RESOLUTION_X,
                        tank->hitBox.bottomLeft.y + tank->hitBox.bottomSlope * RESOLUTION_X);

    // left slope
    if (tank->hitBox.leftColConditionMet)
    {
        SDL_SetRenderDrawColor(*ren, 255, 0, 0, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(*ren, 0, 0, 255, 255);
    }

    if (tank->angle != 0)
    {
        SDL_RenderDrawLineF(*ren,
                            tank->hitBox.topLeft.x - RESOLUTION_X,
                            tank->hitBox.topLeft.y - tank->hitBox.leftSlope * RESOLUTION_X,
                            tank->hitBox.topLeft.x + RESOLUTION_X,
                            tank->hitBox.topLeft.y + tank->hitBox.leftSlope * RESOLUTION_X);
    }
    else
    {
        SDL_RenderDrawLineF(*ren,
                            tank->hitBox.topLeft.x,
                            tank->hitBox.topLeft.y - RESOLUTION_Y,
                            tank->hitBox.topLeft.x,
                            tank->hitBox.topLeft.y + RESOLUTION_X);
    }

    // right slope
    if (tank->hitBox.rightColConditionMet)
    {
        SDL_SetRenderDrawColor(*ren, 255, 0, 0, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(*ren, 0, 0, 255, 255);
    }
    if (tank->angle != 0)
    {
        SDL_RenderDrawLineF(*ren,
                            tank->hitBox.topRight.x - RESOLUTION_X,
                            tank->hitBox.topRight.y - tank->hitBox.rightSlope * RESOLUTION_X,
                            tank->hitBox.topRight.x + RESOLUTION_X,
                            tank->hitBox.topRight.y + tank->hitBox.rightSlope * RESOLUTION_X);
    }
    else
    {
        SDL_RenderDrawLineF(*ren,
                            tank->hitBox.topRight.x,
                            tank->hitBox.topRight.y - RESOLUTION_Y,
                            tank->hitBox.topRight.x,
                            tank->hitBox.topRight.y + RESOLUTION_X);

    }
}

static void renderArray(SDL_Window** win, SDL_Renderer** ren, int** array, int x, int y, int width, int height, SDL_Color fg, SDL_Color bg)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (array[i][j] == 1)
            {
                SDL_SetRenderDrawColor(*ren, fg.r, fg.g, fg.b, fg.a);
                SDL_RenderDrawPoint(*ren, i + x, j + y);
            }
            else if (bg.a != 0)
            {
                SDL_SetRenderDrawColor(*ren, bg.r, bg.g, bg.b, bg.a);
                SDL_RenderDrawPoint(*ren, i + x, j + y);
            }
        }
    }
}

static void matchAllIntegerRectsToFloatRects(textures_s* textures)
{
    matchIntegerRectToFloatRect(&textures->bullet.rect, &textures->bullet.fRect);

    for (int i = 0; i < playerCount; i++)
    {
        matchIntegerRectToFloatRect(&textures->tank[i].rect, &textures->tank[i].fRect);
        matchIntegerRectToFloatRect(&textures->tank[i].healthBar.rect, &textures->tank[i].healthBar.fRect);
        matchIntegerRectToFloatRect(&textures->tank[i].healthBar.filledRect, &textures->tank[i].healthBar.filledFRect);
        matchIntegerRectToFloatRect(&textures->tank[i].gun.rect, &textures->tank[i].gun.fRect);
    }
}

static void matchIntegerRectToFloatRect(SDL_Rect* intRect, SDL_FRect* floatRect)
{
    intRect->x = (int)floatRect->x;
    intRect->y = (int)floatRect->y;
    intRect->w = (int)floatRect->w;
    intRect->h = (int)floatRect->h;
}

static void renderBottomBar(SDL_Renderer* ren, textures_s* textures)
{
    int bottomBarTurn;

    if (textures->bullet.active)
    {
        if (turn == 0)
        {
            bottomBarTurn = playerCount;
        }
        else
        {
            bottomBarTurn = turn - 1;
        }
    }
    else
    {
        bottomBarTurn = turn;
    }

    //SDL_Rect bottomBarTankRect = {0, RESOLUTION_Y - textures->tank[bottomBarTurn].rect.h, textures->tank[bottomBarTurn].rect.w, textures->tank[turn].rect.h};
    SDL_Rect bottomBarTankRect;
    bottomBarTankRect.x = 0;
    bottomBarTankRect.y = RESOLUTION_Y - textures->tank[bottomBarTurn].rect.h;
    bottomBarTankRect.h = textures->tank[bottomBarTurn].rect.w;
    bottomBarTankRect.w = textures->tank[bottomBarTurn].rect.h;
    //SDL_RenderCopy(ren, textures->tank[bottomBarTurn].combinedTexture, NULL, &bottomBarTankRect);
}