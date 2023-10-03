//
// Created by clovis on 28/09/23.
//

#include "main.h"
#include "terraingen.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>

terrain_s* generateTerrain(int width, int height)
{
    terrain_s* terrain;

    terrain = (terrain_s*)malloc(sizeof(terrain_s));

    srand(time(NULL));

    //int midpointCount = 12;
    //int midpoints[midpointCount];

    terrain->groundPointsCount = 0;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (j >= 360)
            {
                terrain->terrainArray[i][j] = 1;
                terrain->groundLevel[i] = 360;
                terrain->sdlGroundPoints[terrain->groundPointsCount].x = i;
                terrain->sdlGroundPoints[terrain->groundPointsCount].y = j;
                terrain->groundPointsCount++;
            }
            else
            {
                terrain->terrainArray[i][j] = 0;
            }
        }
    }

    return terrain;
}