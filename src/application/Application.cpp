//
// Created by pedro on 29/03/23.
//

#include "Application.h"

#include <utility>

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>
#include <imgui.h>

Application::Application(std::string title) {
    // Create SDL Window
    m_window = std::make_shared<Window>(
            Window::Settings{std::move(title)}
    );
}

Application::~Application() = default;

void Application::imgui_init() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io{ImGui::GetIO()};

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Enable docking
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Get proper display scaling for high DPI displays
    const float font_scaling_factor{m_window->get_scale()};

    // Font size will be 18pt
    const float font_size{18.0F * font_scaling_factor};

    // Load font and set as default with proper scaling
    io.Fonts->AddFontFromFileTTF(
            "fonts/Montserrat-Regular.ttf",
            font_size
    );
    io.FontDefault = io.Fonts->AddFontFromFileTTF(
            "fonts/Montserrat-Regular.ttf",
            font_size
    );
    io.FontGlobalScale = 1.0F / font_scaling_factor;

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(
            m_window->get_native_window(),
            m_window->get_native_renderer()
    );
    ImGui_ImplSDLRenderer_Init(
            m_window->get_native_renderer()
    );

}

void Application::run() {
    imgui_init();

    m_running = true;
    while (m_running) {
        poll_events();

        m_window->clear();

        gui_start_frame();
        gui_draw();
        gui_render();

        m_window->render();
    }
}

void Application::poll_events() {
    bool events_pending{true};

    while (events_pending) {
        WindowEvent event = m_window->poll_event();

        // Process imgui events
        if (event.pending) {
            if (event.type == WindowEventType::EXIT) {
                stop();
            }

            ImGui_ImplSDL2_ProcessEvent(&event.sdl_event);
            if (
                    event.sdl_event.type == SDL_WINDOWEVENT &&
                    event.sdl_event.window.windowID ==
                    SDL_GetWindowID(m_window->get_native_window())
                    ) {
                on_event(event.sdl_event.window);
            }
        }

        events_pending = event.pending;
    }
}

void Application::stop() {
    m_running = false;
}

void Application::on_minimize() {
    m_minimized = true;
}

void Application::on_shown() {
    m_minimized = false;
}

void Application::on_close() {
    stop();
}

void Application::on_event(const SDL_WindowEvent &event) {
    switch (event.event) {
        case SDL_WINDOWEVENT_CLOSE:
            return on_close();
        case SDL_WINDOWEVENT_MINIMIZED:
            return on_minimize();
        case SDL_WINDOWEVENT_SHOWN:
            return on_shown();
    }
}

void Application::gui_start_frame() {
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void Application::gui_draw() {
    if (!m_minimized) {
        ImGui::DockSpaceOverViewport();

        draw_menu_bar();

        ImGui::ShowDemoWindow();
    }
}

void Application::draw_menu_bar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Load Rom")) {
            }
            if (ImGui::MenuItem("Exit")) {
                stop();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Application::gui_render() {
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}
