#include <iostream>
#include <restbed>
#include <memory>
#include <cstdlib>
#include <string>
void post_request_handler(const std::shared_ptr<restbed::Session> session) {
    const auto request = session->get_request();
    auto& body = request->get_body();
    std::cout << "Content-Length: " << request->get_header("Content-Length") << std::endl;
    std::cout << "Content-Type: " << request->get_header("Content-Type") << std::endl;
    std::string post_content = std::string((char*)body.data(), body.size());
    std::cout << "Got POST request: " << post_content << std::endl;
    session->close(restbed::OK, "Hello!", { { "Content-Length", "6" }, { "Content-Type", "text" } });
}
int main(int argc, const char* argv[]) {
    int port = atoi(argv[1]);
    std::cout << "Hosting on port: " << port << std::endl;
    auto resource = std::make_shared<restbed::Resource>();
    resource->set_path("/api");
    resource->set_method_handler("POST", post_request_handler);
    auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(port);
    settings->set_default_header("Connection", "close");
    restbed::Service service;
    service.publish(resource);
    service.start(settings);
    return 0;
}