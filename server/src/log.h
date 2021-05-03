#pragma once
#include <string>
#include <vector>
#include <api-standard.h>
class log {
public:
    log(const log&) = delete;
    log operator=(const log&) = delete;
    ~log();
    void set_file_path(const std::string& path);
    void serialize();
    void deserialize();
    std::string dump();
    void push_back(const apistandard::logmessage& message);
    size_t size() const;
    const apistandard::logmessage& operator[](size_t index) const;
    void clear();
    static log& get() {
        static log instance;
        return instance;
    }
private:
    std::vector<apistandard::logmessage> m;
    std::string m_path;
    log();
};