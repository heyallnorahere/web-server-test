#include "log.h"
#include <fstream>
#include <nlohmann/json.hpp>
using log_ = class log;
void from_json(const nlohmann::json& j, log_& l) {
    l.clear();
    for (const auto& m : j) {
        l.push_back(m.get<apistandard::logmessage>());
    }
}
void to_json(nlohmann::json& j, const log_& l) {
    for (size_t i = 0; i < l.size(); i++) {
        j.push_back(l[i]);
    }
}
log::~log() {
    if (!this->m_path.empty()) {
        this->serialize();
    }
}
static bool file_exists(const std::string& path) {
    FILE* f = fopen(path.c_str(), "rb");
    bool exists = f;
    if (f) fclose(f);
    return exists;
}
void log::set_file_path(const std::string& path) {
    this->m_path = path;
    if (file_exists(path)) {
        this->deserialize();
    }
}
void log::serialize() {
    std::ofstream file(this->m_path);
    nlohmann::json json_data = *this;
    file << json_data.dump(4);
    file.close();
}
void log::deserialize() {
    std::ifstream file(this->m_path);
    nlohmann::json json_data;
    file >> json_data;
    json_data.get_to(*this);
    file.close();
}
std::string log::dump() {
    nlohmann::json json_data = *this;
    return json_data.dump();
}
void log::push_back(const apistandard::logmessage& message) {
    auto copy = message;
    copy.id = this->m.size();
    this->m.push_back(copy);
    this->serialize();
}
void log::remove(size_t id) {
    this->m.remove_if([&](const apistandard::logmessage& other) { return other == this->operator[](id); });
    this->serialize();
}
size_t log::size() const {
    return this->m.size();
}
const apistandard::logmessage& log::operator[](size_t id) const {
    auto it = this->m.begin();
    std::advance(it, id);
    return *it;
}
void log::clear() {
    this->m.clear();
}
log::log() { }