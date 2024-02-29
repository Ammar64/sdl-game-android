#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include "objects.h"
#include "client_udp.h"
#include "server_udp.h"
#include "network.h"
#include "physic.h"
#include "constans.h"
#include "font.h"
#include "menu.h"
#include "constans.h"
#include "utils.h"
#include "ui.h"

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

int WINDOW_WIDTH;
int WINDOW_HEIGHT;

int PLAYER_WIDTH;
int PLAYER_HEIGHT;

int TILE_SIZE;

bool finger_was_up;
SDL_Texture *tex_window;
SDL_Rect tex_window_dst;
struct Player players[MAX_PLAYERS];
int number_of_players = 0;
int16_t my_id = -1;
int16_t bullets_client[256];
int bullets_number = 0;

SDL_Texture *load_texture(SDL_Renderer *renderer, char *file) {
    SDL_Surface *bitmap = NULL;
    SDL_Texture *texture = NULL;
    bitmap = SDL_LoadBMP(file);
    texture = SDL_CreateTextureFromSurface(renderer, bitmap);
    SDL_FreeSurface(bitmap);
    return texture;
}

void init_players() {
    int i;
    for (i = 0; i < MAX_PLAYERS; i++) {
        players[i].position.x = SPAWN_X;
        players[i].position.y = SPAWN_Y;
        players[i].position.w = GAME_PLAYER_SIZE;
        players[i].position.h = GAME_PLAYER_SIZE;
        players[i].left_key = SDLK_LEFT;
        players[i].right_key = SDLK_RIGHT;
        players[i].up_key = SDLK_UP;
        players[i].down_key = SDLK_DOWN;
        players[i].attack_key = SDLK_z;
        players[i].face = 1;
        players[i].shoot = false;
        players[i].y_speed = 0;
        players[i].can_jump = false;
        players[i].reloading = false;
        players[i].kills = 0;
        players[i].deaths = 0;
    }
}

void receive_new_id(int id) {
    my_id = id;
    number_of_players = id;
    printf("my_id is now: %d\n", my_id);
}

void check_if_its_new_player(int id) {
    if (id > number_of_players) {
        number_of_players = id;
        printf("new max player, now %d\n", number_of_players + 1);
    }
}


void *client_loop(void *arg) {
    int socket = *((int *) arg);
    int16_t tab[BUF_MAX];
    int length;
    int id, bullets_in_array;
    while (1) {
        length = client_listen(socket, tab);
        id = tab[0];
        if (id == -1) {
            receive_new_id(tab[1]);
        }
        SDL_Log("I'm called %i", id);
        if (id >= 0) {
            check_if_its_new_player(id);
            players[id].position.x = tab[1];
            players[id].position.y = tab[2];
            players[id].kills = tab[3];
            players[id].deaths = tab[4];
        }
        if (id == -2) {
            bullets_in_array = (length - sizeof(int16_t)) / (sizeof(int16_t) * 2);
            memcpy(bullets_client, tab + 1, sizeof(int16_t) * 2 * bullets_in_array);
            bullets_number = bullets_in_array;
        }
        usleep(50);
    }
}


