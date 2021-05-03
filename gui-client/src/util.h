#pragma once
#include <string>
#include <vector>
namespace guifrontend {
    namespace util {
        bool file_exists(const std::string& path);
        enum class request_type {
            GET,
            POST
        };
        struct response {
            std::string data;
            int code;
        };
        response request(request_type type, const std::string& url, const std::vector<std::pair<std::string, std::string>>& headers = std::vector<std::pair<std::string, std::string>>(), const std::string& data = "");
    }
}