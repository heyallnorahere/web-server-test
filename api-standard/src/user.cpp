#include "user.h"
#include <map>
namespace apistandard {
    void from_json(const nlohmann::json& j, getuser& gu) {
        j["id"].get_to(gu.id);
        j["displayname"].get_to(gu.displayname);
        j["admin"].get_to(gu.admin);
        j["avatar"].get_to(gu.avatar);
    }
    void from_json(const nlohmann::json& j, newuser& nu) {
        j["displayname"].get_to(nu.displayname);
        j["password"].get_to(nu.password);
    }
    void from_json(const nlohmann::json& j, user& u) {
        j["id"].get_to(u.id);
        j["displayname"].get_to(u.displayname);
        j["password"].get_to(u.password);
        j["admin"].get_to(u.admin);
        j["avatar"].get_to(u.avatar);
    }
    void to_json(nlohmann::json& j, const getuser& gu) {
        j["id"] = gu.id;
        j["displayname"] = gu.displayname;
        j["admin"] = gu.admin;
        j["avatar"] = gu.avatar;
    }
    void to_json(nlohmann::json& j, const newuser& nu) {
        j["displayname"] = nu.displayname;
        j["password"] = nu.password;
    }
    void to_json(nlohmann::json& j, const user& u) {
        j["id"] = u.id;
        j["displayname"] = u.displayname;
        j["password"] = u.password;
        j["admin"] = u.admin;
        j["avatar"] = u.avatar;
    }
    user create_user(size_t id, const std::string& displayname, const std::string& password) {
        user u;
        u.id = id;
        u.displayname = displayname;
        u.password = password;
        u.admin = false;
        return u;
    }
    static std::map<std::string, std::string*> make_property_map(const user& u) {
        std::map<std::string, std::string*> property_map;
        property_map["displayname"] = (std::string*)&u.displayname;
        property_map["password"] = (std::string*)&u.password;
        return property_map;
    }
    setting get_setting(const user& u, const std::string& name) {
        auto pm = make_property_map(u);
        std::string value;
        auto it = pm.find(name);
        if (it != pm.end()) {
            value = *(it->second);
        }
        return { name, value };
    }
    void set_setting(user& u, const setting& s) {
        auto pm = make_property_map(u);
        auto it = pm.find(s.name);
        if (it != pm.end()) {
            *(it->second) = s.value;
        }
    }
}