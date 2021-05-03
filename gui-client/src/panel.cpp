#include "panel.h"
namespace guifrontend {
    panel::panel(bool visible) {
        this->m_visible = visible;
    }
    panel::~panel() { }
    void panel::set_visible(bool visibility) {
        this->m_visible = visibility;
    }
    void panel::on_attach() { }
}