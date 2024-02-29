#include "objects.h"
#include "physic.h"
#include "constans.h"
#include "ui.h"


void resolve_player_key_down(int key, struct Player *player) {
    if (key == player->left_key) {
        player->left = true;
    }
    if (key == player->right_key) {
        player->right = true;
    }
    if (key == player->up_key) {
        player->up = true;
    }
    if (key == player->down_key) {
        player->down = true;
    }
    if (key == player->attack_key) {
        player->shoot = player->face;
    }
}

void resolve_player_key_up(int key, struct Player *player) {
    if (key == player->left_key) {
        player->left = false;
    }
    if (key == player->right_key) {
        player->right = false;
    }
    if (key == player->up_key) {
        player->up = false;
    }
    if (key == player->down_key) {
        player->down = false;
    }
    if (key == player->attack_key) {
        player->shoot = false;
    }
}


void resolve_keyboard(SDL_Event *e, struct Player *player) {
    if (e->type == SDL_KEYDOWN) {
        resolve_player_key_down(e->key.keysym.sym, player);
    }
    if (e->type == SDL_KEYUP) {
        resolve_player_key_up(e->key.keysym.sym, player);
    }
}

void resolve_touch(Button *buttons[], SDL_Event *e,
                   struct Player *player) { // left = 0, right = 1, jump = 2
    bool any_button_pressed = false;

    bool leftpressed = false;
    bool rightpressed = false;
    bool uppressed = false;
    bool shot = false;

    int num_fingers = SDL_GetNumTouchFingers(e->tfinger.touchId);
    for (int i = 0; i < num_fingers; i++) {

        any_button_pressed = false;
        SDL_Finger *finger = SDL_GetTouchFinger(e->tfinger.touchId, i);

        if (isButtonPressed(buttons[0], finger)) { // left
            any_button_pressed = true;
            player->left = true;
            SetButtonTexPressed(buttons[0]);
            leftpressed = true;
        } else if (!leftpressed) {
            player->left = false;
            SetButtonTexNormal(buttons[0]);
        }

        if (isButtonPressed(buttons[1], finger)) { // right
            any_button_pressed = true;
            player->right = true;
            SetButtonTexPressed(buttons[1]);
            rightpressed = true;
        } else if( !rightpressed ){
            player->right = false;
            SetButtonTexNormal(buttons[1]);

        }

        if (isButtonPressed(buttons[2], finger)) { // jump
            any_button_pressed = true;
            player->up = true;
            SetButtonTexPressed(buttons[2]);
            uppressed = true;
        } else if(!uppressed){
            player->up = false;
            SetButtonTexNormal(buttons[2]);
        }

        if (!any_button_pressed && finger_was_up) {
            player->shoot = player->face;
            shot = true;
            finger_was_up = false;
        } else if(!shot) {
            player->shoot = false;
        }
    }
    if (num_fingers <= 0) {
        player->left = false;
        player->right = false;
        player->up = false;
        player->shoot = false;
        for( int i = 0 ; i < 3 ; i++ ) {
            SetButtonTexNormal(buttons[i]);
        }
    }
}

void set_player_pos(struct Player *player, float x, float y) {
    player->position.x = x;
    player->position.y = y;
}

struct Bullet init_bullet(int x, int y, int face) {
    struct Bullet b;
    b.position.x = x;
    b.position.y = y;
    b.position.w = GAME_BULLET_SIZE;
    b.position.h = GAME_BULLET_SIZE;
    b.face = face;
    return b;
}


