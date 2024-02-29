#include "utils.h"
#include "constans.h"
int toGameX(int x) {
    float scale = (float) WINDOW_WIDTH / GAME_WIDTH;
    return x * scale;
}
int toGameY(int y){
    float scale = (float) WINDOW_HEIGHT / GAME_HEIGHT;
    return y * scale;
}
