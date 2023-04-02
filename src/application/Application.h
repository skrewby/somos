/**
 * APPLICATION
 *      The entry point for the program's main business logic
 */

#ifndef SOMOS_APPLICATION_H
#define SOMOS_APPLICATION_H

#include <memory>

#include "Window.h"

class Application {
public:
    explicit Application(std::string title);
    Application() = delete;
    ~Application();

    void run();
private:
    std::shared_ptr<Window> m_window{};

    // Flow control variables
    bool m_running{false};
    bool m_minimized{false};

    // Program flow control functions
    void stop();
    void on_minimize();
    void on_shown();
    void on_close();

    // GUI
    void imgui_init();
    void draw_menu_bar();
    void on_event(const SDL_WindowEvent &event);
    void gui_start_frame();
    void gui_draw();
    void gui_render();

    void poll_events();
};


#endif //SOMOS_APPLICATION_H
