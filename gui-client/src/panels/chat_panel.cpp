#include "chat_panel.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <api-standard.h>
#include <vector>
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
        void message_log(const std::string& address) {
            auto response = util::request(util::request_type::GET, address + "/log");
            assert(response.code == 200);
            nlohmann::json json_data = nlohmann::json::parse(response.data);
            auto log = json_data.get<std::vector<apistandard::logmessage>>();
            ImGui::BeginChild("Message Log", { 200, 200 }, ImGuiWindowFlags_AlwaysVerticalScrollbar);
            for (const auto& msg : log) {
                ImGui::Separator();
                ImGui::Text(msg.from.c_str());
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
            message_log(address);
            ImGui::Columns(2);
            ImGui::Combo("##ColorInput", &index, items.data(), items.size());
            ImGui::SameLine();
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