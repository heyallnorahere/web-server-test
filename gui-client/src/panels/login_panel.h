#pragma once
#include "../panel.h"
#include <string>
namespace guifrontend {
    namespace panels {
        class login_panel : public panel {
        public:
            enum class status {
                LOGGED_IN,
                FAILED,
                IDLE,
                USER_NOT_FOUND,
            };
            login_panel(size_t settings_index);
            virtual void render() override;
            size_t get_logged_in_user();
        private:
            void send_request(const std::string& displayname, const std::string& password);
            size_t m_settings_panel_index;
            status m_status;
            size_t m_user;
        };
    }
}