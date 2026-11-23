#include "game.hpp"

#include <cmath>

#include "defaults.hpp"


Game::Game(ResourceManager& resources, SDL_Renderer* renderer) :
    m_renderer(renderer),
    m_res_mng(resources),
    m_cam(renderer)
{}


void Game::draw() {
    switch(m_scene) {
    case Scene::MENU:
        tile_tex_bg(m_res_mng.get_texture(ResourceManager::TextureId::MENU_BLOCK), SDL_GetTicksNS() * 110e-9f);
        draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::INTRO), {417.0f, 162.0f});
        switch(m_sel_item) {
        case 0:
            draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::MENU_OPT_01), {0.0f, 0.0f});
            break;
        case 1:
            draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::MENU_OPT_02), {0.0f, 0.0f});
            break;
        case 2:
            draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::MENU_OPT_03), {0.0f, 0.0f});
            break;
        case 3:
            draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::MENU_OPT_04), {0.0f, 0.0f});
            break;
        default:
            m_sel_item = 0;
            break;
        }
        break;
    case Scene::PLAY:
        tile_tex_bg(m_res_mng.get_texture(ResourceManager::TextureId::MENU_BLOCK), SDL_GetTicksNS() * 110e-9f);
        draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::INTRO), {417.0f, 162.0f});
        switch(m_sel_item) {
        case 0:
            draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::MENU_OPT_01_S), {0.0f, 0.0f});
            break;
        case 1:
            draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::MENU_OPT_02_S), {0.0f, 0.0f});
            break;
        case 2:
            draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::MENU_OPT_03_S), {0.0f, 0.0f});
            break;
        default:
            m_sel_item = 0;
            break;
        }
        break;
    case Scene::INSTR:
        tile_tex_bg(m_res_mng.get_texture(ResourceManager::TextureId::MENU_BLOCK), SDL_GetTicksNS() * 110e-9f);
        draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::INTRO), {417.0f, 162.0f});
        draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::INSTR), {0.0f, 0.0f});
        break;
    case Scene::RECORDS:
        tile_tex_bg(m_res_mng.get_texture(ResourceManager::TextureId::MENU_BLOCK), SDL_GetTicksNS() * 110e-9f);
        draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::INTRO), {417.0f, 162.0f});
        draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::REC_BACK), {0.0f, 0.0f});
        break;
    case Scene::GAMING:
    {
        if (!m_player) {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                "Player not initialized",
                "Something went wrong! Player was not correctly initialized.\nPlease report this error!",
                nullptr);
            m_scene = Scene::MENU;
            break;
        }
        
        auto* block_tex = m_res_mng.get_texture(ResourceManager::TextureId::BLOCK);
        auto* block_end_tex = m_res_mng.get_texture(ResourceManager::TextureId::BLOCK_END);
        auto* block_dirt_tex = m_res_mng.get_texture(ResourceManager::TextureId::DIRT);

        const auto area = m_cam.calculated_limits();

        for(int x = area.x; x < area.x + area.w; ++x) {
            for(int y = area.y; y < area.y + area.h; ++y) {
                switch(m_map->read(x, y)) {
                case Map::BlockId::SOLID:
                    draw_tex_at(block_tex, m_cam.transl({x * block_size_fixed.x, y * block_size_fixed.y}), block_size_fixed);
                    break;
                case Map::BlockId::END:
                    draw_tex_at(block_end_tex, m_cam.transl({x * block_size_fixed.x, y * block_size_fixed.y}), block_size_fixed);
                    break;
                default:
                    draw_tex_at(block_dirt_tex, m_cam.transl({x * block_size_fixed.x, y * block_size_fixed.y}), block_size_fixed);
                    break;
                }
            }
        }

        if (m_player->has_moved()) {
            const size_t idx = static_cast<size_t>(SDL_GetTicksNS() * 2e-9f) % 2;
            const ResourceManager::TextureId id = static_cast<ResourceManager::TextureId>(static_cast<size_t>(ResourceManager::TextureId::PERSON_F01) + idx);

            draw_tex_at(m_res_mng.get_texture(id), m_cam.transl({m_player->draw_get_x(), m_player->draw_get_y()}), m_player->draw_get_rot(), player_size_fixed);
        } else {
            draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::PERSON), m_cam.transl({m_player->draw_get_x(), m_player->draw_get_y()}), m_player->draw_get_rot(), player_size_fixed);            
        }

        if (m_flashlight) {
            m_cam.undo_zoom();

            m_flashlight->refresh();
            m_flashlight->draw();

            m_cam.redo_zoom();
        }
    }
        break;
    case Scene::PAUSE:
        switch(m_sel_item) {
        case 0:
            draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::PAUSE_CONTINUE), {0.0f, 0.0f});
            break;
        case 1:
            draw_tex_at(m_res_mng.get_texture(ResourceManager::TextureId::PAUSE_EXIT), {0.0f, 0.0f});
            break;
        default:
            m_sel_item = 0;
            break;
        }
        {
            const size_t idx = static_cast<size_t>(SDL_GetTicksNS() * 24e-9f) % 30;
            const ResourceManager::TextureId id = static_cast<ResourceManager::TextureId>(static_cast<size_t>(ResourceManager::TextureId::PAUSE_00) + idx);
            draw_tex_at(m_res_mng.get_texture(id), {531.0f, 147.0f});
        }
        break;
    default:
        break;
    }
}

