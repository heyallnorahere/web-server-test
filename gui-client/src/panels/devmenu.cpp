#include "devmenu.h"
#include <imgui.h>
#include "chat_panel.h"
#include "../application.h"
#include <cassert>
namespace guifrontend {
    namespace panels {
        devmenu_impl::devmenu_impl(const std::vector<descriptor>& descriptors) {
            this->m_descriptors = descriptors;
        }
        void devmenu_impl::render(bool* visible) {
            ImGui::Begin("Developer Menu", visible);
            if (ImGui::Button("Reload caches")) {
                size_t chat_index = (size_t)-1;
                for (size_t i = 0; i < this->m_descriptors.size(); i++) {
                    const auto& desc = this->m_descriptors[i];
                    if (desc.name == "Chat") {
                        chat_index = i;
                        break;
                    }
                }
                assert(chat_index != (size_t)-1);
                auto chat_panel_ = (chat_panel*)this->m_descriptors[chat_index].instance.get();
                chat_panel_->clear_cache();
            }
            ImGui::End();
        }
    }
}