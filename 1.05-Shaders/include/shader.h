#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>


class Shader {
public:
  //Shader(const char* vertexPath, const char* fragmentPath);
  Shader(const char* vertexShaderSource, const char* fragmentShaderSource);

  GLuint program;  // ID программы

  void use();
};

#endif // !SHADER_H

