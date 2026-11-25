#pragma once

#include <cstdint>
#include <memory>

#include <SDL3/SDL.h>

class Map {
public:
    enum class BlockId : uint8_t {
        SOLID,
        AIR,
        VISITED,
        NOEXIT,
        END
    };

    Map(int x, int y);

    BlockId read(int x, int y) const;
    bool set(int x, int y, BlockId id);
private:
    void generate();

    bool solve();

    const int m_w, m_h;
    const std::unique_ptr<BlockId[]> m_map;
};