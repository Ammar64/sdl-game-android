#ifndef CONSTANS_H
#define CONSTANS_H

#include "stdbool.h"
#include "SDL_render.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
#define GAME_WIDTH 640
#define GAME_HEIGHT 480
#define FRAME_TIME 16666
extern int TILE_SIZE;
#define GRAVITY 1
#define MAX_VERTICAL_SPEED 25
#define PLAYER_JUMP_POWER 25
#define PLAYER_SPEED 4
extern int PLAYER_WIDTH;
extern int PLAYER_HEIGHT;
#define BULLET_HEIGHT 8 * WINDOW_HEIGHT / 480
#define BULLET_WIDTH 8 * WINDOW_HEIGHT / 480
#define MAX_PLAYERS 10
#define CLIENT_FRAME_TIME 15
#define SERVER_FRAME_TIME 15
#define X_AXIS 1
#define Y_AXIS 0
#define BUF_MAX 256
#define SPAWN_X 310
#define SPAWN_Y 10
#define LEFT_KEY 0x01
#define RIGHT_KEY 0x02
#define UP_KEY 0x04
#define DOWN_KEY 0x08
#define ATTACK_KEY 0x10
#define BUTTON_WIDTH WINDOW_WIDTH / 5
#define BUTTON_HEIGHT WINDOW_HEIGHT / 5


#define GAME_PLAYER_SIZE 16
#define GAME_TILE_SIZE 32
#define GAME_BULLET_SIZE 8
extern bool finger_was_up;
extern SDL_Texture *tex_window;
extern SDL_Rect tex_window_dst;
#endif
