#include "application.h"
#include "panels/login_panel.h"
#include "panels/settings_panel.h"
#include <imgui.h>
void add_panels(guifrontend::application& app) {
    using namespace guifrontend::panels;
    app.add_panel(std::make_shared<login_panel>(1));
    app.add_panel(std::make_shared<settings_panel>());
}
int main(int argc, const char* argv[]) {
    guifrontend::application app("GUI Client");
    add_panels(app);
    while (!app.should_window_close()) {
        app.render();
        app.update_window();
    }
    return 0;
}