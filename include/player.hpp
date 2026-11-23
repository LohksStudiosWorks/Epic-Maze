#pragma once

#include <SDL3/SDL.h>

#include "interpolator.hpp"

class Player {
public:
    Player(const float x, const float y);

    void move(float dir_x, float dir_y);
    bool has_moved() const;

    float draw_get_x() const;
    float draw_get_y() const;
    float draw_get_rot() const;

    float get_x() const;
    float get_y() const;

    void cancel();

    void tick();

    SDL_FRect get_rect() const;
private:
    struct pos { float x{}, y{}; };

    pos m_accel;
    pos m_desired, m_draw;
    float m_target_rot = 90.0f, m_draw_rot = 90.0f, m_final_rot = 90.0f;
    Interpolator m_interp;
    bool m_moved{};
};