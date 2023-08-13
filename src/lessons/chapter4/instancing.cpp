#include "callbacks.h"
#include "myShader.h"
#include "IndicesBuffer.h"
#include "VertexBuffer.h"
#include "model.h"
#include "lights.h"
#include "map"
#include <iostream>

void instancing() {
  Shader shader("resources/shaders/chapter4/instancing.vs", "resources/shaders/chapter4/instancing.fs");
  shader.compile();

  glm::vec2 translations[100];
  int index = 0;
  float offset = 0.1f;
  for (int y = -10; y < 10; y += 2) {
    for (int x = -10; x < 10; x += 2) {
      glm::vec2 translation;
      translation.x = (float)x / 10.0f + offset;
      translation.y = (float)y / 10.0f + offset;
      translations[index++] = translation;
    }
  }

  std::vector<GLfloat> vertices = loadVertices("resources/vertices/instancing");
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  VertexBuffer squareVBO(&vertices[0], vertices.size() * sizeof(GLfloat));
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  VertexBuffer offsetVBO(&translations[0], 100 * sizeof(glm::vec2));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw squares
    shader.use();
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each

    // mvp matrixs
    // glm::mat4 model = glm::mat4(1.0f);
    // glm::mat4 view = Camera::getInstance().GetViewMatrix();
    // glm::mat4 projection = glm::perspective(
    //     glm::radians(Camera::getInstance().Zoom),
    //     (float)windowWidth / (float)windowHeight,
    //     0.1f,
    //     100.0f);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);

  glfwTerminate();
}