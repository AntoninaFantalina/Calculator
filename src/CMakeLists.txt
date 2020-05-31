set(APP_NAME CalculatorApp)

add_executable(${APP_NAME} main.cpp)
add_subdirectory(Calculator)

target_link_libraries(${APP_NAME} CalculatorLib)

if(WITH_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
