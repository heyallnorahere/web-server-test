#include "avatar.h"
namespace apistandard {
    void to_json(nlohmann::json& j, const avatardata& ad) {
        j["data"] = ad.data;
        j["width"] = ad.width;
        j["height"] = ad.height;
        j["channels"] = ad.channels;
    }
    void to_json(nlohmann::json& j, const setavatar& sa) {
        j["user"] = sa.user;
        j["content"] = sa.content;
    }
    void from_json(const nlohmann::json& j, avatardata& ad) {
        if (!j["data"].is_null()) {
            j["data"].get_to(ad.data);
        }
        j["width"].get_to(ad.width);
        j["height"].get_to(ad.height);
        j["channels"].get_to(ad.channels);
    }
    void from_json(const nlohmann::json& j, setavatar& sa) {
        j["user"].get_to(sa.user);
        j["content"].get_to(sa.content);
    }
}