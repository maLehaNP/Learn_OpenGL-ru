/*
 * https://habr.com/ru/articles/311234/
 */

// GLEW нужно подключать до GLFW.
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main (int argc, char *argv[]) {
  // Инициализация GLFW
  glfwInit();

  // Настройка GLFW
  //---------------
  // Задание минимально требуемой версии OpenGL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Установка профайла
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Выключение изменения размера окна
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  //---------------

  // Объект окна
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Ошибка при создании окна GLFW" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);  // Создаем контекст окна

  // GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "Не смог инициализировать GLEW" << std::endl;
    return -1;
  }

  // Viewport
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // Установка колбэка ввода
  glfwSetKeyCallback(window, key_callback);


  // Игровой цикл
  while(!glfwWindowShouldClose(window)) {
    // Проверяет события и вызавает callback'и
    glfwPollEvents();

    // Команды отрисовки здесь
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Заменяет цветной буфер
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  // Закрытие при нажатии ESC
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  	glfwSetWindowShouldClose(window, GL_TRUE);
}

