#pragma once

#include <glm/glm.hpp>

struct Plane3D
{
	glm::vec3 point;
	glm::vec3 normal;
};

static class MathUtils
{
public:
	static double map(double v, double b, double e, double tB, double tE);

};

