#include <iostream>

#include "VertexBuffer.h"
#include "fileSystem.h"
#include "map"
#include "myShader.h"

void AdvancedLighting() {
  int ret = 0;
  // blinn VAO
  std::vector<GLfloat> vertices =
      loadVertices(EditorFoundation::instance()
                       ->join({EditorFoundation::instance()->getResourceDir(),
                               "vertices", "blinn"})
                       .c_str());
  unsigned int planeVAO;
  glGenVertexArrays(1, &planeVAO);
  glBindVertexArray(planeVAO);
  VertexBuffer wood(&vertices[0], vertices.size() * sizeof(GLfloat));
  //  unsigned int planeVAO, planeVBO;
  //  glGenVertexArrays(1, &planeVAO);
  //  glGenBuffers(1, &planeVBO);
  //  glBindVertexArray(planeVAO);
  //  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  //  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
  //  &vertices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  checkGLError();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  checkGLError();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(2);
  checkGLError();
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(6 * sizeof(float)));
  glBindVertexArray(0);
  checkGLError();
  // load textures
  // -------------
  unsigned int floorTexture;
  auto resFolder = EditorFoundation::instance()->getResourceDir();
  std::string imgPath2 = EditorFoundation::instance()->join(
      {EditorFoundation::instance()->getResourceDir(), "textures", "wall.jpg"});
  if (!loadImg(imgPath2.c_str(), &floorTexture)) {
    return;
  }

  // shader configuration
  // --------------------
  Shader shader(EditorFoundation::instance()
                    ->join({EditorFoundation::instance()->getResourceDir(),
                            "shaders", "chapter5", "blinn.vs"})
                    .c_str(),
                EditorFoundation::instance()
                    ->join({EditorFoundation::instance()->getResourceDir(),
                            "shaders", "chapter5", "blinn.fs"})
                    .c_str());
  shader.compile();
  shader.use();
  shader.setInt("texture1", 0);

  // lighting info
  // -------------
  glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw objects
    shader.use();
    glm::mat4 projection = glm::perspective(
        glm::radians(Camera::getInstance().Zoom),
        (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
    glm::mat4 view = Camera::getInstance().GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    // set light uniforms
    shader.setVec3("viewPos", Camera::getInstance().Position);
    shader.setVec3("lightPos", lightPos);
    shader.setInt("blinn", blinn);
    // floor
    glBindVertexArray(planeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &planeVAO);

  glfwTerminate();
}