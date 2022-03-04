cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
    message(STATUS "OS: MacOS")
    set(CMAKE_C_COMPILER /usr/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
    
    link_directories(3rdparty/glfw-3.3.5/mac-arm)
elseif(WIN32)
    message(STATUS "OS: Windows")
    set(CMAKE_FORCE_C_COMPILER gcc)
    set(CMAKE_FORCE_CXX_COMPILER g++)

    link_directories(3rdparty/glfw-3.3.5/win64) 
endif()    

project(OpenGL_test VERSION 0.1.0)

aux_source_directory(src SRC)
aux_source_directory(3rdparty/glad gladSRC)
 
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

add_executable(OpenGL_test ${SRC} ${gladSRC})
target_link_libraries(OpenGL_test glfw3)

if(APPLE)
    target_link_libraries(OpenGL_test "-framework Cocoa" "-framework IOKit" -lpthread)
elseif(WIN32)
    target_link_libraries(OpenGL_test ${OPENGL_LIBRARIES})
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
