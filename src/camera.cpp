#pragma once

#include <includes.hpp>

class Camera {
public:
  vec3 position;
  vec3 forward;
  vec3 up;
  vec3 right;

  Camera() : position(0, 0, 0), forward(0, 0, 1), up(0, 1, 0) {
    right = forward.cross(up);
  }

  Camera(vec3 position, vec3 forward, vec3 up) : position(position), forward(forward), up(up) {
    right = forward.cross(up);
  }

  void getTransformationMatrix() const {
    // TODO: Implement camera transformation matrix calculation
  }
};
