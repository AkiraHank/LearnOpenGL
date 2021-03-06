//
// Created by 谭浩鸣 on 2021/11/30.
//

#ifndef OPENGL_TEST_CALLBACKS_H
#define OPENGL_TEST_CALLBACKS_H
#include "glad/glad.h"
#include "glfw-3.3.5/include/GLFW/glfw3.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//non-callbacks
void processInput(GLFWwindow *window);

//helpers

unsigned int loadImg(const char* path, unsigned int* tex_id);
unsigned int loadImg_clamp(const char* path, unsigned int* tex_id);
 

#endif //OPENGL_TEST_CALLBACKS_H
