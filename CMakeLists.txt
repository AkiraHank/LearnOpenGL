cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    message(STATUS "OS: MacOS")
    set(CMAKE_C_COMPILER /usr/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
    
    link_directories(bin/mac-arm)
elseif(WIN32)
    message(STATUS "OS: Windows")
    set(CMAKE_FORCE_C_COMPILER gcc)
    set(CMAKE_FORCE_CXX_COMPILER g++)

    link_directories(bin/win) 
endif()    

project(OpenGL_test VERSION 0.1.0)

file(GLOB_RECURSE SRC_DIR_LIST "src/*.cpp" "src/*.c" "src/*.cc")
 
include_directories(3rdparty include)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
else()
    message(STATUS "OPENGL found!")
endif()

add_executable(OpenGL_test ${SRC_DIR_LIST})
target_link_libraries(OpenGL_test glfw3)

if(APPLE)
    target_link_libraries(OpenGL_test "-framework Cocoa" "-framework IOKit" -lpthread)
elseif(WIN32)
    target_link_libraries(OpenGL_test ${OPENGL_LIBRARIES})
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
