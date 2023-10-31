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
    SDL_SetRenderDrawColor(*ren, 34, 139, 34, 255);
    SDL_RenderDrawPoints(*ren, terrain->sdlGroundPoints, terrain->groundPointsCount);

    // debug, showDebug = 1 while left shift and left alt are held down, otherwise showDebug = 0
    if (showDebug)
    {
        SDL_SetRenderDrawColor(*ren, 0, 0, 0, 255);
        SDL_RenderDrawPoints(*ren, terrain->debugPoints, terrain->debugPointsCount);

        //SDL_RenderDrawRect(*ren, &textures->tank[0].rect);
        //SDL_RenderDrawRect(*ren, &textures->tank[1].rect);

        SDL_SetRenderDrawColor(*ren, 0, 0, 255, 255);
        renderTankHitboxes(ren, &textures->tank[0]);
        renderTankHitboxes(ren, &textures->tank[1]);
    }

    // Set render target to tank[0] texture
    SDL_SetRenderTarget(*ren, textures->tank[0].combinedTexture);

    // Clear tank[0] texture, then render tank[0] base and tank[0] gun to tank[0] texture
    SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0); // set transparent background
    SDL_RenderClear(*ren);
    SDL_RenderCopyEx(*ren,
                     textures->tank[0].gun.texture,
                     NULL,
                     &textures->tank[0].gun.rect,
                     textures->tank[0].gun.angle,
                     NULL,
                     SDL_FLIP_NONE);
    SDL_RenderCopy(*ren, textures->tank[0].baseTexture, NULL, NULL);

    // Set render target to tank[1] texture
    SDL_SetRenderTarget(*ren, textures->tank[1].combinedTexture);

    // Clear tank[1] texture, then render tank[1] base and tank[1] gun to tank[1] texture
    SDL_SetRenderDrawColor(*ren, 0, 0, 0, 0); // set transparent background
    SDL_RenderClear(*ren);
    SDL_RenderCopyEx(*ren,
                     textures->tank[1].gun.texture,
                     NULL,
                     &textures->tank[1].gun.rect,
                     textures->tank[1].gun.angle,
                     NULL,
                     SDL_FLIP_NONE);
    SDL_RenderCopy(*ren, textures->tank[1].baseTexture, NULL, NULL);

    // Reset render target
    SDL_SetRenderTarget(*ren, NULL);

    // Set tank center point to the bottom of the tank so it stays level with the ground when on a hill
    // This is different from tank.bottomCenter!! tank.bottomCenter is relative to the entire window, this
    // is relative to the tank texture only
    SDL_Point tankCenterPoint = {textures->tank[0].rect.w / 2, textures->tank[0].rect.h};

    // Render tank[0]
    SDL_RenderCopyEx(*ren,
                     textures->tank[0].combinedTexture,
                     NULL,
                     &textures->tank[0].rect,
                     textures->tank[0].angle,
                     &tankCenterPoint,
                     SDL_FLIP_NONE);

    // Render tank[1]
    SDL_RenderCopyEx(*ren,
                     textures->tank[1].combinedTexture,
                     NULL,
                     &textures->tank[1].rect,
                     textures->tank[1].angle,
                     &tankCenterPoint,
                     SDL_FLIP_NONE);

    SDL_RenderPresent(*ren);
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
    SDL_RenderDrawLineF(*ren,
                        tank->hitBox.topLeft.x - RESOLUTION_X,
                        tank->hitBox.topLeft.y - tank->hitBox.topSlope * RESOLUTION_X,
                        tank->hitBox.topLeft.x + RESOLUTION_X,
                        tank->hitBox.topLeft.y + tank->hitBox.topSlope * RESOLUTION_X);

    // bottom slope
    SDL_RenderDrawLineF(*ren,
                        tank->hitBox.bottomLeft.x - RESOLUTION_X,
                        tank->hitBox.bottomLeft.y - tank->hitBox.bottomSlope * RESOLUTION_X,
                        tank->hitBox.bottomLeft.x + RESOLUTION_X,
                        tank->hitBox.bottomLeft.y + tank->hitBox.bottomSlope * RESOLUTION_X);

    // left slope
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