#pragma once
#include <string>
#include <nlohmann/json.hpp>
namespace apistandard {
    struct user {
        size_t id;
        std::string displayname, password;
    };
    void from_json(const nlohmann::json& j, user& u);
    void to_json(nlohmann::json& j, const user& u);
    user create_user(size_t id, std::string displayname, std::string password);
}