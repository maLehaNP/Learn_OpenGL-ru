#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <shader.h>


void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);
void error_callback_glfw(int error, const char* description);
int init();

int main(int argc, char *argv[]) {
  if (!init()) { std::cout << "Ошибка начальной инициализации" << '\n'; return -1; }
  GLFWwindow *window = glfwCreateWindow(800, 600, "Hello Triangle", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Ошибка при создании окна GLFW" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);  // Создаем контекст окна
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "Не смог инициализировать GLEW" << std::endl;
    return -1;
  }
  glfwSetKeyCallback(window, key_callback);
  glfwSetErrorCallback(error_callback_glfw);
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  printf("Renderer: %s.\n", glGetString(GL_RENDERER));
  printf("OpenGL version supported %s.\n", glGetString(GL_VERSION));
  printf("Vendor: %s.\n", glGetString(GL_VENDOR));
  GLint nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &nrAttributes);
  std::cout << "Maximum texture size supported: " << nrAttributes << std::endl;

  // Вершины треугольника
  GLfloat vertices[] = {
    // Позиции          // Цвета
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  // Vertex Buffer Object
  //---------------------
  GLuint VBO; // Идентификатор
  // Создаем буфер
  glGenBuffers(1, &VBO);
  // Привязываем буфер
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Отправляем данные
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  //---------------------

  Shader shader("shaders/shader.vert", "shaders/shader.frag");

  // Vertex Array Object
  //--------------------
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  //--------------------


  // Отрисовка только линий треугольников
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Frame Rate Counter
  double prev_s = glfwGetTime();  // Set the initial 'previous time'.
  double title_countdown_s = 0.1;

  // Игровой цикл
  while (!glfwWindowShouldClose(window)) {
    double curr_s     = glfwGetTime();    // Get the current time.
	  double elapsed_s  = curr_s - prev_s;  // Work out the time elapsed over the last frame.
	  prev_s            = curr_s;           // Set the 'previous time' for the next frame to use.

    // Print the FPS, but not every frame, so it doesn't flicker too much.
	  title_countdown_s -= elapsed_s;
	  if (title_countdown_s <= 0.0 && elapsed_s > 0.0) {
	  	double fps = 1.0 / elapsed_s;

	  	// Create a string and put the FPS as the window title.
	  	char tmp[256];
	  	sprintf(tmp, "FPS: %.2lf | Frame Time: %.2f ms", fps, elapsed_s * 1e3);
	  	glfwSetWindowTitle(window, tmp);
	  	title_countdown_s = 0.1;
	  }

    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //glUseProgram(shaderProgram);
    shader.use();

    // Обновляем цвет формы
    GLfloat timeValue = glfwGetTime();
    GLfloat greenValue = (std::sin(timeValue) / 2) + 0.5;
    //GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    GLint vertexColorLocation = glGetUniformLocation(shader.program, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // Отрисовываем треугольник
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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

int init() {
  if (!glfwInit()) {
    fprintf( stderr, "ERROR: could not start GLFW3.\n" );
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);  // MSAA

  return 1;
}

