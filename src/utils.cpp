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

std::vector<std::string> splitStr(std::string str, char ch) {
  unsigned int pos = str.find(ch);
  std::vector<std::string> result;

  while (pos != std::string::npos) {
    result.push_back(str.substr(0, pos));
    str = str.substr(pos + 1);
    pos = str.find(ch);
  }

  result.push_back(str);
  return result;
}

std::string trimStr(std::string str) {
  const char* whitespace = " \t\n\r\f\v";
  str.erase(0, str.find_first_not_of(whitespace));
  str.erase(str.find_last_not_of(whitespace) + 1);
  return str;
}

class vec2 {
  public:
  float x, y;

  vec2() : x(0), y(0) {}
  vec2(float a, float b) : x(a), y(b) {}

  float dot(vec2 other) const {
    return this->x * other.x + this->y * other.y;
  }

  vec2 operator+(vec2 other) const {
    return vec2(this->x + other.x, this->y + other.y);
  }

  vec2 operator-(vec2 other) const {
    return vec2(this->x - other.x, this->y - other.y);
  }

  vec2 operator*(float scalar) const {
    return vec2(this->x * scalar, this->y * scalar);
  }

  vec2 operator/(float scalar) const {
    if (scalar == 0) {
      LOG("Division by zero error", -1);
      __debugbreak();
    }

    return vec2(this->x / scalar, this->y / scalar);
  }

  vec2 operator*(vec2 other) const {
    return vec2(this->x * other.x, this->y * other.y);
  }

  vec2 operator/(vec2 other) const {
    if (other.x == 0 || other.y == 0) {
      LOG("Division by zero error", -1);
      __debugbreak();
    }

    return vec2(this->x / other.x, this->y / other.y);
  }

  bool operator==(vec2 other) const {
    return floatCompare(this->x, other.x) && floatCompare(this->y, other.y);
  }

  bool operator!=(vec2 other) const {
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

  float dot(vec3 other) const {
    return this->x * other.x + this->y * other.y + this->z * other.z;
  }

  vec3 cross(vec3 other) const {
    return vec3(
      this->y * other.z - this->z * other.y,
      this->z * other.x - this->x * other.z,
      this->x * other.y - this->y * other.x
    );
  }

  vec3 operator+(vec3 other) const {
    return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
  }

  vec3 operator-(vec3 other) const {
    return vec3(this->x - other.x, this->y - other.y, this->z - other.z);
  }

  vec3 operator*(float scalar) const {
    return vec3(this->x * scalar, this->y * scalar, this->z * scalar);
  }

  vec3 operator/(float scalar) const {
    if (scalar == 0) {
      LOG("Division by zero error", -1);
      __debugbreak();
    }

    return vec3(this->x / scalar, this->y / scalar, this->z / scalar);
  }

  vec3 operator*(vec3 other) const {
    return vec3(this->x * other.x, this->y * other.y, this->z * other.z);
  }

  vec3 operator/(vec3 other) const {
    if (other.x == 0 || other.y == 0 || other.z == 0) {
      LOG("Division by zero error", -1);
      __debugbreak();
    }

    return vec3(this->x / other.x, this->y / other.y, this->z / other.z);
  }

  bool operator==(vec3 other) const {
    return floatCompare(this->x, other.x) && floatCompare(this->y, other.y) && floatCompare(this->z, other.z);
  }

  bool operator!=(vec3 other) const {
    return !(*this == other);
  }

  vec2 toVec2() const {
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

class mat2 {
  public:
  float m[2][2];

  mat2() {
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        m[i][j] = i == j ? 1 : 0;
      }
    }
  }

  mat2(float a, float b, float c, float d) {
    m[0][0] = a; m[0][1] = b;
    m[1][0] = c; m[1][1] = d;
  }

  mat2(vec2 col1, vec2 col2) {
    m[0][0] = col1.x; m[0][1] = col1.y;
    m[1][0] = col2.x; m[1][1] = col2.y;
  }

  mat2(float matrix[]) {
    if (sizeof(matrix) / 32 != 4) {
      LOG("Invalid matrix size", -1);
      __debugbreak();
    }

    m[0][0] = matrix[0]; m[0][1] = matrix[1];
    m[1][0] = matrix[2]; m[1][1] = matrix[3];
  }

  vec2 operator*(vec2 v) const {
    return vec2(
      m[0][0] * v.x + m[0][1] * v.y,
      m[1][0] * v.x + m[1][1] * v.y
    );
  }

  mat2 operator*(mat2 other) const {
    mat2 result;
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        result.m[i][j] = 0;
        for (int k = 0; k < 2; k++) {
          result.m[i][j] += this->m[i][k] * other.m[k][j];
        }
      }
    }
    return result;
  }

  mat2 operator/(float scalar) const {
    if (scalar == 0) {
      LOG("Division by zero error", -1);
      __debugbreak();
    }

    return mat2(
      m[0][0] / scalar, m[0][1] / scalar,
      m[1][0] / scalar, m[1][1] / scalar
    );
  }

  float determinant() const {
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
  }

  mat2 adjugate() const {
    return mat2(
      m[1][1], -m[0][1],
      -m[1][0], m[0][0]
    );
  }

  mat2 inverse() const {
    float det = determinant();
    if (det == 0) {
      LOG("Matrix is not invertible", -1);
      __debugbreak();
    }
    return adjugate() / det;
  }
};

