# # # file(GLOB ui_src RELATIVE "ui" "*.cpp")
# # # file(GLOB ui "${PROJECT_SOURCE_DIR}/OllamaGui/src/ui/*.cpp")
# # # file(GLOB include_ui "${PROJECT_SOURCE_DIR}/OllamaGui/include/ui/*.h")

# # # message(STATUS ${ui})
# # # message(STATUS "-----------------------")

# # # message(STATUS ${include_ui})
# # # message(STATUS "-----------------------")
# # cmake_minimum_required(VERSION 3.16)

# # # set(CMAKE_AUTOMOC ON)
# # # set(CMAKE_AUTOUIC ON)
# # # set(CMAKE_AUTORCC ON)

# # project(ollamagui VERSION 1.0.0 LANGUAGES CXX)

# # set(CMAKE_CXX_STANDARD 17)
# # set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # find_package(Qt6 REQUIRED COMPONENTS Widgets)
# # find_package(Qt6 REQUIRED COMPONENTS Network)
# # find_package(Qt6 REQUIRED COMPONENTS Core)


# # qt_standard_project_setup()


# # # set (SOURCES main.cpp ${ui} ${include_ui})
# # set (SOURCES
# #     main.cpp
# #     Widget.cpp
# #     Widget.h
# # )

# # qt_wrap_cpp(${SOURCES})

# # add_executable(ollamagui
# #     ${SOURCES}
# # )

# # # target_sources(ollamagui
# # #     PRIVATE main.cpp ${ui}
# # #     PUBLIC ${include_ui}
# # # )

# # # message(STATUS ${SOURCES})
# # # message(STATUS "-----------------------")
# # # add_library("${PROJECT_SOURCE_DIR}/OllamaGui/include/")

# # target_link_libraries(ollamagui PRIVATE Qt6::Widgets)
# # target_link_libraries(ollamagui PRIVATE Qt6::Network)
# # target_link_libraries(ollamagui PRIVATE Qt6::Core)
# # target_include_directories(ollamagui PRIVATE "${PROJECT_SOURCE_DIR}/OllamaGui/src/")

# file(GLOB INC_SRC "${PROJECT_SOURCE_DIR}/OllamaGui/src/include/*.h")

# find_package(Qt6 REQUIRED COMPONENTS Widgets)
# find_package(Qt6 REQUIRED COMPONENTS Network)
# qt_standard_project_setup()

# qt_add_executable(ollamagui
#     main.cpp
#     Widget.cpp
#     ${INC_SRC}
#     # include/Widget.h
# )

# set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# target_link_libraries(ollamagui PRIVATE Qt6::Widgets)
# target_link_libraries(ollamagui PRIVATE Qt6::Network)
# target_include_directories(ollamagui PRIVATE "${PROJECT_SOURCE_DIR}/OllamaGui/src/include/")

# # one comand:
# # g++ $(pkg-config --cflags --libs Qt6Widgets) -fPIC -o hello ../app/main.cpp