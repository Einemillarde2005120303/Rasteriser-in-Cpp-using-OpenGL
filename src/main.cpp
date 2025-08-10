#include <includes.hpp>

int main() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Learn OpenGL", NULL, NULL);
  if (window == NULL)
  {
    LOG("Failed to create GLFW window", -1);
    __debugbreak();
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    LOG("Failed to initialize GLAD", -1);
    __debugbreak();
  }

  Shader shader = Shader("shaders\\shader.vert", "shaders\\shader.frag");

  glClearColor(1, 1, 1, 1); CHECK();

  float vertices[] = {
    -1, -1,   0, 1,
     1, -1,   1, 1,
    -1,  1,   0, 0,
     1,  1,   1, 0
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 1, 3
  };

  unsigned long indicesLength = sizeof(indices) / sizeof(unsigned int);

  unsigned int vbo, vao, ebo;

  glGenVertexArrays(1, &vao); CHECK();
  glBindVertexArray(vao); CHECK();

  glGenBuffers(1, &vbo); CHECK();
  glBindBuffer(GL_ARRAY_BUFFER, vbo); CHECK();
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); CHECK();

  glGenBuffers(1, &ebo); CHECK();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); CHECK();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); CHECK();

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); CHECK();
  glEnableVertexAttribArray(0); CHECK();

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); CHECK();
  glEnableVertexAttribArray(1); CHECK();

  unsigned char *pixels = new unsigned char[WINDOW_WIDTH * WINDOW_HEIGHT * 3];
  float *depthBuffer = new float[WINDOW_WIDTH * WINDOW_HEIGHT];

  vec3 triangles[] = {
    vec3(0.2, 0.9, 1),
    vec3(-0.7, -0.9, 1e-5f),
    vec3(-0.6, -0.1, 1e-5f),

    vec3(-0.3, 0.9, 1e-5f),
    vec3(-0.5, 0.9, 1e-5f),
    vec3(0.9, -0.9, 1),

    vec3(0.9, -0.4, 1e-5f),
    vec3(0.9, -0.6, 1e-5f),
    vec3(-1.5, -0.6, 1)
  };

  unsigned int sizeTriangles = 9;

  vec3 colours[sizeTriangles / 3];

  for (unsigned int i = 0; i < sizeTriangles / 3; i++) {
    colours[i] = vec3(randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f));
  }

  unsigned int texture;
  glGenTextures(1, &texture); CHECK();
  glBindTexture(GL_TEXTURE_2D, texture); CHECK();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CHECK();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CHECK();

  glUseProgram(shader.id); CHECK();

  glUniform1i(glGetUniformLocation(shader.id, "s_texture"), 0); CHECK();
  glActiveTexture(GL_TEXTURE0); CHECK();
  glBindTexture(GL_TEXTURE_2D, texture); CHECK();

  while (!glfwWindowShouldClose(window)) {
    createImage(pixels, depthBuffer, triangles, colours, sizeTriangles);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels); CHECK();

    glClear(GL_COLOR_BUFFER_BIT); CHECK();
    glDrawElements(GL_TRIANGLES, indicesLength, GL_UNSIGNED_INT, 0); CHECK();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  delete[] pixels;
  delete[] depthBuffer;

  glfwTerminate();

  return 0;
}
