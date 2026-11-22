#include "resource_manager.hpp"

#include <battery/embed.hpp>
#include <cmath>

//static const_mem_resource<SDL_Surface> load_surface_from_mem(const b::EmbedInternal::EmbeddedFile& embed);
static const_mem_resource<TTF_Font> load_font_from_mem(const b::EmbedInternal::EmbeddedFile& embed, float ptsize);
static const_mem_resource<SDL_Texture> load_texture_from_mem(SDL_Renderer* renderer, const b::EmbedInternal::EmbeddedFile& embed);

ResourceManager::ResourceManager(SDL_Renderer* renderer) :
    m_font(load_font_from_mem(b::embed<"resources/DejaVuSans.ttf">(), 24.0f)),
    m_text_eng(TTF_CreateRendererTextEngine(renderer)),
    m_text(TTF_CreateText(m_text_eng, m_font.res, "Test", 0)),
    m_renderer_ref(renderer)
{
    m_vignette = load_texture_from_mem(m_renderer_ref, b::embed<"resources/vignette.png">());
}


void ResourceManager::set_scene(Scene new_scene) {
    m_scene = new_scene;
}

const ResourceManager::Scene& ResourceManager::get_scene() const {
    return m_scene;
}

bool ResourceManager::load_scene_assets() {
    for(auto& i : m_loaded_textures)
        SDL_DestroyTexture(i.second.res);

    m_loaded_textures.clear();

    switch(m_scene) {
    case Scene::MENU:
        m_loaded_textures[TextureId::INTRO] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/intro.png">());
        m_loaded_textures[TextureId::MENU_BLOCK] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_block.png">());
        m_loaded_textures[TextureId::MENU_OPT_01] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_opt_01.png">());
        m_loaded_textures[TextureId::MENU_OPT_02] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_opt_02.png">());
        m_loaded_textures[TextureId::MENU_OPT_03] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_opt_03.png">());
        m_loaded_textures[TextureId::MENU_OPT_04] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_opt_04.png">());
        break;
    case Scene::PLAY:

        break;
    case Scene::INSTR:

        break;
    case Scene::RECORDS:

        break;
    case Scene::GAMING:

        break;
    case Scene::PAUSE:

        break;
    }

    return true;
}

SDL_Texture* ResourceManager::get_texture(TextureId id) const {
    const auto tex = m_loaded_textures.find(id);
    if (tex == m_loaded_textures.end()) return nullptr;
    return tex->second.res;
}

bool ResourceManager::draw_menu_block_background() {
    const auto tex = m_loaded_textures.find(TextureId::MENU_BLOCK);
    if (tex == m_loaded_textures.end()) return false;

    float w, h;
    SDL_Rect rect;
    SDL_FRect frect;

    SDL_GetRenderViewport(m_renderer_ref, &rect);
    SDL_GetTextureSize(tex->second.res, &w, &h);

    frect.x = fmodf(SDL_GetTicksNS() * 110e-9f, w) - w;
    frect.y = fmodf(SDL_GetTicksNS() * 110e-9f, h) - h;
    frect.w = rect.w - frect.x;
    frect.h = rect.h - frect.y;

    SDL_RenderTextureTiled(m_renderer_ref, tex->second.res, NULL, 1.0f, &frect);

    return true;
}

bool draw_menu_option(size_t opt) {

}

bool draw_start_option(size_t opt) {

}

void ResourceManager::draw_vignette() {
    SDL_RenderTexture(m_renderer_ref, m_vignette.res, nullptr, nullptr);
}



/*const_mem_resource<SDL_Surface> load_surface_from_mem(const b::EmbedInternal::EmbeddedFile& embed) {
    const_mem_resource<SDL_Surface> mem;
    mem.mem = std::unique_ptr<char[]>(new char[embed.size()]);
    memcpy(mem.mem.get(), embed.data(), embed.size());

    SDL_IOStream* io = SDL_IOFromConstMem(mem.mem.get(), embed.size());
    mem.res = SDL_LoadSurface_IO(io, true); // true = auto-close IO stream
    return mem;
}*/

const_mem_resource<TTF_Font> load_font_from_mem(const b::EmbedInternal::EmbeddedFile& embed, float ptsize) {
    const_mem_resource<TTF_Font> mem;
    mem.mem = std::unique_ptr<char[]>(new char[embed.size()]);
    memcpy(mem.mem.get(), embed.data(), embed.size());

    SDL_IOStream* io = SDL_IOFromConstMem(mem.mem.get(), embed.size());
    mem.res = TTF_OpenFontIO(io, true, ptsize); // true = auto-close IO stream
    return mem;
}

