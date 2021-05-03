#pragma once
#include <string>
#include <nlohmann/json.hpp>
namespace apistandard {
    struct login {
        size_t id;
        std::string password;
    };
    struct login_verification {
        size_t id;
        bool is_valid;
    };
    void to_json(nlohmann::json& j, const login& l);
    void to_json(nlohmann::json& j, const login_verification& lv);
    void from_json(const nlohmann::json& j, login& l);
    void from_json(const nlohmann::json& j, login_verification& lv);
}