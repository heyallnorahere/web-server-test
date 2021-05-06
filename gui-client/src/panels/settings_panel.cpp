#include "settings_panel.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include "../util.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <api-standard.h>
namespace guifrontend {
    namespace panels {
        void to_json(nlohmann::json& j, const settings_panel::data& d) {
            j["serveraddress"] = d.server_address;
        }
        void from_json(const nlohmann::json& j, settings_panel::data& d) {
            j["serveraddress"].get_to(d.server_address);
        }
        settings_panel::settings_panel() {
            this->m_config_file = "clientdata/config.json";
            if (util::file_exists(this->m_config_file)) {
                this->deserialize();
            } else {
                this->m.server_address = "http://127.0.0.1:1234";
            }
            this->check_settings();
        }
        void settings_panel::render() {
            ImGui::Begin("Settings", &this->m_visible);
            static data copy = this->m;
            ImGui::InputText("Server Address", &copy.server_address);
            if (ImGui::Button("Apply")) {
                this->m = copy;
                this->check_settings();
                copy = this->m;
            }
            ImGui::End();
        }
        settings_panel::data settings_panel::get_settings() {
            return this->m;
        }
        void settings_panel::serialize() {
            nlohmann::json json_data = this->m;
            std::ofstream file(this->m_config_file);
            file << json_data.dump(4);
            file.close();
        }
        void settings_panel::deserialize() {
            nlohmann::json json_data;
            std::ifstream file(this->m_config_file);
            file >> json_data;
            file.close();
            json_data.get_to(this->m);
        }
        void settings_panel::check_settings() {
            // check server status; if there is no server running
            // at that address, or the server is down, clear the
            // address setting
            auto response = util::request(util::request_type::GET, this->m.server_address + "/status");
            if (response.code != 200) {
                this->m.server_address.clear();
            } else {
                nlohmann::json json_data = nlohmann::json::parse(response.data);
                auto status = json_data.get<apistandard::status>();
                if (!status.up) {
                    this->m.server_address.clear();
                }
            }
            // update file
            this->serialize();
        }
    }
}