#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>

#include "resource_manager.hpp"
#include "game.hpp"

class Core {
public:
    Core(int argc, char *argv[]);
    ~Core();

    SDL_AppResult handle(SDL_Event& event);
    SDL_AppResult draw();
private:
    struct sdl_data {
        SDL_Window* window{};
        SDL_Renderer* renderer{};
        
    };

    //FpsCounter m_fps;
    sdl_data m_sdl;
    std::unique_ptr<ResourceManager> m_res_mgr;
    std::unique_ptr<Game> m_game;
};