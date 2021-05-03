#include "login.h"
namespace apistandard {
    void to_json(nlohmann::json& j, const login& l) {
        j["id"] = l.id;
        j["password"] = l.password;
    }
    void to_json(nlohmann::json& j, const login_verification& lv) {
        j["id"] = lv.id;
        j["is_valid"] = lv.is_valid;
    }
    void from_json(const nlohmann::json& j, login& l) {
        j["id"].get_to(l.id);
        j["password"].get_to(l.password);
    }
    void from_json(const nlohmann::json& j, login_verification& lv) {
        j["id"].get_to(lv.id);
        j["is_valid"].get_to(lv.is_valid);
    }
}