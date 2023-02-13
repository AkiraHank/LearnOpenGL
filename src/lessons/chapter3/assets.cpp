#include "callbacks.h"
#include "myShader.h"
#include "IndicesBuffer.h"
#include "VertexBuffer.h"
#include "model.h"
#include "lights.h"
#include <iostream>

void drawAssets() {
  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile shaders
  // -------------------------
  Shader ourShader("resources/shaders/chapter3/loadObject.vs", "resources/shaders/chapter3/loadObject.fs");
  ourShader.compile();
  // load models
  // -----------

  // MODELS' NAME
  /// nanosuit.obj
  /// Alhaitham.pmx
  /// yaoyao.pmx
  /// Shogun.pmx
  /// Klee.pmx
  Model ourModel("resources/objects/Shogun/Shogun.pmx");

  // init lights
  PointLight pointLight1("resources/shaders/lights/pointLight.vs", "resources/shaders/lights/pointLight.fs");
  pointLight1.init();

  // draw in wireframe
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    // per-frame time logic
    // --------------------
    glm::vec3 lightPosition(5.8f, 13.8f, 3.8f);
    lightPosition.x = cos(glfwGetTime()) * 10.0f;
    lightPosition.z = sin(glfwGetTime()) * 10.0f;
    // lightPosition.z = 10.0f + cos(glfwGetTime()) * 20.0f;
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // don't forget to enable shader before setting uniforms
    ourShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(
        glm::radians(camera->Zoom),
        (float)windowWidth / (float)windowHeight,
        0.1f,
        100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));     // it's a bit too big for our scene, so scale it down
    ourShader.setMat4("model", model);
    ourShader.setVec3("pointLightPosition", lightPosition);
    ourShader.setVec3("viewPos", camera->Position);
    ourModel.Draw(ourShader);

    // draw lights
    pointLight1.setMVP(glm::translate(model, lightPosition), view, projection);
    pointLight1.setShiness(32.0);
    pointLight1.draw();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
}