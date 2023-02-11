#include "callbacks.h"
#include "myShader.h"
#include "IndicesBuffer.h"
#include "VertexBuffer.h"

void drawLightsTest() {
  glEnable(GL_DEPTH_TEST);
  std::vector<GLfloat> vertices = loadVertices("resources/vertices/textureLight");

  GLuint VBO, cubeVAO, lightCubeVAO, EBO;
  // vertex buffer for both
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(GLfloat),
               &vertices[0],
               GL_STATIC_DRAW);

  // VAO for cube
  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);
  // VBO already bound, so no need to bind VBO here
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // index buffer for cube
  // glGenBuffers(1, &EBO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // ============================================================================
  // VAO for lightCube
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  // ----------------- sphere -----------------
  /*2-计算球体顶点*/
  const GLfloat PI = 3.14159265358979323846f;
  // 将球横纵划分成50*50的网格
  const int Y_SEGMENTS = 50;
  const int X_SEGMENTS = 50;
  std::vector<float> sphereVertices;
  std::vector<int> sphereIndices;
  sphereVertices.reserve(3 * Y_SEGMENTS * X_SEGMENTS);
  sphereIndices.reserve(6 * Y_SEGMENTS * X_SEGMENTS);
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

  GLuint lightSphereVBO, lightSphereEBO;
  glGenBuffers(1, &lightSphereVBO);
  glBindBuffer(GL_ARRAY_BUFFER, lightSphereVBO);
  glBufferData(GL_ARRAY_BUFFER,
               sphereVertices.size() * sizeof(GLfloat),
               &sphereVertices[0],
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glGenBuffers(1, &lightSphereEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightSphereEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

  // ----------------- sphere -----------------

  // TODO: supposed to bind VBO here but actually doesn't have to. why?
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  // glEnableVertexAttribArray(0);

  GLuint diffuseMap, specularMap;
  std::string imgPath1 = "resources/textures/container2.png";
  if (!loadImg(imgPath1.c_str(), &diffuseMap)) {
    return;
  }
  std::string imgPath2 = "resources/textures/container2_specular.png";
  if (!loadImg(imgPath2.c_str(), &specularMap)) {
    return;
  }

  Shader lightingShader("resources/shaders/chapter2/light.vs", "resources/shaders/chapter2/light.fs");
  Shader lightCubeShader("resources/shaders/chapter2/lightCube.vs", "resources/shaders/chapter2/lightCube.fs");
  lightCubeShader.compile();
  lightingShader.compile();

  lightingShader.use();
  // light properties
  // point light
  glm::vec3 pointLightPositions[] = {
      glm::vec3(-7.7f, 0.2f, 2.0f),
      glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f),
      glm::vec3(3.0f, 9.0f, -3.0f)};
  for (int i = 0; i < 4; ++i) {
    lightingShader.setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.2f, 0.2f, 0.2f);
    lightingShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", 0.5f, 0.5f, 0.5f);
    lightingShader.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
    lightingShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
    lightingShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
    lightingShader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
  }
  // direct light
  lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
  lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
  lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
  lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
  // material properties
  lightingShader.setInt("material.diffuse", 0);
  lightingShader.setInt("material.specular", 1);
  lightingShader.setFloat("material.shininess", 32.0f);

  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);
    // clear
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightingShader.use();
    // lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("viewPos", camera->Position);

    glm::mat4 projection = glm::perspective(
        glm::radians(camera->Zoom),
        (float)windowWidth / (float)windowHeight,
        0.1f,
        100.0f);
    lightingShader.setMat4("projection", projection);
    glm::mat4 view = camera->GetViewMatrix();
    lightingShader.setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glBindVertexArray(cubeVAO);
    glDisable(GL_CULL_FACE);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};
    for (int i = 0; i < 10; i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      lightingShader.setMat4("model", model);
      glm::mat3 model_it = glm::mat3(glm::transpose(glm::inverse(model)));
      lightingShader.setMat3("model_it", model_it);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    // glm::mat4 model = glm::mat4(1.0f);
    // lightingShader.setMat4("model", model);
    // glDrawArrays(GL_TRIANGLES, 0, 36);

    // light sphere
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);
    for (int i = 0; i < 4; ++i) {
      model = glm::mat4(1.0f);
      model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
      model = glm::translate(model, pointLightPositions[i]);
      lightCubeShader.setMat4("model", model);
      glBindVertexArray(lightCubeVAO);
      glEnable(GL_CULL_FACE);
      glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
      // glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  lightCubeShader.clean();
  glBindVertexArray(0);

  glfwTerminate();
}