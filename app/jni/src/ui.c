#include "ui.h"
#include "constans.h"
struct Button {
    int x;
    int y;
    int w;
    int h;
    SDL_Color color;
    SDL_Color border_color;
    SDL_Texture *tex;
    SDL_Texture *normal_state_tex;
    SDL_Texture *pressed_state_tex;
    int state;
};

Button *MakeButton(SDL_Renderer *renderer,int x, int y, int w, int h){
    Button *button = calloc( 1, sizeof(Button) );
    button->x = x;
    button->y = y;
    button->w = w;
    button->h = h;
    button->normal_state_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(renderer, button->normal_state_tex);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 180);
    SDL_Rect rect = {0, 0, w, h};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);

    button->pressed_state_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(renderer, button->pressed_state_tex);
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 90);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_SetRenderTarget(renderer, tex_window);

    button->tex = button->normal_state_tex;
    return button;
}
SDL_bool isButtonPressed(Button *b, SDL_Finger *finger){
        int fx = finger->x * SCREEN_WIDTH;
        int fy = finger->y * SCREEN_HEIGHT;

        if( fx >= b->x && fx <= b->x + b->w &&
            fy >= b->y && fy <= b->y + b->h ){
            return SDL_TRUE;
        }
    return SDL_FALSE;
}
void RenderButton(Button *b, SDL_Renderer *renderer){
    SDL_Rect dst = {b->x, b->y, b->w, b->h};
    SDL_RenderCopy(renderer, b->tex, NULL, &dst);
}
void SetButtonTexNormal(Button *b) {
    b->tex = b->normal_state_tex;
}
void SetButtonTexPressed(Button *b) {
    b->tex = b->pressed_state_tex;
}
void DestroyButton(Button *b) {
    SDL_DestroyTexture(b->normal_state_tex);
    SDL_DestroyTexture(b->pressed_state_tex);
    free(b);
}