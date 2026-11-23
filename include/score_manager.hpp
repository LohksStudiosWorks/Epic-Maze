#pragma once

#include <cstdint>
#include <string>

#include <nlohmann/json.hpp>
#include <vector>

class ScoreManager {
public:
    ScoreManager();
    ~ScoreManager();
    
    void save();

    std::vector<uint64_t> get_scores(uint64_t level);
    bool add_score(uint64_t level, uint64_t value);
private:
    nlohmann::json m_json;
    bool m_changed = false;
};