cmake_minimum_required(VERSION 3.10) # CMake install : https://cmake.org/download/
project(window LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DSOURCE_DIR="${CMAKE_SOURCE_DIR}")
find_package(Qt6 COMPONENTS Widgets REQUIRED) # Qt COMPONENTS
aux_source_directory(./src srcs)
add_subdirectory(Canvas)
add_subdirectory(utils)
add_subdirectory(GLService)
include_directories(./include)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
file(GLOB copyResource "./lib/*.dll")
file(COPY ${copyResource} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ${copyResource} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
add_executable(${PROJECT_NAME}
    WIN32
    ${srcs}
    ./include/Ela/ElaWidgetTools.qrc
    res.qrc
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets CanvasLib 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/elawidgettools.lib
)