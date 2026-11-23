#include "core.hpp"

#include <cmath>

#include "defaults.hpp"

Core::Core(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
#if defined(SDL_PLATFORM_LINUX)
    if (!SDL_GetEnvironmentVariable(SDL_GetEnvironment(), "SDL_VIDEO_DRIVER")) {
        SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");
    }
    else {
        SDL_Log("Overwrite of environment variable SDL_VIDEO_DRIVER detected. Note: Wayland may cause memory leaks, so take care!");
    }
#endif
#if defined(SDL_PLATFORM_ANDROID)
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
    SDL_SetHint(SDL_HINT_ANDROID_TRAP_BACK_BUTTON, "1");
#endif

    SDL_Log("Initializing video, audio, and events...");
    SDL_Init(SDL_INIT_VIDEO); //  | SDL_INIT_AUDIO | SDL_INIT_EVENTS

    SDL_Log("Initializing ttf...");
    TTF_Init();

    SDL_Log("Setup of screen and peripherals...");

    SDL_SetAppMetadata("Epic Maze v" GAME_VERSION, GAME_VERSION, "com.lsw.epicmaze");
    SDL_CreateWindowAndRenderer("Epic Maze v" GAME_VERSION, c_default_app_size.x, c_default_app_size.y, c_default_app_flags, &m_sdl.window, &m_sdl.renderer);
    SDL_SetRenderLogicalPresentation(m_sdl.renderer, c_target_app_region.x, c_target_app_region.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    if (!SDL_SetRenderVSync(m_sdl.renderer, 1)) {
        SDL_Log("VSync cannot be enabled: %s", SDL_GetError());
    }

    m_res_mgr = std::make_unique<ResourceManager>(m_sdl.renderer);
    m_game = std::make_unique<Game>(*m_res_mgr, m_sdl.renderer);

    SDL_AddTimer(50, [](void* g, SDL_TimerID tid, Uint32 interval){ 
        (void)tid;
        ((Game*)g)->tick(interval);
        return interval;
    }, (void*)m_game.get());
}

Core::~Core() {
    m_game.reset();
    m_res_mgr.reset();
}

SDL_AppResult Core::handle(SDL_Event& event) {
    switch(event.type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_UP:
        switch(event.key.scancode) {
        case SDL_SCANCODE_F11:
            SDL_SetWindowFullscreen(m_sdl.window, !(SDL_GetWindowFlags(m_sdl.window) & SDL_WINDOW_FULLSCREEN));
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return m_game->interact(event);
}

SDL_AppResult Core::draw() {
    SDL_RenderClear(m_sdl.renderer);

    m_game->draw();
   
    SDL_RenderTexture(m_sdl.renderer, m_res_mgr->get_texture(ResourceManager::TextureId::VIGNETTE), nullptr, nullptr);
    SDL_RenderPresent(m_sdl.renderer);

    return SDL_APP_CONTINUE;
}
