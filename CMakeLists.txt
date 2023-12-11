cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
 

if(APPLE)
    message(STATUS "OS: MacOS")
    message(STATUS "Architecture: ${ARCHITECTURE}")
    set(CMAKE_C_COMPILER /usr/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
    link_directories(bin/mac-arm)
elseif(WIN32)
    message(STATUS "OS: Windows")
    message(STATUS "Architecture: ${ARCHITECTURE}")
    set(CMAKE_C_COMPILER E:/mingw64/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER E:/mingw64/bin/g++.exe)
    link_directories(bin/win)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
# if(CMAKE_COMPILER_IS_GNUCXX)
#     message("compiler is GNUCXX")
# else()
#     message("compiler is not GNUCXX")
# endif()

project(LearnOpengl VERSION 0.1.0)

file(GLOB_RECURSE SRC_DIR_LIST "src/*.cpp" "src/*.c" "src/*.cc")

include_directories(3rdparty include)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif()

add_executable(LearnOpengl ${SRC_DIR_LIST})
target_link_libraries(LearnOpengl glfw3 assimp)

if(APPLE)
    target_link_libraries(LearnOpengl "-framework Cocoa" "-framework IOKit" -lpthread)
elseif(WIN32)
    target_link_libraries(LearnOpengl ${OPENGL_LIBRARIES})
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
