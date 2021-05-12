#pragma once
#include "../panel.h"
#include "../image.h"
#include <map>
#include <memory>
namespace guifrontend {
    namespace panels {
        class chat_panel : public panel {
        public:
            chat_panel(size_t settings_index, size_t login_index);
            virtual void render() override;
            void clear_cache();
            std::map<size_t, std::shared_ptr<image>> get_cache();
        private:
            std::map<size_t, std::shared_ptr<image>> m_cache;
            size_t m_settings_panel_index, m_login_panel_index;
        };
    }
}