#include "drag_move.hpp"

void DragMove::set(float x, float y) {
    m_fin_x = m_orig_x = x;
    m_fin_y = m_orig_y = y;
    m_moved = false;
}

void DragMove::move(float x, float y) {
    m_fin_x = x;
    m_fin_y = y;
    m_moved = true;
}

bool DragMove::moved() const {
    return m_moved;
}

float DragMove::get_origin_x() const {
    return m_orig_x;
}

float DragMove::get_origin_y() const {
    return m_orig_y;
}

float DragMove::get_dx() const {
    return m_fin_x - m_orig_x;
}

float DragMove::get_dy() const {
    return m_fin_y - m_orig_y;
}
