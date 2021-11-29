#include "Triangulation.h"

#include <stdlib.h>
#include <string>
#include <pcl/point_types.h>
#include <ctime>
#include "Helper.h"
#include "Triangle.h"
#include "Pivoter.h"
#include "Front.h"
#include "Config.h"

const glm::vec3 KINECT_NORMAL{ 0.f, 0.f, -1.f };

std::vector<Vertex>* Triangulation::ballPivotingTriangulation(const std::vector<Vertex>& points)
{
	double ballRadius = Config::getBallRadius();

	clock_t begin = clock();

	pcl::PointCloud<pcl::PointNormal>::Ptr cloud(new pcl::PointCloud<pcl::PointNormal>());
	if (!Helper::getCloudAndNormals(points, cloud))
	{
		std::cout << "ERROR: point cloud loading failed\n";
		return nullptr;
	}
	std::cout << "Loaded " << cloud->size() << " points in cloud\n";

	Pivoter pivoter(cloud, ballRadius);
	Front front;
	std::vector<TrianglePtr> mesh;

	std::cout << "Building mesh with ball r=" << ballRadius << "\n";
	while (true)
	{
		// Pivot from the current front
		EdgePtr edge;
		while ((edge = front.getActiveEdge()) != nullptr)
		{
			std::pair<int, TrianglePtr> data = pivoter.pivot(edge);
			if (data.first != -1 && (!pivoter.isUsed(data.first) || front.inFront(data.first)))
			{
				std::cout << "Point added" << std::endl;
				mesh.push_back(data.second);
				front.joinAndFix(data, pivoter);
			}
			else
			{
				front.setInactive(edge);
				std::cout << "Set edge inactive" << std::endl;
			}
		}

		// Find a new seed
		TrianglePtr seed;
		std::cout << "Searching a seed\n";
		if ((seed = pivoter.findSeed()) != NULL)
		{
			mesh.push_back(seed);
			front.addEdges(seed);
		}
		else {
			std::cout << "Seed was null" << std::endl;
			break;
		}
	}

	if (mesh.size() <= 0) {
		std::cout << "No triangle is constructed" << std::endl;
		return new std::vector<Vertex>();
	}

	std::vector<Vertex>* resultMesh = new std::vector<Vertex>();
	resultMesh->reserve(3 * mesh.size());
	std::cout << "triangles count = " << mesh.size() << std::endl;

	for (TrianglePtr tPtr : mesh) {
		if (tPtr) {
			bool shouldInvert = false;
			glm::vec3 triNormal = calcNormal(*tPtr.get());
			if (glm::dot(triNormal, KINECT_NORMAL) < 0.f)
				shouldInvert = true;
			for (int i = 0; i < 3; i++) {
				pcl::PointNormal* p = tPtr->getVertex((shouldInvert) ? (2 - i) : i).first;
				if (!p) {
					std::cout << "Triangle point was null" << std::endl;
					break;
				}
				Vertex vert;
				vert.pos = glm::vec3{ p->x, p->y, p->z };
				vert.normal = (shouldInvert) ? -triNormal : triNormal;
				resultMesh->push_back(vert);
			}
		}
		else
			std::cout << "Triangle ptr was nullptr" << std::endl;
	}

	return resultMesh;
}

glm::vec3 Triangulation::calcNormal(const Triangle& triangle)
{
	glm::vec3 p0{ triangle.getVertex(0).first->x, triangle.getVertex(0).first->y,
		triangle.getVertex(0).first->z };
	glm::vec3 p1{ triangle.getVertex(1).first->x, triangle.getVertex(1).first->y,
		triangle.getVertex(1).first->z };
	glm::vec3 p2{ triangle.getVertex(2).first->x, triangle.getVertex(2).first->y,
		triangle.getVertex(2).first->z };

	glm::vec3 a = p1 - p0;
	glm::vec3 b = p2 - p0;

	return glm::normalize(glm::cross(a, b));
}
