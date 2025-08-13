#pragma once

#include <includes.hpp>

std::tuple<std::vector<vec3>, std::vector<vec3>, std::vector<vec2>, std::vector<unsigned int>> parseObjFile(std::string path) {
  std::vector<vec3> vertices;
  std::vector<vec3> vertexNormals;
  std::vector<vec2> vertexTextures;
  std::vector<unsigned int> faces;

  std::vector<std::string> lines = splitStr(trimStr(readFile(path)), '\n');

  for (const std::string& line : lines) {
    if (line.starts_with("v ")) {
      std::vector<std::string> parts = splitStr(line.substr(2), ' ');
      vertices.emplace_back(std::stof(parts[0]), std::stof(parts[1]), std::stof(parts[2]));
    } else if (line.starts_with("vn ")) {
      std::vector<std::string> parts = splitStr(line.substr(3), ' ');
      vertexNormals.emplace_back(std::stof(parts[0]), std::stof(parts[1]), std::stof(parts[2]));
    } else if (line.starts_with("vt ")) {
      std::vector<std::string> parts = splitStr(line.substr(3), ' ');
      vertexTextures.emplace_back(std::stof(parts[0]), std::stof(parts[1]));
    } else if (line.starts_with("f ")) {
      std::vector<std::string> parts = splitStr(line.substr(2), ' ');
      for (const std::string& part : parts) {
        std::vector<std::string> indices = splitStr(part, '/');
        faces.push_back(static_cast<unsigned int>(std::stoi(indices[0]) - 1));
        faces.push_back(static_cast<unsigned int>(std::stoi(indices[1]) - 1));
        faces.push_back(static_cast<unsigned int>(std::stoi(indices[2]) - 1));
      }
    }
  }

  return std::make_tuple(vertices, vertexNormals, vertexTextures, faces);
}

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

void drawTriangles(std::vector<unsigned char> pixels, std::vector<float> depthBuffer, std::vector<vec3> triangles, std::vector<vec3> colours) {
  for (unsigned int tri = 0; tri < triangles.size() / 3; tri++) {
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

void createImage(std::vector<unsigned char> pixels) {
  std::tuple<std::vector<vec3>, std::vector<vec3>, std::vector<vec2>, std::vector<unsigned int>> objData = parseObjFile("assets/scene.obj");

  std::vector<vec3> vertices = std::get<0>(objData);
  std::vector<vec3> normals = std::get<1>(objData);
  std::vector<vec2> texCoords = std::get<2>(objData);
  std::vector<unsigned int> indices = std::get<3>(objData);

  std::vector<vec3> triangles;
  for (unsigned int i = 0; i < indices.size() / 9; i += 9) {
    triangles.push_back(vertices[indices[i + 0]]);
    triangles.push_back(vertices[indices[i + 3]]);
    triangles.push_back(vertices[indices[i + 6]]);
  }

  std::vector<vec3> colours(triangles.size());

  for (unsigned int i = 0; i < triangles.size(); i++) {
    colours[i] = vec3(randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f));
  }

  std::vector<float> depthBuffer(WINDOW_WIDTH * WINDOW_HEIGHT, INFINITY);

  std::fill(pixels.begin(), pixels.end(), 0);

  drawTriangles(pixels, depthBuffer, triangles, colours);
}