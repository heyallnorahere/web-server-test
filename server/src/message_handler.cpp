#include "handlers.h"
#include <api-standard.h>
#include <iostream>
void print_colored(const apistandard::message& message) {
    std::cout << message.content << std::endl;
}
struct response_struct {
    std::string content;
    apistandard::color color = apistandard::RED | apistandard::GREEN | apistandard::BLUE;
    bool succeeded = false;
};
void to_json(nlohmann::json& j, const response_struct& rs) {
    j["content"] = rs.content;
    j["color"] = rs.color;
    j["succeeded"] = rs.succeeded;
}
void message_handler(const std::shared_ptr<restbed::Session> session) {
    const auto request = session->get_request();
    size_t length = request->get_header("Content-Length", 0);
    session->fetch(length, [request](std::shared_ptr<restbed::Session> session, const restbed::Bytes& body) {
        std::string data = std::string((char*)body.data(), body.size());
        std::string contenttype = request->get_header("Content-Type");
        response_struct response;
        if (contenttype == "application/json") {
            nlohmann::json json_data = nlohmann::json::parse(data);
            apistandard::message message = json_data.get<apistandard::message>();
            print_colored(message);
            response.content = message.content;
            response.color = message.color;
            response.succeeded = true;
        } else if (contenttype == "text/plain") {
            std::cout << data << std::endl;
            response.content = data;
            response.succeeded = true;
        }
        nlohmann::json json_data = response;
        session->close(restbed::OK, json_data.dump());
    });
}