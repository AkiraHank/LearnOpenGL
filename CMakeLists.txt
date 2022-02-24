cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

project(OpenGL_test VERSION 0.1.0)

aux_source_directory(src SRC)
aux_source_directory(3rdparty/glad gladSRC)

link_directories(3rdparty/glfw-3.3.5 )
include_directories(3rdparty include)

add_executable(OpenGL_test ${SRC} ${gladSRC})

target_link_libraries(OpenGL_test -lglfw3 -lpthread "-framework Cocoa" "-framework IOKit")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
