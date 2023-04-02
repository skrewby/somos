//
// Created by pedro on 29/03/23.
//

#include "Window.h"

Window::Window(const Window::Settings &settings) {
    unsigned int init_flags{
            SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER
    };

    if (SDL_Init(init_flags) != 0) {
        // TODO: Better error handling
        return;
    }

    auto window_flags{
            static_cast<SDL_WindowFlags>(
                    SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
            )
    };
    constexpr int window_center_flag{SDL_WINDOWPOS_CENTERED};

    window = SDL_CreateWindow(
            settings.title.c_str(),
            window_center_flag,
            window_center_flag,
            settings.width,
            settings.height,
            window_flags
    );

    auto renderer_flags{
            static_cast<SDL_RendererFlags>(
                    SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
            )
    };
    renderer = SDL_CreateRenderer(
            window, -1, renderer_flags
    );

    if (renderer == nullptr) {
        // TODO: Better Error Handling
        return;
    }

    // Set render scale for high DPI displays
    const float scale{get_scale()};
    SDL_RenderSetScale(renderer, scale, scale);
}

Window::~Window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

float Window::get_scale() const {
    int window_width{0};
    int window_height{0};
    SDL_GetWindowSize(
            window,
            &window_width, &window_height
    );

    int render_output_width{0};
    int render_output_height{0};
    SDL_GetRendererOutputSize(
            renderer,
            &render_output_width, &render_output_height
    );

    const auto scale_x{
            static_cast<float>(render_output_width) /
            static_cast<float>(window_width)
    };

    return scale_x;
}

static WindowEventType get_event_type(const SDL_Event &event) {
    WindowEventType type;

    switch (event.type) {
        case SDL_QUIT:
            type = WindowEventType::EXIT;
            break;
        default:
            type = NO_EVENT;
            break;
    }

    return type;
}

WindowEvent Window::poll_event() {
    SDL_Event event{};

    bool pending = SDL_PollEvent(&event) == 1;
    WindowEventType type = pending ? get_event_type(event) : NO_EVENT;

    return {pending, type, event};
}

SDL_Window *Window::get_native_window() const {
    return window;
}

SDL_Renderer *Window::get_native_renderer() const {
    return renderer;
}

void Window::clear() {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderClear(renderer);
}

void Window::render() {
    SDL_RenderPresent(renderer);
}
