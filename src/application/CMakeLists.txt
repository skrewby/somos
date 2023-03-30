set(LIBRARY_NAME application)
set(SOURCE_FILES
        Application.h
        Application.cpp
        )

add_library(${LIBRARY_NAME} SHARED STATIC ${SOURCE_FILES})
target_include_directories(${LIBRARY_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/window)
target_link_libraries(${LIBRARY_NAME} PUBLIC window)

install(TARGETS ${LIBRARY_NAME} DESTINATION ${SOMOS_INSTALL_LIB_DIR})
install(FILES Application.h DESTINATION ${SOMOS_INSTALL_INCLUDE_DIR})