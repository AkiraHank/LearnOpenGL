#include "callbacks.h"
#include "myShader.h"
#include "IndicesBuffer.h"
#include "VertexBuffer.h"
#include "model.h"
#include "lights.h"
#include <iostream>

void stencilTest() {
}

void depthAndStencilTest() {
  glEnable(GL_DEPTH_TEST);
  std::vector<GLfloat> vertices = loadVertices("resources/vertices/textureLight");

  GLuint cubeVBO, cubeVAO, lightCubeVAO, EBO;
  // vertex buffer for both
  glGenBuffers(1, &cubeVBO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER,
               vertices.size() * sizeof(GLfloat),
               &vertices[0],
               GL_STATIC_DRAW);

  // VAO for cube
  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);
  // VBO already bound, so no need to bind VBO here
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  std::vector<GLfloat> planeVertices = loadVertices("resources/vertices/planeVertices");
  unsigned int planeVAO, planeVBO;
  glGenVertexArrays(1, &planeVAO);
  glGenBuffers(1, &planeVBO);
  glBindVertexArray(planeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  glBufferData(
      GL_ARRAY_BUFFER,
      planeVertices.size() * sizeof(GLfloat),
      &planeVertices[0],
      GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glBindVertexArray(0);

  GLuint cubeTexture, floorTexture;
  std::string imgPath1 = "resources/textures/container2.png";
  if (!loadImg(imgPath1.c_str(), &cubeTexture)) {
    return;
  }
  std::string imgPath2 = "resources/textures/wall.jpg";
  if (!loadImg(imgPath2.c_str(), &floorTexture)) {
    return;
  }

  Shader shader("resources/shaders/chapter4/depth.vs", "resources/shaders/chapter4/depth.fs");
  shader.compile();
  shader.use();
  shader.setInt("texture1", 0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = Camera::getInstance().GetViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(Camera::getInstance().Zoom),
        (float)windowWidth / (float)windowHeight,
        0.1f,
        100.0f);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    // cubes
    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // floor
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    shader.setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &planeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &planeVBO);

  glfwTerminate();
}