const_mem_resource<SDL_Texture> load_texture_from_mem(SDL_Renderer* renderer, const b::EmbedInternal::EmbeddedFile& embed) {
    const_mem_resource<SDL_Texture> mem;
    mem.mem = std::unique_ptr<char[]>(new char[embed.size()]);
    memcpy(mem.mem.get(), embed.data(), embed.size());

    SDL_IOStream* io = SDL_IOFromConstMem(mem.mem.get(), embed.size());
    SDL_Surface* surface = IMG_Load_IO(io, true);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
    SDL_DestroySurface(surface);
    mem.res = texture;
    return mem;
}


// m_loaded_textures[TextureId::BLOCK_END] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/block_end.png">())
// m_loaded_textures[TextureId::BLOCK] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/block.png">())
// m_loaded_textures[TextureId::ICON] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/icon.png">())
// m_loaded_textures[TextureId::INSTR] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/instr.png">())
// m_loaded_textures[TextureId::INTRO] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/intro.png">())
// m_loaded_textures[TextureId::MENU_BLOCK] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_block.png">())
// m_loaded_textures[TextureId::MENU_OPT_01] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_opt_01.png">())
// m_loaded_textures[TextureId::MENU_OPT_01_S] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_opt_01_s.png">())
// m_loaded_textures[TextureId::MENU_OPT_02] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_opt_02.png">())
// m_loaded_textures[TextureId::MENU_OPT_02_S] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_opt_02_s.png">())
// m_loaded_textures[TextureId::MENU_OPT_03] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_opt_03.png">())
// m_loaded_textures[TextureId::MENU_OPT_03_S] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_opt_03_s.png">())
// m_loaded_textures[TextureId::MENU_OPT_04] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/menu_opt_04.png">())
// m_loaded_textures[TextureId::PAUSE_00] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_00.png">())
// m_loaded_textures[TextureId::PAUSE_01] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_01.png">())
// m_loaded_textures[TextureId::PAUSE_02] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_02.png">())
// m_loaded_textures[TextureId::PAUSE_03] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_03.png">())
// m_loaded_textures[TextureId::PAUSE_04] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_04.png">())
// m_loaded_textures[TextureId::PAUSE_05] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_05.png">())
// m_loaded_textures[TextureId::PAUSE_06] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_06.png">())
// m_loaded_textures[TextureId::PAUSE_07] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_07.png">())
// m_loaded_textures[TextureId::PAUSE_08] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_08.png">())
// m_loaded_textures[TextureId::PAUSE_09] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_09.png">())
// m_loaded_textures[TextureId::PAUSE_10] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_10.png">())
// m_loaded_textures[TextureId::PAUSE_11] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_11.png">())
// m_loaded_textures[TextureId::PAUSE_12] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_12.png">())
// m_loaded_textures[TextureId::PAUSE_13] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_13.png">())
// m_loaded_textures[TextureId::PAUSE_14] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_14.png">())
// m_loaded_textures[TextureId::PAUSE_15] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_15.png">())
// m_loaded_textures[TextureId::PAUSE_16] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_16.png">())
// m_loaded_textures[TextureId::PAUSE_17] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_17.png">())
// m_loaded_textures[TextureId::PAUSE_18] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_18.png">())
// m_loaded_textures[TextureId::PAUSE_19] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_19.png">())
// m_loaded_textures[TextureId::PAUSE_20] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_20.png">())
// m_loaded_textures[TextureId::PAUSE_21] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_21.png">())
// m_loaded_textures[TextureId::PAUSE_22] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_22.png">())
// m_loaded_textures[TextureId::PAUSE_23] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_23.png">())
// m_loaded_textures[TextureId::PAUSE_24] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_24.png">())
// m_loaded_textures[TextureId::PAUSE_25] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_25.png">())
// m_loaded_textures[TextureId::PAUSE_26] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_26.png">())
// m_loaded_textures[TextureId::PAUSE_27] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_27.png">())
// m_loaded_textures[TextureId::PAUSE_28] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_28.png">())
// m_loaded_textures[TextureId::PAUSE_29] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_29.png">())
// m_loaded_textures[TextureId::PAUSE_CONTINUE] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_continue.png">())
// m_loaded_textures[TextureId::PAUSE_EXIT] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/pause_exit.png">())
// m_loaded_textures[TextureId::PERSON_F01] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/person_f01.png">())
// m_loaded_textures[TextureId::PERSON_F02] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/person_f02.png">())
// m_loaded_textures[TextureId::PERSON] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/person.png">())
// m_loaded_textures[TextureId::REC_BACK] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/rec_back.png">())
// m_loaded_textures[TextureId::SAVING_SCORE] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/saving_score.png">())
// m_loaded_textures[TextureId::VIGNETTE] = load_texture_from_mem(m_renderer_ref, b::embed<"resources/vignette.png">())