
#include "gui.hpp"

#include <stdio.h>

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace ARAS
{
    GUI::GUI()
    {
    }

    GUI::~GUI()
    {
    }

    bool GUI::RunGUI()
    {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return false;
            // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        const char *glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        const char *glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
        // GL 3.0 + GLSL 130
        const char *glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
        window = glfwCreateWindow(960, 480, "AstroAir Server GUI", NULL, NULL);
        if (window == NULL)
            return 1;
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 16.0f);

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();

        return true;
    }

    bool GUI::RunTerminalUi()
    {
        using namespace ftxui;
        auto screen = ScreenInteractive::Fullscreen();
        int shift = 0;

        int tab_index = 0;
        std::vector<std::string> tab_entries = {
            "Camera", "Telescope", "Focuser", "Guider", "Web Server", "config",
        };
        auto tab_selection = Toggle(&tab_entries, &tab_index);
        auto tab_content = Container::Tab(
        {
        },
        &tab_index);

        auto main_container = Container::Vertical({
            tab_selection,
            tab_content,
        });

        auto main_renderer = Renderer(main_container, [&] {
            return vbox({
                text("FTXUI Demo") | bold | hcenter,
                tab_selection->Render() | hcenter,
                tab_content->Render() | flex,
            });
        });

        bool refresh_ui_continue = true;
        std::thread refresh_ui([&]{
            while (refresh_ui_continue) {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(0.05s);
                shift++;
                screen.PostEvent(Event::Custom);
        }});
        screen.Loop(main_renderer);
        refresh_ui_continue = false;
        refresh_ui.join();
    }
}
