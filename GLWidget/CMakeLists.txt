set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets OpenGL)
file(GLOB_RECURSE GLWidget *.cpp *.h)
add_library(GLWidgetLib ${GLWidget})
target_link_libraries(
    GLWidgetLib Qt6::Widgets Qt6::OpenGLWidgets
)