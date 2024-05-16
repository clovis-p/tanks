//
// Created by clovis on 28/09/23.
//

#ifndef TANKS_EVENTS_H
#define TANKS_EVENTS_H

#include "terrain/terrain.h"
#include "ui.h"

void handleGlobalEvents(int *quit);
void handleGameEvents(terrain_s* terrain, textures_s* textures);
void handleMenuEvents(menutextures_s* menuTextures);

extern int showDebug;

#endif //TANKS_EVENTS_H
