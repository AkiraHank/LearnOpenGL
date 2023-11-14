#include "lights.h"

PointLight::PointLight(const std::string& vertexPath,
                       const std::string& fragPath)
    : myShader(std::make_unique<Shader>(vertexPath.c_str(), fragPath.c_str())) {
}

void PointLight::init() {
  myShader->compile();
  std::vector<float> sphereVertices;
  std::vector<int> sphereIndices;

  // ----------------- sphere -----------------
  /*2-计算球体顶点*/
  const GLfloat PI = 3.14159265358979323846f;
  // 将球横纵划分成50*50的网格

  // 生成球的顶点
  for (int y = 0; y <= Y_SEGMENTS; y++) {
    for (int x = 0; x <= X_SEGMENTS; x++) {
      float xSegment = (float)x / (float)X_SEGMENTS;
      float ySegment = (float)y / (float)Y_SEGMENTS;
      float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
      float yPos = std::cos(ySegment * PI);
      float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
      sphereVertices.push_back(xPos);
      sphereVertices.push_back(yPos);
      sphereVertices.push_back(zPos);
    }
  }

  // 生成球的Indices
  for (int i = 0; i < Y_SEGMENTS; i++) {
    for (int j = 0; j < X_SEGMENTS; j++) {
      sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
      sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
      sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
      sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
      sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
      sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
    }
  }
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  GLuint lightSphereVBO, lightSphereEBO;
  glGenBuffers(1, &lightSphereVBO);
  glBindBuffer(GL_ARRAY_BUFFER, lightSphereVBO);
  glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(GLfloat),
               &sphereVertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glGenBuffers(1, &lightSphereEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightSphereEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int),
               &sphereIndices[0], GL_STATIC_DRAW);
  // vertex and index buffer for lightCube
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // ----------------- sphere -----------------

  // TODO: supposed to bind VBO here but actually doesn't have to. why?
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
  // (void*)0); glEnableVertexAttribArray(0);
}

void PointLight::setMVP(const glm::mat4& model, const glm::mat4& view,
                        const glm::mat4& projection) {
  myShader->use();
  myShader->setMat4("model", model);
  myShader->setMat4("view", view);
  myShader->setMat4("projection", projection);
}

void PointLight::setShiness(float shiness) {
  myShader->use();
  myShader->setFloat("lightShiness", shiness);
}

void PointLight::draw() {
  myShader->use();
  glBindVertexArray(lightVAO);
  glEnable(GL_CULL_FACE);
  glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
  glDisable(GL_CULL_FACE);
}