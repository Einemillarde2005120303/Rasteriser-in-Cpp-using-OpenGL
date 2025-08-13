#include <includes.hpp>

class Camera {
  public:
  Mat4 transform;

  float n = 0.1f;
  float f = 10;

  Camera() : transform(Mat4(Mat4::IDENTITY)) {}

  Vec4 transformPoint(Vec4 p) const {
    Vec4 relP = transform.inverse() * p;
    return Vec4(p.x / p.z, p.y / p.z, p.z);
  }
};