#include "application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cmath>
namespace guifrontend {
    application::application(const std::string& title, double version) {
        this->init_window(title, version);
        this->init_imgui();
    }
    application::~application() {
        this->terminate_imgui();
        this->terminate_window();
    }
    void application::render() {
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        for (auto p : this->m_panels) {
            if (p->m_visible) {
                p->render();
            }
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    void application::update_window() {
        glfwSwapBuffers((GLFWwindow*)this->m_window);
        glfwPollEvents();
    }
    bool application::should_window_close() {
        return glfwWindowShouldClose((GLFWwindow*)this->m_window);
    }
    void application::add_panel(const std::shared_ptr<panel>& p) {
        this->m_panels.push_back(p);
    }
    void application::init_window(const std::string& title, double version) {
        if (!glfwInit()) {
            exit(-1);
        }
        this->m_gl_version = version;
        int major = floor(version);
        int minor = (version - (double)major) * 10;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        this->m_window = glfwCreateWindow(800, 600, title.c_str(), NULL, NULL);
        if (!this->m_window) {
            exit(-1);
        }
        glfwMakeContextCurrent((GLFWwindow*)this->m_window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }
    void application::terminate_window() {
        glfwDestroyWindow((GLFWwindow*)this->m_window);
        glfwTerminate();
    }
    void application::init_imgui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)this->m_window, true);
        std::string shaderversion = "#version " + std::to_string((int)(this->m_gl_version * 100));
        ImGui_ImplOpenGL3_Init(shaderversion.c_str());
    }
    void application::terminate_imgui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}