cmake_minimum_required(VERSION 3.28)

project(EpicMaze
    VERSION 3.0.0
    DESCRIPTION "EpicMaze is now a remake made in SDL3"
    LANGUAGES CXX
)

include(cmake/installSDL3plus.cmake)


file(GLOB_RECURSE MAIN_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

sdl3plus_add_game(${PROJECT_NAME}
    SOURCES
        ${MAIN_SOURCES}
    ICON
        resources/icon.png
)

sdl3plus_add_android_key_file(${PROJECT_NAME} resources/release-key.jks)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(${PROJECT_NAME} PUBLIC GAME_VERSION="${CMAKE_PROJECT_VERSION}")

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

b_embed(${PROJECT_NAME} resources/DejaVuSans.ttf)
b_embed(${PROJECT_NAME} resources/block_end.png)
b_embed(${PROJECT_NAME} resources/block.png)
b_embed(${PROJECT_NAME} resources/dirt.png)
b_embed(${PROJECT_NAME} resources/icon.png)
b_embed(${PROJECT_NAME} resources/instr.png)
b_embed(${PROJECT_NAME} resources/intro.png)
b_embed(${PROJECT_NAME} resources/menu_block.png)
b_embed(${PROJECT_NAME} resources/menu_opt_01.png)
b_embed(${PROJECT_NAME} resources/menu_opt_01_s.png)
b_embed(${PROJECT_NAME} resources/menu_opt_02.png)
b_embed(${PROJECT_NAME} resources/menu_opt_02_s.png)
b_embed(${PROJECT_NAME} resources/menu_opt_03.png)
b_embed(${PROJECT_NAME} resources/menu_opt_03_s.png)
b_embed(${PROJECT_NAME} resources/menu_opt_04.png)
b_embed(${PROJECT_NAME} resources/pause_00.png)
b_embed(${PROJECT_NAME} resources/pause_01.png)
b_embed(${PROJECT_NAME} resources/pause_02.png)
b_embed(${PROJECT_NAME} resources/pause_03.png)
b_embed(${PROJECT_NAME} resources/pause_04.png)
b_embed(${PROJECT_NAME} resources/pause_05.png)
b_embed(${PROJECT_NAME} resources/pause_06.png)
b_embed(${PROJECT_NAME} resources/pause_07.png)
b_embed(${PROJECT_NAME} resources/pause_08.png)
b_embed(${PROJECT_NAME} resources/pause_09.png)
b_embed(${PROJECT_NAME} resources/pause_10.png)
b_embed(${PROJECT_NAME} resources/pause_11.png)
b_embed(${PROJECT_NAME} resources/pause_12.png)
b_embed(${PROJECT_NAME} resources/pause_13.png)
b_embed(${PROJECT_NAME} resources/pause_14.png)
b_embed(${PROJECT_NAME} resources/pause_15.png)
b_embed(${PROJECT_NAME} resources/pause_16.png)
b_embed(${PROJECT_NAME} resources/pause_17.png)
b_embed(${PROJECT_NAME} resources/pause_18.png)
b_embed(${PROJECT_NAME} resources/pause_19.png)
b_embed(${PROJECT_NAME} resources/pause_20.png)
b_embed(${PROJECT_NAME} resources/pause_21.png)
b_embed(${PROJECT_NAME} resources/pause_22.png)
b_embed(${PROJECT_NAME} resources/pause_23.png)
b_embed(${PROJECT_NAME} resources/pause_24.png)
b_embed(${PROJECT_NAME} resources/pause_25.png)
b_embed(${PROJECT_NAME} resources/pause_26.png)
b_embed(${PROJECT_NAME} resources/pause_27.png)
b_embed(${PROJECT_NAME} resources/pause_28.png)
b_embed(${PROJECT_NAME} resources/pause_29.png)
b_embed(${PROJECT_NAME} resources/pause_continue.png)
b_embed(${PROJECT_NAME} resources/pause_exit.png)
b_embed(${PROJECT_NAME} resources/person_f01.png)
b_embed(${PROJECT_NAME} resources/person_f02.png)
b_embed(${PROJECT_NAME} resources/person.png)
b_embed(${PROJECT_NAME} resources/rec_back.png)
b_embed(${PROJECT_NAME} resources/saving_score.png)
b_embed(${PROJECT_NAME} resources/vignette.png)