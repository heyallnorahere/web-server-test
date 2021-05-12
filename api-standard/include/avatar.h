#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>
#include "login.h"
namespace apistandard {
    struct avatardata {
        std::vector<uint8_t> data;
        int width, height, channels;
    };
    struct setavatar {
        login user;
        avatardata content;
    };
    void to_json(nlohmann::json& j, const avatardata& ad);
    void to_json(nlohmann::json& j, const setavatar& sa);
    void from_json(const nlohmann::json& j, avatardata& ad);
    void from_json(const nlohmann::json& j, setavatar& sa);
}