class mat3 {
  public:
  float m[3][3];

  mat3() {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        m[i][j] = i == j ? 1 : 0;
      }
    }
  }

  mat3(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
    m[0][0] = a; m[0][1] = b; m[0][2] = c;
    m[1][0] = d; m[1][1] = e; m[1][2] = f;
    m[2][0] = g; m[2][1] = h; m[2][2] = i;
  }

  mat3(vec3 col1, vec3 col2, vec3 col3) {
    m[0][0] = col1.x; m[0][1] = col1.y; m[0][2] = col1.z;
    m[1][0] = col2.x; m[1][1] = col2.y; m[1][2] = col2.z;
    m[2][0] = col3.x; m[2][1] = col3.y; m[2][2] = col3.z;
  }

  vec3 operator*(vec3 v) {
    return vec3(
      m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
      m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
      m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
    );
  }

  mat3 operator*(mat3 other) const {
    mat3 result;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        result.m[i][j] = 0;
        for (int k = 0; k < 3; k++) {
          result.m[i][j] += this->m[i][k] * other.m[k][j];
        }
      }
    }
    return result;
  }

  mat3 operator/(float scalar) const {
    if (scalar == 0) {
      LOG("Division by zero error", -1);
      __debugbreak();
    }
    return mat3(
      m[0][0] / scalar, m[0][1] / scalar, m[0][2] / scalar,
      m[1][0] / scalar, m[1][1] / scalar, m[1][2] / scalar,
      m[2][0] / scalar, m[2][1] / scalar, m[2][2] / scalar
    );
  }

  float determinant() const {
    return (
      m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
      m[0][1] * (m[1][0] * m[2][2] - m[2][0] * m[1][2]) +
      m[0][2] * (m[1][0] * m[2][1] - m[2][0] * m[1][1])
    );
  }

  mat3 adjugate() const {
    mat3 adj;
    adj.m[0][0] =  (m[1][1]*m[2][2] - m[1][2]*m[2][1]);
    adj.m[0][1] = -(m[0][1]*m[2][2] - m[0][2]*m[2][1]);
    adj.m[0][2] =  (m[0][1]*m[1][2] - m[0][2]*m[1][1]);

    adj.m[1][0] = -(m[1][0]*m[2][2] - m[1][2]*m[2][0]);
    adj.m[1][1] =  (m[0][0]*m[2][2] - m[0][2]*m[2][0]);
    adj.m[1][2] = -(m[0][0]*m[1][2] - m[0][2]*m[1][0]);

    adj.m[2][0] =  (m[1][0]*m[2][1] - m[1][1]*m[2][0]);
    adj.m[2][1] = -(m[0][0]*m[2][1] - m[0][1]*m[2][0]);
    adj.m[2][2] =  (m[0][0]*m[1][1] - m[0][1]*m[1][0]);
    return adj;
  }

  mat3 inverse() const {
    float det = this->determinant();
    if (det == 0) {
      LOG("Matrix is not invertible", -1);
      __debugbreak();
    }
    return this->adjugate() / det;
  }
};