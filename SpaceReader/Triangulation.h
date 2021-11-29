#pragma once

#include "GLRenderSystem.h"

class Triangle;

static class Triangulation
{
public:
	static std::vector<Vertex>* ballPivotingTriangulation(const std::vector<Vertex>& points);
	static glm::vec3 calcNormal(const Triangle& triangle);
};
