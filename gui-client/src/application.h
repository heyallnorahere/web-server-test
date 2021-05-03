#pragma once
#include <string>
#include <memory>
#include <vector>
#include "panel.h"
namespace guifrontend {
    class application {
    public:
        application(const std::string& title, double version = 3.3);
        ~application();
        void render();
        void update_window();
        bool should_window_close();
        void add_panel(const std::shared_ptr<panel>& p);
        std::shared_ptr<panel> get_panel(size_t index);
        application(const application&) = delete;
        application operator=(const application&) = delete;
    private:
        void init_window(const std::string& title, double version);
        void terminate_window();
        void init_imgui();
        void terminate_imgui();
        std::vector<std::shared_ptr<panel>> m_panels;
        void* m_window;
        double m_gl_version;
    };
}