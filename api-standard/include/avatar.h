#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>
namespace apistandard {
    struct avatar {
        bool exists = false;
        size_t id = 0;
        std::string get_relative_path() const;
    };
    struct avatardata {
        std::vector<uint8_t> data;
        int width, height, channels;
    };
    void to_json(nlohmann::json& j, const avatar& a);
    void to_json(nlohmann::json& j, const avatardata& ad);
    void from_json(const nlohmann::json& j, avatar& a);
    void from_json(const nlohmann::json& j, avatardata& ad);
}