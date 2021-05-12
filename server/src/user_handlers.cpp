#include "handlers.h"
#include <api-standard.h>
#include <nlohmann/json.hpp>
#include <base64.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "userdatabase.h"
#include <iostream>
#include <fstream>
#include <cassert>
static bool file_exists(const std::string& path) {
    std::ifstream file(path);
    if (file.is_open()) {
        file.close();
        return true;
    }
    return false;
}
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
extern std::string data_directory;
void get_avatar_handler(const std::shared_ptr<restbed::Session> session) {
    auto request = session->get_request();
    std::string file = request->get_path_parameter("file", "default.png");
    std::string path = data_directory + "/avatar/" + file;
    if (!file_exists(path)) {
        path = data_directory + "/avatar/default.png";
        // if there is no default avatar, the client will crash, due to stb_image returning null on failure,
        // so we just get this out of the way right now.
        assert(file_exists(path));
    }
    apistandard::avatardata data;
    uint8_t* bytes = stbi_load(path.c_str(), &data.width, &data.height, &data.channels, NULL);
    if (bytes) {
        for (int i = 0; i < data.width * data.height * data.channels; i++) {
            data.data.push_back(bytes[i]);
        }
        stbi_image_free(bytes);
    }
    nlohmann::json json_data = data;
    session->close(restbed::OK, json_data.dump());
}
void set_avatar_handler(const std::shared_ptr<restbed::Session> session) {
    auto request = session->get_request();
    size_t length = request->get_header("Content-Length", 0);
    session->fetch(length, [request](std::shared_ptr<restbed::Session> session, const restbed::Bytes& body) {
        std::string data = std::string((char*)body.data(), body.size());
        nlohmann::json json_data = nlohmann::json::parse(data);
        auto requestdata = json_data.get<apistandard::setavatar>();
        std::string file = request->get_path_parameter("file", "");
        if (file.empty()) {
            session->close(restbed::NOT_FOUND);
            return;
        }
        size_t id = (size_t)-1;
        // this is the the c way of doing it; couldnt think of any others
        sscanf(file.c_str(), "%llu.png", &id);
        assert(id != (size_t)-1);
        if (id >= userdatabase::get_database().get_user_count()) {
            session->close(restbed::NOT_FOUND);
            return;
        }
        if (!userdatabase::get_database().verify_creds(requestdata.user.id, requestdata.user.password) || (id != requestdata.user.id)) {
            session->close(restbed::FORBIDDEN);
            return;
        }
        auto imagedata = requestdata.content;
        std::string path = data_directory + "/avatar/" + file;
        stbi_write_png(path.c_str(), imagedata.width, imagedata.height, imagedata.channels, imagedata.data.data(), imagedata.width * imagedata.channels);
        session->close(restbed::OK);
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
    auto get_avatar = std::make_shared<restbed::Resource>();
    get_avatar->set_path("/avatar/{file: [a-z0-9]*.png}/get");
    get_avatar->set_method_handler("GET", get_avatar_handler);
    auto set_avatar = std::make_shared<restbed::Resource>();
    set_avatar->set_path("/avatar/{file: [0-9].png}/set");
    set_avatar->set_method_handler("POST", set_avatar_handler);
    service.publish(get_user);
    service.publish(get_user_count);
    service.publish(user_setting);
    service.publish(new_user);
    service.publish(verify_user);
    service.publish(get_avatar);
    service.publish(set_avatar);
}