#include <iostream>
#include <restbed>
#include <memory>
#include <cstdlib>
#include <string>
#include "handlers.h"
#include "userdatabase.h"
static void post_request_handler(const std::shared_ptr<restbed::Session> session) {
    const auto request = session->get_request();
    size_t length = request->get_header("Content-Length", 0);
    std::cout << "Content-Length: " << length << std::endl;
    std::cout << "Content-Type: " << request->get_header("Content-Type") << std::endl;
    session->fetch(length, [](std::shared_ptr<restbed::Session> session, const restbed::Bytes& body) {
        std::string post_content = std::string((char*)body.data(), body.size());
        std::cout << "Got POST request: " << post_content << std::endl;
        session->close(restbed::OK, "Hello!");
    });
}
static bool file_exists(const std::string& path) {
    FILE* f = fopen(path.c_str(), "rb");
    bool exists = f;
    if (f) fclose(f);
    return exists;
}
int main(int argc, const char* argv[]) {
    std::string data_directory = "serverdata";
    if (argc > 2) {
        data_directory = argv[3];
    }
    userdatabase::database = std::make_shared<userdatabase>();
    userdatabase::database->set_file_path(data_directory + "/users.json");
    if (file_exists(userdatabase::database->get_file_path())) {
        userdatabase::database->deserialize();
    }
    int port = atoi(argv[1]);
    std::cout << "Hosting on port: " << port << std::endl;
    auto print = std::make_shared<restbed::Resource>();
    print->set_path("/print");
    print->set_method_handler("POST", post_request_handler);
    auto message = std::make_shared<restbed::Resource>();
    message->set_path("/message");
    message->set_method_handler("POST", message_handler);
    auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(port);
    settings->set_default_header("Connection", "close");
    restbed::Service service;
    service.publish(print);
    service.publish(message);
    add_user_handlers(service);
    service.start(settings);
    return 0;
}