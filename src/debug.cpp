#include <includes.hpp>

#define CHECK() { \
  bool success = true; \
  while (unsigned int errorCode = glGetError()) \
  { \
    LOG(ERROR_CODES.at(errorCode), errorCode); \
    success = false; \
  } \
  if (!success) __debugbreak(); \
}

#define LOG(error, errorCode) { \
  std::cerr << "[" << __DATE__ << " " << __TIME__ << "] " << "[Error] " << __FILE__ << ":" << __LINE__ << " " << errorCode << " " << error << std::endl; \
}

const std::unordered_map<int, std::string> ERROR_CODES = {
  {GL_INVALID_ENUM, "GL_INVALID_ENUM"},
  {GL_INVALID_VALUE, "GL_INVALID_VALUE"},
  {GL_INVALID_OPERATION, "GL_INVALID_OPERATION"},
  {GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW"},
  {GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW"},
  {GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY"},
  {GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION"},
  {GL_CONTEXT_LOST, "GL_CONTEXT_LOST"}
};