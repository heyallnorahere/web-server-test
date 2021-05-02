#pragma once
#include <list>
#include <memory>
#include <string>
#include <api-standard.h>
class userdatabase {
public:
    ~userdatabase();
    apistandard::user get(size_t id);
    void set(size_t id, const apistandard::user& newer);
    bool verify_creds(size_t id, const std::string& password);
    size_t find(const std::string& displayname);
    size_t new_user(const std::string& displayname, const std::string& password);
    void delete_user(size_t id);
    void serialize();
    void deserialize();
    void set_file_path(const std::string& path);
    std::string get_file_path();
    static userdatabase& get_database();
private:
    std::list<apistandard::user> m;
    std::string m_path;
    userdatabase();
    static userdatabase instance;
};