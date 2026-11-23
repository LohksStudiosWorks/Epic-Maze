#pragma once

#include <cstdint>
#include <SDL3/SDL.h>

constexpr SDL_Point c_default_app_size = {1280, 720};
constexpr SDL_Point c_target_app_region = {1920, 1080};
constexpr SDL_WindowFlags c_default_app_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
#if defined(SDL_PLATFORM_ANDROID)
    | SDL_WINDOW_FULLSCREEN
#endif
;

// Game things
constexpr SDL_FPoint block_size_fixed = { 192.0f, 192.0f };
constexpr SDL_FPoint player_size_fixed = { 48.0f, 48.0f };
constexpr SDL_Point max_area_visible = { 
    static_cast<int>(2.5f + c_target_app_region.x / block_size_fixed.x),
    static_cast<int>(2.5f + c_target_app_region.y / block_size_fixed.y)
};

constexpr SDL_Point c_shadows_orig_resolution = {1920, 1080};
constexpr SDL_Point c_shadows_resolution = {48 * 8, 27 * 8};

constexpr SDL_FPoint c_touch_size = {256.0f, 256.0f};

constexpr float c_levels_zoom[3] = {
    0.5f,
    1.0f,
    1.4f
};