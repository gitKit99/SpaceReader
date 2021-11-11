#pragma once

#include <utility>
#include <vector>
#include <string>

#include "GLRenderSystem.h"

typedef std::pair<Vertex, int> PointData;

class Edge
{
public:
	Edge();
	Edge(const PointData& v0, const PointData& v1, const PointData& opposite, glm::vec3 center);
	Edge(const Edge& other);
	~Edge();

	Edge& operator=(const Edge& other);
	bool operator<(const Edge& other) const;
	bool operator==(const Edge& other) const;
	bool operator!=(const Edge& other) const;

	inline void setActive(const bool _active)
	{
		active = _active;
	}

	inline bool isActive() const
	{
		return active;
	}

	inline PointData getVertex(const int n) const
	{
		if (n < 2)
			return vertices[n];
		else
			return std::make_pair<Vertex, int>(Vertex{}, -1);
	}

	inline PointData getOppositeVertex()
	{
		return oppositeVertex;
	}

	inline glm::vec3 getMiddlePoint() const
	{
		return middlePoint;
	}

	inline glm::vec3 getBallCenter() const
	{
		return ballCenter;
	}

	inline double getPivotingRadius() const
	{
		return pivotingRadius;
	}

private:
	inline void setId()
	{
		static long currentId = 0;
		id = currentId++;
	}

	std::vector<PointData> vertices;
	PointData oppositeVertex;
	glm::vec3 ballCenter;
	glm::vec3 middlePoint;
	double pivotingRadius;
	bool active;

	long id;
	std::string str;
};

