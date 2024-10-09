set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Widgets) 
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/GLService)
include_directories(${PROJECT_SOURCE_DIR}/utils ./GPU ./Drawables)
add_subdirectory(./GPU)
add_subdirectory(./Drawables)
file(GLOB_RECURSE Canvas *.cpp *.h)
add_library(CanvasLib ${Canvas})
target_link_libraries(
    CanvasLib Qt6::Widgets UtilsLib GPULib DrawablesLib
)

