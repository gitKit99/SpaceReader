#include "TriView.h"

#include <iostream>

TriView::TriView(GLRenderSystem& inRs, const std::vector<Vertex>& points, 
	const std::string& title, unsigned int width, unsigned int height, Application& app)
	: View(inRs, title, width, height, app)
{
	setData(points);

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
