cmake_minimum_required(VERSION 2.8)

# find opencv
find_package(OpenCV REQUIRED)
# compilation flags
set(CMAKE_CXX_FLAGS "-Wall -g -O2 -std=c++11")

# or the automatic version
file(GLOB_RECURSE SRC_FILES *.cpp)

# for each sample file, make an exe
foreach(SRC_FILE ${SRC_FILES})
    get_filename_component(FILE ${SRC_FILE} NAME_WE)
    add_executable(${FILE} ${SRC_FILE})
    target_link_libraries(${FILE} ${OpenCV_LIBS})
endforeach()

