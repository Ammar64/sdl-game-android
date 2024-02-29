cmake_minimum_required(VERSION 3.6)

project(MY_APP)

find_library(SDL2 SDL2)
find_library(SDL2_ttf SDL2_ttf)

add_library(main SHARED)

target_sources(main PRIVATE
        client_udp.c
        font.c
        list.c
        main.c
        menu.c
        network.c
        objects.c
        physic.c
        server_udp.c
        utils.c
        ui.c)

find_library(pthread pthread)
target_link_libraries(main SDL2 SDL2_ttf)


