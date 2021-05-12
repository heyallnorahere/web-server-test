#include "devmenu.h"
#include <imgui.h>
namespace guifrontend {
    namespace panels {
        devmenu_impl::devmenu_impl(const std::vector<descriptor>& descriptors) {
            this->m_descriptors = descriptors;
        }
        void devmenu_impl::render(bool* visible) {
            ImGui::Begin("Developer Menu", visible);
            if (ImGui::Button("Reload caches")) {
                // todo: implement
            }
            ImGui::End();
        }
    }
}