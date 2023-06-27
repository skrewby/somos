set(SOMOS_INCLUDE_DIRECTORIES
  ${PROJECT_SOURCE_DIR}/src/application
  ${PROJECT_SOURCE_DIR}/src/window
  ${PROJECT_SOURCE_DIR}/src/sms
)

set(SOMOS_TEST_FILES
  SMSTest.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(somos_tests ${SOMOS_TEST_FILES})
target_include_directories(somos_tests PRIVATE ${SOMOS_INCLUDE_DIRECTORIES})
target_link_libraries(somos_tests PUBLIC window application sms GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(somos_tests)

# install(TARGETS ${PROJECT_NAME} DESTINATION ${SOMOS_INSTALL_BIN_DIR})
