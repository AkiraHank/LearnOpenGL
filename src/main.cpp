#include "callbacks.h"
#include "myShader.h"
#include "VertexBuffer.h"
#include "IndicesBuffer.h"
#include <memory>

float mixVal = 0.2f;
float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 400;
float lastY = 300;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int windowWidth = 800;
int windowHeight = 600;

GLFWwindow* window = nullptr;

void init();
void triangle();
void demo1();
void demo2();

int main(int argv, char** args) {
  init();
  demo1();
  return 0;
}

void demo1() {
  glEnable(GL_DEPTH_TEST);
  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3};
  auto ibPtr = std::make_shared<IndicesBuffer>(indices, 6);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
  auto vbPtr = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure
  // vertex attributes(s).
  glBindVertexArray(VAO);

  // set vertex attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  // glEnableVertexAttribArray(2);

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

  shader.init();
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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader.use();
    shader.setFloat("mixVal", mixVal);

    glm::mat4 view = glm::mat4(1.0f);
    // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    shader.setMat4("view", view);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / windowHeight, 0.1f, 100.0f);
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

void demo2() {
  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3};
  auto ibPtr = std::make_shared<IndicesBuffer>(indices, 6);
  float vertices[] = {
      -0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      0.5f,
      -0.5f,
      0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      -0.5f,
      -0.5f,

      -0.5f,
      -0.5f,
      0.5f,
      0.5f,
      -0.5f,
      0.5f,
      0.5f,
      0.5f,
      0.5f,
      0.5f,
      0.5f,
      0.5f,
      -0.5f,
      0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      0.5f,

      -0.5f,
      0.5f,
      0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      -0.5f,
      -0.5f,
      -0.5f,
      -0.5f,
      -0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      0.5f,
      0.5f,

      0.5f,
      0.5f,
      0.5f,
      0.5f,
      0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      0.5f,
      0.5f,
      0.5f,
      0.5f,

      -0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      0.5f,
      0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      0.5f,
      -0.5f,
      -0.5f,
      -0.5f,

      -0.5f,
      0.5f,
      -0.5f,
      0.5f,
      0.5f,
      -0.5f,
      0.5f,
      0.5f,
      0.5f,
      0.5f,
      0.5f,
      0.5f,
      -0.5f,
      0.5f,
      0.5f,
      -0.5f,
      0.5f,
      -0.5f,
  };
  auto vbPtr = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure
  // vertex attributes(s).
  glBindVertexArray(VAO);

  // set vertex attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  // glEnableVertexAttribArray(2);
}

void init() {
  glfwInit();
  glfwSetErrorCallback(error_callback);

  // window related
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    exit(-1);
  }
  printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
  printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
  printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
  printf("OpenGL SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  // FIXME: this will lead to wrong display area, need to figure out the reason
  // glViewport(0, 0, windowWidth, windowHeight);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // ====== window related end here ========

  // ======= OpenGL settings
  // ======= OpenGL settings end here
}

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
  shader.init();

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