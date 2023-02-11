#include "callbacks.h"
#include "myShader.h"
#include "IndicesBuffer.h"
#include "VertexBuffer.h"

void coord_system() {
  glEnable(GL_DEPTH_TEST);
  unsigned int indices[] = {
      0, 1, 3, 1, 2, 3};
  auto ibPtr = std::make_shared<IndicesBuffer>(indices, 6);

  GLfloat vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

      0.5f,
      0.5f,
      0.5f,
      1.0f,
      0.0f,
      0.5f,
      0.5f,
      -0.5f,
      1.0f,
      1.0f,
      0.5f,
      -0.5f,
      -0.5f,
      0.0f,
      1.0f,
      0.5f,
      -0.5f,
      -0.5f,
      0.0f,
      1.0f,
      0.5f,
      -0.5f,
      0.5f,
      0.0f,
      0.0f,
      0.5f,
      0.5f,
      0.5f,
      1.0f,
      0.0f,

      -0.5f,
      -0.5f,
      -0.5f,
      0.0f,
      1.0f,
      0.5f,
      -0.5f,
      -0.5f,
      1.0f,
      1.0f,
      0.5f,
      -0.5f,
      0.5f,
      1.0f,
      0.0f,
      0.5f,
      -0.5f,
      0.5f,
      1.0f,
      0.0f,
      -0.5f,
      -0.5f,
      0.5f,
      0.0f,
      0.0f,
      -0.5f,
      -0.5f,
      -0.5f,
      0.0f,
      1.0f,

      -0.5f,
      0.5f,
      -0.5f,
      0.0f,
      1.0f,
      0.5f,
      0.5f,
      -0.5f,
      1.0f,
      1.0f,
      0.5f,
      0.5f,
      0.5f,
      1.0f,
      0.0f,
      0.5f,
      0.5f,
      0.5f,
      1.0f,
      0.0f,
      -0.5f,
      0.5f,
      0.5f,
      0.0f,
      0.0f,
      -0.5f,
      0.5f,
      -0.5f,
      0.0f,
      1.0f};
  auto vbPtr = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure
  // vertex attributes(s).
  glBindVertexArray(VAO);

  // set vertex attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int texture1, texture2;
  std::string imgPath1, imgPath2, shaderPath1, shaderPath2;

#ifdef WIN32
  system("chdir");
  imgPath1 = "..\\..\\..\\resources\\textures\\container.jpg";
  imgPath2 = "..\\..\\..\\resources\\textures\\awesomeface.jpg";
  shaderPath1 = "..\\..\\..\\resources\\shaders\\chapter1\\triangle.vs";
  shaderPath2 = "..\\..\\..\\resources\\shaders\\chapter1\\triangle.fs";
#elif defined __APPLE__
  system("pwd");
  imgPath1 = "resources/textures/wall.jpg";
  imgPath2 = "resources/textures/container.jpg";
  shaderPath1 = "resources/shaders/chapter1/triangle.vs";
  shaderPath2 = "resources/shaders/chapter1/triangle.fs";
#endif

  loadImg(imgPath1.c_str(), &texture1);
  loadImg(imgPath2.c_str(), &texture2);

  Shader shader(shaderPath1.c_str(), shaderPath2.c_str());

  shader.compile();
  shader.use();
  shader.setInt("ourTexture1", 0);
  shader.setInt("ourTexture2", 1);

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

  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    glBindVertexArray(VAO);
    // GL_TEXTURE0 is activated by default, so actually I don't need to do this.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader.use();
    shader.setFloat("mixVal", 0.5);

    glm::mat4 view = glm::mat4(1.0f);
    // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
    view = camera->GetViewMatrix();
    shader.setMat4("view", view);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera->Zoom), (float)windowWidth / windowHeight, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    for (int i = 0; i < 10; ++i) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      if (i % 3) {
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      } else {
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(100.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      }

      shader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}