int main(int argc, char *argv[]) {
    finger_was_up = true;
    struct sockaddr_in server_addr, client_addr;
    int sock_server, sock_client;
    char *server_ip_addr = NULL;

    char menu = 's';
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
    SDL_Texture *tex = NULL;
    SDL_Texture *bullet = NULL;
    SDL_Texture *map = NULL;
    TTF_Init();
    TTF_Font *font;

    font = TTF_OpenFont("resources/m5x7.ttf", 70);
    SDL_Rect displayBounds;
    SDL_GetDisplayBounds(0, &displayBounds);

    SCREEN_WIDTH = displayBounds.h;
    SCREEN_HEIGHT = displayBounds.w;

    WINDOW_HEIGHT = SCREEN_HEIGHT;
    WINDOW_WIDTH = WINDOW_HEIGHT * (4.0f / 3.0f);

    TILE_SIZE = SCREEN_HEIGHT / 15;
    PLAYER_WIDTH = (float) TILE_SIZE * (1.0f / 2.0f);
    PLAYER_HEIGHT = PLAYER_WIDTH;
    init_players();

    SDL_Log("Player size (%i, %i)", PLAYER_WIDTH, PLAYER_HEIGHT);
    window = SDL_CreateWindow(
            "game",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_FULLSCREEN);


    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    tex_window = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                   WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_Rect r = {
            (SCREEN_WIDTH / 2) - (WINDOW_WIDTH / 2),
            0,
            WINDOW_WIDTH,
            WINDOW_HEIGHT
    };
    tex_window_dst = r;
    SDL_SetRenderTarget(renderer, tex_window);
    map = get_map_texture(renderer);
    tex = load_texture(renderer, "resources/player.bmp");
    bullet = load_texture(renderer, "resources/bullet.bmp");
    int i;
    server_or_client(renderer, &menu, font);
    if (menu == 'c') {
        server_ip_addr = malloc(16 * sizeof(char));
        ask_for_ip(renderer, font, server_ip_addr);
    }
    pthread_t thread_id_server, thread_id_client, thread_id_server_send;
    server_addr = server_sock_addr(server_ip_addr);
    client_addr = client_sock_addr();
    if (menu == 's') {
        prepare_server(&sock_server, &server_addr);
        pthread_create(&thread_id_server, NULL, server_receive_loop, &sock_server);
        pthread_create(&thread_id_server_send, NULL, server_send_loop, &sock_server);
        SDL_Log("Initialized Server threads");
    }
    prepare_client(&sock_client, &client_addr);
    pthread_create(&thread_id_client, NULL, client_loop, &sock_client);

    while (my_id < 0) {
        send_to_server(sock_client, server_addr, my_id, 0);
        usleep(100);
    }

    SDL_Rect bullet_pos;
    bullet_pos.w = BULLET_WIDTH;
    bullet_pos.h = BULLET_HEIGHT;


    SDL_Event e;


    Button *buttons[] = {
            MakeButton(renderer, 30, SCREEN_HEIGHT - BUTTON_HEIGHT - 30, BUTTON_WIDTH,
                       BUTTON_HEIGHT),
            MakeButton(renderer, 30 + BUTTON_WIDTH + 20, SCREEN_HEIGHT - BUTTON_HEIGHT - 30,
                       BUTTON_WIDTH, BUTTON_HEIGHT),
            MakeButton(renderer, SCREEN_WIDTH - BUTTON_WIDTH - 120,
                       SCREEN_HEIGHT - BUTTON_HEIGHT - 30, BUTTON_WIDTH, BUTTON_HEIGHT)
    };
    while (1) {
        while (SDL_PollEvent(&e)) {
            while (e.type == SDL_QUIT) {
                goto breakAll;
            }
            resolve_touch(buttons, &e, &players[my_id]);
            resolve_keyboard(&e, &players[my_id]);
            if (e.type == SDL_FINGERUP) {
                finger_was_up = true;
            }
        }
        send_to_server(sock_client, server_addr, my_id, key_state_from_player(&players[my_id]));
        usleep(30);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, map, NULL, NULL);
        for (i = 0; i <= number_of_players; i++) {
            SDL_Rect dst = {
                    toGameX(players[i].position.x),
                    toGameY(players[i].position.y),
                    PLAYER_WIDTH,
                    PLAYER_HEIGHT
            };
            //SDL_Log("Play (%i, %i, %i, %i)", dst.x, dst.y, dst.w, dst.h);
            SDL_RenderCopy(renderer, tex, NULL, &dst);
        }

        disp_text(renderer, "kills", font, toGameX(400), toGameY(10));
        for (i = 0; i <= number_of_players; i++) {
            char kills[10] = {};
            sprintf(kills, "%d", players[i].kills);
            disp_text(renderer, kills, font, toGameX(400), toGameY(30) + i * toGameY(20));
        }

        disp_text(renderer, "deaths", font, toGameX(460), toGameY(10));
        for (i = 0; i <= number_of_players; i++) {
            char deaths[10] = {};
            sprintf(deaths, "%d", players[i].deaths);
            disp_text(renderer, deaths, font, toGameX(460), toGameY(30) + i * toGameY(20));
        }

        for (i = 0; i < bullets_number; i++) {
            bullet_pos.x = toGameX(bullets_client[i * 2]);
            bullet_pos.y = toGameY(bullets_client[i * 2 + 1]);
            SDL_RenderCopy(renderer, bullet, NULL, &bullet_pos);
        }

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, tex_window, NULL, &tex_window_dst);
        for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
            RenderButton(buttons[i], renderer);
        }
        SDL_RenderPresent(renderer);
        SDL_SetRenderTarget(renderer, tex_window);
    }
    breakAll:

    for (int i = 0; i < 3; i++) {
        DestroyButton(buttons[i]);
    }
    close(sock_client);
    close(sock_server);
    pthread_kill(thread_id_client, SIGUSR1);
    pthread_kill(thread_id_server, SIGUSR1);
    pthread_kill(thread_id_server_send, SIGUSR1);
    SDL_DestroyTexture(tex);
    SDL_DestroyTexture(bullet);
    SDL_DestroyTexture(map);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
