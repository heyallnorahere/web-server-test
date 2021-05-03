#pragma once
#include <cstdlib>
namespace guifrontend {
    class application;
    class panel {
    public:
        panel(bool visible = true);
        virtual ~panel();
        virtual void render() = 0;
        void set_visible(bool visibility);
    protected:
        // i probably SHOULD use shared_ptr, but i dont want to deal with header shenanigans
        application* m_parent;
        bool m_visible;
        virtual void on_attach();
        friend class application;
    };
}