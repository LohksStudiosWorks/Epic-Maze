#include "map.hpp"

#include <stdexcept>
#include <cmath>

#include <algorithm>
#include <array>
#include <cstdint>
#include <queue>
#include <random>
#include <stdexcept>
#include <vector>

static std::mt19937& random_engine();

Map::Map(int x, int y)
    : m_w(x), m_h(y), m_map(std::make_unique<BlockId[]>((x < 0 || y < 0) ? 1 : x * y))
{
    if (x < 0 || y < 0) throw std::invalid_argument("Map size invalid!");

    generate();
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

void Map::generate() {
    do {
        std::fill_n(
            m_map.get(),
            static_cast<std::size_t>(m_w) * m_h,
            BlockId::SOLID
        );

        const int cols = (m_w + 1) / 2;
        const int rows = (m_h + 1) / 2;

        std::vector<bool> visited(cols * rows, false);

        auto room_index = [cols](int x, int y) {
            return y * cols + x;
        };

        std::vector<std::pair<int, int>> stack;

        stack.emplace_back(0, 0);
        visited[0] = true;

        set(0, 0, BlockId::AIR);

        constexpr std::array<std::pair<int, int>, 4> directions{{
            { 0, -1 },
            { 0,  1 },
            {-1,  0 },
            { 1,  0 }
        }};

        auto& rng = random_engine();

        while (!stack.empty()) {
            const auto [cx, cy] = stack.back();

            auto shuffled = directions;

            std::shuffle(
                shuffled.begin(),
                shuffled.end(),
                rng
            );

            bool found = false;

            for (const auto& [dx, dy] : shuffled) {
                const int nx = cx + dx;
                const int ny = cy + dy;

                if (nx < 0 || ny < 0 || nx >= cols || ny >= rows)
                    continue;

                if (visited[room_index(nx, ny)])
                    continue;

                set(cx * 2 + dx, cy * 2 + dy, BlockId::AIR);
                set(nx * 2, ny * 2, BlockId::AIR);

                visited[room_index(nx, ny)] = true;

                stack.emplace_back(nx, ny);

                found = true;
                break;
            }

            if (!found)
                stack.pop_back();
        }

        std::uniform_int_distribution<int> exit_distribution(0, rows - 1);

        const int exit_y = exit_distribution(rng) * 2;
        const int exit_x = m_w - 1;

        for (int x = (cols - 1) * 2; x <= exit_x; ++x)
            set(x, exit_y, BlockId::AIR);

        set(exit_x, exit_y, BlockId::END);
    } while (!solve());
}

bool Map::solve() {
    const int total = m_w * m_h;

    const int start = 0;
    int end = -1;

    for (int x = m_w - 1; x >= 0; --x) {
        for (int y = 0; y < m_h; ++y) {
            if (read(x, y) == BlockId::END) {
                end = y * m_w + x;
                break;
            }
        }

        if (end != -1)
            break;
    }

    if (end == -1)
        return false;

    std::vector<int> parent(total, -1);
    std::queue<int> queue;

    queue.push(start);
    parent[start] = start;

    constexpr std::array<std::pair<int, int>, 4> directions{{
        { 0, -1 },
        { 0,  1 },
        {-1,  0 },
        { 1,  0 }
    }};

    while (!queue.empty()) {
        const int current = queue.front();
        queue.pop();

        if (current == end)
            break;

        const int x = current % m_w;
        const int y = current / m_w;

        for (const auto& [dx, dy] : directions) {
            const int nx = x + dx;
            const int ny = y + dy;

            if (nx < 0 || ny < 0 || nx >= m_w || ny >= m_h)
                continue;

            const int next = ny * m_w + nx;

            if (parent[next] != -1)
                continue;

            const BlockId block = read(nx, ny);

            if (block != BlockId::AIR &&
                block != BlockId::VISITED &&
                block != BlockId::END)
                continue;

            parent[next] = current;
            queue.push(next);
        }
    }

    if (parent[end] == -1)
        return false;

    for (int current = end; current != start; current = parent[current]) {
        if (current != end)
            m_map[current] = BlockId::VISITED;
    }

    if (start != end)
        m_map[start] = BlockId::VISITED;

    return true;
}


std::mt19937& random_engine() {
    static thread_local std::mt19937 engine{
        std::random_device{}()
    };

    return engine;
}