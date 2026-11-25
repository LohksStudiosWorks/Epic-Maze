#pragma once

#include <SDL3/SDL.h>

#include "interpolator.hpp"

class Camera {
public:
    Camera(SDL_Renderer* render);

    void reset();
    void tick();

    void follow(const float x, const float y);

    SDL_FPoint transl(const SDL_FPoint& p) const;
    SDL_FPoint get_offset() const;

    void set_zoom(float zoom);
    void undo_zoom();
    void redo_zoom();
    float get_zoom() const;

    SDL_Rect calculated_limits() const;
private:
    static float lazy_move(const float actual, const float desired, const float laziness);

    SDL_Renderer* const m_render;
    SDL_FPoint m_draw{}, m_calc{}, m_cache{};
    Interpolator m_interp;
    float m_zoom = 1.0f;
};