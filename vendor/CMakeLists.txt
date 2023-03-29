include(FetchContent)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
        GIT_TAG release-2.0.22
)
add_subdirectory(sdl)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
        # docking-latest
        GIT_TAG cb8ead1f7198924f97e52973d115e1d4eaeda2f3
)
add_subdirectory(imgui)