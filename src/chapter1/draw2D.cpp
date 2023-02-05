#include "callbacks.h"
#include "myShader.h"

void triangle() {
  float vertices[] = {
      // 位置              // 颜色
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右下
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
      0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
  };
  GLuint VBO, VAO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  //  note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // ==================== VAO2 =====================
  float vertices1[] = {
      // 位置              // 颜色
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下
      0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
      0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // 顶部
  };
  GLuint VBO1, VAO1;
  glGenBuffers(1, &VBO1);
  glGenVertexArrays(1, &VAO1);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO1);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // ==================== VAO2 =====================

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  // glBindVertexArray(0);
  GLuint texture;
  std::string imgPath1 = "resources/textures/wall.jpg";
  loadImg(imgPath1.c_str(), &texture);

  std::string shaderPath1 = "resources/shaders/chapter0/triangle.vs";
  std::string shaderPath2 = "resources/shaders/chapter0/triangle.fs";

  Shader shader(shaderPath1.c_str(), shaderPath2.c_str());
  shader.compile();

  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    shader.use();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glBindVertexArray(0); // no need to unbind it every time

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  shader.clean();

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return;
}

void EBO_test() {
  const char* vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 aPos;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   gl_Position =  vec4(aPos, 1.0f);\n"
                                   "}\0";

  const char* fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   FragColor = vec4(0.8, 0.8, 0.8, 1.0);\n"
                                     "}\0";

  GLfloat vertices[8][3] = {
      {-0.5f, 0.5f, -0.5f},
      {-0.5f, 0.5f, 0.5f},
      {0.5f, 0.5f, 0.5f},
      {0.5f, 0.5f, -0.5f},
      {-0.5f, -0.5f, -0.5f},
      {-0.5f, -0.5f, 0.5f},
      {0.5f, -0.5f, 0.5f},
      {0.5f, -0.5f, -0.5f}};

  GLuint indices[12][3] = {
      {0, 1, 3},
      {0, 3, 4},
      {0, 1, 4},
      {2, 1, 3},
      {2, 6, 3},
      {2, 6, 1},
      {5, 1, 4},
      {5, 6, 1},
      {5, 4, 6},
      {7, 4, 3},
      {7, 6, 3},
      {7, 4, 6}};

  GLuint VBO, ibo, VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // index buffer 实现对顶点的复用，降低对GPU内存的消耗
  glGenBuffers(1, &ibo);                      //  创建一块 buffer，并将获得的id存在变量buffer中
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // 说明buffer的用途：将这样的一块 Buffer 用来存变量
                                              // 由于opengl是一个状态机模型，这里glBindBuffer就是当前绑定的绘制对象
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, indices, GL_STATIC_DRAW);

  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  Shader shader(vertexShaderSource, fragmentShaderSource, false);

  shader.compile();

  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);
    // render
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    shader.use();

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  shader.clean();
  glBindVertexArray(0);

  glfwTerminate();
  // ------------------------------------------------------------------
}
