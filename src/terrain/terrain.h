//
// Created by clovis on 28/09/23.
//

#ifndef TANKS_TERRAIN_H
#define TANKS_TERRAIN_H

#define TERRAIN_TYPE_MIDPOINT 0
#define TERRAIN_TYPE_SINE 1

#include <SDL2/SDL.h>

typedef struct terrain_s
{
    SDL_Point sdlGroundPoints[1280*720];
    int terrainArray[1280][720];
    int groundLevel[1280];
    int groundPointsCount;
    SDL_Point debugPoints[1280*720];
    int debugPointsCount;
} terrain_s;

terrain_s* generateTerrain(int width, int height, int type);
terrain_s* generateMidpointTerrain(int width, int height, int precision);
terrain_s* generateSineTerrain(int width, int height);


#endif //TANKS_TERRAIN_H
