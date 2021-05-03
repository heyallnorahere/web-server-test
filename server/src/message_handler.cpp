#include "handlers.h"
#include <api-standard.h>
#include <iostream>
#include "userdatabase.h"
#ifdef SYSTEM_UNIX
void print_colored_unix(const apistandard::message& message) {
    std::string displayname = "Anonymous user";
    if (message.from.exists) {
        displayname = userdatabase::get_database().get(message.from.l.id).displayname;
    }
    // didnt realize i did this until just now, as of writing this code
    uint32_t terminal_color_code = 30 + message.color;
    std::cout << displayname << " says: \033[" << terminal_color_code << "m" << message.content << "\033[37m" << std::endl;
}
#define print_colored print_colored_unix
#else
#error PLATFORM NOT SUPPORTED!
#endif
struct response_struct {
    std::string content;
    apistandard::color color = apistandard::RED | apistandard::GREEN | apistandard::BLUE;
    std::string error = "";
    bool succeeded = false;
};
void to_json(nlohmann::json& j, const response_struct& rs) {
    j["content"] = rs.content;
    j["color"] = rs.color;
    j["error"] = rs.error;
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
            response.content = message.content;
            response.color = message.color;
            if (message.from.exists && !userdatabase::get_database().verify_creds(message.from.l.id, message.from.l.password)) {
                response.error = "Credentials were incorrect";
                json_data = response;
                session->close(restbed::UNAUTHORIZED, json_data.dump());
                return;
            }
            print_colored(message);
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