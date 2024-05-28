cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

project(ollamagui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)
qt_standard_project_setup()

# list of all cpp files by component
file(GLOB GEN_SRC "${PROJECT_SOURCE_DIR}/OllamaGui/src/*.cpp")
file(GLOB UI_SRC "${PROJECT_SOURCE_DIR}/OllamaGui/src/ui/*.cpp")
file(GLOB API_SRC "${PROJECT_SOURCE_DIR}/OllamaGui/src/api/*.cpp")
set(SOURCES ${GEN_SRC} ${UI_SRC} ${API_SRC})

file(GLOB UI_QML "${PROJECT_SOURCE_DIR}/OllamaGui/src/ui/*.ui")

# list of all header files by component
file(GLOB API_HEADERS "${PROJECT_SOURCE_DIR}/OllamaGui/include/api/*.h")
file(GLOB UI_HEADERS "${PROJECT_SOURCE_DIR}/OllamaGui/include/ui/*.h")
set(HEADERS ${UI_HEADERS} ${API_HEADERS})
# set(HEADERS ${UI_HEADERS})

qt_add_executable(ollamagui
    MANUAL_FINALIZATION
    ${UI_QML}
    ${HEADERS}
    ${SOURCES}
)

# include sources
target_include_directories(ollamagui PRIVATE "${PROJECT_SOURCE_DIR}/OllamaGui/src/")
target_include_directories(ollamagui PRIVATE "${PROJECT_SOURCE_DIR}/OllamaGui/src/ui/")
target_include_directories(ollamagui PRIVATE "${PROJECT_SOURCE_DIR}/OllamaGui/src/api/")
# include headers
target_include_directories(ollamagui PUBLIC "${PROJECT_SOURCE_DIR}/OllamaGui/include/")

target_link_libraries(ollamagui PRIVATE Qt6::Widgets)
target_link_libraries(ollamagui PRIVATE Qt6::Network)
target_link_libraries(ollamagui PRIVATE Qt6::Gui)
target_link_libraries(ollamagui PRIVATE Qt6::Core5Compat)
# target_link_libraries(ollamagui PUBLIC ${API_HEADERS})

# include(GNUInstallDirs)
# install(TARGETS ollamagui
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(ollamagui)
# endif()
