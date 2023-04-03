/**
 * APPLICATION
 *      The entry point for the program's main business logic
 */

#ifndef SOMOS_APPLICATION_H
#define SOMOS_APPLICATION_H

#include <memory>
#include <vector>

#include "Window.h"
#include "SMS.h"

class Application {
public:
    explicit Application(std::string title);
    Application() = delete;
    ~Application();

    void run();
private:
    std::shared_ptr<Window> m_window{};

    // The emulator itself
    SMS m_sms;

    // Flow control variables
    bool m_running{false};
    bool m_minimized{false};

    // GUI variables
    bool m_show_cart_memory_viewer{false};
    bool m_show_about_window{false};

    // Program flow control functions
    void stop();
    void on_minimize();
    void on_shown();
    void on_close();

    // ------------------------------------------  GUI  ------------------------------------------
    void imgui_init();

    // MENU
    void draw_menu_bar();
    void draw_menu_console();
    void draw_menu_debug();
    void draw_menu_options();

    // DEBUG
    void draw_debug_windows();
    void draw_cart_mem_viewer();

    // OPTIONS
    void draw_options_windows();
    void draw_about_window();

    void on_event(const SDL_WindowEvent &event);
    void gui_start_frame();
    void gui_draw();
    void gui_render();
    void poll_events();
    // ----------------------------------------  END GUI  ----------------------------------------

    void load_rom();
    std::string choose_rom_file();
    std::vector<uint8_t> read_rom_file(const std::string& path);
};


#endif //SOMOS_APPLICATION_H
