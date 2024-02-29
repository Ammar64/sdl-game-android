#include "font.h"
#include "constans.h"

void disp_text(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y) {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect pos;
    SDL_Color color = {255, 255, 255};
    surface = TTF_RenderText_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    pos.w = surface->w;
    pos.h = surface->h;
    if( x == -1 ){ // center text
        x = (WINDOW_WIDTH / 2) - (pos.w / 2);
    }
    if( y == -1){ //center text
        y = (WINDOW_HEIGHT / 2) - (pos.h / 2);
    }
    pos.x = x;
    pos.y = y;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &pos);
}
