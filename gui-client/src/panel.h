#pragma once
namespace guifrontend {
    class application;
    class panel {
    public:
        panel(bool visible = true);
        virtual void render() = 0;
    protected:
        bool m_visible;
        friend class application;
    };
}