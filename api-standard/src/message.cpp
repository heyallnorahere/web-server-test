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
    void to_json(nlohmann::json& j, const message& m) {
        j["content"] = m.content;
        j["color"] = m.color;
        if (m.from.exists) {
            j["from"] = m.from.l;
        } else {
            j["from"] = nullptr;
        }
    }
    message create_message(const std::string& content, color color) {
        message m;
        m.content = content;
        m.color = color;
        return m;
    }
}