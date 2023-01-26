#ifndef MYSHADER_H
#define MYSHADER_H

#include "glad/glad.h" // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
  public:
  // 程序ID
  unsigned int ID;
  // 构造器读取并构建着色器
  Shader() = delete;
  Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath);
  ~Shader() {
    glDeleteProgram(ID);
  }
  void init();
  // 使用/激活程序
  void use();
  // uniform工具函数
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setMat4(const std::string& name, const glm::mat4&);

  void clean();

  private:
  std::string vertexShaderCode;
  std::string fragShaderCode;
};

#endif