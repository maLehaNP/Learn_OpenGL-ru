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
#include <learnopengl/rendering.h>

#define TIMES_SAMPLE_AMOUNT 120


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void updateFrameStat();
void doMovement();

// Окно
GLFWwindow* window;
int screenWidth = 1920;
int screenHeight = 1080;
// Статистика кадра
double deltaTime = 0.;
double prevTime = 0.;
double updateCd = 0.1;
char tmp[256];
// Камера
Camera camera;
bool keys[512];  // Состояния клавиш
double lastX;
double lastY;
bool disableCursor = true;


int main(int argc, char *argv[]) {
  window = initWindow(screenWidth, screenHeight, "Colors");
  printRenderInfo();

  //glfwWindowHint(GLFW_SAMPLES, 8);  // MSAA
  //glfwSwapInterval(1);              // Enable vsync

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  glEnable(GL_DEPTH_TEST);  // Включаем провеку глубины

  // Вместо проверки firstMouse
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  lastX = xpos;
  lastY = ypos;
  camera = Camera();

  // Куб с векторами нормали
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };

  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // VAO источника света
  GLuint lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  // Шейдер освещенного куба
  Shader lightingShader("shaders/lighting_shader.vert", "shaders/lighting_shader.frag", "Lighting");
  lightingShader.use();
  GLint objectColorLoc = glGetUniformLocation(lightingShader.program, "objectColor");
  GLint lightColorLoc  = glGetUniformLocation(lightingShader.program, "lightColor");
  //GLint lightPosLoc = glGetUniformLocation(lightingShader.program, "lightPos");
  //GLint viewPosLoc = glGetUniformLocation(lightingShader.program, "viewPos");
  glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
  glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);  // зададим цвет источника света (белый)

  // Шейдер источника света
  Shader lampShader("shaders/lighting_shader.vert", "shaders/lamp_shader.frag", "Lamp");
  // Координаты источника света
  glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

  // Отводим немного камеру
  camera.pos = glm::vec3(0.0f, 1.0f, 5.0f);


  lightingShader.setFloat3("material.ambient",  1.0f, 0.5f, 0.31f);
  lightingShader.setFloat3("material.diffuse",  1.0f, 0.5f, 0.31f);
  lightingShader.setFloat3("material.specular", 0.5f, 0.5f, 0.5f);
  lightingShader.setFloat("material.shininess", 32.0f);

  lightingShader.setFloat3("light.ambient",  0.2f, 0.2f, 0.2f);
  lightingShader.setFloat3("light.diffuse",  0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
  lightingShader.setFloat3("light.specular", 1.0f, 1.0f, 1.0f);


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
    // Frame stat
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

    glm::mat4 view = camera.getViewMat();
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (GLfloat)screenWidth/(GLfloat)screenHeight, 0.1f, 100.0f);

    // Освещенный куб
    lightingShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    GLint modelLoc = glGetUniformLocation(lightingShader.program, "model");
    GLint viewLoc = glGetUniformLocation(lightingShader.program, "view");
    GLint projLoc = glGetUniformLocation(lightingShader.program, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    //glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    //glUniform3f(viewPosLoc, camera.pos.x, camera.pos.y, camera.pos.z);

    lightingShader.setFloat3("light.position", lightPos.x, lightPos.y, lightPos.z);
    lightingShader.setFloat3("viewPos", camera.pos.x, camera.pos.y, camera.pos.z);

    glm::vec3 lightColor;
    //lightColor.x = sin(glfwGetTime() * 2.0f);
    //lightColor.y = sin(glfwGetTime() * 0.7f);
    //lightColor.z = sin(glfwGetTime() * 1.3f);
    // Нормализуем ([-1, 1] -> [0, 1])
    lightColor.x = sin(glfwGetTime() * 2.0f) / 2.0f + 0.5f;
    lightColor.y = sin(glfwGetTime() * 0.7f) / 2.0f + 0.5f;
    lightColor.z = sin(glfwGetTime() * 1.3f) / 2.0f + 0.5f;

    glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f);  // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);  // low influence

    lightingShader.setVec3("light.ambient", ambientColor);
    lightingShader.setVec3("light.diffuse", diffuseColor);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Куб-лампа
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lampShader.use();
    modelLoc = glGetUniformLocation(lampShader.program, "model");
    viewLoc = glGetUniformLocation(lampShader.program, "view");
    projLoc = glGetUniformLocation(lampShader.program, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

		if (queryRes == GL_TRUE)
		  glEndQuery(GL_TIME_ELAPSED);
		glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &queryRes);
		if (queryRes == GL_TRUE) {
		  glGetQueryObjectiv(query, GL_QUERY_RESULT, &gpuTime);
    }
		double renderEndTime = glfwGetTime();
		renderTime = renderEndTime - renderStartTime;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
      ImGui::Begin("Hello, world!");
      ImGui::Text(tmp);
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
      ImGui::Text("Camera:");
      ImGui::Text("  pos   (%.2f %.2f %.2f)", camera.pos.x, camera.pos.y, camera.pos.z);
      ImGui::Text("  front (%.2f %.2f %.2f)", camera.front.x, camera.front.y, camera.front.z);
      ImGui::Text("  pitch %.2f", camera.pitch);
      ImGui::Text("  yaw   %.2f", camera.yaw);
      ImGui::Text("  FOV   %.2f", camera.fov);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
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
  if(keys[GLFW_KEY_SPACE])
    camera.processMove(UP, deltaTime);
  if(keys[GLFW_KEY_LEFT_CONTROL])
    camera.processMove(DOWN, deltaTime);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  screenWidth = width;
  screenHeight = height;
}

