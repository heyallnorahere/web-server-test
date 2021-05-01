#include <iostream>
#include <restbed>
#include <memory>
#include <cstdlib>
#include <string>(
void post_request_handler(const std::shared_ptr<restbed::Session> session) {
    const auto request = session->get_request();
    size_t content_length = request->get_header<size_t>("Content-Length", 0);
    session->fetch(content_length, [](const std::shared_ptr<restbed::Session> session, const restbed::Bytes& body) {
        std::cout << "Got POST request: " << std::string((char*)body.data(), body.size()) << std::endl;
        session->close(restbed::OK, "Hello!", { { "Content-Length", "6" }, { "Content-Type", "text" } });
    });
}
int main(int argc, const char* argv[]) {
    auto resource = std::make_shared<restbed::Resource>();
    resource->set_path("/api");
    resource->set_method_handler("POST", post_request_handler);
    auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(8080);
    settings->set_default_header("Connection", "close");
    restbed::Service service;
    service.publish(resource);
    service.start(settings);
    return 0;
}