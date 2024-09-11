cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(window LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "E:/Qt/6.6.3/msvc2019_64") # Qt Kit Dir
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 COMPONENTS Widgets REQUIRED) # Qt COMPONENTS
aux_source_directory(./src srcs)
include_directories(./include)
# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
file(GLOB copyResource "./lib/*.dll")
file(COPY ${copyResource} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ${copyResource} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

add_executable(${PROJECT_NAME}
    WIN32 # If you need a terminal for debug, please comment this statement 
    ${srcs} 
) 
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2.lib
) # Qt5 Shared Library