//
// Created by pedro on 29/03/23.
//

#include "Application.h"

#include <utility>
#include <iostream>
#include <fstream>

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>
#include <imgui.h>
#include "imgui_memory_editor.h"
#include "nfd.h"

Application::Application(std::string title) {
    // Create SDL Window
    m_window = std::make_shared<Window>(
            Window::Settings{std::move(title)}
    );
    NFD_Init();
}

Application::~Application() {
    NFD_Quit();
}

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

        draw_debug_windows();
    }
}

void Application::draw_menu_bar() {
    if (ImGui::BeginMainMenuBar()) {
        draw_menu_file();
        draw_menu_debug();

        ImGui::EndMainMenuBar();
    }
}

void Application::draw_menu_file() {
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Load Rom")) {
            load_rom();
        }
        if (ImGui::MenuItem("Exit")) {
            stop();
        }
        ImGui::EndMenu();
    }
}


void Application::draw_menu_debug() {
    if (ImGui::BeginMenu("Debug")) {
        ImGui::Checkbox("Cartridge Viewer", &m_show_cart_memory_viewer);

        ImGui::EndMenu();
    }
}

void Application::draw_debug_windows() {
    if (m_show_cart_memory_viewer) {
        draw_cart_mem_viewer();
    }
}

void Application::draw_cart_mem_viewer() {
    ImGui::Begin("Cartridge", &m_show_cart_memory_viewer);
    if(m_sms.cart_loaded()) {
        static MemoryEditor mem_edit;
        auto cart = m_sms.dump_cartridge_data();
        mem_edit.DrawWindow("Cartridge", &cart[0], cart.size());
    } else {
        ImGui::TextWrapped("NO CART LOADED");
    }
    ImGui::End();
}


void Application::gui_render() {
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

std::string Application::choose_rom_file() {
    // TODO: Check Window compatibility (nfd.h uses wchar_t when on Windows
    std::string path;

    nfdchar_t* outPath;
    nfdfilteritem_t filterItem[1] = {{"SMS Roms", "sms"}};
    // TODO: SDL Window doesn't respond when Dialog is open and Abort option is given
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, nullptr);
    if (result == NFD_OKAY) {
        path = std::string(outPath);
        NFD_FreePath(outPath);
    }

    return path;
}

std::vector<uint8_t> Application::read_rom_file(const std::string &path) {
    // TODO: Error handling
    std::ifstream f{path, std::ios::binary};
    std::vector<uint8_t> rom{std::istreambuf_iterator<char>{f}, {}};

    return rom;
}

void Application::load_rom() {
    auto path = choose_rom_file();
    if (!path.empty()) {
        m_sms.load_cartridge(read_rom_file(path));
    }
}
