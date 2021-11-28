#pragma once

#include "GLRenderSystem.h"

static class Triangulation
{
public:
	static std::vector<Vertex>* ballPivotingTriangulation(const std::vector<Vertex>& points);
};
