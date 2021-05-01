#include "message.h"
namespace apistandard {
    void from_json(const nlohmann::json& j, message& m) {
        j["content"].get_to(m.content);
        j["color"].get_to(m.color);
    }
    void to_json(nlohmann::json& j, const message& m) {
        j["content"] = m.content;
        j["color"] = m.color;
    }
    message create_message(const std::string& content, color color) {
        message m;
        m.content = content;
        m.color = color;
        return m;
    }
}