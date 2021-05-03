#include "application.h"
#include "panels/login_panel.h"
#include "panels/settings_panel.h"
#include "panels/chat_panel.h"
#include <imgui.h>
void add_panels(guifrontend::application& app) {
    using namespace guifrontend::panels;
    // add the panels to the application
    app.add_panel(std::make_shared<login_panel>(1));
    app.add_panel(std::make_shared<settings_panel>());
    app.add_panel(std::make_shared<chat_panel>(1, 0));
    // give the application the names of the panels
    app.set_panel_name_map({ "Login", "Settings", "Chat" });
}
int main(int argc, const char* argv[]) {
    // create the application
    guifrontend::application app("GUI Client");
    // add and configure panels
    add_panels(app);
    // run the application
    while (!app.should_window_close()) {
        app.render();
        app.update_window();
    }
    // the app will be staticly destructed
    return 0;
}