#pragma once

#include <cstdint>
#include <memory>

class Map {
public:
    enum class BlockId : uint8_t {
        AIR,
        SOLID,
        END
    };

    Map(int x, int y);

    BlockId read(int x, int y) const;
    bool set(int x, int y, BlockId id);
private:
    const int m_w, m_h;
    const std::unique_ptr<BlockId[]> m_map;
};