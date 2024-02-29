#include "menu.h"
#include "utils.h"


void server_or_client(SDL_Renderer *renderer, char *menu, TTF_Font *font){
    SDL_Event e;
    int pressed = false;
    SDL_StartTextInput();
    while (!pressed) {
        if(!SDL_IsScreenKeyboardShown(SDL_RenderGetWindow(renderer))){
            SDL_StartTextInput();
        }
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_c) {
                    *menu = 'c';
                    pressed = true;
                } else if (e.key.keysym.sym == SDLK_s) {
                    *menu = 's';
                    pressed = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(renderer);
        disp_text(renderer, "[s]erver or [c]lient?", font, -1, toGameY(100));
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, tex_window, NULL, &tex_window_dst);
        SDL_RenderPresent(renderer);
        SDL_SetRenderTarget(renderer, tex_window);
    }
    SDL_StopTextInput();
}


void ask_for_ip(SDL_Renderer *renderer, TTF_Font *font, char *ip) {
    memset(ip, ' ', 15);
    SDL_Event e;
    int position = 0;
    int ok = false;
    while (!ok) {
        if(!SDL_IsScreenKeyboardShown(SDL_RenderGetWindow(renderer))){
            SDL_StartTextInput();
        }
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if ((e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) || e.key.keysym.sym == SDLK_PERIOD) {
                    if (position > 14) {
                        position = 14;
                    }
                    ip[position] = e.key.keysym.sym;
                    position++;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    position--;
                    if (position < 0) {
                        position = 0;
                    }
                    ip[position] = ' ';
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    ip[position] = 0;
                    ok = true;
                }
            }
        }
        usleep(200);
        SDL_RenderClear(renderer);

        disp_text(renderer, "ip addres", font, -1, toGameY(100));
        disp_text(renderer, ip, font, -1, toGameY(130));

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, tex_window, NULL, &tex_window_dst);
        SDL_RenderPresent(renderer);
        SDL_SetRenderTarget(renderer, tex_window);

    }
    SDL_StopTextInput();
}
