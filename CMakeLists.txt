cmake_minimum_required(VERSION 3.13)
project(Stitcher16 VERSION 0.0.1
    DESCRIPTION "Driver for 16-bit OpenCV stitching"
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV 4.1.2 REQUIRED COMPONENTS core imgcodecs stitching)

file(GLOB BINSRCS CONFIGURE_DEPENDS src/*.cpp)
add_executable(stitcher16 ${BINSRCS})
target_include_directories(stitcher16 PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(stitcher16 ${OpenCV_LIBS})
target_compile_options(stitcher16 PRIVATE
    -Wall -Wextra
)
