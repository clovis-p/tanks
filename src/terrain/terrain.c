//
// Created by clovis on 28/09/23.
//

#include <SDL2/SDL.h>

#include "terrain.h"

terrain_s* generateTerrain(SDL_Renderer* ren, int width, int height, int type)
{
    int startTime = SDL_GetTicks();

    terrain_s *terrain;

    if (type == TERRAIN_TYPE_MIDPOINT)
    {
        terrain = generateMidpointTerrain(ren, width, height, 6);
    }

    terrain->texture = SDL_CreateTexture(ren,
                                        SDL_PIXELFORMAT_RGBA8888,
                                        SDL_TEXTUREACCESS_TARGET,
                                        RESOLUTION_X,
                                        RESOLUTION_Y);
    SDL_SetTextureBlendMode(terrain->texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(terrain->texture, 255);

    SDL_SetRenderTarget(ren, terrain->texture);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);

    SDL_SetRenderDrawColor(ren, 34, 139, 34, 255);
    SDL_RenderDrawPoints(ren, terrain->sdlGroundPoints, terrain->groundPointsCount);

    SDL_SetRenderTarget(ren, NULL);

    printf("Done (%dms)\n", SDL_GetTicks() - startTime);

    return terrain;
}

void refreshTerrain(SDL_Renderer* ren, terrain_s* terrain)
{
    for (int i = 0; i < (int)(RESOLUTION_X * RESOLUTION_Y); i++)
    {
        terrain->sdlGroundPoints[i].x = -1;
        terrain->sdlGroundPoints[i].y = -1;
    }

    terrain->groundPointsCount = 0;
    int i = 0;

    for (int x = 0; x < RESOLUTION_X; x++)
    {
        for (int y = 0; y < RESOLUTION_Y; y++)
        {
            if (terrain->terrainArray[x][y] == 1)
            {
                terrain->sdlGroundPoints[i].x = x;
                terrain->sdlGroundPoints[i].y = y;
                i++;

                if (y > 0 && terrain->terrainArray[x][y - 1] == 0)
                {
                    terrain->groundLevel[x] = y;
                }
            }
        }
    }

    terrain->groundPointsCount = i;

    SDL_SetRenderTarget(ren, terrain->texture);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
    SDL_RenderClear(ren);

    SDL_SetRenderDrawColor(ren, 34, 139, 34, 255);
    SDL_RenderDrawPoints(ren, terrain->sdlGroundPoints, terrain->groundPointsCount);

    SDL_SetRenderTarget(ren, NULL);

    terrain->antiAliasingTexture = generateAntiAliasingTexture(ren, terrain);

    terrain->refreshNeeded = 0;
}


// Function to draw a circle in a 2D array
void drawCrater(terrain_s* terrain, int x, int y, int radius)
{
    for (int i = 0; i < RESOLUTION_X; i++)
    {
        for (int j = 0; j < RESOLUTION_Y; j++)
        {
            if (i > x && i < x + radius * 2 && j > y && j < y + radius * 2)
            {
                terrain->terrainArray[i][j] = 0;
            }
        }
    }

    terrain->refreshNeeded = 1;

    /*
    // Check if the center and radius are within bounds
    if (x > 0 && x <= RESOLUTION_X && y > 0 && y <= RESOLUTION_Y && radius > 0)
    {
        printf("Invalid parameters.\n");
        return;
    }

    // Midpoint circle drawing algorithm
    int centerX = x;
    int centerY = y;
    int r = radius;
    int xCoord = 0;
    int yCoord = r;
    int d = 3 - 2 * r;

    while (xCoord <= yCoord)
    {
        // Draw the circle points in the array
        if (centerX + xCoord >= 0 && centerX + xCoord < RESOLUTION_X && centerY + yCoord >= 0 && centerY + yCoord < RESOLUTION_Y)
            terrain->terrainArray[centerY + yCoord][centerX + xCoord] = 0;
        if (centerX - xCoord >= 0 && centerX - xCoord < RESOLUTION_X && centerY + yCoord >= 0 && centerY + yCoord < RESOLUTION_Y)
            terrain->terrainArray[centerY + yCoord][centerX - xCoord] = 0;
        if (centerX + xCoord >= 0 && centerX + xCoord < RESOLUTION_X && centerY - yCoord >= 0 && centerY - yCoord < RESOLUTION_Y)
            terrain->terrainArray[centerY - yCoord][centerX + xCoord] = 0;
        if (centerX - xCoord >= 0 && centerX - xCoord < RESOLUTION_X && centerY - yCoord >= 0 && centerY - yCoord < RESOLUTION_Y)
            terrain->terrainArray[centerY - yCoord][centerX - xCoord] = 0;

        if (centerX + yCoord >= 0 && centerX + yCoord < RESOLUTION_Y && centerY + xCoord >= 0 && centerY + xCoord < RESOLUTION_X)
            terrain->terrainArray[centerY + xCoord][centerX + yCoord] = 0;
        if (centerX - yCoord >= 0 && centerX - yCoord < RESOLUTION_Y && centerY + xCoord >= 0 && centerY + xCoord < RESOLUTION_X)
            terrain->terrainArray[centerY + xCoord][centerX - yCoord] = 0;
        if (centerX + yCoord >= 0 && centerX + yCoord < RESOLUTION_Y && centerY - xCoord >= 0 && centerY - xCoord < RESOLUTION_X)
            terrain->terrainArray[centerY - xCoord][centerX + yCoord] = 0;
        if (centerX - yCoord >= 0 && centerX - yCoord < RESOLUTION_Y && centerY - xCoord >= 0 && centerY - xCoord < RESOLUTION_X)
            terrain->terrainArray[centerY - xCoord][centerX - yCoord] = 0;

        // Update coordinates and decision parameter
        xCoord++;
        if (d > 0)
        {
            yCoord--;
            d = d + 4 * (xCoord - yCoord) + 10;
        }
        else
        {
            d = d + 4 * xCoord + 6;
        }
    }

    terrain->refreshNeeded = 1;

     */
}