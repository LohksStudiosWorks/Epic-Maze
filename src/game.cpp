#include "game.hpp"

#include <cmath>
#include <algorithm>
#include <format>

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
        {
            const char* title;
            float off_x = 0.0f;
            constexpr float off_y = 370.0f;

            switch(m_sel_item) {
            case 0:
                title = "SCORES - EASY";
                off_x = 690.0f;
                break;
            case 1:
                title = "SCORES - NORMAL";
                off_x = 620.0f;
                break;
            case 2:
                title = "SCORES - UNBELIEVABLE";
                off_x = 530.0f;
                break;
            }

            m_res_mng.draw_text(title, off_x, off_y, {255,255,255,255});

            auto scores = m_score.get_scores(m_sel_item);
            for(size_t p = 0; p < 7; ++p) {
                const uint64_t val = p < scores.size() ? scores[p] : 0;
                m_res_mng.draw_text(std::format("{}⁰: {}", p + 1, val), 430.0f, 27.0f + off_y + 64.0f * (p + 1), {255,255,255,255});
            }
        }

        break;
    case Scene::PAUSE:
        [[ fallthrough ]];
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
        
        m_cam.redo_zoom();

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

        // --- From here no fancy zoom:
        m_cam.undo_zoom();

        {
            uint64_t cpy = m_elapsed_time / 100ull;
            char time_str[16];
            unsigned c = static_cast<unsigned>(cpy % 10);
            cpy /= 10;
            unsigned sec = static_cast<unsigned>(cpy % 60);
            cpy /= 60;
            unsigned min = static_cast<unsigned>(cpy % 60);
            cpy /= 60;

            snprintf(time_str, 16, "%02u:%02u:%02u.%u", static_cast<unsigned>(cpy), min, sec, c);

            m_res_mng.draw_text(time_str, c_target_app_region.x - 422.0f, 22.0f, {  0,  0,  0,255});
            m_res_mng.draw_text(time_str, c_target_app_region.x - 420.0f, 20.0f, {255,255,255,255});
        }

        if (m_flashlight) {
            if (m_refresh_flashlight) {
                m_flashlight->refresh();
                m_refresh_flashlight = false;
            }   
            m_flashlight->draw();
        }
        
        // Add PAUSE on top
        if (m_scene == Scene::PAUSE) {
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
        }
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
                m_sel_item = 0;
                m_scene = Scene::INSTR;
                break;
            case 2:
                m_sel_item = 0;
                m_scene = Scene::RECORDS;
                break;
            default:
                m_sel_item = 0;
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
                m_cam.set_zoom(c_levels_zoom[0]);
                break;
            case 1:
                m_map = std::make_shared<Map>(100, 35);
                m_cam.set_zoom(c_levels_zoom[1]);
                break;
            case 2:
                m_map = std::make_shared<Map>(150, 80);
                m_cam.set_zoom(c_levels_zoom[2]);
                m_flashlight = std::make_unique<Flashlight>(m_player, m_map, m_cam, m_renderer);
                m_flashlight->refresh();
                break;
            }

            m_scene = Scene::GAMING;
            break;
        case Scene::PAUSE:
            switch(m_sel_item) {
            case 1:
                m_scene = Scene::MENU;
                m_flashlight.reset();
                m_sel_item = 0;
                break;
            default: // case 0
                m_sel_item = 0;
                m_scene = Scene::GAMING;
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
            } else if (event.key.scancode == SDL_SCANCODE_ESCAPE || event.key.scancode == SDL_SCANCODE_AC_BACK) {
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
            } else if (event.key.scancode == SDL_SCANCODE_ESCAPE || event.key.scancode == SDL_SCANCODE_AC_BACK) {
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
            if (event.key.scancode == SDL_SCANCODE_W || event.key.scancode == SDL_SCANCODE_A || event.key.scancode == SDL_SCANCODE_UP) {
                m_sel_item = (m_sel_item + 2) % 3;
            } else if (event.key.scancode == SDL_SCANCODE_S || event.key.scancode == SDL_SCANCODE_D || event.key.scancode == SDL_SCANCODE_DOWN) {
                m_sel_item = (m_sel_item + 1) % 3;
            } else  {
                advance_menu();
            }
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            if (event.wheel.integer_y < 0) {
                m_sel_item = (m_sel_item + 1) % 3;
            } else {
                m_sel_item = (m_sel_item + 2) % 3;
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:            
            m_sel_item = (m_sel_item + 1) % 3;
            break;
        default:
            break;
        }
        break;
    case Scene::GAMING:
        switch(event.type) {
        case SDL_EVENT_KEY_DOWN:
            if (event.key.scancode == SDL_SCANCODE_ESCAPE || event.key.scancode == SDL_SCANCODE_AC_BACK) {
                m_scene = Scene::PAUSE;
                m_sel_item = 0;
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            [[ fallthrough ]];
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == 1) {
                m_touch.set(event.motion.x, event.motion.y);
            }
            break;
        case SDL_EVENT_MOUSE_MOTION:
            if (event.button.button == 1) {
                m_touch.move(event.motion.x, event.motion.y);
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
            } else if (event.key.scancode == SDL_SCANCODE_ESCAPE || event.key.scancode == SDL_SCANCODE_AC_BACK) {
                m_scene = Scene::GAMING;
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

    if (m_touch.moved()) {
        const float dx = std::clamp(m_touch.get_dx(), -256.0f, 256.0f);
        const float dy = std::clamp(m_touch.get_dy(), -256.0f, 256.0f);

        m_player->move(dx / 256.0f, dy / 256.0f);

    } else { // non "touch"

        int len;
        const bool* state = SDL_GetKeyboardState(&len);

        if (len < SDL_SCANCODE_W || len < SDL_SCANCODE_S || len < SDL_SCANCODE_A || len < SDL_SCANCODE_D) {
            SDL_Log("Weird: Keyboard state is too small.\n");
            return;
        }

        const float dir_y = static_cast<float>(state[SDL_SCANCODE_S] - state[SDL_SCANCODE_W]);
        const float dir_x = static_cast<float>(state[SDL_SCANCODE_D] - state[SDL_SCANCODE_A]);

        m_player->move(dir_x, dir_y);
    }

    m_player->tick();

    const int px = static_cast<int>(std::floor((m_player->get_x() + player_size_fixed.x * 0.5f) / block_size_fixed.x));
    const int py = static_cast<int>(std::floor((m_player->get_y() + player_size_fixed.y * 0.5f) / block_size_fixed.y));
    const SDL_FRect p_rect = m_player->get_rect();

    if (m_map->read(px, py) == Map::BlockId::END) {
        m_score.add_score(m_sel_difficulty, 1'000'000'000'000'000 / (m_elapsed_time + 1));
        m_score.save();
        m_scene = Scene::RECORDS;
    }

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

void Game::lazy_draw() {

    switch(m_scene) {
    case Scene::GAMING:
        m_refresh_flashlight = true;
        break;
    default:
        break;
    }
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