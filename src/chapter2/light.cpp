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
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // index buffer for cube
  // glGenBuffers(1, &EBO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // ============================================================================
  // VAO for lightCube
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);
  // TODO: supposed to bind VBO here but actually doesn't have to. why?
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // vertex and index buffer for lightCube
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  Shader lightingShader("resources/shaders/chapter2/light.vs", "resources/shaders/chapter2/light.fs");
  Shader lightCubeShader("resources/shaders/chapter2/lightCube.vs", "resources/shaders/chapter2/lightCube.fs");
  lightCubeShader.compile();
  lightingShader.compile();

  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);
    // clear
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
    lightPos.y = 1.0f + cos(glfwGetTime()) * 2.0f;

    lightingShader.use();
    lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    // lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("lightPos", lightPos);
    lightingShader.setVec3("viewPos", camera->Position);
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f); // note that all light colors are set at full intensity
    lightingShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    lightingShader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
    lightingShader.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
    lightingShader.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);

    glm::mat4 projection = glm::perspective(
        glm::radians(camera->Zoom),
        (float)windowWidth / (float)windowHeight,
        0.1f,
        100.0f);
    lightingShader.setMat4("projection", projection);
    glm::mat4 view = camera->GetViewMatrix();
    lightingShader.setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    lightingShader.setMat4("model", model);
    glm::mat3 model_it = glm::mat3(glm::transpose(glm::inverse(model)));
    lightingShader.setMat3("model_it", model_it);

    glBindVertexArray(cubeVAO);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightCubeShader.setMat4("model", model);

    glBindVertexArray(lightCubeVAO);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  lightCubeShader.clean();
  glBindVertexArray(0);

  glfwTerminate();
}