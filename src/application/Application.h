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
    std::shared_ptr<Window> window{};
    bool running{false};

    void poll_events();
};


#endif //SOMOS_APPLICATION_H
