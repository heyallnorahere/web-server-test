#include "setting.h"
namespace apistandard {
    void to_json(nlohmann::json& j, const setting& s) {
        j["name"] = s.name;
        j["value"] = s.value;
    }
    void from_json(const nlohmann::json& j, setting& s) {
        j["name"].get_to(s.name);
        j["value"].get_to(s.value);
    }
}