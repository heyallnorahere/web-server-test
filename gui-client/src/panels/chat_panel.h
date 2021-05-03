#pragma once
#include "../panel.h"
namespace guifrontend {
    namespace panels {
        class chat_panel : public panel {
        public:
            chat_panel(size_t settings_index, size_t login_index);
            virtual void render() override;
        private:
            size_t m_settings_panel_index, m_login_panel_index;
        };
    }
}