#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>


class Shader {
public:
  Shader(const char* vertexPath, const char* fragmentPath, const char* shaderName);

  GLuint program;     // ID программы
  const char* name;  // Наименование шейдера

  void use();

  GLuint compileShader(GLenum type, const GLchar* source);

  void setInt(const GLchar* name, GLint v0);
  void setFloat(const GLchar* name, GLfloat v0);
  void setFloat3(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2);
  void setVec3(const GLchar* name, glm::vec3 vec);
};


Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* name = "") : name(name) {
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

  printf("[+] Shader program %s (%u) linked\n", name, program);
}

void Shader::use() {
  glUseProgram(program);
}

GLuint Shader::compileShader(GLenum type, const GLchar* source) {
  GLuint shader;
  shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  const char* typeName;
  if (type == GL_VERTEX_SHADER)
    typeName = "VERTEX";
  else if (type == GL_FRAGMENT_SHADER)
    typeName = "FRAGMENT";

  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    //std::cout << "ERROR::SHADER::" << typeName << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    printf("ERROR::SHADER::%s::COMPILATION_FAILED\n%s\n", typeName, infoLog);
  }

  printf("[+] Shader %u (%s) compiled\n", shader, typeName);

  return shader;
}

void Shader::setInt(const GLchar* name, GLint v0) {
  glUniform1i(glGetUniformLocation(program, name), v0);
}

void Shader::setFloat(const GLchar* name, GLfloat v0) {
  glUniform1f(glGetUniformLocation(program, name), v0);
}

void Shader::setFloat3(const GLchar* name, GLfloat v0, GLfloat v1, GLfloat v2) {
  glUniform3f(glGetUniformLocation(program, name), v0, v1, v2);
}

void Shader::setVec3(const GLchar* name, glm::vec3 vec) {
  glUniform3f(glGetUniformLocation(program, name), vec.x, vec.y, vec.z);
}

#endif // !SHADER_H

