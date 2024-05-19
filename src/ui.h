//
// Created by clovis on 5/15/24.
//

#ifndef TANKS_UI_H
#define TANKS_UI_H

#include <SDL2/SDL_ttf.h>

typedef struct button_s
{
    SDL_Texture* textTexture;
    SDL_Texture* buttonTexture;
    SDL_Rect rect;
    SDL_Rect textRect;
    int actionFlag;
    int pressed;
    int isHovered;
} button_s;

typedef struct menutextures_s
{
    SDL_Texture* title;
    SDL_Rect titleRect;
    button_s* startButton;
    SDL_Texture* background;
} menutextures_s;

void createTextTexture(SDL_Texture** textTexture, SDL_Rect* textRect, SDL_Renderer* ren, char text[], TTF_Font* font, SDL_Color color);
button_s* createButton(SDL_Renderer* ren, char text[], TTF_Font* font, SDL_Color bg, SDL_Color fg, int x, int y, int w, int h);
button_s* createArrowButton(SDL_Renderer* ren, SDL_Color bg, SDL_Color fg, int centerX, int centerY, int w, int h);
void destroyButton(button_s* button);
void updateButton(SDL_Event* event, button_s* button);

#endif //TANKS_UI_H
