#include <includes.hpp>

const unsigned int WINDOW_WIDTH;
const unsigned int WINDOW_HEIGHT;

std::string readFile(std::string path);

bool floatCompare(float a, float b);

float randomFloat(float min, float max);

std::vector<std::string> splitStr(std::string str, char ch);

std::string trimStr(std::string str);

class Vec4 {
  public:
  union {
    struct { float x; float y; float z; float w; };
    struct { float r; float g; float b; float a; };
  };

  Vec4();
  Vec4(float a, float b);
  Vec4(float a, float b, float c);
  Vec4(float a, float b, float c, float d);


  static float dot(const Vec4 &v1, const Vec4 &v2);

  static Vec4 cross(const Vec4 &v1, Vec4 &v2);
};

Vec4 operator+(const Vec4 &v1, const Vec4 &v2);

Vec4 operator-(const Vec4 &v1, const Vec4 &v2);

Vec4 operator*(const Vec4 &v, float s);

Vec4 operator/(const Vec4 &v, float s);

Vec4 operator/(float s, const Vec4 &v);

Vec4 operator*(const Vec4 &v1, const Vec4 &v2);

Vec4 operator/(const Vec4 &v1, const Vec4 &v2);

class Mat4 {
  public:
  float m[4][4];

  const static Mat4 IDENTITY;

  Mat4();

  Mat4(Vec4 ihat, Vec4 jhat, Vec4 khat, Vec4 lhat);

  Mat4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p);

  float determinant() const;

  Mat4 adjugate() const;

  Mat4 inverse() const;
};

Mat4 operator*(const Mat4 &a, const Mat4 &b);

Vec4 operator*(const Mat4 &m, const Vec4 &v);
