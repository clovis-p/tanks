//
// Created by clovis on 28/09/23.
//

#include "terrain.h"

int generateRandomNumber(int min, int max);
int getCenterMidpointNumber(int midpointCount);
int isOdd(int number);
int calculateMidpointNumber(int index, int highestIndex, int k);
double calculateLinearSlope(int y2, int y1, int x2, int x1);

terrain_s* generateTerrain(int width, int height, int type)
{
    if (type == TERRAIN_TYPE_MIDPOINT)
    {
        return generateMidpointTerrain(width, height, 6);
    }
}