//
// Created by 谭浩鸣 on 2021/11/30.
//

#ifndef OPENGL_TEST_CALLBACKS_H
#define OPENGL_TEST_CALLBACKS_H
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "Camera.h"
#include <iostream>

// global vars
extern GLFWwindow* window;
extern Camera* camera;
extern int windowWidth;
extern int windowHeight;

// callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void error_callback(int code, const char* msg);

// non-callbacks
void processInput(GLFWwindow* window);
void init();
// helpers

unsigned int loadImg(const char* path, unsigned int* tex_id);
unsigned int loadImg_clamp(const char* path, unsigned int* tex_id);

#endif // OPENGL_TEST_CALLBACKS_H
