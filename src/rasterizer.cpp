#include <includes.hpp>

float signedTriangleArea(vec2 a, vec2 b, vec2 c) {
  vec2 ab = b - a;
  return (c - a).dot(vec2(ab.y, -ab.x)) * 0.5f;
}

bool pointInsideTriangle(vec2 p, vec2 a, vec2 b, vec2 c, vec3* barycentricCoords) {
  float area = signedTriangleArea(a, b, c);
  if (area == 0) return false;

  float areaABP = signedTriangleArea(a, b, p) / area;
  float areaBCP = signedTriangleArea(b, c, p) / area;
  float areaCAP = signedTriangleArea(c, a, p) / area;

  float invAreaSum = 1 / (areaABP + areaBCP + areaCAP);
  barycentricCoords->x = areaBCP * invAreaSum;
  barycentricCoords->y = areaCAP * invAreaSum;
  barycentricCoords->z = areaABP * invAreaSum;

  return (areaABP >= 0 && areaBCP >= 0 && areaCAP >= 0);
}

void drawTriangles(unsigned char pixels[], float depthBuffer[], vec3 triangles[], vec3 colours[], unsigned int sizeTriangles) {
  for (unsigned int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
    depthBuffer[i] = INFINITY;
    pixels[i * 3 + 0] = 0;
    pixels[i * 3 + 1] = 0;
    pixels[i * 3 + 2] = 0;
  }

  for (unsigned int tri = 0; tri < sizeTriangles / 3; tri++) {
    vec3 v1 = (triangles[tri * 3 + 0] + vec3(1, -1, 0)) * vec3(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * -0.5f, 1);
    vec3 v2 = (triangles[tri * 3 + 1] + vec3(1, -1, 0)) * vec3(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * -0.5f, 1);
    vec3 v3 = (triangles[tri * 3 + 2] + vec3(1, -1, 0)) * vec3(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * -0.5f, 1);

    int xs[] = {(int)v1.x, (int)v2.x, (int)v3.x};
    int ys[] = {(int)v1.y, (int)v2.y, (int)v3.y};
    vec3 depths = vec3(v1.z, v2.z, v3.z);

    unsigned int minX = std::max(0, *std::min_element(std::begin(xs), std::end(xs)));
    unsigned int minY = std::max(0, *std::min_element(std::begin(ys), std::end(ys)));

    unsigned int maxX = std::min((int)WINDOW_WIDTH - 1, *std::max_element(std::begin(xs), std::end(xs)));
    unsigned int maxY = std::min((int)WINDOW_HEIGHT - 1, *std::max_element(std::begin(ys), std::end(ys)));

    for (unsigned int y = minY; y <= maxY; y++) {
      for (unsigned int x = minX; x <= maxX; x++) {
        unsigned int pIndex = y * WINDOW_WIDTH + x;
        vec3 weights;
        if (!pointInsideTriangle(vec2(x, y), v1.toVec2(), v2.toVec2(), v3.toVec2(), &weights)) continue;
        float depth = 1 / weights.dot(1 / depths);
        if (depth > depthBuffer[pIndex]) continue;
        depthBuffer[pIndex] = depth;
        pixels[pIndex * 3 + 0] = (unsigned char)(colours[tri].r * 255);
        pixels[pIndex * 3 + 1] = (unsigned char)(colours[tri].g * 255);
        pixels[pIndex * 3 + 2] = (unsigned char)(colours[tri].b * 255);
      }
    }
  }
}

void createImage(unsigned char pixels[], float depthBuffer[]) {
  vec3 triangles[] = {
    vec3(0.2, 0.9, 1),
    vec3(-0.7, -0.9, 1e-5f),
    vec3(-0.6, -0.1, 1e-5f),

    vec3(-0.3, 0.9, 1e-5f),
    vec3(-0.5, 0.9, 1e-5f),
    vec3(0.9, -0.9, 1),

    vec3(0.9, -0.4, 1e-5f),
    vec3(0.9, -0.6, 1e-5f),
    vec3(-1.5, -0.6, 1)
  };

  unsigned int sizeTriangles = 9;

  vec3 colours[sizeTriangles / 3];

  for (unsigned int i = 0; i < sizeTriangles / 3; i++) {
    colours[i] = vec3(randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f));
  }

  drawTriangles(pixels, depthBuffer, triangles, colours, sizeTriangles);
}