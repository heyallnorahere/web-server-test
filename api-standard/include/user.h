#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "setting.h"
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
    user create_user(size_t id, const std::string& displayname, const std::string& password);
    setting get_setting(const user& u, const std::string& name);
    void set_setting(user& u, const setting& s);
}