SDL_AppResult Game::interact(SDL_Event& event) {
    if(event.type == SDL_EVENT_MOUSE_MOTION || event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_WHEEL)
        SDL_ConvertEventToRenderCoordinates(m_renderer, &event);

    const auto advance_menu = [&]{
        switch(m_scene) {
        case Scene::MENU:
            switch(m_sel_item) {
            case 0:
                m_sel_item = 0;
                m_scene = Scene::PLAY;
                break;
            case 1:                    
                m_scene = Scene::INSTR;
                break;
            case 2:
                m_scene = Scene::RECORDS;
                break;
            default:
                m_scene = Scene::EXIT;
                break;
            }
        break;
        case Scene::PLAY:
            m_sel_difficulty = m_sel_item;
            m_sel_item = 0;
            m_elapsed_time = 0;
            m_cam.reset();
            m_player = std::make_shared<Player>(50.0f, 50.0f);

            switch(m_sel_difficulty) {
            case 0:
                m_map = std::make_shared<Map>(35, 22);
                m_cam.set_zoom(0.5f);
                break;
            case 1:
                m_map = std::make_shared<Map>(100, 35);
                m_cam.set_zoom(1.0f);
                break;
            case 2:
                m_map = std::make_shared<Map>(150, 80);
                m_cam.set_zoom(1.4f);
                m_flashlight = std::make_unique<Flashlight>(m_player, m_map, m_cam, m_renderer);
                break;
            }

            m_scene = Scene::GAMING;
            break;
        case Scene::PAUSE:
            switch(m_sel_item) {
            case 1:
                m_scene = Scene::MENU;
                m_cam.undo_zoom();
                m_flashlight.reset();
                m_sel_item = 0;
                break;
            default: // case 0
                m_sel_item = 0;
                m_scene = Scene::GAMING;
                m_cam.redo_zoom();
                break;
            }
        break;
        case Scene::INSTR:
            m_scene = Scene::MENU;
            break;
        case Scene::RECORDS:
            m_scene = Scene::MENU;
            break;
        default:
            m_scene = Scene::MENU;
            break;
        }
    };

    switch(m_scene) {
    case Scene::MENU:
        switch(event.type) {
        case SDL_EVENT_MOUSE_MOTION:
            if (event.motion.y <= 544) m_sel_item = 0;
            else if (event.motion.y <= 674) m_sel_item = 1;
            else if (event.motion.y <= 804) m_sel_item = 2;
            else m_sel_item = 3;
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            m_sel_item = (4 + m_sel_item - (event.wheel.y > 0.0f ? 1 : -1)) % 4;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event.key.scancode == SDL_SCANCODE_W || event.key.scancode == SDL_SCANCODE_UP) {
                m_sel_item = (m_sel_item + 3) % 4;
            } else if (event.key.scancode == SDL_SCANCODE_S || event.key.scancode == SDL_SCANCODE_DOWN) {
                m_sel_item = (m_sel_item + 1) % 4;
            } else if (event.key.scancode == SDL_SCANCODE_KP_ENTER || event.key.scancode == SDL_SCANCODE_RETURN) {
                advance_menu();
            } else if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                m_scene = Scene::EXIT;
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            advance_menu();
            break;
        default:
            break;
        }
        break;
    case Scene::PLAY:
        switch(event.type) {
        case SDL_EVENT_MOUSE_MOTION:
            if (event.motion.y <= 544) m_sel_item = 0;
            else if (event.motion.y <= 674) m_sel_item = 1;
            else m_sel_item = 2;
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            m_sel_item = (3 + m_sel_item - (event.wheel.y > 0.0f ? 1 : -1)) % 3;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event.key.scancode == SDL_SCANCODE_W || event.key.scancode == SDL_SCANCODE_UP) {
                m_sel_item = (m_sel_item + 2) % 3;
            } else if (event.key.scancode == SDL_SCANCODE_S || event.key.scancode == SDL_SCANCODE_DOWN) {
                m_sel_item = (m_sel_item + 1) % 3;
            } else if (event.key.scancode == SDL_SCANCODE_KP_ENTER || event.key.scancode == SDL_SCANCODE_RETURN) {
                advance_menu();
            } else if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                m_scene = Scene::MENU;
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            advance_menu();
            break;
        default:
            break;
        }
        break;
    case Scene::INSTR:
        switch(event.type) {
        case SDL_EVENT_KEY_DOWN:
            advance_menu();
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            advance_menu();
            break;
        default:
            break;
        }
        break;
    case Scene::RECORDS:
        switch(event.type) {
        case SDL_EVENT_KEY_DOWN:
            advance_menu();
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            advance_menu();
            break;
        default:
            break;
        }
        break;
    case Scene::GAMING:
        switch(event.type) {
        case SDL_EVENT_KEY_DOWN:
            if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                m_scene = Scene::PAUSE;
                m_cam.undo_zoom();
                m_flashlight.reset();
                m_sel_item = 0;
            }
            break;
        default:
            break;
        }
        break;
    case Scene::PAUSE:
        switch(event.type) {
        case SDL_EVENT_MOUSE_MOTION:
            if (event.motion.y <= 705) m_sel_item = 0;
            else m_sel_item = 1;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event.key.scancode == SDL_SCANCODE_W || event.key.scancode == SDL_SCANCODE_UP) {
                m_sel_item = !m_sel_item;
            } else if (event.key.scancode == SDL_SCANCODE_S || event.key.scancode == SDL_SCANCODE_DOWN) {
                m_sel_item = !m_sel_item;
            } else if (event.key.scancode == SDL_SCANCODE_KP_ENTER || event.key.scancode == SDL_SCANCODE_RETURN) {
                advance_menu();
            } else if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                m_scene = Scene::GAMING;
                m_cam.redo_zoom();
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            advance_menu();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return m_scene == Scene::EXIT ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}


