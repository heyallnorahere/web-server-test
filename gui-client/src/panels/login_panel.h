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
            struct login {
                size_t id;
                std::string password;
            };
            login_panel(size_t settings_index);
            virtual void render() override;
            login get_login();
        private:
            void send_request(const std::string& displayname, const std::string& password, const std::string& address);
            size_t m_settings_panel_index;
            status m_status;
            login m_login;
        };
    }
}