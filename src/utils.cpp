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

class Vec4 {
  public:
  union {
    struct { float x; float y; float z; float w; };
    struct { float r; float g; float b; float a; };
  };

  Vec4() : x(0), y(0), z(0), w(0) {}
  Vec4(float a, float b) : x(a), y(b), z(0), w(0) {}
  Vec4(float a, float b, float c) : x(a), y(b), z(c), w(0) {}
  Vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}


  static float dot(const Vec4 &v1, const Vec4 &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
  }

  static Vec4 cross(const Vec4 &v1, Vec4 &v2) {
    return Vec4(
      v1.y * v2.z - v1.z * v2.y,
      v1.z * v2.x - v1.x * v2.z,
      v1.x * v2.y - v1.y * v2.x,
      0
    );
  }
};

Vec4 operator+(const Vec4 &v1, const Vec4 &v2) {
  return Vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

Vec4 operator-(const Vec4 &v1, const Vec4 &v2) {
  return Vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

Vec4 operator*(const Vec4 &v, float s) {
  return Vec4(v.x * s, v.y * s, v.z * s, v.w * s);
}

Vec4 operator/(const Vec4 &v, float s) {
  return Vec4(v.x / s, v.y / s, v.z / s, v.w / s);
}

Vec4 operator/(float s, const Vec4 &v) {
  return Vec4(s / v.x, s / v.y, s / v.z, s / v.w);
}

Vec4 operator*(const Vec4 &v1, const Vec4 &v2) {
  return Vec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
}

Vec4 operator/(const Vec4 &v1, const Vec4 &v2) {
  return Vec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
}

class Mat4 {
  public:
  float m[4][4];

  const static Mat4 IDENTITY;

  Mat4() {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        m[i][j] = 0;
      }
    }
  }

  Mat4(Vec4 ihat, Vec4 jhat, Vec4 khat, Vec4 lhat) {
    m[0][0] = ihat.x;  m[1][0] = jhat.x;  m[2][0] = khat.x;  m[3][0] = lhat.x;
    m[0][1] = ihat.y;  m[1][1] = jhat.y;  m[2][1] = khat.y;  m[3][1] = lhat.y;
    m[0][2] = ihat.z;  m[1][2] = jhat.z;  m[2][2] = khat.z;  m[3][2] = lhat.z;
    m[0][3] = ihat.w;  m[1][3] = jhat.w;  m[2][3] = khat.w;  m[3][3] = lhat.w;
  }

  Mat4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p) {
    this->m[0][0] = a; this->m[1][0] = e; this->m[2][0] = i; this->m[3][0] = m;
    this->m[0][1] = b; this->m[1][1] = f; this->m[2][1] = j; this->m[3][1] = n;
    this->m[0][2] = c; this->m[1][2] = g; this->m[2][2] = k; this->m[3][2] = o;
    this->m[0][3] = d; this->m[1][3] = h; this->m[2][3] = l; this->m[3][3] = p;
  }

  float determinant() const {
    float det = 0.0f;
    for (int col = 0; col < 4; ++col) {
      float minor[3][3];
      for (int i = 1; i < 4; ++i) {
        int mj = 0;
        for (int j = 0; j < 4; ++j) {
          if (j == col) continue;
          minor[mj][i - 1] = m[j][i];
          ++mj;
        }
      }
      float minorDet =
        minor[0][0] * (minor[1][1] * minor[2][2] - minor[1][2] * minor[2][1]) -
        minor[0][1] * (minor[1][0] * minor[2][2] - minor[1][2] * minor[2][0]) +
        minor[0][2] * (minor[1][0] * minor[2][1] - minor[1][1] * minor[2][0]);
      det += ((col % 2 == 0) ? 1.0f : -1.0f) * m[col][0] * minorDet;
    }
    return det;
  }

  Mat4 adjugate() const {
    Mat4 result;
    for (int col = 0; col < 4; ++col) {
      for (int row = 0; row < 4; ++row) {
        float minor[3][3];
        int mi = 0;
        for (int i = 0; i < 4; ++i) {
          if (i == col) continue;
          int mj = 0;
          for (int j = 0; j < 4; ++j) {
            if (j == row) continue;
            minor[mj][mi] = m[i][j];
            ++mj;
          }
          ++mi;
        }
        float minorDet =
          minor[0][0] * (minor[1][1] * minor[2][2] - minor[1][2] * minor[2][1]) -
          minor[0][1] * (minor[1][0] * minor[2][2] - minor[1][2] * minor[2][0]) +
          minor[0][2] * (minor[1][0] * minor[2][1] - minor[1][1] * minor[2][0]);
        result.m[col][row] = ((col + row) % 2 == 0 ? 1.0f : -1.0f) * minorDet;
      }
    }
    Mat4 adj;
    for (int col = 0; col < 4; ++col)
      for (int row = 0; row < 4; ++row)
        adj.m[col][row] = result.m[row][col];
    return adj;
  }

  Mat4 inverse() const {
    float det = determinant();
    if (det == 0) {
      LOG("Matrix is not invertible", -1);
      __debugbreak();
    }
    Mat4 adj = adjugate();
    Mat4 result;
    for (int col = 0; col < 4; ++col)
      for (int row = 0; row < 4; ++row)
        result.m[col][row] = adj.m[col][row] / det;
    return result;
  }
};

Mat4 operator*(const Mat4 &a, const Mat4 &b) {
  Mat4 result;
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      result.m[col][row] =
        a.m[0][row] * b.m[col][0] +
        a.m[1][row] * b.m[col][1] +
        a.m[2][row] * b.m[col][2] +
        a.m[3][row] * b.m[col][3];
    }
  }
  return result;
}

Vec4 operator*(const Mat4 &m, const Vec4 &v) {
  return Vec4(
    m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0] * v.w,
    m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1] * v.w,
    m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2] * v.w,
    m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] * v.w
  );
}

const Mat4 Mat4::IDENTITY = Mat4(
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
);
