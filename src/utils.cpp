#include <includes.hpp>

const unsigned int WINDOW_WIDTH = 960;
const unsigned int WINDOW_HEIGHT = 540;

std::string readFile(std::string path)
{
  std::ifstream file(path);
  if (!file)
  {
    LOG(std::format("Failed to read file: {}", path), -1);
    return "";
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}