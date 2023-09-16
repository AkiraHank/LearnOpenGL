#include "myShader.h"
#include "model.h"
#include "map"

void planet() {
  glEnable(GL_DEPTH_TEST);
  int ret = 0;
  Shader shader("/Users/hank/Documents/Projects/LearnOpenGL/resources/shaders/chapter4/planet.vs",
                "/Users/hank/Documents/Projects/LearnOpenGL/resources/shaders/chapter4/planet.fs");
  shader.compile();
  Shader rockShader("/Users/hank/Documents/Projects/LearnOpenGL/resources/shaders/chapter4/rock.vs",
                    "/Users/hank/Documents/Projects/LearnOpenGL/resources/shaders/chapter4/planet.fs");
  rockShader.compile();

  Model planet("/Users/hank/Documents/Projects/LearnOpenGL/resources/objects/planet/planet.obj");
  Model rock("/Users/hank/Documents/Projects/LearnOpenGL/resources/objects/rock/rock.obj");

  // generate a large list of semi-random model transformation matrices
  // ------------------------------------------------------------------
  const unsigned int amount = 5000;
  glm::mat4* modelMatrices;
  modelMatrices = new glm::mat4[amount];
  srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
  float radius = 50.0;
  float offset = 2.5f;
  for (unsigned int i = 0; i < amount; i++) {
    glm::mat4 model = glm::mat4(1.0f);
    // 1. translation: displace along circle with 'radius' in range [-offset, offset]
    float angle = (float)i / (float)amount * 360.0f;
    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float x = sin(angle) * radius + displacement;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float z = cos(angle) * radius + displacement;
    model = glm::translate(model, glm::vec3(x, y, z));

    // 2. scale: Scale between 0.05 and 0.25f
    auto scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
    model = glm::scale(model, glm::vec3(scale));

    // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
    auto rotAngle = static_cast<float>((rand() % 360));
    model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

    // 4. now add to list of matrices
    modelMatrices[i] = model;
  }

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

  for (const auto& mesh : rock.getMeshes()) {
    unsigned int VAO = mesh.VAO;
    glBindVertexArray(VAO);
    // 顶点属性
    GLsizei vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
  }

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // mvp matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = Camera::getInstance().GetViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(Camera::getInstance().Zoom),
        (float)windowWidth / (float)windowHeight,
        0.1f,
        100.0f);

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    planet.Draw(shader);

    rockShader.use();
    rockShader.setInt("texture_diffuse1", 0);
    rockShader.setMat4("projection", projection);
    rockShader.setMat4("view", view);
    // draw meteorites
    for (const auto& mesh : rock.getMeshes()) {
      glBindVertexArray(mesh.VAO);
      glDrawElementsInstanced(
          GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, amount);
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  // glDeleteVertexArrays(1, &VAO);

  glfwTerminate();
}