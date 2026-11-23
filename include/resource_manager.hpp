#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <unordered_map>
#include <memory>

template<typename T>
struct const_mem_resource {
    T* res;
    std::unique_ptr<char[]> mem;
};

class ResourceManager {
public:
    enum class TextureId {
        BLOCK_END,
        BLOCK,
        DIRT,
        ICON,
        INSTR,
        INTRO,
        MENU_BLOCK,
        MENU_OPT_01,
        MENU_OPT_01_S,
        MENU_OPT_02,
        MENU_OPT_02_S,
        MENU_OPT_03,
        MENU_OPT_03_S,
        MENU_OPT_04,
        PAUSE_00,
        PAUSE_01,
        PAUSE_02,
        PAUSE_03,
        PAUSE_04,
        PAUSE_05,
        PAUSE_06,
        PAUSE_07,
        PAUSE_08,
        PAUSE_09,
        PAUSE_10,
        PAUSE_11,
        PAUSE_12,
        PAUSE_13,
        PAUSE_14,
        PAUSE_15,
        PAUSE_16,
        PAUSE_17,
        PAUSE_18,
        PAUSE_19,
        PAUSE_20,
        PAUSE_21,
        PAUSE_22,
        PAUSE_23,
        PAUSE_24,
        PAUSE_25,
        PAUSE_26,
        PAUSE_27,
        PAUSE_28,
        PAUSE_29,
        PAUSE_CONTINUE,
        PAUSE_EXIT,
        PERSON_F01,
        PERSON_F02,
        PERSON,
        REC_BACK,
        SAVING_SCORE,
        VIGNETTE
    };

    ResourceManager(SDL_Renderer* renderer);

    SDL_Texture* get_texture(TextureId id) const;
    void draw_text(const std::string_view& text, const float x, const float y, const SDL_Color color);
private:
    const_mem_resource<TTF_Font> m_font;
    TTF_TextEngine* m_text_eng;
    TTF_Text* m_text;
    SDL_Renderer* m_renderer_ref;

    const std::unordered_map<TextureId, const_mem_resource<SDL_Texture>> m_loaded_textures;
};