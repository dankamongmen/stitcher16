cmake_minimum_required(VERSION 3.13)
project(Stitcher16 VERSION 0.0.1
    DESCRIPTION "Driver for 16-bit OpenCV stitching"
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
find_package(OpenCV 4.1.2 REQUIRED COMPONENTS core imgcodecs stitching highgui)
pkg_check_modules(TIFF REQUIRED libtiff-4)

#SET(CMAKE_EXE_LINKER_FLAGS "-static")

# simple method ultimately proved insufficient for our needs
#file(GLOB SBINSRCS CONFIGURE_DEPENDS src/simple.cpp)
#add_executable(stitcher16 ${SBINSRCS})
#target_include_directories(stitcher16 PRIVATE ${OpenCV_INCLUDE_DIRS} ${TIFF_INCLUDE_DIRS})
#target_link_libraries(stitcher16 ${OpenCV_LIBS})
#target_compile_options(stitcher16 PRIVATE
#    -Wall -Wextra
#)

file(GLOB DBINSRCS CONFIGURE_DEPENDS src/detailed.cpp)
add_executable(detailed16 ${DBINSRCS})
target_include_directories(detailed16 PRIVATE ${OpenCV_INCLUDE_DIRS} ${TIFF_INCLUDE_DIRS})
target_link_libraries(detailed16 ${OpenCV_LIBS})
target_compile_options(detailed16 PRIVATE
    -Wall -Wextra
)

file(GLOB TBINSRCS CONFIGURE_DEPENDS src/tiff.cpp)
add_executable(tiff16 ${TBINSRCS})
target_include_directories(tiff16 PRIVATE ${OpenCV_INCLUDE_DIRS} ${TIFF_INCLUDE_DIRS})
target_link_libraries(tiff16 ${OpenCV_LIBS})
target_compile_options(tiff16 PRIVATE
    -Wall -Wextra
)
