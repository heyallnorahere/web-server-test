#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "login.h"
#define BINARY_FLAG(bits) (1 << (bits))
namespace apistandard {
    enum _color : uint32_t {
        RED = BINARY_FLAG(0),
        GREEN = BINARY_FLAG(1),
        BLUE = BINARY_FLAG(2)
    };
    using color = uint32_t;
    struct user;
    struct message {
        std::string content;
        color color;
        struct {
            login l;
            bool exists = false;
        } from;
    };
    void from_json(const nlohmann::json& j, message& m);
    void to_json(nlohmann::json& j, const message& m);
    message create_message(const std::string& content, color color);
}