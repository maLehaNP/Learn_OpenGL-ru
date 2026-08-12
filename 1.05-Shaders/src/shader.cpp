#include <shader.h>
#include <iostream>
#include <fstream>
#include <sstream>


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  const GLchar* vertexShaderSource;
  const GLchar* fragmentShaderSource;

  std::ifstream vertFile;
  std::ifstream fragFile;
  std::string vertexCode;
  std::string fragmentCode;

  try {
    vertFile.open(vertexPath);
    fragFile.open(fragmentPath);

    std::stringstream vShaderStream, fShaderStream;
    // Считываем данные в потоки
    vShaderStream << vertFile.rdbuf();
    fShaderStream << fragFile.rdbuf();
    // Преобразовываем потоки в массив GLchar
    vertexCode   = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    vertFile.close();
    fragFile.close();
  }
  catch(std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  vertexShaderSource = vertexCode.c_str();
  fragmentShaderSource = fragmentCode.c_str();

  GLuint vertexShader, fragmentShader;
  vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER_PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
  }

  // Удаляем шейдеры
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() {
  glUseProgram(program);
}

GLuint Shader::compileShader(GLenum type, const GLchar* source) {
  GLuint shader;
  shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);

    std::cout << "ERROR::SHADER::";
    if (type == GL_VERTEX_SHADER)
      std::cout << "VERTEX";
    else if (type == GL_FRAGMENT_SHADER)
      std::cout << "FRAGMENT";
    std::cout << "::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  return shader;
}

