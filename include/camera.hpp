#include <includes.hpp>

class Camera {
  public:
  Mat4 transform;

  float n;
  float f;

  Camera();

  Vec4 transformPoint(Vec4 p) const;
};