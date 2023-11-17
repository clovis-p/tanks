//
// Created by clovis on 28/09/23.
//

#ifndef TANKS_TERRAIN_H
#define TANKS_TERRAIN_H

#define TERRAIN_TYPE_MIDPOINT 0
#define TERRAIN_TYPE_SINE 1

#include <SDL2/SDL.h>

#include "../main.h"

typedef struct terrain_s
{
    SDL_Point sdlGroundPoints[RESOLUTION_X * RESOLUTION_Y];
    int groundPointsCount;
    int terrainArray[RESOLUTION_X][RESOLUTION_Y];
    int groundLevel[RESOLUTION_X];
    SDL_Point debugPoints[RESOLUTION_X * RESOLUTION_Y];
    int debugPointsCount;
    SDL_Texture* texture;
    SDL_Texture* antiAliasingTexture;
    int refreshNeeded;
} terrain_s;

SDL_Texture* generateAntiAliasingTexture(SDL_Renderer* ren, terrain_s* terrain);
terrain_s* generateTerrain(SDL_Renderer* ren, int width, int height, int type);
terrain_s* generateMidpointTerrain(SDL_Renderer* ren, int width, int height, int precision);
void refreshTerrain(SDL_Renderer* ren, terrain_s* terrain);
void drawCrater(terrain_s* terrain, int x, int y, int radius);
terrain_s* generateSineTerrain(int width, int height);


#endif //TANKS_TERRAIN_H
