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
			pcl::PointNormal* tN = tPtr->getVertex(0).first;
			glm::vec3 triNormal = glm::normalize(glm::vec3{ tN->normal_x, tN->normal_y, tN->normal_z });
			for (int i = 0; i < 3; i++) {
				pcl::PointNormal* p = tPtr->getVertex(i).first;
				if (!p) {
					std::cout << "Triangle point was null" << std::endl;
					break;
				}
				Vertex vert;
				vert.pos = glm::vec3{ p->x, p->y, p->z };
				vert.normal = triNormal;
				resultMesh->push_back(vert);
			}
		}
		else
			std::cout << "Triangle ptr was nullptr" << std::endl;
	}

	return resultMesh;
}
