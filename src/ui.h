//
// Created by clovis on 5/15/24.
//

#ifndef TANKS_UI_H
#define TANKS_UI_H

#include <SDL2/SDL_ttf.h>

#define ARROW_DIRECTION_UP 0
#define ARROW_DIRECTION_RIGHT 1
#define ARROW_DIRECTION_DOWN 2
#define ARROW_DIRECTION_LEFT 3

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
    SDL_Texture* playerCountDisplay;
    SDL_Rect playerCountDisplayRect;
    int updatePlayerCount;
    button_s* increasePlayerCountButton;
    SDL_Rect increasePlayerCountButtonRect;
    button_s* decreasePlayerCountButton;
    SDL_Rect decreasePlayerCountButtonRect;
    SDL_Color uiFg;
    SDL_Color uiBg;
    TTF_Font* titleFont;
    TTF_Font* freeSans;
} menutextures_s;

SDL_Texture* createTextTexture(SDL_Rect* textRect, SDL_Renderer* ren, char text[], TTF_Font* font, SDL_Color color);
button_s* createButton(SDL_Renderer* ren, char text[], TTF_Font* font, SDL_Color bg, SDL_Color fg, int x, int y, int w, int h);
button_s* createArrowButton(SDL_Renderer* ren, SDL_Color bg, SDL_Color fg, int centerX, int centerY, int w, int h, int direction);
void destroyButton(button_s* button);
void updateButton(SDL_Event* event, button_s* button);
void updatePlayerCountDisplay(SDL_Renderer* ren, menutextures_s* menuTextures);

#endif //TANKS_UI_H
