#include <curl/curl.h>
#include <string>
#include <iostream>
#include <cassert>
#include <map>
#include <api-standard.h>
struct data_struct {
    std::string data;
    size_t pos = 0;
};
size_t read_callback(char* buffer, size_t size, size_t nitems, void* userdata) {
    data_struct& data = *(data_struct*)userdata;
    std::string to_write = data.data.substr(data.pos, nitems * size);
    size_t bytes_written = 0;
    for (size_t i = 0; i < to_write.length(); i++) {
        buffer[i] = to_write[i];
        bytes_written++;
    }
    std::cout << "Wrote " << bytes_written << " bytes" << std::endl;
    data.pos += bytes_written;
    return bytes_written;
}
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    data_struct& data = *(data_struct*)userdata;
    data.data.append(std::string(ptr, size * nmemb));
    return nmemb;
}
void check(CURLcode code) {
    assert(code == CURLE_OK);
}
int main(int argc, const char* argv[]) {
    std::string address = "http://" + std::string(argv[1]) + "/message";
    std::string content = argv[2];
    apistandard::color color = 0;
    {
        std::map<std::string, apistandard::color> colors = {
            { "red", apistandard::RED },
            { "green", apistandard::GREEN },
            { "blue", apistandard::BLUE },
            { "black", 0 },
            { "white", apistandard::RED | apistandard::GREEN | apistandard::BLUE },
            { "yellow", apistandard::RED | apistandard::GREEN },
            { "lightblue", apistandard::GREEN | apistandard::BLUE },
            { "purple", apistandard::RED | apistandard::BLUE },
        };
        color = colors[argv[3]];
    }
    CURL* c = curl_easy_init();
    assert(c);
    check(curl_easy_setopt(c, CURLOPT_URL, address.c_str()));
    check(curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, false));
    check(curl_easy_setopt(c, CURLOPT_POST, 1L));
    data_struct read, write;
    apistandard::message message = apistandard::create_message(content, color);
    nlohmann::json json_data = message;
    read.data = json_data.dump();
    check(curl_easy_setopt(c, CURLOPT_READFUNCTION, read_callback));
    check(curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_callback));
    check(curl_easy_setopt(c, CURLOPT_READDATA, (void*)&read));
    check(curl_easy_setopt(c, CURLOPT_WRITEDATA, (void*)&write));
    check(curl_easy_setopt(c, CURLOPT_POSTFIELDSIZE, read.data.length()));
    curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    check(curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers));
    //check(curl_easy_setopt(c, CURLOPT_POSTFIELDS, read.data.c_str()));
    std::cout << "Sending POST request to \"" << address << "\" with data \"" << read.data << "\"" << std::endl;
    check(curl_easy_perform(c));
    curl_slist_free_all(headers);
    curl_easy_cleanup(c);
    std::cout << "Got response: " << write.data << std::endl;
    return 0;
}