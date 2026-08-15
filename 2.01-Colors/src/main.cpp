#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include <iostream>
#include <math.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include "rendering.h"

#define TIMES_SAMPLE_AMOUNT 120


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void updateFrameStat();
void doMovement();

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Окно
GLFWwindow* window;
int screenWidth = 1920;
int screenHeight = 1080;
// Статистика кадра
double deltaTime = 0.;
double prevTime = 0.;
double updateCd = 0.1;
// Камера
Camera camera;
bool keys[512];  // Состояния клавиш
double lastX;
double lastY;

bool disableCursor = true;

char tmp[256];


int main(int argc, char *argv[]) {
  window = initWindow(screenWidth, screenHeight, "Colors");
  printRenderInfo();

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


  // ImGui
  //----------------------------------------------------------------------------
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();
  //----------------------------------------------------------------------------


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

  shader.use();
  shader.setInt("ourTexture", 0);
  shader.setInt("ourTexture2", 1);
  GLint modelLoc = glGetUniformLocation(shader.program, "model");
  GLint viewLoc = glGetUniformLocation(shader.program, "view");
  GLint projLoc = glGetUniformLocation(shader.program, "projection");

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

  // Вместо проверки firstMouse
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  lastX = xpos;
  lastY = ypos;
  camera = Camera();


  // Our state
  bool show_demo_window = false;

  double animTime = 0.;
  bool animateCubes = true;


  // Доп. статистика кадра
	double renderTime = 0.0;
	int gpuTime = 0;

	int times_offset = 0;
	float delta_times[TIMES_SAMPLE_AMOUNT]{};
	float render_times[TIMES_SAMPLE_AMOUNT]{};
	float gpu_times[TIMES_SAMPLE_AMOUNT]{};

	double deltaTimeSum = 0.0f;
	double renderTimeSum = 0.0f;
	double gpuTimeSum = 0.0f;

	// GPU query
	GLuint query;
	glGenQueries(1, &query);
	GLint queryRes = GL_TRUE;


  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // Игровой цикл
  while (!glfwWindowShouldClose(window)) {
    double curTime = glfwGetTime();
    deltaTime = curTime - prevTime;
    prevTime  = curTime;
    updateFrameStat();

		double renderStartTime = glfwGetTime();

		double dTimeDelta = deltaTime * 1000. - delta_times[times_offset];
		double rTimeDelta = renderTime * 1000. - render_times[times_offset];
    double gTimeDelta = gpuTime / 1000000. - gpu_times[times_offset];
		delta_times[times_offset] = deltaTime * 1000.0f;
		render_times[times_offset] = renderTime * 1000.0f;
		gpu_times[times_offset] = gpuTime / 1000000.;
		times_offset = (times_offset + 1) % TIMES_SAMPLE_AMOUNT;

    glfwPollEvents();
    doMovement();  // Обрабатываем нажатые клавиши

		if (queryRes == GL_TRUE)
		  glBeginQuery(GL_TIME_ELAPSED, query);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();

    glm::mat4 view = camera.getViewMat();
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (GLfloat)screenWidth/(GLfloat)screenHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
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
      //float angle = sin(glfwGetTime() + M_PI * (i + 1) / 11);

      if (animateCubes)
        //animTime = glfwGetTime();
        animTime += deltaTime / 10.;
      float angle = sin(animTime + M_PI * (i + 1) / 11);

      model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);


		if (queryRes == GL_TRUE)
		  glEndQuery(GL_TIME_ELAPSED);  // Просто запрос +0,01мс рендера

    // Частые запросы результата бьют по процу (1мс рендера),
    // поэтому нужно запрашивать его готовность
		glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &queryRes);
		if (queryRes == GL_TRUE) {
		  glGetQueryObjectiv(query, GL_QUERY_RESULT, &gpuTime);
    }
		//glGetQueryObjectiv(query, GL_QUERY_RESULT_NO_WAIT, &gpuTime);  // (requires OpenGL 4.4 or ARB_query_buffer_object)
    //printf("gpuTime: %6d ns %f ms\n", gpuTime, gpuTime / 1000000.);  // Эта строчка добавляет 0.04 мс

		double renderEndTime = glfwGetTime();
		renderTime = renderEndTime - renderStartTime;
    //printf("renderTime=%f\n", renderTime);


    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (show_demo_window)
      ImGui::ShowDemoWindow(); // Show demo window! :)

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
      ImGui::Begin("Hello, world!");
      ImGui::Text(tmp);
      ImGui::Checkbox("Animate cubes", &animateCubes);

		  //float deltaTimeSum = 0.;
      //float dTimeMax = 0.0;
      //for (int i = 0; i < TIMES_SAMPLE_AMOUNT; i++) {
      //  deltaTimeSum += delta_times[i];
      //  dTimeMax = (delta_times[i] > dTimeMax) ? delta_times[i] : dTimeMax;
      //}
      deltaTimeSum += dTimeDelta;
      renderTimeSum += rTimeDelta;
      gpuTimeSum += gTimeDelta;

		  char overlay[32];
		  sprintf(overlay, "mov avg %f ms", deltaTimeSum / TIMES_SAMPLE_AMOUNT);
		  ImGui::PlotLines("Frame Time", delta_times, TIMES_SAMPLE_AMOUNT, times_offset, overlay, 0.0f, 20.0f, ImVec2(0, 100));
		  sprintf(overlay, "mov avg %f ms", renderTimeSum / TIMES_SAMPLE_AMOUNT);
		  ImGui::PlotLines("Render Time", render_times, TIMES_SAMPLE_AMOUNT, times_offset, overlay, 0.0f, 2.0f, ImVec2(0, 100));
		  sprintf(overlay, "mov avg %f ms", gpuTimeSum / TIMES_SAMPLE_AMOUNT);
		  ImGui::PlotLines("GPU Time", gpu_times, TIMES_SAMPLE_AMOUNT, times_offset, overlay, 0.0f, 2.0f, ImVec2(0, 100));

      ImGui::Checkbox("Demo Window", &show_demo_window);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  glfwTerminate();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  return 0;
}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  // Закрытие при нажатии ESC
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  // Cursor Mode
  if (key == GLFW_KEY_C && action == GLFW_PRESS) {
    disableCursor = !disableCursor;
    glfwSetInputMode(window, GLFW_CURSOR, (disableCursor) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
  }

  // Сохраняем инфу о нажатых клавишах
  if(action == GLFW_PRESS)
    keys[key] = true;
  else if(action == GLFW_RELEASE)
    keys[key] = false;
}

void updateFrameStat() {
  updateCd -= deltaTime;
  if (updateCd <= 0.) {
    double fps = 1. / deltaTime;
    snprintf(tmp, sizeof tmp, "FPS: %.2lf | delta: %.2f ms", fps, deltaTime * 1e3);
    //glfwSetWindowTitle(window, tmp);
    updateCd = 0.1;
  }
}

void doMovement() {
  if(keys[GLFW_KEY_W])
    camera.processMove(FORWARD, deltaTime);
  if(keys[GLFW_KEY_S])
    camera.processMove(BACKWARD, deltaTime);
  if(keys[GLFW_KEY_A])
    camera.processMove(LEFT, deltaTime);
  if(keys[GLFW_KEY_D])
    camera.processMove(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (disableCursor) {
    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos;
    camera.processMouse(xoffset, yoffset);
  }
  lastX = xpos;
  lastY = ypos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.processScroll(xoffset, yoffset);
}

