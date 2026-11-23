#include "player.hpp"

#include <cmath>
#include <cstdio>

#include "defaults.hpp"

Player::Player(const float x, const float y) :
    m_desired({x, y})
{}

void Player::move(int dir_x, int dir_y) {
    constexpr float thrust = 5.0f;
    constexpr float boost = 3.0f;

    m_accel.x += dir_x * thrust * (m_accel.x * dir_x < 0.0f ? boost : 1.0f); /* boost when dir changes */
    m_accel.y += dir_y * thrust * (m_accel.y * dir_y < 0.0f ? boost : 1.0f); /* boost when dir changes */

    m_moved = dir_x != 0 || dir_y != 0;

    if (m_moved) {
        constexpr float RAD_TO_DEG = 180.0f / 3.14159265358979f;

        m_final_rot = std::fmodf(std::atan2(
            static_cast<float>(dir_x),
            static_cast<float>(-dir_y)
        ) * RAD_TO_DEG, 360.0f);
    }
}

bool Player::has_moved() const {
    return m_moved;
}

float Player::draw_get_x() const {
    const double prop = m_interp.get_prop();
    return (1.0 - prop) * m_draw.x + prop * m_desired.x;
}

float Player::draw_get_y() const {
    const double prop = m_interp.get_prop();
    return (1.0 - prop) * m_draw.y + prop * m_desired.y;
}

float Player::draw_get_rot() const {
    const double prop = m_interp.get_prop();
    return (1.0 - prop) * m_draw_rot + prop * m_target_rot;
}

float Player::get_x() const {
    return m_desired.x;
}

float Player::get_y() const {
    return m_desired.y;
}

void Player::cancel() {
    const float dir_x = m_desired.x - m_draw.x;
    const float dir_y = m_desired.y - m_draw.y;

    if (m_accel.x * dir_x >= 0.0f) m_accel.x = 0.0f;
    if (m_accel.y * dir_y >= 0.0f) m_accel.y = 0.0f;

    m_desired.x = m_draw.x;
    m_desired.y = m_draw.y;
}


void Player::tick() {
    constexpr float momentum = 0.9f;

    m_accel.x *= momentum;
    m_accel.y *= momentum;

    m_draw = m_desired;
    m_draw_rot = m_target_rot;

    m_desired.x += m_accel.x;
    m_desired.y += m_accel.y;
    
    m_target_rot += std::remainder(m_final_rot - m_target_rot, 360.0f) * 0.5f;
    m_target_rot = std::fmod(m_target_rot + 360.0f, 360.0f);

    m_interp.set();
}

SDL_FRect Player::get_rect() const {
    return SDL_FRect{
        .x = m_desired.x,
        .y = m_desired.y,
        .w = player_size_fixed.x,
        .h = player_size_fixed.y
    };
}