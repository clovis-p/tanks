//
// Created by clovis on 5/15/24.
//

#ifndef TANKS_UI_H
#define TANKS_UI_H

#include <SDL2/SDL_ttf.h>

typedef struct menutextures_s
{
    SDL_Texture* title;
    SDL_Rect titleRect;
    SDL_Texture* startButton;
    SDL_Texture* background;
} menutextures_s;

void createTextTexture(SDL_Texture** textTexture, SDL_Rect* textRect, SDL_Renderer* ren, char text[], TTF_Font* font, SDL_Color color);

#endif //TANKS_UI_H
