#pragma once

#include <SDL3/SDL.h>

#include "resource_manager.hpp"
#include "map.hpp"
#include "player.hpp"
#include "camera.hpp"

class Game {
public:
    Game(ResourceManager& resources, SDL_Renderer* renderer);

    void draw();
    SDL_AppResult interact(SDL_Event& event);
    void tick(Uint32 elapsed);
private:
    void draw_tex_at(SDL_Texture* tex, const SDL_FPoint pos);
    void draw_tex_at(SDL_Texture* tex, const SDL_FPoint pos, const SDL_FPoint force_size);
    void draw_tex_at(SDL_Texture* tex, const SDL_FPoint pos, const float rot, const SDL_FPoint force_size);
    void tile_tex_bg(SDL_Texture* tex, const float off);

    enum class Scene {
        MENU,
        PLAY,
        INSTR,
        RECORDS,
        GAMING,
        PAUSE,

        EXIT
    };

    std::unique_ptr<Map> m_map;
    std::unique_ptr<Player> m_player;

    SDL_Renderer* const m_renderer;
    ResourceManager& m_res_mng;
    Camera m_cam;
    
    Scene m_scene = Scene::MENU;
    uint16_t m_sel_item = 0;
    uint16_t m_sel_difficulty = 0;
    uint64_t m_elapsed_time = 0;
};