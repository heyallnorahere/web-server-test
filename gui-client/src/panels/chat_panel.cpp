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
        void chat_panel::render() {
            ImGui::Begin("Chat", &this->m_visible);
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
            ImGui::Columns(3);
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
                auto login = (login_panel*)this->m_parent->get_panel(this->m_login_panel_index).get();
                auto settings = (settings_panel*)this->m_parent->get_panel(this->m_settings_panel_index).get();
                auto login_struct = login->get_login();
                if (login_struct.id != (size_t)-1) {
                    message_struct.from.l = { login_struct.id, login_struct.password };
                    message_struct.from.exists = true;
                }
                nlohmann::json json_data = message_struct;
                std::string address = settings->get_settings().server_address;
                util::request(util::request_type::POST, address + "/message", { { "Content-Type", "application/json" } }, json_data.dump());
            }
            ImGui::Columns(1);
            ImGui::End();
        }
    }
}