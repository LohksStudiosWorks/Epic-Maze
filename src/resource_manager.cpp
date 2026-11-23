#include "resource_manager.hpp"

#include <battery/embed.hpp>
#include <cmath>

//static const_mem_resource<SDL_Surface> load_surface_from_mem(const b::EmbedInternal::EmbeddedFile& embed);
static std::unordered_map<ResourceManager::TextureId, const_mem_resource<SDL_Texture>> build_const_mem(SDL_Renderer* renderer);
static const_mem_resource<TTF_Font> load_font_from_mem(const b::EmbedInternal::EmbeddedFile& embed, float ptsize);
static const_mem_resource<SDL_Texture> load_texture_from_mem(SDL_Renderer* renderer, const b::EmbedInternal::EmbeddedFile& embed);

ResourceManager::ResourceManager(SDL_Renderer* renderer) :
    m_font(load_font_from_mem(b::embed<"resources/DejaVuSans.ttf">(), 24.0f)),
    m_text_eng(TTF_CreateRendererTextEngine(renderer)),
    m_text(TTF_CreateText(m_text_eng, m_font.res, "Test", 0)),
    m_renderer_ref(renderer),
    m_loaded_textures(build_const_mem(renderer))
{}


SDL_Texture* ResourceManager::get_texture(TextureId id) const {
    const auto tex = m_loaded_textures.find(id);
    if (tex == m_loaded_textures.end()) return nullptr;
    return tex->second.res;
}



