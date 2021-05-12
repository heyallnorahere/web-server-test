#pragma once
#include "../panel.h"
#include <vector>
#include <memory>
#include <string>
namespace guifrontend {
    namespace panels {
        class devmenu_impl {
        public:
            struct descriptor {
                std::shared_ptr<panel> instance;
                std::string name;
            };
            devmenu_impl(const std::vector<descriptor>& descriptors);
            void render(bool* visible);
        private:
            std::vector<descriptor> m_descriptors;
        };
        template<typename DescriptorType> class devmenu : public panel {
        public:
            devmenu(const std::vector<DescriptorType>& descriptors) {
                std::vector<devmenu_impl::descriptor> _descriptors;
                for (const auto& desc : descriptors) {
                    _descriptors.push_back({ desc.instance, desc.name });
                }
                this->m_impl = std::make_shared<devmenu_impl>(_descriptors);
            }
            virtual void render() override {
                this->m_impl->render(&this->m_visible);
            }
        private:
            std::shared_ptr<devmenu_impl> m_impl;
        };
    }
}