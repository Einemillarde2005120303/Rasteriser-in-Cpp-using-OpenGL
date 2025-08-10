#pragma once

#include <includes.hpp>

const unsigned int WINDOW_WIDTH = 960;
const unsigned int WINDOW_HEIGHT = 540;

std::string readFile(std::string path) {
  std::ifstream file(path);
  if (!file) {
    LOG(std::format("Failed to read file: {}", path), -1);
    __debugbreak();
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

bool floatCompare(float a, float b) {
  return std::abs(a - b) <= 1e-5f;
}

float randomFloat(float min, float max) {
  static std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<float> dist(min, max);
  return dist(rng);
}

class vec2 {
public:
  float x, y;

  vec2() : x(0), y(0) {}
  vec2(float a, float b) : x(a), y(b) {}

  float dot(vec2 other) {
    return this->x * other.x + this->y * other.y;
  }

  vec2 operator+(vec2 other) {
    return vec2(this->x + other.x, this->y + other.y);
  }

  vec2 operator-(vec2 other) {
    return vec2(this->x - other.x, this->y - other.y);
  }

  vec2 operator*(float scalar) {
    return vec2(this->x * scalar, this->y * scalar);
  }

  vec2 operator/(float scalar) {
    if (scalar == 0) {
      LOG("Division by zero error", -1);
      __debugbreak();
    }

    return vec2(this->x / scalar, this->y / scalar);
  }

  vec2 operator*(vec2 other) {
    return vec2(this->x * other.x, this->y * other.y);
  }

  vec2 operator/(vec2 other) {
    if (other.x == 0 || other.y == 0) {
      LOG("Division by zero error", -1);
      __debugbreak();
    }

    return vec2(this->x / other.x, this->y / other.y);
  }

  bool operator==(vec2 other) {
    return floatCompare(this->x, other.x) && floatCompare(this->y, other.y);
  }

  bool operator!=(vec2 other) {
    return !(*this == other);
  }
};

vec2 operator/(float scalar, const vec2 &v) {
  return vec2(scalar / v.x, scalar / v.y);
}

class vec3 {
public:
  union {
    struct { float x, y, z; };
    struct { float r, g, b; };
  };

  vec3() : x(0), y(0), z(0) {}
  vec3(float a, float b, float c) : x(a), y(b), z(c) {}
  vec3(vec2 v, float c) : x(v.x), y(v.y), z(c) {}

  float dot(vec3 other) {
    return this->x * other.x + this->y * other.y + this->z * other.z;
  }

  vec3 cross(vec3 other) {
    return vec3(
      this->y * other.z - this->z * other.y,
      this->z * other.x - this->x * other.z,
      this->x * other.y - this->y * other.x
    );
  }

  vec3 operator+(vec3 other) {
    return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
  }

  vec3 operator-(vec3 other) {
    return vec3(this->x - other.x, this->y - other.y, this->z - other.z);
  }

  vec3 operator*(float scalar) {
    return vec3(this->x * scalar, this->y * scalar, this->z * scalar);
  }

  vec3 operator/(float scalar) {
    if (scalar == 0) {
      LOG("Division by zero error", -1);
      __debugbreak();
    }

    return vec3(this->x / scalar, this->y / scalar, this->z / scalar);
  }

  vec3 operator*(vec3 other) {
    return vec3(this->x * other.x, this->y * other.y, this->z * other.z);
  }

  vec3 operator/(vec3 other) {
    if (other.x == 0 || other.y == 0 || other.z == 0) {
      LOG("Division by zero error", -1);
      __debugbreak();
    }

    return vec3(this->x / other.x, this->y / other.y, this->z / other.z);
  }

  bool operator==(vec3 other) {
    return floatCompare(this->x, other.x) && floatCompare(this->y, other.y) && floatCompare(this->z, other.z);
  }

  bool operator!=(vec3 other) {
    return !(*this == other);
  }

  vec2 toVec2() {
    return vec2(this->x, this->y);
  }
};

vec3 operator/(float scalar, vec3 v) {
  if (v.x == 0 || v.y == 0 || v.z == 0) {
    LOG("Division by zero error", -1);
    __debugbreak();
  }
  return vec3(scalar / v.x, scalar / v.y, scalar / v.z);
}

class vec4 {
public:
  union {
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
  };

  vec4() : x(0), y(0), z(0), w(0) {}
  vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
  vec4(vec2 v, float z, float w) : x(v.x), y(v.y), z(z), w(w) {}
  vec4(vec3 v, float d) : x(v.x), y(v.y), z(v.z), w(d) {}

  vec2 toVec2() {
    return vec2(this->x, this->y);
  }

  vec3 toVec3() {
    return vec3(this->x, this->y, this->z);
  }
};