/**
 * WINDOW
 *      Module responsible for window management and drawing through SDL
 */

#ifndef SOMOS_WINDOW_H
#define SOMOS_WINDOW_H

#include <string>
#include "SDL.h"

enum WindowEventType {
    EXIT,
    NO_EVENT,
};
struct WindowEvent {
    bool pending{false};
    WindowEventType type{NO_EVENT};
};

class Window {
public:
    struct Settings {
        std::string title;
        const int width{1280};
        const int height{720};
    };

    explicit Window(const Settings &settings);
    ~Window();

    WindowEvent poll_event();

    [[nodiscard]] float get_scale() const;

private:
    SDL_Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};
};


#endif //SOMOS_WINDOW_H
