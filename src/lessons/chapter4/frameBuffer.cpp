#include "callbacks.h"
#include "myShader.h"
#include "IndicesBuffer.h"
#include "VertexBuffer.h"
#include "model.h"
#include "lights.h"
#include "map"
#include <iostream>

void frameBuffer() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  std::vector<GLfloat> vertices = loadVertices("resources/vertices/cubeVertices");
  GLuint cubeVBO, cubeVAO, lightCubeVAO, EBO;
  // VAO for cube
  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);
  // vertex buffer for cube
  VertexBuffer cubeVB(&vertices[0], vertices.size() * sizeof(GLfloat));
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  std::vector<GLfloat> planeVertices = loadVertices("resources/vertices/planeVertices");
  unsigned int planeVAO, planeVBO;
  glGenVertexArrays(1, &planeVAO);
  glBindVertexArray(planeVAO);
  VertexBuffer planeVB(&planeVertices[0], planeVertices.size() * sizeof(GLfloat));
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  std::vector<GLfloat> transparentVertices = loadVertices("resources/vertices/grass");
  unsigned int grassVAO, grassVBO;
  glGenVertexArrays(1, &grassVAO);
  glBindVertexArray(grassVAO);
  VertexBuffer grassVB(&transparentVertices[0], transparentVertices.size() * sizeof(GLfloat));
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  // frame buffer related
  unsigned int fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  // 生成纹理
  unsigned int texColorBuffer;
  glGenTextures(1, &texColorBuffer);
  glBindTexture(GL_TEXTURE_2D, texColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  // 将它附加到当前绑定的帧缓冲对象
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
  // 渲染缓冲对象
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  // 接下来，作为完成帧缓冲之前的最后一步，我们将渲染缓冲对象附加到帧缓冲的深度和模板附件上：
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  GLuint cubeTexture, floorTexture, grassTexture;
  std::string imgPath1 = "resources/textures/container.jpg";
  if (!loadImg(imgPath1.c_str(), &cubeTexture)) {
    return;
  }
  std::string imgPath2 = "resources/textures/wall.jpg";
  if (!loadImg(imgPath2.c_str(), &floorTexture)) {
    return;
  }
  std::string imgPath3 = "resources/textures/blending_transparent_window.png";
  if (!loadImg(imgPath3.c_str(), &grassTexture)) {
    return;
  }

  Shader shader("resources/shaders/chapter4/stencil_testing.vs", "resources/shaders/chapter4/stencil_testing.fs");
  // Shader shaderSingleColor(
  //     "resources/shaders/chapter4/stencil_testing.vs",
  //     "resources/shaders/chapter4/stencil_single_color.fs");
  // shaderSingleColor.compile();
  shader.compile();
  shader.use();
  shader.setInt("texture1", 0);
  // glEnable(GL_STENCIL_TEST);
  // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  std::vector<glm::vec3> vegetation{
      glm::vec3(-1.5f, 0.0f, -2.48f),
      glm::vec3(1.5f, 0.0f, 3.51f),
      glm::vec3(2.0f, 0.0f, 3.7f),
      glm::vec3(-2.3f, 0.0f, -2.3f),
      glm::vec3(2.5f, 0.0f, -2.6f)};

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // render
    // ------
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // floor
    // glStencilMask(0x00);
    shader.use();
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    shader.setMat4("model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // mvp matrixs
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = Camera::getInstance().GetViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(Camera::getInstance().Zoom),
        (float)windowWidth / (float)windowHeight,
        0.1f,
        100.0f);

    // first render pass: normal cubes, writing 1s to the stencil buffer
    // ---------------------------------------------------------------------------------------------------------------------
    // glStencilFunc(GL_ALWAYS, 1, 0xFF);
    // glStencilMask(0xFF);
    shader.use();

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
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

    // grass
    shader.use();
    glBindVertexArray(grassVAO);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    std::map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < vegetation.size(); i++) {
      float distance = glm::length(Camera::getInstance().Position - vegetation[i]);
      sorted[distance] = vegetation[i];
    }
    for (auto pos = sorted.rbegin(); pos != sorted.rend(); ++pos) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, pos->second);
      shader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);

    // second render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
    // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing
    // the objects' size differences, making it look like borders.
    // ---------------------------------------------------------------------------------------------------------------------
    // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // glStencilMask(0x00);
    // glDisable(GL_DEPTH_TEST);
    // shaderSingleColor.use();
    // shaderSingleColor.setMat4("view", view);
    // shaderSingleColor.setMat4("projection", projection);
    // float scale = 1.1f;

    // // borders
    // glBindVertexArray(cubeVAO);
    // model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    // model = glm::scale(model, glm::vec3(scale, scale, scale));
    // shaderSingleColor.setMat4("model", model);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    // model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(scale, scale, scale));
    // shaderSingleColor.setMat4("model", model);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    // glBindVertexArray(0);
    // glStencilMask(0xFF);
    // glStencilFunc(GL_ALWAYS, 0, 0xFF);
    /// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &planeVAO);
  glDeleteBuffers(1, &grassVAO);

  glfwTerminate();
}