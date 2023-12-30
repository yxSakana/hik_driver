cmake_minimum_required(VERSION 3.25)
project(prj_hik_driver LANGUAGES CXX)

# C++ Version && C++ Include Directory Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/vision_debug_widgets)
add_subdirectory(src/google_logger)
add_subdirectory(src/HikDriver)

add_executable(prj_hik_driver src/main.cpp)

target_link_libraries(prj_hik_driver PUBLIC
    hik_driver
    hik_ui
    google_logger)