std::unordered_map<ResourceManager::TextureId, const_mem_resource<SDL_Texture>> build_const_mem(SDL_Renderer* renderer) {
    std::unordered_map<ResourceManager::TextureId, const_mem_resource<SDL_Texture>> texmap;

    texmap[ResourceManager::TextureId::BLOCK_END] = load_texture_from_mem(renderer, b::embed<"resources/block_end.png">());
    texmap[ResourceManager::TextureId::BLOCK] = load_texture_from_mem(renderer, b::embed<"resources/block.png">());
    texmap[ResourceManager::TextureId::DIRT] = load_texture_from_mem(renderer, b::embed<"resources/dirt.png">());
    texmap[ResourceManager::TextureId::ICON] = load_texture_from_mem(renderer, b::embed<"resources/icon.png">());
    texmap[ResourceManager::TextureId::INSTR] = load_texture_from_mem(renderer, b::embed<"resources/instr.png">());
    texmap[ResourceManager::TextureId::INTRO] = load_texture_from_mem(renderer, b::embed<"resources/intro.png">());
    texmap[ResourceManager::TextureId::MENU_BLOCK] = load_texture_from_mem(renderer, b::embed<"resources/menu_block.png">());
    texmap[ResourceManager::TextureId::MENU_OPT_01] = load_texture_from_mem(renderer, b::embed<"resources/menu_opt_01.png">());
    texmap[ResourceManager::TextureId::MENU_OPT_01_S] = load_texture_from_mem(renderer, b::embed<"resources/menu_opt_01_s.png">());
    texmap[ResourceManager::TextureId::MENU_OPT_02] = load_texture_from_mem(renderer, b::embed<"resources/menu_opt_02.png">());
    texmap[ResourceManager::TextureId::MENU_OPT_02_S] = load_texture_from_mem(renderer, b::embed<"resources/menu_opt_02_s.png">());
    texmap[ResourceManager::TextureId::MENU_OPT_03] = load_texture_from_mem(renderer, b::embed<"resources/menu_opt_03.png">());
    texmap[ResourceManager::TextureId::MENU_OPT_03_S] = load_texture_from_mem(renderer, b::embed<"resources/menu_opt_03_s.png">());
    texmap[ResourceManager::TextureId::MENU_OPT_04] = load_texture_from_mem(renderer, b::embed<"resources/menu_opt_04.png">());
    texmap[ResourceManager::TextureId::PAUSE_00] = load_texture_from_mem(renderer, b::embed<"resources/pause_00.png">());
    texmap[ResourceManager::TextureId::PAUSE_01] = load_texture_from_mem(renderer, b::embed<"resources/pause_01.png">());
    texmap[ResourceManager::TextureId::PAUSE_02] = load_texture_from_mem(renderer, b::embed<"resources/pause_02.png">());
    texmap[ResourceManager::TextureId::PAUSE_03] = load_texture_from_mem(renderer, b::embed<"resources/pause_03.png">());
    texmap[ResourceManager::TextureId::PAUSE_04] = load_texture_from_mem(renderer, b::embed<"resources/pause_04.png">());
    texmap[ResourceManager::TextureId::PAUSE_05] = load_texture_from_mem(renderer, b::embed<"resources/pause_05.png">());
    texmap[ResourceManager::TextureId::PAUSE_06] = load_texture_from_mem(renderer, b::embed<"resources/pause_06.png">());
    texmap[ResourceManager::TextureId::PAUSE_07] = load_texture_from_mem(renderer, b::embed<"resources/pause_07.png">());
    texmap[ResourceManager::TextureId::PAUSE_08] = load_texture_from_mem(renderer, b::embed<"resources/pause_08.png">());
    texmap[ResourceManager::TextureId::PAUSE_09] = load_texture_from_mem(renderer, b::embed<"resources/pause_09.png">());
    texmap[ResourceManager::TextureId::PAUSE_10] = load_texture_from_mem(renderer, b::embed<"resources/pause_10.png">());
    texmap[ResourceManager::TextureId::PAUSE_11] = load_texture_from_mem(renderer, b::embed<"resources/pause_11.png">());
    texmap[ResourceManager::TextureId::PAUSE_12] = load_texture_from_mem(renderer, b::embed<"resources/pause_12.png">());
    texmap[ResourceManager::TextureId::PAUSE_13] = load_texture_from_mem(renderer, b::embed<"resources/pause_13.png">());
    texmap[ResourceManager::TextureId::PAUSE_14] = load_texture_from_mem(renderer, b::embed<"resources/pause_14.png">());
    texmap[ResourceManager::TextureId::PAUSE_15] = load_texture_from_mem(renderer, b::embed<"resources/pause_15.png">());
    texmap[ResourceManager::TextureId::PAUSE_16] = load_texture_from_mem(renderer, b::embed<"resources/pause_16.png">());
    texmap[ResourceManager::TextureId::PAUSE_17] = load_texture_from_mem(renderer, b::embed<"resources/pause_17.png">());
    texmap[ResourceManager::TextureId::PAUSE_18] = load_texture_from_mem(renderer, b::embed<"resources/pause_18.png">());
    texmap[ResourceManager::TextureId::PAUSE_19] = load_texture_from_mem(renderer, b::embed<"resources/pause_19.png">());
    texmap[ResourceManager::TextureId::PAUSE_20] = load_texture_from_mem(renderer, b::embed<"resources/pause_20.png">());
    texmap[ResourceManager::TextureId::PAUSE_21] = load_texture_from_mem(renderer, b::embed<"resources/pause_21.png">());
    texmap[ResourceManager::TextureId::PAUSE_22] = load_texture_from_mem(renderer, b::embed<"resources/pause_22.png">());
    texmap[ResourceManager::TextureId::PAUSE_23] = load_texture_from_mem(renderer, b::embed<"resources/pause_23.png">());
    texmap[ResourceManager::TextureId::PAUSE_24] = load_texture_from_mem(renderer, b::embed<"resources/pause_24.png">());
    texmap[ResourceManager::TextureId::PAUSE_25] = load_texture_from_mem(renderer, b::embed<"resources/pause_25.png">());
    texmap[ResourceManager::TextureId::PAUSE_26] = load_texture_from_mem(renderer, b::embed<"resources/pause_26.png">());
    texmap[ResourceManager::TextureId::PAUSE_27] = load_texture_from_mem(renderer, b::embed<"resources/pause_27.png">());
    texmap[ResourceManager::TextureId::PAUSE_28] = load_texture_from_mem(renderer, b::embed<"resources/pause_28.png">());
    texmap[ResourceManager::TextureId::PAUSE_29] = load_texture_from_mem(renderer, b::embed<"resources/pause_29.png">());
    texmap[ResourceManager::TextureId::PAUSE_CONTINUE] = load_texture_from_mem(renderer, b::embed<"resources/pause_continue.png">());
    texmap[ResourceManager::TextureId::PAUSE_EXIT] = load_texture_from_mem(renderer, b::embed<"resources/pause_exit.png">());
    texmap[ResourceManager::TextureId::PERSON_F01] = load_texture_from_mem(renderer, b::embed<"resources/person_f01.png">());
    texmap[ResourceManager::TextureId::PERSON_F02] = load_texture_from_mem(renderer, b::embed<"resources/person_f02.png">());
    texmap[ResourceManager::TextureId::PERSON] = load_texture_from_mem(renderer, b::embed<"resources/person.png">());
    texmap[ResourceManager::TextureId::REC_BACK] = load_texture_from_mem(renderer, b::embed<"resources/rec_back.png">());
    texmap[ResourceManager::TextureId::SAVING_SCORE] = load_texture_from_mem(renderer, b::embed<"resources/saving_score.png">());
    texmap[ResourceManager::TextureId::VIGNETTE] = load_texture_from_mem(renderer, b::embed<"resources/vignette.png">());

    return texmap;
}


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
