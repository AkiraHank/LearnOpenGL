#include "callbacks.h"
#include "myShader.h"
#include "IndicesBuffer.h"
#include "VertexBuffer.h"
#include "model.h"
#include "lights.h"
#include "map"
#include <iostream>

void skybox() {
#pragma region // VAO & VBO
  // cube
  std::vector<GLfloat> vertices = loadVertices("resources/vertices/cubeVertices");
  GLuint cubeVAO, lightCubeVAO, EBO;
  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);
  VertexBuffer cubeVB(&vertices[0], vertices.size() * sizeof(GLfloat));
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // plane
  std::vector<GLfloat> planeVertices = loadVertices("resources/vertices/planeVertices");
  unsigned int planeVAO;
  glGenVertexArrays(1, &planeVAO);
  glBindVertexArray(planeVAO);
  VertexBuffer planeVB(&planeVertices[0], planeVertices.size() * sizeof(GLfloat));
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  // quad
  std::vector<GLfloat> quadVertices = loadVertices("resources/vertices/grass");
  unsigned int quadVAO;
  glGenVertexArrays(1, &quadVAO);
  glBindVertexArray(quadVAO);
  VertexBuffer quadVB(&quadVertices[0], quadVertices.size() * sizeof(GLfloat));
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  // skybox
  std::vector<GLfloat> skyboxVertices = loadVertices("resources/vertices/skybox");
  unsigned int skyboxVAO;
  glGenVertexArrays(1, &skyboxVAO);
  glBindVertexArray(skyboxVAO);
  VertexBuffer skyboxVB(&skyboxVertices[0], skyboxVertices.size() * sizeof(GLfloat));
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
#pragma endregion

#pragma region // frame buffer
  // frame buffer related
  unsigned int fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  // 生成纹理
  unsigned int texColorBuffer;
  glGenTextures(1, &texColorBuffer);
  glBindTexture(GL_TEXTURE_2D, texColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // 将它附加到当前绑定的帧缓冲对象
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
  // 渲染缓冲对象
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
  // 接下来，作为完成帧缓冲之前的最后一步，我们将渲染缓冲对象附加到帧缓冲的深度和模板附件上：
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

  GLuint cubeTexture, floorTexture;
  std::string imgPath1 = "resources/textures/container.jpg";
  if (!loadImg(imgPath1.c_str(), &cubeTexture)) {
    return;
  }
  std::string imgPath2 = "resources/textures/wall.jpg";
  if (!loadImg(imgPath2.c_str(), &floorTexture)) {
    return;
  }

  std::vector<std::string> faces = {
      "resources/textures/skybox/right.jpg",
      "resources/textures/skybox/left.jpg",
      "resources/textures/skybox/top.jpg",
      "resources/textures/skybox/bottom.jpg",
      "resources/textures/skybox/front.jpg",
      "resources/textures/skybox/back.jpg"};
  GLuint cubemapTexture;
  if (!loadCubemap(faces, &cubemapTexture)) {
    return;
  }

  Shader shader("resources/shaders/chapter4/stencil_testing.vs", "resources/shaders/chapter4/stencil_testing.fs");
  Shader cubeShader("resources/shaders/chapter4/cubes.vs", "resources/shaders/chapter4/cubes.fs");
  Shader screenShader("resources/shaders/chapter4/framebuffer_screen.vs", "resources/shaders/chapter4/framebuffer_screen.fs");
  Shader skyboxShader("resources/shaders/chapter4/cubemap.vs", "resources/shaders/chapter4/cubemap.fs");

  shader.compile();
  shader.use();
  shader.setInt("texture1", 0);

  screenShader.compile();
  screenShader.use();
  screenShader.setInt("screenTexture", 0);

  skyboxShader.compile();
  skyboxShader.use();
  skyboxShader.setInt("skybox", 0);

  cubeShader.compile();
  cubeShader.use();
  cubeShader.setInt("skybox", 0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // render
    // ------
    // glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // mvp matrixs
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = Camera::getInstance().GetViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(Camera::getInstance().Zoom),
        (float)windowWidth / (float)windowHeight,
        0.1f,
        100.0f);

    // floor
    shader.use();
    glBindVertexArray(planeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // cubes
    cubeShader.use();
    cubeShader.setMat4("view", view);
    cubeShader.setMat4("projection", projection);
    cubeShader.setVec3("cameraPos", Camera::getInstance().Position);
    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubemapTexture);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    cubeShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    cubeShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // skybox
    glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
    skyboxShader.use();
    skyboxShader.setMat4("view", glm::mat4(glm::mat3(view)));
    skyboxShader.setMat4("projection", projection);
    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default

    /// frame buffer related
    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // // glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    // glClear(GL_COLOR_BUFFER_BIT);

    // screenShader.use();
    // glBindVertexArray(quadVAO);
    // glBindTexture(GL_TEXTURE_2D, texColorBuffer); // use the color attachment texture as the texture of the quad plane
    // glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &planeVAO);
  glDeleteVertexArrays(1, &quadVAO);
  glDeleteVertexArrays(1, &skyboxVAO);
  glDeleteRenderbuffers(1, &rbo);
  glDeleteFramebuffers(1, &fbo);

  glfwTerminate();
}