#include "myShader.h"
#include "IndicesBuffer.h"
#include "VertexBuffer.h"
#include "model.h"
#include "lights.h"
#include "map"
#include <iostream>

int geometryShader() {
  // Shader shader("resources/shaders/chapter4/geo.vs", "resources/shaders/chapter4/geo.fs", "resources/shaders/chapter4/geo.gs");
  // shader.compile();

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  std::vector<GLfloat> vertices = loadVertices("resources/vertices/geo");
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  VertexBuffer geoVB(&vertices[0], vertices.size() * sizeof(GLfloat));
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  Model nanosuit("resources/objects/nanosuit/nanosuit.obj");
  Shader ourShader("resources/shaders/chapter4/explode.vs", "resources/shaders/chapter4/explode.fs", "resources/shaders/chapter4/explode.gs");
  ourShader.compile();
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw points
    // shader.use();
    // glBindVertexArray(VAO);
    // glDrawArrays(GL_POINTS, 0, 4);

    // mvp matrixs
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = Camera::getInstance().GetViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(Camera::getInstance().Zoom),
        (float)windowWidth / (float)windowHeight,
        0.1f,
        100.0f);
    // draw model
    ourShader.use();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);
    ourShader.setMat4("model", model);

    // add time component to geometry shader in the form of a uniform
    ourShader.setFloat("time", static_cast<float>(glfwGetTime()));
    nanosuit.Draw(ourShader);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);

  glfwTerminate();
  return 0;
}