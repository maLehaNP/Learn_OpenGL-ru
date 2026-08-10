// #define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);
void error_callback_glfw(int error, const char* description);

int main(int argc, char *argv[]) {
  if (!glfwInit()) {
    fprintf( stderr, "ERROR: could not start GLFW3.\n" );
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  glfwWindowHint(GLFW_SAMPLES, 8);  // MSAA

  // Объект окна
  GLFWwindow *window = glfwCreateWindow(800, 600, "Hello Triangle", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Ошибка при создании окна GLFW" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window); // Создаем контекст окна

  // GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "Не смог инициализировать GLEW" << std::endl;
    return -1;
  }

  // (https://antongerdelan.net/opengl/hellotriangle.html)
  // Try to call some OpenGL functions, and print some more version info.
  printf( "Renderer: %s.\n", glGetString( GL_RENDERER ) );
  printf( "OpenGL version supported %s.\n", glGetString( GL_VERSION ) );

  // Viewport
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // Установка колбэка ввода
  glfwSetKeyCallback(window, key_callback);

  // Register the error callback function that we wrote earlier.
  glfwSetErrorCallback(error_callback_glfw);


  // Вершины треугольника
  GLfloat vertices[] = {
    -1.0f, -0.5f, 0.0f,
     0.0f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
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

  // Шейдеры
  //----------------------------------------------------------------------------
  // Исходный код GLSL
  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 position;\n"
      "void main()\n"
      "{\n"
      "  gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
      "}\0";
  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // Привязываем исх. код
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  // Компилируем шейдер
  glCompileShader(vertexShader);

  // Проверка успешности сборки шейдера
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  const char *fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 color;\n"
      "void main()\n"
      "{\n"
      "  color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\0";
  GLuint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  //----------------------------------------------------------------------------

  // Шейдерная программа (Shader Program)
  //-------------------------------------
  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  // Присоединяем шейдеры
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  // Связываем
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
  }

  // Удаляем шейдеры
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  //-------------------------------------

  // Vertex Array Object
  //--------------------
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
  //--------------------


  // Прямоугольник
  GLfloat vertices2[] = {
     1.0f,  0.5f, 0.0f,  // Верхний правый угол
     1.0f, -0.5f, 0.0f,  // Нижний правый угол
     0.0f, -0.5f, 0.0f,  // Нижний левый угол
     0.0f,  0.5f, 0.0f   // Верхний левый угол
  };
  GLuint indices[] = {  // Помните, что мы начинаем с 0!
    0, 1, 3,   // Первый треугольник
    1, 2, 3    // Второй треугольник
  };

  // Element Buffer Object
  //----------------------------------------------------------------------------
  GLuint EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  //----------------------------------------------------------------------------

  GLuint VBO2;
  glGenBuffers(1, &VBO2);

  GLuint VAO2;
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);
      glBindBuffer(GL_ARRAY_BUFFER, VBO2);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
      glEnableVertexAttribArray(0);
  glBindVertexArray(0);


  // Отрисовка только линий треугольников
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Frame Rate Counter
  double prev_s = glfwGetTime();  // Set the initial 'previous time'.
  double title_countdown_s = 0.1;

  // Игровой цикл
  while (!glfwWindowShouldClose(window)) {
    double curr_s     = glfwGetTime();   // Get the current time.
	  double elapsed_s  = curr_s - prev_s; // Work out the time elapsed over the last frame.
	  prev_s            = curr_s;          // Set the 'previous time' for the next frame to use.

    // Print the FPS, but not every frame, so it doesn't flicker too much.
	  title_countdown_s -= elapsed_s;
	  if ( title_countdown_s <= 0.0 && elapsed_s > 0.0 ) {
	  	double fps        = 1.0 / elapsed_s;

	  	// Create a string and put the FPS as the window title.
	  	char tmp[256];
	  	sprintf( tmp, "FPS %.2lf", fps );
	  	glfwSetWindowTitle(window, tmp );
	  	title_countdown_s = 0.1;
	  }

    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    // Отрисовываем треугольник
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    // Прямоугольник
    glBindVertexArray(VAO2);
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

