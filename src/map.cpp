#include "map.hpp"

#include <stdexcept>

Map::Map(int x, int y)
    : m_w(x), m_h(y), m_map(std::make_unique<BlockId[]>((x < 0 || y < 0) ? 1 : x * y))
{
    if (x < 0 || y < 0) throw std::invalid_argument("Map size invalid!");
    
    for (int xx = 0; xx < m_w; ++xx) {
        for (int yy = 0; yy < m_h; ++yy) {
            set(xx, yy, ((xx + yy) % 2 == 0) ? Map::BlockId::SOLID : Map::BlockId::AIR);
        }
    }
}

Map::BlockId Map::read(int x, int y) const {
    if (x >= m_w || y >= m_h || x < 0 || y < 0) return Map::BlockId::SOLID;
    return *(m_map.get() + y * m_w + x);
}

bool Map::set(int x, int y, BlockId id) {
    if (x >= m_w || y >= m_h || x < 0 || y < 0) return false;
    *(m_map.get() + y * m_w + x) = id;
    return true;
}