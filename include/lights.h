#ifndef LIGHTS_H
#define LIGHTS_H
#include <vector>
#include <memory>
#include "myShader.h"
#include "glad/glad.h"

class PointLight {
public:
  PointLight() = delete;
  PointLight(const std::string& vertexPath, const std::string& fragPath);

  void init();
  void setMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
  void setShiness(float shiness);

  void draw();

private:
  std::unique_ptr<Shader> myShader;
  std::vector<GLfloat> vertices;
  std::vector<GLuint> indeces;

  GLuint lightVAO;
  const int Y_SEGMENTS = 50;
  const int X_SEGMENTS = 50;
};

class directLight {
};
#endif