void Game::tick(Uint32 elapsed) {
    if (m_scene != Scene::GAMING) return;

    //SDL_Log("Tick! %lu\n", m_elapsed_time);
    m_elapsed_time += static_cast<uint64_t>(elapsed);

    if (!m_player) return;

    int len;
    const bool* state = SDL_GetKeyboardState(&len);

    if (len < SDL_SCANCODE_W || len < SDL_SCANCODE_S || len < SDL_SCANCODE_A || len < SDL_SCANCODE_D) {
        SDL_Log("Weird: Keyboard state is too small.\n");
        return;
    }

    const int dir_y = state[SDL_SCANCODE_S] - state[SDL_SCANCODE_W];
    const int dir_x = state[SDL_SCANCODE_D] - state[SDL_SCANCODE_A];

    m_player->move(dir_x, dir_y);
    m_player->tick();

    const int px = static_cast<int>(std::floor((m_player->get_x() + player_size_fixed.x * 0.5f) / block_size_fixed.x));
    const int py = static_cast<int>(std::floor((m_player->get_y() + player_size_fixed.y * 0.5f) / block_size_fixed.y));
    const SDL_FRect p_rect = m_player->get_rect();

    for(int x = px - 1; x <= px + 1; ++x) {
        for(int y = py - 1; y <= py + 1; ++y) {
            if (m_map->read(x, y) != Map::BlockId::SOLID)
                continue;

            const SDL_FRect transl{
                .x = x * block_size_fixed.x,
                .y = y * block_size_fixed.y,
                .w = block_size_fixed.x,
                .h = block_size_fixed.y
            };

            if (SDL_HasRectIntersectionFloat(&p_rect, &transl)) {
                m_player->cancel();
                break;
            }
        }
    }

    m_cam.follow(m_player->get_x(), m_player->get_y());
    m_cam.tick();
}


void Game::draw_tex_at(SDL_Texture* tex, const SDL_FPoint pos) {
    SDL_FRect dest;
    SDL_GetTextureSize(tex, &dest.w, &dest.h);
    dest.x = pos.x;
    dest.y = pos.y;
    SDL_RenderTexture(m_renderer, tex, nullptr, &dest);
}
void Game::draw_tex_at(SDL_Texture* tex, const SDL_FPoint pos, const SDL_FPoint force_size) {
    SDL_FRect dest{
        .x = pos.x,
        .y = pos.y,
        .w = force_size.x,
        .h = force_size.y
    };
    SDL_RenderTexture(m_renderer, tex, nullptr, &dest);
}

void Game::draw_tex_at(SDL_Texture* tex, const SDL_FPoint pos, const float rot, const SDL_FPoint force_size) {
    SDL_FRect dest{
        .x = pos.x,
        .y = pos.y,
        .w = force_size.x,
        .h = force_size.y
    };
    SDL_RenderTextureRotated(m_renderer, tex, nullptr, &dest, rot, nullptr, SDL_FLIP_NONE);
}

void Game::tile_tex_bg(SDL_Texture* tex, const float off) {
    float w, h;

    SDL_Rect rect;
    SDL_FRect frect;

    SDL_GetRenderViewport(m_renderer, &rect);
    SDL_GetTextureSize(tex, &w, &h);

    frect.x = fmodf(off, w) - w;
    frect.y = fmodf(off, h) - h;
    frect.w = rect.w - frect.x;
    frect.h = rect.h - frect.y;

    SDL_RenderTextureTiled(m_renderer, tex, nullptr, 1.0f, &frect);
}