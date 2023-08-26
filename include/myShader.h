#ifndef MYSHADER_H
#define MYSHADER_H

#include "glad/glad.h" // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "callbacks.h"

class Shader {
public:
  // 构造器读取并构建着色器
  Shader() = delete;
  Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath, const GLchar* geometryShaderPath = nullptr, bool useFile = true);
  ~Shader() {
    glDeleteProgram(ID);
  }
  void compile();
  // 使用/激活程序
  void use();
  // uniform工具函数
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setMat2(const std::string& name, const glm::mat2& mat) const;
  void setMat3(const std::string& name, const glm::mat3& mat) const;
  void setMat4(const std::string& name, const glm::mat4&);
  void setVec2(const std::string& name, const glm::vec2& value) const;
  void setVec2(const std::string& name, float x, float y) const;
  void setVec3(const std::string& name, const glm::vec3& value) const;
  void setVec3(const std::string& name, float x, float y, float z) const;
  void clean();

  unsigned int getID() {
    return this->ID;
  }

private:
  // 程序ID
  unsigned int ID = -1;
  std::string vertexShaderCode;
  std::string fragShaderCode;
  std::string geometryShaderCode;
};

#endif