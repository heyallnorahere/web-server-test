#include "handlers.h"
#include <api-standard.h>
#include <nlohmann/json.hpp>
#include <base64.h>
#include "userdatabase.h"
#include <iostream>
void get_user_handler(const std::shared_ptr<restbed::Session> session) {
    size_t id = session->get_request()->get_path_parameter("id", 0);
    auto user = userdatabase::get_database().get(id);
    apistandard::getuser getuserdata;
    getuserdata.id = user.id;
    getuserdata.displayname = user.displayname;
    getuserdata.admin = user.admin;
    nlohmann::json json_data = getuserdata;
    session->close(restbed::OK, json_data.dump());
}
void get_user_count_handler(const std::shared_ptr<restbed::Session> session) {
    size_t count = userdatabase::get_database().get_user_count();
    session->close(restbed::OK, std::to_string(count));
}
void get_user_setting_handler(const std::shared_ptr<restbed::Session> session) {
    auto request = session->get_request();
    std::string authorization = request->get_header("Authorization");
    std::string base64_auth = authorization.substr(authorization.find_first_of(' ') + 1);
    std::string creds = base64_decode(base64_auth);
    std::string displayname = creds.substr(0, creds.find_first_of(':'));
    size_t id = userdatabase::get_database().find(displayname);
    // id should be validated from user_setting_auth_handler()
    auto user = userdatabase::get_database().get(id);
    std::string settingname = request->get_path_parameter("settingname");
    auto setting = apistandard::get_setting(user, settingname);
    if (setting.value.empty()) {
        session->close(restbed::NOT_FOUND);
        return;
    }
    nlohmann::json json_data = setting;
    session->close(restbed::OK, json_data.dump());
}
void post_user_setting_handler(const std::shared_ptr<restbed::Session> session) {
    auto request = session->get_request();
    size_t contentlength = request->get_header("Content-Length", 0);
    session->fetch(contentlength, [request](std::shared_ptr<restbed::Session> session, const restbed::Bytes& body) {
        std::string authorization = request->get_header("Authorization");
        std::string base64_auth = authorization.substr(authorization.find_first_of(' ') + 1);
        std::string creds = base64_decode(base64_auth);
        std::string displayname = creds.substr(0, creds.find_first_of(':'));
        size_t id = userdatabase::get_database().find(displayname);
        // see other /user/setting handler
        auto user = userdatabase::get_database().get(id);
        std::string settingname = request->get_path_parameter("settingname");
        std::string received_data = std::string((char*)body.data(), body.size());
        nlohmann::json json_data = nlohmann::json::parse(received_data);
        auto setting = json_data.get<apistandard::setting>();
        if (settingname != setting.name) {
            session->close(restbed::FORBIDDEN);
            return;
        }
        apistandard::set_setting(user, setting);
        userdatabase::get_database().set(id, user);
        auto new_setting = apistandard::get_setting(user, settingname);
        if (new_setting.value.empty()) {
            session->close(restbed::NOT_FOUND);
        } else {
            nlohmann::json json_data = new_setting;
            session->close(restbed::OK, json_data.dump());
        }
    });
}
void user_setting_auth_handler(const std::shared_ptr<restbed::Session> session, const std::function<void(const std::shared_ptr<restbed::Session> session)>& callback) {
    auto authorize = [&]() {
        session->close(restbed::UNAUTHORIZED, { { "WWW-Authenticate", "Basic realm=\"web-server-test\"" } });
    };
    auto request = session->get_request();
    std::string authorization = request->get_header("Authorization");
    std::string base64_auth = authorization.substr(authorization.find_first_of(' ') + 1);
    std::string creds = base64_decode(base64_auth);
    std::string displayname = creds.substr(0, creds.find_first_of(':'));
    size_t id = userdatabase::get_database().find(displayname);
    if (id == (size_t)-1) {
        authorize();
        return;
    }
    auto user = userdatabase::get_database().get(id);
    std::string correct_creds = displayname + ":" + user.password;
    std::string correct_auth = "Basic " + base64_encode(correct_creds);
    if (authorization != correct_auth) {
        authorize();
    } else {
        callback(session);
    }
}
void new_user_handler(const std::shared_ptr<restbed::Session> session) {
    auto request = session->get_request();
    size_t length = request->get_header("Content-Length", 0);
    session->fetch(length, [](std::shared_ptr<restbed::Session> session, const restbed::Bytes& body) {
        std::string data = std::string((char*)body.data(), body.size());
        nlohmann::json json_data = nlohmann::json::parse(data);
        apistandard::newuser newuser = json_data.get<apistandard::newuser>();
        size_t id = userdatabase::get_database().new_user(newuser.displayname, newuser.password);
        std::cout << "Created new user: " << newuser.displayname << " (ID: " << id << ")" << std::endl;
        session->close(restbed::OK, std::to_string(id));
    });
}
void verify_user_handler(const std::shared_ptr<restbed::Session> session) {
    auto request = session->get_request();
    size_t length = request->get_header("Content-Length", 0);
    session->fetch(length, [](std::shared_ptr<restbed::Session> session, const restbed::Bytes& body) {
        std::string data = std::string((char*)body.data(), body.size());
        nlohmann::json json_data = nlohmann::json::parse(data);
        auto login = json_data.get<apistandard::login>();
        bool valid = userdatabase::get_database().verify_creds(login.id, login.password);
        apistandard::login_verification verification;
        verification.id = login.id;
        verification.is_valid = valid;
        json_data = verification;
        session->close(restbed::OK, json_data.dump());
    });
}
void add_user_handlers(restbed::Service& service) {
    auto get_user = std::make_shared<restbed::Resource>();
    get_user->set_path("/user/{id: [0-9]}");
    get_user->set_method_handler("GET", get_user_handler);
    auto get_user_count = std::make_shared<restbed::Resource>();
    get_user_count->set_path("/user/count");
    get_user_count->set_method_handler("GET", get_user_count_handler);
    auto user_setting = std::make_shared<restbed::Resource>();
    user_setting->set_path("/user/setting/{settingname: [a-z]*}");
    user_setting->set_method_handler("GET", get_user_setting_handler);
    user_setting->set_method_handler("POST", post_user_setting_handler);
    user_setting->set_authentication_handler(user_setting_auth_handler);
    auto new_user = std::make_shared<restbed::Resource>();
    new_user->set_path("/user/new");
    new_user->set_method_handler("POST", new_user_handler);
    auto verify_user = std::make_shared<restbed::Resource>();
    verify_user->set_path("/user/verify");
    verify_user->set_method_handler("POST", verify_user_handler);
    service.publish(get_user);
    service.publish(get_user_count);
    service.publish(user_setting);
    service.publish(new_user);
    service.publish(verify_user);
}