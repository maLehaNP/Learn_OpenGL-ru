#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#define STB_IMAGE_IMPLEMENTATION


void error_callback_glfw(int error, const char* description) {
  fprintf(stderr, "GLFW ERROR: code %i msg: %s.\n", error, description);
}

GLFWwindow* initWindow(int width = 800, int height = 600, const char* title = "") {
  if (!glfwInit())
    fprintf(stderr, "ERROR: could not start GLFW3.\n");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (window == nullptr) {
    printf("Ошибка при создании окна GLFW\n");
    glfwTerminate();
  }
  glfwMakeContextCurrent(window);  // Создаем контекст окна

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    printf("Не смог инициализировать GLEW\n");

  glfwSetErrorCallback(error_callback_glfw);

  glViewport(0, 0, width, height);

  return window;
}

void printRenderInfo() {
  printf("Renderer: %s.\n", glGetString(GL_RENDERER));
  printf("OpenGL version supported %s.\n", glGetString(GL_VERSION));
  printf("Vendor: %s.\n", glGetString(GL_VENDOR));
  GLint nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &nrAttributes);
  std::cout << "Maximum texture size supported: " << nrAttributes << std::endl;
  std::cout << '\n';
}

GLuint createTexture(char const* filename) {
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannels;
  unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (nrChannels == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    printf("Неудалось загрузить текстуру %s\n", filename);
  }
  stbi_image_free(data);
  return texture;
}

