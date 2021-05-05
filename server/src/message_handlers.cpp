#include "handlers.h"
#include <api-standard.h>
#include <iostream>
#include "userdatabase.h"
#include "log.h"
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
#elif defined(_MSC_VER)
#include <Windows.h>
void print_colored_windows(const apistandard::message& message) {
    std::string displayname = "Anonymous user";
    if (message.from.exists) {
        displayname = userdatabase::get_database().get(message.from.l.id).displayname;
    }
    std::cout << displayname << " says: " << std::flush;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD color = 0;
    if (message.color & apistandard::RED) color |= FOREGROUND_RED;
    if (message.color & apistandard::GREEN) color |= FOREGROUND_GREEN;
    if (message.color & apistandard::BLUE) color |= FOREGROUND_BLUE;
    SetConsoleTextAttribute(console, color);
    WriteConsoleA(console, message.content.c_str(), message.content.length(), NULL, NULL);
    SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    std::cout << std::endl;
}
#define print_colored print_colored_windows
#else
#error PLATFORM NOT SUPPORTED!
#endif
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
apistandard::logmessage create_logmessage(const apistandard::message& m) {
    apistandard::logmessage lm;
    lm.content = m.content;
    lm.color = m.color;
    if (m.from.exists) {
        auto u = userdatabase::get_database().get(m.from.l.id);
        lm.from = u.displayname;
    } else {
        lm.from = "Anonymous";
    }
    return lm;
}
void message_handler(const std::shared_ptr<restbed::Session> session) {
    const auto request = session->get_request();
    size_t length = request->get_header("Content-Length", 0);
    session->fetch(length, [request](std::shared_ptr<restbed::Session> session, const restbed::Bytes& body) {
        std::string data = std::string((char*)body.data(), body.size());
        response_struct response;
        nlohmann::json json_data = nlohmann::json::parse(data);
        apistandard::message message = json_data.get<apistandard::message>();
        response.content = message.content;
        response.color = message.color;
        if (message.from.exists && !userdatabase::get_database().verify_creds(message.from.l.id, message.from.l.password)) {
            response.content = "Credentials were incorrect!";
            json_data = response;
            session->close(restbed::UNAUTHORIZED, json_data.dump());
            return;
        }
        print_colored(message);
        log::get().push_back(create_logmessage(message));
        response.succeeded = true;
        json_data = response;
        session->close(restbed::OK, json_data.dump());
    });
}
void delete_message_handler(const std::shared_ptr<restbed::Session> session) {
    const auto request = session->get_request();
    size_t length = request->get_header("Content-Length", 0);
    session->fetch(length, [](std::shared_ptr<restbed::Session> session, const restbed::Bytes& body) {
        std::string data = std::string((char*)body.data(), body.size());
        nlohmann::json json_data = nlohmann::json::parse(data);
        apistandard::deletemessage dm = json_data.get<apistandard::deletemessage>();
        response_struct response;
        if (dm.id >= log::get().size()) {
            response.content = "Message does not exist!";
            json_data = response;
            session->close(restbed::BAD_REQUEST, json_data.dump());
            return;
        }
        if (!userdatabase::get_database().verify_creds(dm.auth.id, dm.auth.password)) {
            response.content = "Credentials were incorrect!";
            json_data = response;
            session->close(restbed::UNAUTHORIZED, json_data.dump());
            return;
        }
        auto user = userdatabase::get_database().get(dm.auth.id);
        if (!user.admin) {
            response.content = "Insufficient permissions!";
            json_data = response;
            session->close(restbed::UNAUTHORIZED, json_data.dump());
            return;
        }
        auto message = log::get()[dm.id];
        log::get().remove(dm.id);
        response.content = message.content;
        response.color = message.color;
        response.succeeded = true;
        json_data = response;
        session->close(restbed::OK, json_data.dump());
    });
}
void add_message_handlers(restbed::Service& service) {
    auto message = std::make_shared<restbed::Resource>();
    message->set_path("/message");
    message->set_method_handler("POST", message_handler);
    auto delete_message = std::make_shared<restbed::Resource>();
    delete_message->set_path("/message/delete");
    delete_message->set_method_handler("POST", message_handler);
    service.publish(message);
    service.publish(delete_message);
}