//
// Created by 谭浩鸣 on 2021/11/30.
//
#include "callbacks.h"

#include <exception>

#include "stb_image.h"

static float deltaTime = 0.0f;  // 当前帧与上一帧的时间差
static float lastFrame = 0.0f;  // 上一帧的时间
static bool firstMouse = true;
float lastX = 400;
float lastY = 300;

int windowWidth = 2560;
int windowHeight = 1440;
bool blinn = false;

GLFWwindow* window = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    Camera::getInstance().ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    Camera::getInstance().ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    Camera::getInstance().ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    Camera::getInstance().ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    Camera::getInstance().ProcessKeyboard(RESET, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    Camera::getInstance().ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    Camera::getInstance().ProcessKeyboard(DOWN, deltaTime);
}

unsigned int loadImg(const char* path, unsigned int* tex_id) {
  // load texture
  glGenTextures(1, tex_id);
  // 加载并生成纹理
  int width, height, nrComponents;
  // 设置纹理加载时是否颠倒
  try {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

    // if (nrComponents == 4) {
    //   std::cout << path << std::endl;
    //   for (int y = 0; y < height / 20; y++) {
    //     for (int x = 0; x < width / 20; x++) {
    //       unsigned char* pixelOffset = data + (x + y * width) * nrComponents;

    //       unsigned char r = pixelOffset[0];
    //       unsigned char g = pixelOffset[1];
    //       unsigned char b = pixelOffset[2];
    //       unsigned char a = nrComponents >= 4 ? pixelOffset[3] : 0xff;

    //       std::cout << (int)r << " " << (int)g << " " << (int)b << " " <<
    //       (int)a << std::endl;
    //     }
    //   }
    // }

    if (data) {
      GLenum format;
      if (nrComponents == 1)
        format = GL_RED;
      else if (nrComponents == 3)
        format = GL_RGB;
      else if (nrComponents == 4)
        format = GL_RGBA;
      glBindTexture(GL_TEXTURE_2D, *tex_id);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      // 为当前绑定的纹理对象设置环绕、过滤方式
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                      format == GL_RGBA
                          ? GL_CLAMP_TO_EDGE
                          : GL_REPEAT);  // for this tutorial: use GL_CLAMP_TO_EDGE to
                                         // prevent semi-transparent borders. Due to
                                         // interpolation it takes texels from next repeat
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                      format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
      std::cout << "Failed to load texture: " << path << std::endl;
      return false;
    }
    stbi_image_free(data);
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }
  return true;
}

unsigned int loadImg_clamp(const char* path, unsigned int* tex_id) {
  // load texture
  glGenTextures(1, tex_id);
  glBindTexture(GL_TEXTURE_2D, *tex_id);
  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // 加载并生成纹理
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << RED "Failed to load texture: " << path << NONE;
    return false;
  }
  stbi_image_free(data);
  return true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  // global vars setting
  if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    Camera::getInstance().ProcessKeyboard(LIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;  // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
  lastX = xpos;
  lastY = ypos;

  Camera::getInstance().ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  Camera::getInstance().ProcessMouseScroll(static_cast<float>(yoffset));
}

void error_callback(int code, const char* msg) {
  {
    printf(RED "glfw error: %d:%s" NONE, code, msg);
    exit(-1);
  }
}

void init() {
  // init camera
  // =========================
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
    std::cout << RED "Failed to create GLFW window" << NONE;
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << RED "Failed to initialize GLAD" << NONE;
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
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // ====== window related end here ========
  glfwSetKeyCallback(window, key_callback);
  // ======= OpenGL settings
  // ======= OpenGL settings end here
}

std::vector<GLfloat> loadVertices(const std::string& path) {
  std::ifstream verticesFile;
  // 保证ifstream对象可以抛出异常：
  verticesFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::vector<GLfloat> ret;
  try {
    // 打开文件
    verticesFile.open(path);
    std::stringstream verticesStream;
    // 读取文件的缓冲内容到数据流中
    verticesStream << verticesFile.rdbuf();
    // 关闭文件处理器
    verticesFile.close();
    while (verticesStream) {
      GLfloat vertice;
      verticesStream >> vertice;
      ret.push_back(vertice);
    }
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::VERTICES::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
  }
  return ret;
}

unsigned int loadCubemap(std::vector<std::string> faces, GLuint* textureID) {
  glGenTextures(1, textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, *textureID);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
      stbi_image_free(data);
      return false;
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return true;
}
