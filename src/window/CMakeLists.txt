set(LIBRARY_NAME window)
set(SOURCE_FILES
        Window.h
        Window.cpp
        )
add_library(${LIBRARY_NAME} SHARED STATIC ${SOURCE_FILES})
target_link_libraries(${LIBRARY_NAME} PUBLIC SDL2::SDL2)

install(TARGETS ${LIBRARY_NAME} DESTINATION ${SOMOS_INSTALL_LIB_DIR})
install(FILES Window.h DESTINATION ${SOMOS_INSTALL_INCLUDE_DIR})