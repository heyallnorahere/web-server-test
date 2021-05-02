#include "handlers.h"
#include <api-standard.h>
#include <nlohmann/json.hpp>
#include "userdatabase.h"
#include <iostream>
void get_user_handler(const std::shared_ptr<restbed::Session> session) {
    size_t id = session->get_request()->get_path_parameter("id", 0);
    auto user = userdatabase::database->get(id);
    nlohmann::json json_data = user;
    session->close(restbed::OK, json_data.dump());
}
void new_user_handler(const std::shared_ptr<restbed::Session> session) {
    const auto request = session->get_request();
    size_t length = request->get_header("Content-Length", 0);
    session->fetch(length, [](std::shared_ptr<restbed::Session> session, const restbed::Bytes& body) {
        std::string data = std::string((char*)body.data(), body.size());
        nlohmann::json j = nlohmann::json::parse(data);
        apistandard::newuser newuser = j.get<apistandard::newuser>();
        size_t id = userdatabase::database->new_user(newuser.displayname, newuser.password);
        std::cout << "Created new user: " << newuser.displayname << " (ID: " << id << ")" << std::endl;
        session->close(restbed::OK, std::to_string(id));
    });
}
void add_user_handlers(restbed::Service& service) {
    auto get_user = std::make_shared<restbed::Resource>();
    get_user->set_path("/user/{id: [0-9]}");
    get_user->set_method_handler("GET", get_user_handler);
    auto new_user = std::make_shared<restbed::Resource>();
    new_user->set_path("/user/new");
    new_user->set_method_handler("POST", new_user_handler);
    service.publish(get_user);
    service.publish(new_user);
}