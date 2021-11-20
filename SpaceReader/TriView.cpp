#include "TriView.h"

#include <iostream>

TriView::TriView(GLRenderSystem& inRs, const std::vector<Vertex>& points, 
	const std::string& title, unsigned int width, unsigned int height, Application& app)
	: View(inRs, title, width, height, app)
{
	setData(points);

	const double epsilon = 1.e-7;
	std::cout << "Distance: ";
	for (size_t i = 1; i < 5; i++) {
		double distance = glm::distance(points.at(i).pos, points.at(i - 1).pos);
		std::cout << distance << " ";
	}
	std::cout << std::endl;

	std::cout << "TriView data count " << data->size() << std::endl;
	rHelper.drawWithColor = false;
	rHelper.pointSize = 2.5f;
}

void TriView::render()
{
	if (mesh == nullptr && data) {
		rs->renderSpacePoints(*data, rHelper);
	}
}
