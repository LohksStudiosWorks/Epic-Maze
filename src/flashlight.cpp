#include "flashlight.hpp"
#include "defaults.hpp"

#include <cmath>
#include <vector>
#include <algorithm>

Flashlight::Flashlight(std::shared_ptr<Player> player, std::shared_ptr<Map> map, Camera& camera, SDL_Renderer* render) :
    m_renderer(render),
    m_player_follow(player),
    m_map_ref(map),
    m_camera_ref(camera)
{
    m_overlay = SDL_CreateTexture(m_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        c_shadows_resolution.x,
        c_shadows_resolution.y
    );

    SDL_SetTextureBlendMode(m_overlay, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(m_overlay, SDL_SCALEMODE_PIXELART);

    SDL_SetRenderTarget(m_renderer, m_overlay);
    SDL_SetRenderLogicalPresentation(
        m_renderer,
        c_shadows_orig_resolution.x, c_shadows_orig_resolution.y,
        SDL_LOGICAL_PRESENTATION_STRETCH
    );
    SDL_SetRenderTarget(m_renderer, nullptr);

    m_erase_blend = SDL_ComposeCustomBlendMode(
        SDL_BLENDFACTOR_ZERO,
        SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        SDL_BLENDOPERATION_ADD,

        SDL_BLENDFACTOR_ZERO,
        SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        SDL_BLENDOPERATION_ADD
    );
    

    m_light_accumulation = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        c_shadows_resolution.x,
        c_shadows_resolution.y
    );

    SDL_SetTextureBlendMode(m_light_accumulation, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(m_renderer, m_light_accumulation);

    SDL_SetRenderLogicalPresentation(
        m_renderer,
        c_shadows_orig_resolution.x, c_shadows_orig_resolution.y,
        SDL_LOGICAL_PRESENTATION_STRETCH
    );

    SDL_SetRenderTarget(m_renderer, nullptr);

    m_accumulate_blend = SDL_ComposeCustomBlendMode(
        SDL_BLENDFACTOR_ZERO,
        SDL_BLENDFACTOR_ONE,
        SDL_BLENDOPERATION_ADD,
        SDL_BLENDFACTOR_ONE,
        SDL_BLENDFACTOR_ONE,
        SDL_BLENDOPERATION_ADD
    );
}

Flashlight::~Flashlight() {
    SDL_DestroyTexture(m_overlay);
    SDL_DestroyTexture(m_light_accumulation);
}

void Flashlight::refresh() {    
    constexpr float PI = 3.14159265358979323846f;
    constexpr float TWO_PI = 2.0f * PI;
    constexpr float DEG_TO_RAD = PI / 180.0f;

    constexpr int segments = 384;
    constexpr int rings = 16;

    // Soft shadow settings.
    constexpr int light_samples = 8;
    constexpr float light_radius = 12.0f;

    // Existing radial fade.
    constexpr float fade_distance = 2.0f;

    const float zoom = m_camera_ref.get_zoom();

    const SDL_FPoint origin {
        m_player_follow->draw_get_x() + player_size_fixed.x * 0.5f,
        m_player_follow->draw_get_y() + player_size_fixed.y * 0.5f
    };

    const float rotation =
        (m_player_follow->draw_get_rot() - 90.0f) * DEG_TO_RAD;

    const float half_fov = m_fov * 0.5f * DEG_TO_RAD;

    const auto smoothstep = [](float a, float b, float x) {
        const float t = std::clamp((x - a) / (b - a), 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    };

    std::vector<SDL_FPoint> light_offsets;
    light_offsets.reserve(light_samples);

    light_offsets.push_back({ 0.0f, 0.0f });

    for (int i = 0; i < light_samples - 1; ++i) {
        const float angle =
            TWO_PI * static_cast<float>(i) / (light_samples - 1);

        light_offsets.push_back({
            std::cos(angle) * light_radius,
            std::sin(angle) * light_radius
        });
    }

    auto generate_light_geometry = [&](SDL_FPoint sample_origin,
                                       std::vector<SDL_Vertex>& vertices,
                                       std::vector<int>& indices,
                                       float weight) {
        vertices.clear();
        indices.clear();

        const SDL_FPoint screen_position =
            m_camera_ref.transl(sample_origin);

        const SDL_FPoint center {
            screen_position.x * zoom,
            screen_position.y * zoom
        };

        std::vector<SDL_FPoint> endpoints;
        std::vector<float> endpoint_distances;

        endpoints.reserve(segments + 1);
        endpoint_distances.reserve(segments + 1);

        // Cast the flashlight rays.
        for (int s = 0; s <= segments; ++s) {
            const float t = static_cast<float>(s) / segments;

            const float angle =
                rotation + (t * 2.0f - 1.0f) * half_fov;

            const SDL_FPoint direction {
                std::cos(angle),
                std::sin(angle)
            };

            const SDL_FPoint hit =
                cast_ray(sample_origin, direction, m_range);

            const SDL_FPoint screen_hit =
                m_camera_ref.transl(hit);

            endpoints.push_back({
                screen_hit.x * zoom,
                screen_hit.y * zoom
            });

            endpoint_distances.push_back(
                std::hypot(
                    hit.x - sample_origin.x,
                    hit.y - sample_origin.y
                )
            );
        }

        // Generate the radial geometry.
        vertices.reserve((rings + 1) * (segments + 1));
        indices.reserve(rings * segments * 6);

        for (int r = 0; r <= rings; ++r) {
            const float radius = static_cast<float>(r) / rings;

            for (int s = 0; s <= segments; ++s) {
                const float distance =
                    endpoint_distances[s] * radius;

                const float remaining =
                    endpoint_distances[s] - distance;

                const float radial_alpha =
                    smoothstep(0.0f, fade_distance, remaining);

                const float t = static_cast<float>(s) / segments;

                const float edge = std::min(t, 1.0f - t);

                const float angular_alpha =
                    smoothstep(0.0f, 0.12f, edge);

                const float alpha =
                    radial_alpha * angular_alpha * weight;

                const SDL_FPoint endpoint = endpoints[s];

                vertices.push_back({
                    .position = {
                        center.x + (endpoint.x - center.x) * radius,
                        center.y + (endpoint.y - center.y) * radius
                    },
                    .color = {
                        1.0f, 1.0f, 1.0f, alpha
                    },
                    .tex_coord = { 0.0f, 0.0f }
                });
            }
        }

        // Generate triangle indices.
        for (int r = 0; r < rings; ++r) {
            for (int s = 0; s < segments; ++s) {
                const int a = r * (segments + 1) + s;
                const int b = a + 1;
                const int c = a + segments + 1;
                const int d = c + 1;

                indices.insert(indices.end(), {
                    a, c, b,
                    b, c, d
                });
            }
        }
    };

    auto generate_circle = [&](SDL_FPoint center,
                               std::vector<SDL_Vertex>& vertices,
                               std::vector<int>& indices) {
        const int base = static_cast<int>(vertices.size());

        constexpr int circle_segments = 32;
        constexpr int circle_rings = 8;

        const float radius = 35.0f * zoom;

        for (int r = 0; r <= circle_rings; ++r) {
            const float t = static_cast<float>(r) / circle_rings;

            const float alpha =
                1.0f - smoothstep(0.65f, 1.0f, t);

            for (int i = 0; i <= circle_segments; ++i) {
                const float angle =
                    TWO_PI * static_cast<float>(i) / circle_segments;

                vertices.push_back({
                    .position = {
                        center.x + std::cos(angle) * radius * t,
                        center.y + std::sin(angle) * radius * t
                    },
                    .color = {
                        1.0f, 1.0f, 1.0f, alpha
                    },
                    .tex_coord = { 0.0f, 0.0f }
                });
            }
        }

        for (int r = 0; r < circle_rings; ++r) {
            for (int i = 0; i < circle_segments; ++i) {
                const int a =
                    base + r * (circle_segments + 1) + i;

                const int b = a + 1;
                const int c = a + circle_segments + 1;
                const int d = c + 1;

                indices.insert(indices.end(), {
                    a, c, b,
                    b, c, d
                });
            }
        }
    };

    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;

    // ---------------------------------------------------------
    // 1. Accumulate visibility from all light samples.
    // ---------------------------------------------------------

    SDL_SetRenderTarget(m_renderer, m_light_accumulation);

    SDL_SetRenderScale(m_renderer, 1.0f, 1.0f);

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
    SDL_RenderClear(m_renderer);

    SDL_SetRenderDrawBlendMode(m_renderer, m_accumulate_blend);

    for (const SDL_FPoint& offset : light_offsets) {
        const SDL_FPoint sample_origin {
            origin.x + offset.x,
            origin.y + offset.y
        };

        generate_light_geometry(
            sample_origin,
            vertices,
            indices,
            1.0f / light_samples
        );

        SDL_RenderGeometry(
            m_renderer,
            nullptr,
            vertices.data(),
            static_cast<int>(vertices.size()),
            indices.data(),
            static_cast<int>(indices.size())
        );
    }

    // ---------------------------------------------------------
    // 2. Add the central circular light.
    // ---------------------------------------------------------

    const SDL_FPoint screen_position =
        m_camera_ref.transl(origin);

    const SDL_FPoint center {
        screen_position.x * zoom,
        screen_position.y * zoom
    };

    vertices.clear();
    indices.clear();

    generate_circle(center, vertices, indices);

    SDL_RenderGeometry(
        m_renderer,
        nullptr,
        vertices.data(),
        static_cast<int>(vertices.size()),
        indices.data(),
        static_cast<int>(indices.size())
    );

    // ---------------------------------------------------------
    // 3. Clear the darkness mask.
    // ---------------------------------------------------------

    SDL_SetRenderTarget(m_renderer, m_overlay);

    SDL_SetRenderScale(m_renderer, 1.0f, 1.0f);

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);

    SDL_SetRenderDrawColor(
        m_renderer,
        0, 0, 0,
        static_cast<Uint8>(
            std::clamp(m_darkness, 0.0f, 1.0f) * 255.0f
        )
    );

    SDL_RenderClear(m_renderer);

    // ---------------------------------------------------------
    // 4. Erase darkness using the accumulated visibility mask.
    // ---------------------------------------------------------

    SDL_SetTextureBlendMode(m_light_accumulation, m_erase_blend);

    const SDL_FRect destination {
        0.0f,
        0.0f,
        static_cast<float>(c_shadows_orig_resolution.x),
        static_cast<float>(c_shadows_orig_resolution.y)
    };

    SDL_RenderTexture(
        m_renderer,
        m_light_accumulation,
        nullptr,
        &destination
    );

    // ---------------------------------------------------------
    // 5. Restore renderer state.
    // ---------------------------------------------------------

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(m_renderer, nullptr);
}

void Flashlight::draw() {
    const SDL_FRect destination {
        0.0f,
        0.0f,
        static_cast<float>(c_target_app_region.x),
        static_cast<float>(c_target_app_region.y)
    };

    SDL_RenderTexture(
        m_renderer,
        m_overlay,
        nullptr,
        &destination
    );
}

SDL_FPoint Flashlight::cast_ray(SDL_FPoint origin, SDL_FPoint direction, float max_distance) const {
    const float tile_x = block_size_fixed.x;
    const float tile_y = block_size_fixed.y;

    int map_x = static_cast<int>(std::floor(origin.x / tile_x));
    int map_y = static_cast<int>(std::floor(origin.y / tile_y));

    const int step_x = direction.x >= 0.0f ? 1 : -1;
    const int step_y = direction.y >= 0.0f ? 1 : -1;

    const float delta_x = direction.x != 0.0f
        ? tile_x / std::abs(direction.x)
        : INFINITY;

    const float delta_y = direction.y != 0.0f
        ? tile_y / std::abs(direction.y)
        : INFINITY;

    float side_x = direction.x >= 0.0f
        ? ((map_x + 1) * tile_x - origin.x) / direction.x
        : (origin.x - map_x * tile_x) / -direction.x;

    float side_y = direction.y >= 0.0f
        ? ((map_y + 1) * tile_y - origin.y) / direction.y
        : (origin.y - map_y * tile_y) / -direction.y;

    while (true) {
        const float distance = std::min(side_x, side_y);

        if (distance > max_distance)
            break;

        if (side_x < side_y) {
            side_x += delta_x;
            map_x += step_x;
        }
        else {
            side_y += delta_y;
            map_y += step_y;
        }

        if (m_map_ref->read(map_x, map_y) == Map::BlockId::SOLID) {
            return {
                origin.x + direction.x * distance,
                origin.y + direction.y * distance
            };
        }
    }

    return {
        origin.x + direction.x * max_distance,
        origin.y + direction.y * max_distance
    };
}