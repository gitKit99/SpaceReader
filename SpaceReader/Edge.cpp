#include "Edge.h"


Edge::Edge()
{
	vertices.clear();
	oppositeVertex = std::make_pair<Vertex, int>(Vertex{}, -1);
	ballCenter = middlePoint = glm::vec3();
	pivotingRadius = 0;
	active = false;
	setId();
	str = "";
}

Edge::Edge(const PointData& v0, const PointData& v1, const PointData& opposite, 
	glm::vec3 center)
{
	vertices.push_back(v0);
	vertices.push_back(v1);
	oppositeVertex = opposite;

	ballCenter = center;
	middlePoint = glm::vec3{ (v0.first.pos.x + v1.first.pos.x) * 0.5,
							(v0.first.pos.y + v1.first.pos.y) * 0.5,
							(v0.first.pos.z + v1.first.pos.z) * 0.5 };

	pivotingRadius = glm::length(middlePoint - center);

	active = true;
	setId();
}

Edge::Edge(const Edge& other)
{
	vertices = other.vertices;
	oppositeVertex = other.oppositeVertex;

	ballCenter = other.ballCenter;
	pivotingRadius = other.pivotingRadius;

	middlePoint = other.middlePoint;
	active = other.active;
	id = other.id;

	str = other.str;
}

Edge::~Edge()
{
}

Edge& Edge::operator=(const Edge& other)
{
	if (this != &other)
	{
		vertices = other.vertices;
		oppositeVertex = other.oppositeVertex;

		ballCenter = other.ballCenter;
		pivotingRadius = other.pivotingRadius;

		middlePoint = other.middlePoint;
		active = other.active;
		id = other.id;

		str = other.str;
	}

	return *this;
}

bool Edge::operator<(const Edge& other) const
{
	return pivotingRadius < other.pivotingRadius;
}

bool Edge::operator==(const Edge& other) const
{
	bool equals = (vertices[0] == other.vertices[0] || vertices[0] == other.vertices[1]) 
			&& (vertices[1] == other.vertices[0] || vertices[1] == other.vertices[1]);

	return equals;
}

bool Edge::operator!=(const Edge& other) const
{
	return !this->operator==(other);
}
