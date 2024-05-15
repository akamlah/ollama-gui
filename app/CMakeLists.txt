cmake_minimum_required(VERSION 3.16)

project(ollamagui VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)
qt_standard_project_setup()

qt_add_executable(ollamagui
    main.cpp
    OllamaGui.cpp
)

target_link_libraries(ollamagui PRIVATE Qt6::Widgets)
target_link_libraries(ollamagui PRIVATE Qt6::Network)

# one comand:
# g++ $(pkg-config --cflags --libs Qt6Widgets) -fPIC -o hello ../app/main.cpp