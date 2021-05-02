#include "user.h"
namespace apistandard {
    void from_json(const nlohmann::json& j, user& u) {
        j["id"].get_to(u.id);
        j["displayname"].get_to(u.displayname);
        j["password"].get_to(u.password);
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