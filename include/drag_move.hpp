#pragma once

#include <cstdint>

class DragMove {
public:
    void set(float x, float y);
    void move(float x, float y);

    bool moved() const;
    float get_origin_x() const;
    float get_origin_y() const;

    float get_dx() const;
    float get_dy() const;
private:
    float m_orig_x{}, m_orig_y{};
    float m_fin_x{}, m_fin_y{};
    bool m_moved = false;
};