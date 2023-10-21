//
// Created by clovis on 09/10/23.
//

#include <math.h>

#include "../main.h"
#include "terrain.h"

// this doesn't work yet

terrain_s* generateSineTerrain(int width, int height)
{
    terrain_s* terrain;

    terrain = (terrain_s*)malloc(sizeof(terrain_s));

    // Initialize arrays
    for (int i = 0; i < width; i++) {
        terrain->groundLevel[i] = 0;
        for (int j = 0; j < height; j++) {
            terrain->terrainArray[i][j] = 0;
            terrain->sdlGroundPoints[i * j].x = 0;
            terrain->sdlGroundPoints[i * j].y = 0;
        }
    }

    for (int y = 0; y < height; y++) {
        // Calculate the sine value for the current x
        double sineValue = sin(y * (M_PI / 180));

        // Convert the sine value (-1 to 1) to a row index
        int x = (int)((1 - sineValue) * (width - 1) / 2);

        terrain->terrainArray[x][y] = 1;
        terrain->groundLevel[x] = y;
        terrain->sdlGroundPoints[terrain->groundPointsCount].x = x;
        terrain->groundPointsCount++;
    }

    return terrain;
}

void drawSineWave(int width, int height) {
    int screen[height][width];
}