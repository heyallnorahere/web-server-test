#pragma once
#include "../panel.h"
#include <string>
namespace guifrontend {
    namespace panels {
        class settings_panel : public panel {
        public:
            struct data {
                std::string server_address;
            };
            settings_panel(size_t login_index, size_t chat_index);
            virtual void render() override;
            void serialize();
            void deserialize();
            data get_settings();
        private:
            void check_settings();
            data m;
            std::string m_config_file;
            size_t m_login_panel_index, m_chat_panel_index;
        };
    }
}