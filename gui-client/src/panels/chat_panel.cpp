#include "chat_panel.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <api-standard.h>
#include <vector>
#include <chrono>
#include "login_panel.h"
#include "settings_panel.h"
#include "../application.h"
#include "../util.h"
namespace guifrontend {
    namespace panels {
        chat_panel::chat_panel(size_t settings_index, size_t login_index) {
            this->m_settings_panel_index = settings_index;
            this->m_login_panel_index = login_index;
        }
        static double get_time() {
            using namespace std::chrono;
            return duration_cast<duration<double>>(system_clock::now().time_since_epoch()).count();
        }
        static void render_message(const apistandard::logmessage& msg, bool admin, size_t id, apistandard::login login, const std::string& address) {
            ImGui::Separator();
            ImGui::TextUnformatted(msg.from.c_str());
            ImVec4 color(0.f, 0.f, 0.f, 1.f);
            apistandard::color color_bits = msg.color;
            if (color_bits & apistandard::RED) {
                color.x = 1.f;
            }
            if (color_bits & apistandard::GREEN) {
                color.y = 1.f;
            }
            if (color_bits & apistandard::BLUE) {
                color.z = 1.f;
            }
            ImGui::TextColored(color, msg.content.c_str());
            if (admin) {
                ImGui::SameLine();
                if (ImGui::Button("Delete")) {
                    apistandard::deletemessage dm;
                    dm.id = id;
                    dm.auth = login;
                    nlohmann::json json_data = dm;
                    // todo: handle response in a log of some sort
                    util::request(util::request_type::POST, address + "/message/delete", { { "Content-Type", "application/json" } }, json_data.dump());
                }
            }
        }
        static void message_log(const std::string& address, login_panel* panel) {
            static std::vector<apistandard::logmessage> log;
            static double last_request = get_time();
            double current_time = get_time();
            constexpr double interval = 0.25;
            if (current_time - last_request >= interval) {
                auto response = util::request(util::request_type::GET, address + "/log");
                if (response.code == 200) {
                    nlohmann::json json_data = nlohmann::json::parse(response.data);
                    log.clear();
                    if (!json_data.is_null()) json_data.get_to(log);
                } else {
                    log.clear();
                }
                last_request = current_time;
            }
            ImGui::BeginChild("Message Log", { 400.f, 200.f }, ImGuiWindowFlags_AlwaysVerticalScrollbar);
            auto login = panel->get_login();
            auto user_response = util::request(util::request_type::GET, address + "/user/" + std::to_string(login.id));
            bool admin = false;
            if (user_response.code == 200) {
                nlohmann::json json_data = nlohmann::json::parse(user_response.data);
                auto user = json_data.get<apistandard::getuser>();
                admin = user.admin;
            }
            for (size_t i = 0; i < log.size(); i++) {
                render_message(log[i], admin, i, { login.id, login.password }, address);
            }
            ImGui::Separator();
            ImGui::EndChild();
        }
        void chat_panel::render() {
            ImGui::Begin("Chat", &this->m_visible);
            auto settings = (settings_panel*)this->m_parent->get_panel(this->m_settings_panel_index).get();
            std::string address = settings->get_settings().server_address;
            static std::string message;
            static int index = 0;
            std::vector<const char*> items = {
                "White",
                "Red",
                "Yellow",
                "Green",
                "Light blue",
                "Blue",
                "Purple",
                "Black",
            };
            message_log(address, (login_panel*)this->m_parent->get_panel(this->m_login_panel_index).get());
            ImGui::Columns(3);
            ImGui::SetColumnWidth(0, 100.f);
            ImGui::SetColumnWidth(1, 250.f);
            ImGui::Combo("##ColorInput", &index, items.data(), items.size());
            ImGui::NextColumn();
            ImGui::InputText("##MessageInput", &message);
            ImGui::NextColumn();
            if (ImGui::Button("Send")) {
                apistandard::color color;
                switch (index) {
                case 0:
                    color = apistandard::RED | apistandard::GREEN | apistandard::BLUE;
                    break;
                case 1:
                    color = apistandard::RED;
                    break;
                case 2:
                    color = apistandard::RED | apistandard::GREEN;
                    break;
                case 3:
                    color = apistandard::GREEN;
                    break;
                case 4:
                    color = apistandard::GREEN | apistandard::BLUE;
                    break;
                case 5:
                    color = apistandard::BLUE;
                    break;
                case 6:
                    color = apistandard::RED | apistandard::BLUE;
                    break;
                case 7:
                    color = 0;
                    break;
                }
                auto message_struct = apistandard::create_message(message, color);
                message.clear();
                auto login = (login_panel*)this->m_parent->get_panel(this->m_login_panel_index).get();
                auto login_struct = login->get_login();
                if (login_struct.id != (size_t)-1) {
                    message_struct.from.l = { login_struct.id, login_struct.password };
                    message_struct.from.exists = true;
                }
                nlohmann::json json_data = message_struct;
                // we dont care about the response for now
                util::request(util::request_type::POST, address + "/message", { { "Content-Type", "application/json" } }, json_data.dump());
            }
            ImGui::Columns(1);
            ImGui::End();
        }
    }
}