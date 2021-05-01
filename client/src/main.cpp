#include <curl/curl.h>
#include <string>
#include <iostream>
#include <cassert>
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
int main(int argc, const char* argv[]) {
    std::string address = "http://" + std::string(argv[1]) + "/api";
    CURL* c = curl_easy_init();
    curl_easy_setopt(c, CURLOPT_URL, address.c_str());
    curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(c, CURLOPT_POST, 1L);
    data_struct read, write;
    read.data = "This was from the client program";
    curl_easy_setopt(c, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(c, CURLOPT_READDATA, (void*)&read);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, (void*)&write);
    curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: text");
    curl_easy_setopt(c, CURLOPT_HTTPHEADER, headers);
    std::cout << "Sending POST request to \"" << address << "\" with data \"" << read.data << "\"" << std::endl;
    CURLcode code = curl_easy_perform(c);
    assert(code == CURLE_OK);
    curl_slist_free_all(headers);
    curl_easy_cleanup(c);
    std::cout << "Got response: " << write.data << std::endl;
    return 0;
}