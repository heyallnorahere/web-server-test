#pragma once
#include <string>
#include <nlohmann/json.hpp>
namespace apistandard {
    struct getuser {
        size_t id;
        std::string displayname;
    };
    struct newuser {
        std::string displayname, password;
    };
    struct user {
        size_t id;
        std::string displayname, password;
    };
    void from_json(const nlohmann::json& j, getuser& gu);
    void from_json(const nlohmann::json& j, newuser& nu);
    void from_json(const nlohmann::json& j, user& u);
    void to_json(nlohmann::json& j, const getuser& gu);
    void to_json(nlohmann::json& j, const newuser& nu);
    void to_json(nlohmann::json& j, const user& u);
    user create_user(size_t id, std::string displayname, std::string password);
}