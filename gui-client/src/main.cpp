#include "application.h"
#include "panels/login_panel.h"
#include "panels/settings_panel.h"
#include "panels/chat_panel.h"
#include "panels/devmenu.h"
#include <imgui.h>
#include <utility>
#include <vector>
struct panel_descriptor {
    std::shared_ptr<guifrontend::panel> instance;
    std::string name;
};
// a little hacky template stuff
template<typename T, typename... A> static void add_panel(std::vector<panel_descriptor>& descriptors, const std::string& name, A&&... args) {
    descriptors.push_back({ std::make_shared<T>(std::forward<A>(args)...), name });
}
static std::vector<panel_descriptor> make_descriptors() {
    using namespace guifrontend::panels;
    std::vector<panel_descriptor> descriptors;
    add_panel<login_panel>(descriptors, "Login", 1);
    add_panel<settings_panel>(descriptors, "Settings");
    add_panel<chat_panel>(descriptors, "Chat", 1, 0);
    return descriptors;
}
static void add_panels(guifrontend::application& app) {
    // get the descriptors
    auto descriptors = make_descriptors();
    // if the WST_BUILD_DEVELOPER_MENU option was turned on, add the developer menu to the list of descriptors
#ifdef BUILD_DEVELOPER_MENU
    add_panel<guifrontend::panels::devmenu<panel_descriptor>>(descriptors, "Developer Menu", descriptors);
#endif
    // make a vector of strings to later feed to the application
    std::vector<std::string> name_map;
    // add the panels to the application
    for (const auto& desc : descriptors) {
        // push the name of the panel into the vector
        name_map.push_back(desc.name);
        // add the panel instance to the application
        app.add_panel(desc.instance);
    }
    // give the application the names of the panels
    app.set_panel_name_map(name_map);
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