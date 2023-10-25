//
// Created by clovis on 28/09/23.
//

#include "terrain.h"

terrain_s* generateTerrain(int width, int height, int type)
{
    if (type == TERRAIN_TYPE_MIDPOINT)
    {
        return generateMidpointTerrain(width, height, 7);
    }
}