#include "avatar.h"
namespace apistandard {
    std::string avatar::get_relative_path() const {
        return "/avatar/" + (this->exists ? std::to_string(this->id) : "default") + ".png/get";
    }
    void to_json(nlohmann::json& j, const avatar& a) {
        j["exists"] = a.exists;
        j["id"] = a.id;
    }
    void to_json(nlohmann::json& j, const avatardata& ad) {
        j["data"] = ad.data;
        j["width"] = ad.width;
        j["height"] = ad.height;
        j["channels"] = ad.channels;
    }
    void from_json(const nlohmann::json& j, avatar& a) {
        j["exists"].get_to(a.exists);
        j["id"].get_to(a.id);
    }
    void from_json(const nlohmann::json& j, avatardata& ad) {
        if (!j["data"].is_null()) {
            j["data"].get_to(ad.data);
        }
        j["width"].get_to(ad.width);
        j["height"].get_to(ad.height);
        j["channels"].get_to(ad.channels);
    }
}