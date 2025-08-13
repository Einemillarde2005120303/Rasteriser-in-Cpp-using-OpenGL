#include <includes.hpp>

std::tuple<std::vector<Vec4>, std::vector<Vec4>, std::vector<Vec4>, std::vector<unsigned int>> parseObjFile(std::string path);

float signedTriangleArea(Vec4 a, Vec4 b, Vec4 c);

bool pointInsideTriangle(Vec4 p, Vec4 a, Vec4 b, Vec4 c, Vec4* barycentricCoords);

void drawTriangles(std::vector<unsigned char> pixels, std::vector<float> depthBuffer, std::vector<Vec4> triangles, std::vector<Vec4> colours);

void createImage(std::vector<unsigned char> pixels, Camera camera);
