//
// Created by clovis on 28/09/23.
//

#include "terrain.h"

terrain_s* generateTerrain(SDL_Renderer* ren, int width, int height, int type)
{
    terrain_s *terrain;

    if (type == TERRAIN_TYPE_MIDPOINT)
    {
        terrain = generateMidpointTerrain(ren, width, height, 6);
    }

    return terrain;
}