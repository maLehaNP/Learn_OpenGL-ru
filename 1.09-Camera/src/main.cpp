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
//#include "camera.h"


void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);
void error_callback_glfw(int error, const char* description);

GLFWwindow* initWindow();
void printRenderInfo();
GLuint createTexture(char const* filename);
void updateFrameStat();
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


GLFWwindow* window;
int screenWidth;
int screenHeight;
// Статистика кадра
double deltaTime = 0.;
double prevTime = 0.;
double updateCd = 0.1;

// Камера
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

bool keys[1024]; // Состояния клавиш

// Углы поворота камеры
GLfloat yaw   = -90.0f;
GLfloat pitch =  0.0f;

//GLfloat lastX = 400, lastY = 300;
GLfloat lastX = screenWidth  / 2.0f;
GLfloat lastY = screenHeight / 2.0f;
//bool firstMouse = true;  // Проверка первого включения

float fov = 45.0f;

//Camera camera;



int main(int argc, char *argv[]) {
  window = initWindow();
  printRenderInfo();
  stbi_set_flip_vertically_on_load(true);
  glEnable(GL_DEPTH_TEST);  // Включаем провеку глубины

  Shader shader("shaders/shader.vert", "shaders/shader.frag");
  GLuint texture = createTexture("../textures/container.jpg");
  GLuint texture2 = createTexture("../textures/awesomeface.png");

  // Куб
  float vertices2[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  GLuint VBO2, VAO2;
  glGenBuffers(1, &VBO2);
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
  glBindVertexArray(0);

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 projection;
  projection = glm::perspective(
    glm::radians(45.0f),
    static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
    0.1f, 100.0f
  );

  shader.use();
  shader.setInt("ourTexture", 0);
  shader.setInt("ourTexture2", 1);
  GLint modelLoc = glGetUniformLocation(shader.program, "model");
  GLint viewLoc = glGetUniformLocation(shader.program, "view");
  GLint projLoc = glGetUniformLocation(shader.program, "projection");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };


  // Пространство Камеры/Вида
  //-------------------------
  // 1. Позиция камеры
  //glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  // 2. Направление камеры
  glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
  // 3. Правая ось
  //glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  // Направление вверх (в мировом пространстве)
  //glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
  // 4. Ось вверх
  //glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

  // Матрица LookAt
  /*
   *          |Rx Ry Rz 0|   |1 0 0 -Px|
   * LookAt = |Ux Uy Uz 0| * |0 1 0 -Py|,
   *          |Dx Dy Dz 0|   |0 0 1 -Pz|
   *          |0  0  0  1|   |0 0 0  1 |
   * где R — правый вектор, U — вектор, указывающий вверх,
   * D — вектор направления камеры, а P — позиция камеры.
   */
  view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  //-------------------------

  // Захватить и скрыть указатель мыши
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwSetCursorPosCallback(window, mouse_callback);

  // Вместо проверки firstMouse
  //glfwSetCursorPos(window, (double)screenWidth / 2., (double)screenHeight / 2.);  // Не оч робит
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  lastX = xpos;
  lastY = ypos;

  glfwSetScrollCallback(window, scroll_callback);


  //camera = Camera();


  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  // Отрисовка только линий треугольников
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Игровой цикл
  while (!glfwWindowShouldClose(window)) {
    updateFrameStat();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    // Обрабатываем нажатые клавиши
    do_movement();

    shader.use();


    glm::mat4 view;

    // Камера по кругу
    //GLfloat radius = 10.0f;
    //GLfloat camX = sin(glfwGetTime()) * radius;
    //GLfloat camZ = cos(glfwGetTime()) * radius;
    //view = glm::lookAt(
    //  glm::vec3(camX, 0.0, camZ),
    //  cameraTarget,
    //  cameraUp
    //);

    // Управляемая камера
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    //view = glm::lookAt(cameraPos, cameraTarget, cameraUp);  // Смотрит в ноль

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // Трансфокация
    projection = glm::perspective(glm::radians(fov), (GLfloat)screenWidth/(GLfloat)screenHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Много кубов
    glBindVertexArray(VAO2);
    for (int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = sin(glfwGetTime() + M_PI * (i + 1) / 11);
      model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
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

  //GLfloat cameraSpeed = 0.05f;
  //if(key == GLFW_KEY_W)
  //  cameraPos += cameraSpeed * cameraFront;
  //if(key == GLFW_KEY_S)
  //  cameraPos -= cameraSpeed * cameraFront;
  //if(key == GLFW_KEY_A)
  //  cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  //if(key == GLFW_KEY_D)
  //  cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

  // Сохраняем инфу о нажатых клавишах
  //if(action == GLFW_PRESS)
  //  keys[key] = true;
  //else if(action == GLFW_RELEASE)
  //  keys[key] = false;
  // Мой вариант
  keys[key] = (action == GLFW_PRESS) ? true : false;
}

void error_callback_glfw(int error, const char* description) {
  fprintf(stderr, "GLFW ERROR: code %i msg: %s.\n", error, description);
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

  glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
  glViewport(0, 0, screenWidth, screenHeight);

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
  int width, height, nrChannels;
  unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
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

void updateFrameStat() {
  double curTime = glfwGetTime();
  deltaTime = curTime - prevTime;
  prevTime  = curTime;
  updateCd -= deltaTime;
  if (updateCd <= 0.) {
    double fps = 1. / deltaTime;
    char tmp[256];
    snprintf(tmp, sizeof tmp, "FPS: %.2lf | delta: %.2f ms", fps, deltaTime * 1e3);
    glfwSetWindowTitle(window, tmp);
    updateCd = 0.1;
  }
}

void do_movement() {
  // Обновляем состояние клавиш
  //GLfloat cameraSpeed = 0.01f;
  GLfloat cameraSpeed = 2.0f * deltaTime;  // Для скорости как предыдущей нужен множитель 0.6
  if(keys[GLFW_KEY_W])
  	cameraPos += cameraSpeed * cameraFront;
  if(keys[GLFW_KEY_S])
  	cameraPos -= cameraSpeed * cameraFront;
  if(keys[GLFW_KEY_A])
  	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if(keys[GLFW_KEY_D])
  	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

  // С классом камеры
  //if(keys[GLFW_KEY_W])
  //  camara.processMove(FORWARD, deltaTime);
  //if(keys[GLFW_KEY_S])
  //  camara.processMove(BACKWARD, deltaTime);
  //if(keys[GLFW_KEY_A])
  //  camara.processMove(LEFT, deltaTime);
  //if(keys[GLFW_KEY_D])
  //  camara.processMove(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  //static GLfloat lastX, lastY;

  //if (firstMouse)  // эта переменная была проинициализирована значением true
  //{
  //  lastX = xpos;
  //  lastY = ypos;
  //  firstMouse = false;
  //}
  /*
   * От переводчика: не совсем понятно, зачем нужна проверка с переменной firstMouse,
   * когда можно было бы воспользоваться функцией glfwSetCursorPos или наоборот
   * — инициализировать переменные lastX и lastY функцией glfwGetCursorPos.
   */

  GLfloat xoffset = xpos - lastX;
  // Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз
  GLfloat yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  GLfloat sensitivity = 0.05f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw   += xoffset;
  pitch += yoffset;

  // Ограничения вращения камеры
  if(pitch > 89.0f)
    pitch =  89.0f;
  if(pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  front.y = sin(glm::radians(pitch));
  front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
  cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  if(fov >= 1.0f && fov <= 45.0f)
  	fov -= yoffset;
  if(fov <= 1.0f)
  	fov = 1.0f;
  if(fov >= 45.0f)
  	fov = 45.0f;
}

