set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Core) 
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE utils *.cpp *.h)
add_library(UtilsLib ${utils})
target_link_libraries(UtilsLib Qt6::Core
)