#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "login.h"
#define BITFLAG(bits) (1 << (bits))
namespace apistandard {
    enum _color : uint32_t {
        RED = BITFLAG(0),
        GREEN = BITFLAG(1),
        BLUE = BITFLAG(2),
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
    struct logmessage {
        std::string content;
        color color;
        std::string from;
        size_t id;
        bool operator==(const logmessage& other) const {
            return this->id == other.id;
        }
    };
    struct deletemessage {
        size_t id;
        login auth;
    };
    void from_json(const nlohmann::json& j, message& m);
    void from_json(const nlohmann::json& j, logmessage& m);
    void from_json(const nlohmann::json& j, deletemessage& m);
    void to_json(nlohmann::json& j, const message& m);
    void to_json(nlohmann::json& j, const logmessage& m);
    void to_json(nlohmann::json& j, const deletemessage& m);
    message create_message(const std::string& content, color color);
}