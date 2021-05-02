#include "application.h"
#include <imgui.h>
class testpanel : public guifrontend::panel {
public:
    testpanel() { }
    virtual void render() override {
        ImGui::Begin("test panel");
        ImGui::Text("testing");
        ImGui::End();
    }
};
int main(int argc, const char* argv[]) {
    using namespace guifrontend;
    application app("GUI Client");
    app.add_panel(std::make_shared<testpanel>());
    while (!app.should_window_close()) {
        app.render();
        app.update_window();
    }
    return 0;
}