//
// Created by clovis on 5/15/24.
//

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "main.h"
#include "ui.h"

static int isButtonHovered(button_s* button);

SDL_Texture* createTextTexture(SDL_Rect* textRect, SDL_Renderer* ren, char text[], TTF_Font* font, SDL_Color color)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);

    SDL_QueryTexture(textTexture, NULL, NULL, &textRect->w, &textRect->h);

    SDL_FreeSurface(textSurface);

    return textTexture;
}

button_s* createButton(SDL_Renderer* ren, char text[], TTF_Font* font, SDL_Color bg, SDL_Color fg, int x, int y, int w, int h)
{
    button_s* button;

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, fg);
    button->textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_QueryTexture(button->textTexture, NULL, NULL, &button->textRect.w, &button->textRect.h);
    button->rect.w = w;
    button->rect.h = h;
    button->textRect.x = button->rect.x + (button->rect.w - button->textRect.w) / 2;
    button->textRect.y = button->rect.y + (button->rect.h - button->textRect.h) / 2;

    return button;
}

button_s* createArrowButton(SDL_Renderer* ren, SDL_Color bg, SDL_Color fg, int centerX, int centerY, int w, int h)
{
    button_s* startButton = (button_s*)malloc(sizeof(button_s));
    startButton->textTexture = NULL;
    startButton->buttonTexture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

    startButton->rect.x = centerX - w / 2;
    startButton->rect.y = centerY - h / 2;
    startButton->rect.w = w;
    startButton->rect.h = h;

    startButton->isHovered = 0;
    startButton->actionFlag = 0;
    startButton->pressed = 0;

    SDL_SetRenderTarget(ren, startButton->buttonTexture);
    SDL_SetRenderDrawColor(ren, bg.r, bg.g, bg.b, bg.a);

    SDL_RenderClear(ren);

    aatrigonRGBA(ren, (Sint16)(w / 4.0), (Sint16)(h / 4.0),
                 (Sint16)(w / 4.0), (Sint16)(h / 4.0 * 3),
                 (Sint16)(w / 4.0 * 3), (Sint16)(h / 2),
                 fg.r, fg.b, fg.b, fg.a);

    filledTrigonRGBA(ren, (Sint16)(w / 4.0), (Sint16)(h / 4.0),
                 (Sint16)(w / 4.0), (Sint16)(h / 4.0 * 3),
                 (Sint16)(w / 4.0 * 3), (Sint16)(h / 2),
                 fg.r, fg.b, fg.b, fg.a);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_SetRenderTarget(ren, NULL);

    return startButton;
}

void destroyButton(button_s* button)
{
    SDL_DestroyTexture(button->buttonTexture);

    if (button->textTexture != NULL)
    {
        SDL_DestroyTexture(button->textTexture);
    }

    free(button);
}

void updateButton(SDL_Event* event, button_s* button)
{
    button->isHovered = isButtonHovered(button);

    if (event->button.type == SDL_MOUSEBUTTONDOWN && event->button.which == 0 && button->isHovered)
    {
        button->actionFlag = 1;
    }
}

void updatePlayerCountDisplay(SDL_Renderer* ren, menutextures_s* menuTextures)
{
    if (menuTextures->updatePlayerCount)
    {
        char buf[2];
        if (playerCount <= 4)
        {
            sprintf(buf, "%d", playerCount);
        }

        SDL_DestroyTexture(menuTextures->playerCountDisplay);
        menuTextures->playerCountDisplay = createTextTexture(&menuTextures->playerCountDisplayRect,
                                                             ren, buf, menuTextures->freeSans, menuTextures->uiFg);
    }
}

static int isButtonHovered(button_s* button)
{
    int ret = 0;

    int mx, my;
    SDL_GetMouseState(&mx, &my);

    if (mx >= button->rect.x && mx <= button->rect.x + button->rect.w &&
        my >= button->rect.y && my <= button->rect.y + button->rect.h)
    {
        ret = 1;
    }

    return ret;
}
