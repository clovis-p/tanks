//
// Created by clovis on 09/10/23.
//

//
// Created by clovis on 28/09/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "terrain.h"

static int generateRandomNumber(int min, int max);
static int getCenterMidpointNumber(int midpointCount);
static int isOdd(int number);
static int calculateMidpointNumber(int index, int highestIndex, int k);
static double calculateLinearSlope(int y2, int y1, int x2, int x1);

terrain_s* generateMidpointTerrain(int width, int height, int precision)
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

    srand(time(NULL));

    struct midpoint_s
    {
        int x;
        int y;
        int no;
        int index;
        int isGenerated;
    };

    const int TERRAIN_GEN_PRECISION = precision;
    int highestIndex = TERRAIN_GEN_PRECISION + 1;
    int midpointCount = (int)(pow(2, TERRAIN_GEN_PRECISION + 1) + 1);
    struct midpoint_s midpoint[midpointCount];
    struct midpoint_s closestLeftGeneratedMidpoint, closestRightGeneratedMidpoint;
    int lastMidpointNumber = -1;
    double widthBetweenMidpoints = (double)width / (midpointCount - 1);
    int currentIndex = 0;

    int currentMidpointNumber;

    int midpointsToGenerateThisIteration = 1;

    // Initialise isGenerated for all midpoints
    for (int i = 0; i < midpointCount - 1; i++)
    {
        midpoint[i].isGenerated = 0;
    }

    int finished = 0;
    while (!finished)
    {
        // First midpoint
        midpoint[0].x = 0;
        midpoint[0].y = generateRandomNumber((int)(0.1 * height), (int)(0.7 * height));
        midpoint[0].no = 0;
        midpoint[0].index = 0;
        midpoint[0].isGenerated = 1;

        // Last midpoint
        midpoint[midpointCount - 1].x = width - 1;
        midpoint[midpointCount - 1].y = generateRandomNumber((int)(0.1 * height), (int)(0.9 * height));
        midpoint[midpointCount - 1].no = 0;
        midpoint[midpointCount - 1].index = 0;
        midpoint[midpointCount - 1].isGenerated = 1;

        // Making sure the first and last midpoints are far apart enough, otherwise start over
        if (midpoint[0].y - midpoint[midpointCount - 1].y > 100 || midpoint[0].y - midpoint[midpointCount - 1].y < -100)
        { // this is stupid but it works
            finished = 1;
        }
    }

    int totalGeneratedMidpoints = 2; // Start at 2 because the first and last points are already done
    int k = 0;
    while (totalGeneratedMidpoints < midpointCount)
    {
        if (lastMidpointNumber == -1)
        {
            currentMidpointNumber = getCenterMidpointNumber(midpointCount);
        }
        else
        {
            currentMidpointNumber = getCenterMidpointNumber(lastMidpointNumber + 1) * (k + 1);
        }

        int midpointsGeneratedThisIteration = 0;
        currentIndex++;
        while (midpointsGeneratedThisIteration < midpointsToGenerateThisIteration)
        {
            k++;
            if (isOdd(k))
            {
                if (lastMidpointNumber == -1)
                {
                    midpoint[currentMidpointNumber].index = currentIndex;
                    midpoint[currentMidpointNumber].x = (int)(getCenterMidpointNumber(midpointCount) * widthBetweenMidpoints);
                }
                else
                {
                    currentMidpointNumber = calculateMidpointNumber(currentIndex, highestIndex, k);
                    midpoint[currentMidpointNumber].index = currentIndex;
                    midpoint[currentMidpointNumber].x = (int)(widthBetweenMidpoints * currentMidpointNumber);
                }

                // Find closest left generated midpoint
                int foundIt = 0;
                int n = currentMidpointNumber - 1;
                while (!foundIt)
                {
                    if (midpoint[n].isGenerated == 1)
                    {
                        foundIt = 1;
                        closestLeftGeneratedMidpoint = midpoint[n];
                    }
                    else
                    {
                        n--;
                    }
                }

                // Find closest right generated midpoint
                foundIt = 0;
                n = currentMidpointNumber + 1;
                while (!foundIt)
                {
                    if (midpoint[n].isGenerated == 1)
                    {
                        foundIt = 1;
                        closestRightGeneratedMidpoint = midpoint[n];
                    }
                    else
                    {
                        n++;
                    }
                }

                int differenceBetweenNeighbours;
                static double variationMultiplier = -0.2;

                if (closestLeftGeneratedMidpoint.y > closestRightGeneratedMidpoint.y)
                {
                    differenceBetweenNeighbours = closestLeftGeneratedMidpoint.y - closestRightGeneratedMidpoint.y;
                    midpoint[currentMidpointNumber].y = generateRandomNumber(closestRightGeneratedMidpoint.y - (int)(variationMultiplier * differenceBetweenNeighbours), closestLeftGeneratedMidpoint.y + (int)(variationMultiplier * differenceBetweenNeighbours));
                }
                else if (closestLeftGeneratedMidpoint.y < closestRightGeneratedMidpoint.y)
                {
                    differenceBetweenNeighbours = closestRightGeneratedMidpoint.y - closestLeftGeneratedMidpoint.y;
                    midpoint[currentMidpointNumber].y = generateRandomNumber(closestLeftGeneratedMidpoint.y - (int)(variationMultiplier * differenceBetweenNeighbours), closestRightGeneratedMidpoint.y + (int)(variationMultiplier * differenceBetweenNeighbours));
                }
                else
                {
                    midpoint[currentMidpointNumber].y = closestLeftGeneratedMidpoint.y + generateRandomNumber(-1, 1);
                }

                midpoint[currentMidpointNumber].no = currentMidpointNumber;
                midpoint[currentMidpointNumber].isGenerated = 1;
                lastMidpointNumber = currentMidpointNumber;
                midpointsGeneratedThisIteration++;
                totalGeneratedMidpoints++;
            }
        }
        k = 0;
        midpointsToGenerateThisIteration *= 2;
    }

    terrain->groundPointsCount = 0;
    terrain->debugPointsCount = 0;

    double slope = 0;
    currentMidpointNumber = 0;
    for (int x = 0; x < width; x++) // for every horizontal line
    {

        if (x == midpoint[currentMidpointNumber].x) // if we are on a midpoint
        {
            // write to the terrain array the coordinates of the midpoint and every pixel below it
            for (int y = 0; y < height; y++)
            {
                if (y >= midpoint[currentMidpointNumber].y)
                {
                    if (terrain->groundLevel[midpoint[currentMidpointNumber].x] != midpoint[currentMidpointNumber].y)
                    {
                        terrain->groundLevel[midpoint[currentMidpointNumber].x] = midpoint[currentMidpointNumber].y;
                    }

                    // write the pixel to the terrain arrays
                    terrain->terrainArray[midpoint[currentMidpointNumber].x][y] = 1;
                    terrain->sdlGroundPoints[terrain->groundPointsCount].x = midpoint[currentMidpointNumber].x;
                    terrain->sdlGroundPoints[terrain->groundPointsCount].y = y;
                    terrain->groundPointsCount++;

                    if (1/*showDebug*/)
                    {
                        // write the pixel to the debug array
                        terrain->debugPoints[terrain->debugPointsCount].x = midpoint[currentMidpointNumber].x;
                        terrain->debugPoints[terrain->debugPointsCount].y = y;
                        terrain->debugPointsCount++;
                    }
                }
                else
                {
                    terrain->terrainArray[midpoint[currentMidpointNumber].x][y] = 0;
                }
            }

            currentMidpointNumber++;
        }
        else // if we aren't on a midpoint
        {
            currentMidpointNumber--;
            // if we are right after a midpoint, calculate the slope between that midpoint and the next, and fill in the pixels below it
            if (x == midpoint[currentMidpointNumber].x + 1)
            {
                slope = 0;
                slope = calculateLinearSlope(midpoint[currentMidpointNumber].y, midpoint[currentMidpointNumber + 1].y, midpoint[currentMidpointNumber].x, midpoint[currentMidpointNumber + 1].x);
                //printf("Slope between %d and %d: %f\n", midpoint[currentMidpointNumber].y, midpoint[currentMidpointNumber + 1].y, slope);
            }
            terrain->groundLevel[x/* + (int)widthBetweenMidpoints*/] = (int)(slope * (x - midpoint[currentMidpointNumber].x) + midpoint[currentMidpointNumber].y);
            for (int y = 0; y < height; y++)
            { // fill in the pixels below the slope
                if (y >= terrain->groundLevel[x])
                {
                    terrain->terrainArray[x][y] = 1;
                    terrain->sdlGroundPoints[terrain->groundPointsCount].x = x;
                    terrain->sdlGroundPoints[terrain->groundPointsCount].y = y;
                    terrain->groundPointsCount++;
                    if (y == terrain->groundLevel[x])
                    {
                        //printf("Ground level at %d: %d\n", x, terrain->groundLevel[x]);
                    }
                }
                else
                {
                    terrain->terrainArray[x][y] = 0;
                }
            }
            currentMidpointNumber++;
        }
    }
    return terrain;
}

static int generateRandomNumber(int min, int max)
{
    return rand() % (max - min) + min;
}

static int getCenterMidpointNumber(int midpointCount)
{
    switch (midpointCount)
    {
        case 3:
            return 1;
        case 5:
            return 2;
        case 9:
            return 4;
        case 17:
            return 8;
        case 33:
            return 16;
        case 65:
            return 32;
        case 129:
            return 64;
        case 257:
            return 128;
        case 513:
            return 256;
        case 1025:
            return 512;
        default:
            return 0;
    }
}

static int isOdd(int number)
{
    return number % 2;
}

static int calculateMidpointNumber(int index, int highestIndex, int k)
{
    // k = midpoint number within it's index
    int midpointNumber = 0;
    int a = 0;
    int b = 0;

    if (isOdd(highestIndex))
    {
        b = (highestIndex + 1) / 2;
        a = index - b;
        a = -a;
        a += (b - 1);
    }
    else
    {
        b = (highestIndex / 2) + 1;
        a = index - b;
        a = -a;
        a += (b - 2);
    }

    midpointNumber = (int)pow(2, a) * k;

    return midpointNumber;
}

static double calculateLinearSlope(int y2, int y1, int x2, int x1)
{
    return (double)(y2 - y1) / (x2 - x1);
}