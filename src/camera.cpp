#include "camera.hpp"

#include <cmath>

#include "defaults.hpp"

void Camera::reset() {
    m_cache = m_calc = m_draw = {};
}

void Camera::tick() {
    constexpr float smooth_factor = 0.125f;
    constexpr float laziness_x = 0.3f * c_target_app_region.x;
    constexpr float laziness_y = 0.125f * c_target_app_region.y;
    m_draw = m_calc;

    m_calc.x += (lazy_move(m_calc.x, m_cache.x, laziness_x) - m_calc.x) * smooth_factor;
    m_calc.y += (lazy_move(m_calc.y, m_cache.y, laziness_y) - m_calc.y) * smooth_factor;

    m_interp.set();
}

void Camera::follow(const float x, const float y) {
    m_cache.x = x - c_target_app_region.x * 0.5f;
    m_cache.y = y - c_target_app_region.y * 0.5f;
}

SDL_FPoint Camera::transl(const SDL_FPoint& p) const {
    const double prop = m_interp.get_prop();
    const float cam_x = (1.0 - prop) * m_draw.x + prop * m_calc.x;
    const float cam_y = (1.0 - prop) * m_draw.y + prop * m_calc.y;
    return {
        p.x - cam_x,
        p.y - cam_y
    };
}

SDL_Rect Camera::calculated_limits() const {
    const float min_x = (m_calc.x / block_size_fixed.x) - 1.0f;
    const float min_y = (m_calc.y / block_size_fixed.y) - 1.0f;

    return SDL_Rect{
        .x = static_cast<int>(min_x),
        .y = static_cast<int>(min_y),
        .w = max_area_visible.x,
        .h = max_area_visible.y,
    };
}

float Camera::lazy_move(const float actual, const float desired, const float laziness) {
    const float delta = desired - actual;
    if (fabsf(delta) < laziness) return actual;
    const float correction = delta < 0.0f ? delta + laziness : delta - laziness;
    return actual + correction;
}