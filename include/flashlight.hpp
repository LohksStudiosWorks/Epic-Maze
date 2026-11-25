#pragma once

#include <SDL3/SDL.h>

#include <memory>

#include "player.hpp"
#include "map.hpp"
#include "camera.hpp"

class Flashlight {
public:
    Flashlight(const Flashlight&) = delete;
    Flashlight(Flashlight&&) = delete;
    void operator=(const Flashlight&) = delete;
    void operator=(Flashlight&&) = delete;

    Flashlight(std::shared_ptr<Player> player, std::shared_ptr<Map> map, Camera& camera, SDL_Renderer* render);
    ~Flashlight();

    void refresh();
    void draw();
private:
    SDL_FPoint cast_ray(SDL_FPoint origin, SDL_FPoint direction, float max_distance) const;

    SDL_Renderer* const m_renderer;
    std::shared_ptr<Player> m_player_follow;
    std::shared_ptr<Map> m_map_ref;
    const Camera& m_camera_ref;

    SDL_Texture* m_overlay;
    SDL_Texture* m_light_accumulation;

    static constexpr float m_fov = 90.0f;
    static constexpr float m_range = 450.0f;
    static constexpr float m_darkness = 0.97f;
    
    SDL_BlendMode m_accumulate_blend;
    SDL_BlendMode m_erase_blend;
};