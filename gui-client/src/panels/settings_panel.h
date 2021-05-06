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
            settings_panel();
            virtual void render() override;
            void serialize();
            void deserialize();
            data get_settings();
        private:
            void check_settings();
            data m;
            std::string m_config_file;
        };
    }
}