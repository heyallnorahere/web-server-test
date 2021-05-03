#include "util.h"
#include <cstdio>
#include <cassert>
#include <curl/curl.h>
namespace guifrontend {
    namespace util {
        bool file_exists(const std::string& path) {
            FILE* f = fopen(path.c_str(), "rb");
            bool exists = f;
            if (f) fclose(f);
            return exists;
        }
        struct data_struct {
            std::string data;
            size_t pos = 0;
        };
        static size_t read_callback(char* buffer, size_t size, size_t nitems, data_struct* data) {
            std::string to_write = data->data.substr(data->pos, nitems * size);
            size_t bytes_written = 0;
            for (size_t i = 0; i < to_write.length(); i++) {
                buffer[i] = to_write[i];
                bytes_written++;
            }
            data->pos += bytes_written;
            return bytes_written;
        }
        static size_t write_callback(char* ptr, size_t size, size_t nmemb, data_struct* data) {
            std::string received(ptr, size * nmemb);
            data->data.append(received);
            data->pos += received.length();
            return nmemb;
        }
        response request(request_type type, const std::string& url, const std::vector<std::pair<std::string, std::string>>& headers, const std::string& data) {
            CURL* c = curl_easy_init();
            curl_easy_setopt(c, CURLOPT_URL, url.c_str());
            data_struct read, write;
            read.data = data;
            switch (type) {
            case request_type::POST:
                curl_easy_setopt(c, CURLOPT_POST, 1);
                curl_easy_setopt(c, CURLOPT_POSTFIELDSIZE, read.data.length());
                curl_easy_setopt(c, CURLOPT_READFUNCTION, read_callback);
                curl_easy_setopt(c, CURLOPT_READDATA, &read);
                break;
            default:
                break;
            }
            curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(c, CURLOPT_WRITEDATA, &write);
            curl_slist* header_list = NULL;
            for (auto p : headers) {
                std::string header = p.first + ": " + p.second;
                header_list = curl_slist_append(header_list, header.c_str());
            }
            if (header_list) curl_easy_setopt(c, CURLOPT_HTTPHEADER, header_list);
            curl_easy_perform(c);
            response r;
            long code;
            curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &code);
            r.code = (int)code;
            r.data = write.data;
            curl_easy_cleanup(c);
            return r;
        }
    }
}