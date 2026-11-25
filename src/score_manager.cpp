#include "file_manager.hpp"
#include "score_manager.hpp"

#include <SDL3/SDL.h>

#include <string>
#include <fstream>

ScoreManager::ScoreManager() {
    const std::string filepath = get_app_files_path("scores.json");
    std::ifstream file(filepath, std::ios::in | std::ios::binary);

    SDL_Log("Loading score from: %s", filepath.c_str());
    
    if (file.is_open()) {
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

        m_json = nlohmann::json::parse(content, nullptr, false);
    }

    if (m_json.is_discarded() || m_json.is_null() || !m_json.is_array()) {
        m_json = nlohmann::json::array();
        m_json.push_back(nlohmann::json::array());
        m_json.push_back(nlohmann::json::array());
        m_json.push_back(nlohmann::json::array());
        m_changed = true;
        save();
    }

    // if broken guarantee 3
    while (m_json.size() < 3)
        m_json.push_back(nlohmann::json::array());

}

ScoreManager::~ScoreManager() {
    save();
}

void ScoreManager::save() {
    if (!m_changed) return;

    const std::string filepath = get_app_files_path("scores.json");
    SDL_Log("Attempt to store score at: %s", filepath.c_str());
    std::ofstream file(filepath, std::ios::out | std::ios::binary | std::ios::trunc);
    
    if (file.is_open()) {
        file << m_json;
        SDL_Log("Stored score at: %s", filepath.c_str());
    } else {
        SDL_Log("Failed to open scores file for writing at: %s", filepath.c_str());
    }

    m_changed = false;
}

std::vector<uint64_t> ScoreManager::get_scores(uint64_t level) {
    if (m_json.size() <= level) return {};
    std::vector<uint64_t> vec = m_json[level];
    std::sort(vec.begin(), vec.end(), [](const auto& a, const auto& b){ return a > b; });
    return vec;
}

bool ScoreManager::add_score(uint64_t level, uint64_t value) {
    if (m_json.size() <= level) return false;
    m_json[level].push_back(value);    
    return (m_changed = true);
}
