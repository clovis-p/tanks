//
// Created by clovis on 28/09/23.
//

#include <stdio.h>
#include <SDL2/SDL.h>

#include "main.h"
#include "render.h"
#include "terrain/terrain.h"
#include "events.h"

static void renderArray(SDL_Window** win, SDL_Renderer** ren, int** array, int x, int y, int width, int height, SDL_Color fg, SDL_Color bg);
static void renderTankHitboxes(SDL_Renderer** ren, tank_s* tank);
static void renderTankHealthBar(SDL_Renderer** ren, tank_s* tank);
static void renderTank(SDL_Renderer** ren, tank_s* tank);
static void renderTerrain(SDL_Renderer** ren, terrain_s* terrain);
static void renderDebugInfo(SDL_Renderer** ren, terrain_s* terrain, textures_s* textures);

void render(SDL_Window** win, SDL_Renderer** ren, terrain_s* terrain, textures_s* textures)
{
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
    renderTerrain(ren, terrain);

    // debug, showDebug = 1 while left shift and left alt are held down, otherwise showDebug = 0
    if (showDebug)
    {
        renderDebugInfo(ren, terrain, textures);
    }

    // Render tanks
    for (int i = 0; i < playerCount; i++)
    {
        renderTank(ren, &textures->tank[i]);
    }

    // Render health bars
    for (int i = 0; i < playerCount; i++)
    {
        renderTankHealthBar(ren, &textures->tank[i]);
    }

    SDL_RenderPresent(*ren);
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
    SDL_SetRenderDrawColor(*ren, 34, 139, 34, 255);
    SDL_RenderDrawPoints(*ren, terrain->sdlGroundPoints, terrain->groundPointsCount);
}

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
    SDL_RenderCopyEx(*ren,tank->combinedTexture,NULL,&tank->rect,tank->angle,&tankCenterPoint,SDL_FLIP_NONE);
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