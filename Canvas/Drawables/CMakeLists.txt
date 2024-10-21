set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Core Gui)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/utils ../GPU)
file(GLOB_RECURSE Drawables *.cpp *.h)
add_library(DrawablesLib ${Drawables})
target_link_libraries(
    DrawablesLib Qt6::Core Qt6::Gui UtilsLib GPULib
)
