//
// Created by 谭浩鸣 on 2021/11/30.
//

#ifndef OPENGL_TEST_CALLBACKS_H
#define OPENGL_TEST_CALLBACKS_H
#include "glad/glad.h"
#include "GLFW/glfw3.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 

//non-callbacks
void processInput(GLFWwindow *window);

//static data
extern const char *vertexShaderSource;
extern const char *fragmentShaderSource;
extern const char *fragmentShaderSource_yellow;

#endif //OPENGL_TEST_CALLBACKS_H
