set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Core) 
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/utils)
file(GLOB_RECURSE GPU *.cpp *.h)
add_library(GPULib ${GPU})
target_link_libraries(
    GPULib  UtilsLib Qt6::Core
    ${PROJECT_SOURCE_DIR}/lib/SDL2.lib 
)
