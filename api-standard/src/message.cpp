#include "message.h"
#include "user.h"
namespace apistandard {
    void from_json(const nlohmann::json& j, message& m) {
        j["content"].get_to(m.content);
        j["color"].get_to(m.color);
        if (!j["from"].is_null()) {
            j["from"].get_to(m.from.l);
            m.from.exists = true;
        }
    }
    void from_json(const nlohmann::json& j, logmessage& m) {
        j["content"].get_to(m.content);
        j["color"].get_to(m.color);
        j["from"].get_to(m.from);
    }
    void from_json(const nlohmann::json& j, deletemessage& m) {
        j["id"].get_to(m.id);
        j["auth"].get_to(m.auth);
    }
    void to_json(nlohmann::json& j, const message& m) {
        j["content"] = m.content;
        j["color"] = m.color;
        if (m.from.exists) {
            j["from"] = m.from.l;
        } else {
            j["from"] = nullptr;
        }
    }
    void to_json(nlohmann::json& j, const logmessage& m) {
        j["content"] = m.content;
        j["color"] = m.color;
        j["from"] = m.from;
    }
    void to_json(nlohmann::json& j, const deletemessage& m) {
        j["id"] = m.id;
        j["auth"] = m.auth;
    }
    message create_message(const std::string& content, color color) {
        message m;
        m.content = content;
        m.color = color;
        return m;
    }
}