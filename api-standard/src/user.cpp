#include "user.h"
namespace apistandard {
    void from_json(const nlohmann::json& j, getuser& gu) {
        j["id"].get_to(gu.id);
        j["displayname"].get_to(gu.displayname);
    }
    void from_json(const nlohmann::json& j, newuser& nu) {
        j["displayname"].get_to(nu.displayname);
        j["password"].get_to(nu.password);
    }
    void from_json(const nlohmann::json& j, user& u) {
        j["id"].get_to(u.id);
        j["displayname"].get_to(u.displayname);
        j["password"].get_to(u.password);
    }
    void to_json(nlohmann::json& j, const getuser& gu) {
        j["id"] = gu.id;
        j["displayname"] = gu.displayname;
    }
    void to_json(nlohmann::json& j, const newuser& nu) {
        j["displayname"] = nu.displayname;
        j["password"] = nu.password;
    }
    void to_json(nlohmann::json& j, const user& u) {
        j["id"] = u.id;
        j["displayname"] = u.displayname;
        j["password"] = u.password;
    }
    user create_user(size_t id, std::string displayname, std::string password) {
        user u;
        u.id = id;
        u.displayname = displayname;
        u.password = password;
        return u;
    }
}