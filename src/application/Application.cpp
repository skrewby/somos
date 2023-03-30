//
// Created by pedro on 29/03/23.
//

#include "Application.h"

#include <utility>

Application::Application(std::string title) {
    // Create SDL Window
    window = std::make_shared<Window>(
            Window::Settings{std::move(title)}
    );
}

Application::~Application() = default;

void Application::run() {
    running = true;

    while(running) {
        poll_events();
    }
}

void Application::poll_events() {
    bool events_pending{true};

    while(events_pending) {
        WindowEvent event = window->poll_event();

        events_pending = event.pending;
        if(event.type == WindowEventType::EXIT) {
            running = false;
        }
    }
}
