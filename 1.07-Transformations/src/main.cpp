#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <math.h>
#include <learnopengl/shader.h>


void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);
void error_callback_glfw(int error, const char* description);

GLFWwindow* initWindow();
void printRenderInfo();


int main(int argc, char *argv[]) {
  GLFWwindow* window = initWindow();
  printRenderInfo();

  // Вершины прямоугольника
  GLfloat vertices[] = {
    // Позиции            // Цвета            // Текстурные координаты
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
  };
  GLuint indices[] = {
    0, 1, 3,  // Первый треугольник
    1, 2, 3   // Второй треугольник
  };

  // VBO & EBO
  GLuint VBO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // VAO
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
  glBindVertexArray(0);

  Shader shader("shaders/shader.vert", "shaders/shader.frag");

  stbi_set_flip_vertically_on_load(true);
  // Загрузка текстуры
  int width, height, nrChannels;
  unsigned char* data = stbi_load("../textures/container.jpg", &width, &height, &nrChannels, 0);
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  // Вторая текстура
  data = stbi_load("../textures/awesomeface.png", &width, &height, &nrChannels, 0);
  GLuint texture2;
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);  // Обязательно указать GL_RGBA
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);


  // Пример трансформаций
  glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
  glm::mat4 trans = glm::mat4(1.0f);  // Нужно обязательно проициализировать
  trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
  vec = trans * vec;
  //std::cout << vec.x << vec.y << vec.z << std::endl;
  printf("%f %f %f\n", vec.x, vec.y, vec.z);

  trans = glm::mat4(1.0f);
  trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 0.0, 1.0));
  trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
  shader.use();
  GLuint transformLoc = glGetUniformLocation(shader.program, "transform");
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


  // Отрисовка только линий треугольников
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Статистика кадра
  double deltaTime = 0.;
	double prevTime = glfwGetTime();
  double updateCd = 0.1;

  // Игровой цикл
  while (!glfwWindowShouldClose(window)) {
    double curTime = glfwGetTime();
    deltaTime = curTime - prevTime;
    prevTime = curTime;
    updateCd -= deltaTime;
    if (updateCd <= 0.) {
      double fps = 1. / deltaTime;
      char tmp[256];
      snprintf(tmp, sizeof tmp, "FPS: %.2lf | delta: %.2f ms", fps, deltaTime * 1e3);
      glfwSetWindowTitle(window, tmp);
      updateCd = 0.1;
    }

    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0, 0.0, 1.0));
    GLuint transformLoc = glGetUniformLocation(shader.program, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    // Две текстуры
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.setInt("ourTexture", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    shader.setInt("ourTexture2", 1);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  // Закрытие при нажатии ESC
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void error_callback_glfw(int error, const char* description) {
  fprintf( stderr, "GLFW ERROR: code %i msg: %s.\n", error, description );
}

GLFWwindow* initWindow() {
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3.\n");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);  // MSAA

  GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Ошибка при создании окна GLFW" << std::endl;
    glfwTerminate();
  }
  glfwMakeContextCurrent(window);  // Создаем контекст окна

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "Не смог инициализировать GLEW" << std::endl;
  }

  glfwSetKeyCallback(window, key_callback);
  glfwSetErrorCallback(error_callback_glfw);

  int wWidth, wHeight;
  glfwGetFramebufferSize(window, &wWidth, &wHeight);
  glViewport(0, 0, wWidth, wHeight);

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

