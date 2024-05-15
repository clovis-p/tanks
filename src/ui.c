//
// Created by clovis on 5/15/24.
//

#include <SDL2/SDL_ttf.h>

#include "main.h"
#include "ui.h"

void createTextTexture(SDL_Texture** textTexture, SDL_Rect* textRect, SDL_Renderer* ren, char text[], TTF_Font* font, SDL_Color color)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, color);

    *textTexture = SDL_CreateTextureFromSurface(ren, textSurface);

    SDL_QueryTexture(*textTexture, NULL, NULL, &textRect->w, &textRect->h);

    SDL_FreeSurface(textSurface);
}