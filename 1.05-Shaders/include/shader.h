#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>


class Shader {
public:
  Shader(const char* vertexPath, const char* fragmentPath);

  GLuint program;  // ID программы

  void use();

  GLuint compileShader(GLenum type, const GLchar* source);
};

#endif // !SHADER_H

