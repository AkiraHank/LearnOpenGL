#include "myShader.h"
#include "callbacks.h"
// GLenum glCheckError_(const char* file, int line) {
//   GLenum errorCode;
//   while ((errorCode = glGetError()) != GL_NO_ERROR) {
//     std::string error;
//     switch (errorCode) {
//     case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
//     case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
//     case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
//     case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"; break;
//     case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"; break;
//     case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
//     case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
//     }
//     std::cout << error << " | " << file << " (" << line << ")" << std::endl;
//   }
//   return errorCode;
// }
// #define glCheckError() glCheckError_(__FILE__, __LINE__)

#define checkSetUniformError()                                     \
  auto ret = glGetError();                                         \
  if (ret) {                                                       \
    printf(RED "error %d in %s:%d" NONE, ret, __FILE__, __LINE__); \
    printf(RED "name of uniform: %s\n" NONE, name.c_str());        \
  }

#define checkShaderCompileError()                              \
  std::cout << RED "ERROR::SHADER::VERTEX::COMPILATION_FAILED" \
            << infoLog << NONE;

#define checkShaderLinkingError()                          \
  std::cout << RED "ERROR::SHADER::VERTEX::LINKING_FAILED" \
            << infoLog << NONE;

Shader::Shader(const char* vertexPath, const char* fragmentPath, const GLchar* geometryShaderPath, bool useFile) {
  if (!useFile) {
    this->vertexShaderCode = vertexPath;
    this->fragShaderCode = fragmentPath;
    this->geometryShaderCode = geometryShaderPath != nullptr ? geometryShaderPath : "";
    return;
  }
  // 1. 从文件路径中获取顶点/片段着色器
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  // 保证ifstream对象可以抛出异常：
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // 打开文件
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // 读取文件的缓冲内容到数据流中
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // 关闭文件处理器
    vShaderFile.close();
    fShaderFile.close();
    // 转换数据流到string
    this->vertexShaderCode = vShaderStream.str();
    this->fragShaderCode = fShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cout << RED "ERROR::SHADER::FILE_OT_SUCCESFULLY_READ" << e.what() << NONE;
  }

  if (geometryShaderPath != nullptr) {
    std::ifstream gShaderFile;
    // 保证ifstream对象可以抛出异常：
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      // 打开文件
      gShaderFile.open(geometryShaderPath);
      std::stringstream gShaderStream;
      // 读取文件的缓冲内容到数据流中
      gShaderStream << gShaderFile.rdbuf();
      // 关闭文件处理器
      gShaderFile.close();
      // 转换数据流到string
      this->geometryShaderCode = gShaderStream.str();
    } catch (std::ifstream::failure e) {
      std::cout << RED "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << NONE;
    }
  }
}

void Shader::compile() {
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // 顶点着色器
  vertex = glCreateShader(GL_VERTEX_SHADER);
  const char* vShader = this->vertexShaderCode.c_str();
  glShaderSource(vertex, 1, &vShader, NULL);
  glCompileShader(vertex);
  // 打印编译错误（如果有的话）
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    checkShaderCompileError();
  };

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fShader = this->fragShaderCode.c_str();
  glShaderSource(fragment, 1, &fShader, NULL);
  glCompileShader(fragment);
  // 打印编译错误（如果有的话）
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    checkShaderCompileError();
  };

  // 着色器程序
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  // 打印连接错误（如果有的话）
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    checkShaderLinkingError();
  }

  // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  // geometryShader
  if (this->geometryShaderCode != "") {
    // compile
    unsigned int geomerty;

    // 几何着色器
    geomerty = glCreateShader(GL_GEOMETRY_SHADER);
    const char* gShader = this->geometryShaderCode.c_str();
    glShaderSource(geomerty, 1, &gShader, NULL);
    glCompileShader(geomerty);
    // 打印编译错误（如果有的话）
    glGetShaderiv(geomerty, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(geomerty, 512, NULL, infoLog);
      checkShaderCompileError();
    };

    if (ID == -1) ID = glCreateProgram();
    glAttachShader(ID, geomerty);
    glLinkProgram(ID);
    // 打印连接错误（如果有的话）
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      checkShaderLinkingError();
    }

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(geomerty);
  }
}

void Shader::use() {
  glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  checkSetUniformError();
}

void Shader::setInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  checkSetUniformError();
}

void Shader::setFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  checkSetUniformError();
}
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const {
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  checkSetUniformError();
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  checkSetUniformError();
}

void Shader::setMat4(const std::string& name, const glm::mat4& trans) {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
  checkSetUniformError();
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  checkSetUniformError();
}
void Shader::setVec2(const std::string& name, float x, float y) const {
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  checkSetUniformError();
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  checkSetUniformError();
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  checkSetUniformError();
}

void Shader::clean() {
  glUseProgram(ID);
}