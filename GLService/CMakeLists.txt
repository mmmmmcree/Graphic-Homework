set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets OpenGL)
file(GLOB_RECURSE GLService *.cpp *.h)
add_library(GLServiceLib ${GLService})
target_link_libraries(
    GLServiceLib Qt6::Widgets Qt6::OpenGLWidgets
)