#include <includes.hpp>

class Shader
{
public:
  unsigned int id;

  Shader(std::string vertexPath, std::string fragmentPath)
  {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); CHECK();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); CHECK();

    std::string vertexShaderSrcStr = readFile(vertexPath);
    const char *vertexShaderSrc = vertexShaderSrcStr.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL); CHECK();

    std::string fragmentShaderSrcStr = readFile(fragmentPath);
    const char *fragmentShaderSrc = fragmentShaderSrcStr.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL); CHECK();

    int success;
    char infoLog[512];

    glCompileShader(vertexShader); CHECK();
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); CHECK();
    if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); CHECK();
      LOG(std::format("Failed to compile vertex shader\n{}", infoLog), -1);
    }

    glCompileShader(fragmentShader); CHECK();
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); CHECK();
    if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog); CHECK();
      LOG(std::format("Failed to compile fragment shader\n{}", infoLog), -1);
    }

    id = glCreateProgram(); CHECK();

    glAttachShader(id, vertexShader); CHECK();
    glAttachShader(id, fragmentShader); CHECK();
    glLinkProgram(id); CHECK();

    glGetProgramiv(id, GL_LINK_STATUS, &success); CHECK();
    if(!success) {
      glGetProgramInfoLog(id, 512, NULL, infoLog); CHECK();
      LOG(std::format("Failed to create shader program\n{}", infoLog), -1);
    }

    glDeleteShader(vertexShader); CHECK();
    glDeleteShader(fragmentShader); CHECK();
  }
};