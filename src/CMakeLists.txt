set(SOMOS_INCLUDE_DIRECTORIES
        application
        window
        sms
        )

add_subdirectory(window)
add_subdirectory(application)
add_subdirectory(sms)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${SOMOS_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} PUBLIC window application sms)

install(TARGETS ${PROJECT_NAME} DESTINATION ${SOMOS_INSTALL_BIN_DIR})
