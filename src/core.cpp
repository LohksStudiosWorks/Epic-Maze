#include "core.hpp"

constexpr SDL_Point c_default_app_size = {1280, 720};
constexpr SDL_Point c_target_app_region = {1920, 1080};
constexpr SDL_WindowFlags c_default_app_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;// | SDL_WINDOW_FULLSCREEN;

Core::Core(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    SDL_Log("Initializing video, audio, and events...");
    SDL_Init(SDL_INIT_VIDEO); //  | SDL_INIT_AUDIO | SDL_INIT_EVENTS

    SDL_Log("Initializing ttf...");
    TTF_Init();

    SDL_Log("Setup of screen and peripherals...");

    SDL_SetAppMetadata("Epic Maze v" GAME_VERSION, GAME_VERSION, "com.lsw.epicmaze");
    SDL_CreateWindowAndRenderer("Epic Maze v" GAME_VERSION, c_default_app_size.x, c_default_app_size.y, c_default_app_flags, &m_sdl.window, &m_sdl.renderer);
    SDL_SetRenderLogicalPresentation(m_sdl.renderer, c_target_app_region.x, c_target_app_region.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    m_res_mgr = std::make_shared<ResourceManager>(m_sdl.renderer);
    m_res_mgr->load_scene_assets();
}

Core::~Core() {

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

    return SDL_APP_CONTINUE;
}

SDL_AppResult Core::draw() {
    SDL_RenderClear(m_sdl.renderer);

    m_res_mgr->draw_menu_block_background();

    const auto draw_tex_at = [this](const ResourceManager::TextureId tid, const float x, const float y) {
        auto* tex = m_res_mgr->get_texture(tid);
        SDL_FRect dest;
        SDL_GetTextureSize(tex, &dest.w, &dest.h);
        dest.x = x;
        dest.y = y;
        SDL_RenderTexture(m_sdl.renderer, tex, nullptr, &dest);
    };

    switch(m_res_mgr->get_scene()) {
    case ResourceManager::Scene::MENU:
        draw_tex_at(ResourceManager::TextureId::INTRO, 417.0f, 162.0f);
        break;
    case ResourceManager::Scene::PLAY:

        break;
    case ResourceManager::Scene::INSTR:

        break;
    case ResourceManager::Scene::RECORDS:

        break;
    case ResourceManager::Scene::GAMING:

        break;
    case ResourceManager::Scene::PAUSE:

        break;
    }


    m_res_mgr->draw_vignette();
    SDL_RenderPresent(m_sdl.renderer);

    return SDL_APP_CONTINUE;
}
