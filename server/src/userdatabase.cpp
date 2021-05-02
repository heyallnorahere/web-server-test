#include "userdatabase.h"
#include <fstream>
#include <nlohmann/json.hpp>
static void to_json(nlohmann::json& j, const std::shared_ptr<apistandard::user>& u) {
    // very simple, but it works
    j = *u;
}
static void from_json(const nlohmann::json& j, std::shared_ptr<apistandard::user>& u) {
    apistandard::user user;
    j.get_to(user);
    u = std::make_shared<apistandard::user>(user);
}
userdatabase::~userdatabase() {
    if (!this->m_path.empty()) {
        this->serialize();
    }
}
apistandard::user userdatabase::get(size_t id) {
    auto it = this->m.begin();
    std::advance(it, id);
    return *it;
}
void userdatabase::set(size_t id, const apistandard::user& newer) {
    auto it = this->m.begin();
    std::advance(it, id);
    *it = newer;
    this->serialize();
}
bool userdatabase::verify_creds(size_t id, const std::string& password) {
    auto user = this->get(id);
    return user.password == password;
}
size_t userdatabase::new_user(const std::string& displayname, const std::string& password) {
    size_t id = this->m.size();
    auto user = apistandard::create_user(id, displayname, password);
    this->m.push_back(user);
    this->serialize();
    return id;
}
void userdatabase::delete_user(size_t id) {
    this->m.remove_if([id](auto u) {
        return u.id == id;
    });
    this->serialize();
}
void userdatabase::serialize() {
    nlohmann::json json_data = this->m;
    std::ofstream file(this->m_path);
    file << json_data.dump(4);
    file.close();
}
void userdatabase::deserialize() {
    nlohmann::json json_data;
    std::ifstream file(this->m_path);
    file >> json_data;
    file.close();
    json_data.get_to(this->m);
}
void userdatabase::set_file_path(const std::string& path) {
    this->m_path = path;
}
std::string userdatabase::get_file_path() {
    return this->m_path;
}
std::shared_ptr<userdatabase> userdatabase::database;