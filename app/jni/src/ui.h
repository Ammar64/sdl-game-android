#ifndef SDL_GAME_ANDROID_UI_H
#define SDL_GAME_ANDROID_UI_H

#include <SDL.h>

enum ButtonState {
    NORMAL,
    PRESSED
};


struct Button;
typedef struct Button Button;

Button *MakeButton(SDL_Renderer *renderer,int x, int y, int w, int h);
SDL_bool isButtonPressed(Button *b, SDL_Finger *finger);
void RenderButton(Button *b, SDL_Renderer *renderer);
void SetButtonTexNormal(Button *b);
void SetButtonTexPressed(Button *b);
void DestroyButton(Button *b);
#endif //SDL_GAME_ANDROID_UI_H
