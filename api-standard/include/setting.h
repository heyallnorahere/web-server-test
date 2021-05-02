#pragma once
#include <string>
#include <nlohmann/json.hpp>
namespace apistandard {
    struct setting {
        std::string name, value;
    };
    void to_json(nlohmann::json& j, const setting& s);
    void from_json(const nlohmann::json& j, setting& s);
}