#include "TriView.h"
#include "Triangulation.h"
#include "Application.h"

#include <iostream>

TriView::TriView(GLRenderSystem& inRs, const std::vector<Vertex>& points, 
	const std::string& title, unsigned int width, unsigned int height, Application& app)
	: View(inRs, title, width, height, app)
{
	setData(points);

	const double epsilon = 1.e-7;
	std::cout << "Distance: ";
	for (size_t i = 1; i < 5; i++) {
		double distance = glm::distance(data->at(i).pos, data->at(i - 1).pos);
		std::cout << distance << " ";
	}
	std::cout << std::endl;

	std::cout << "TriView data count " << data->size() << std::endl;
	rHelper.drawWithColor = false;
	rHelper.pointSize = 1.f;
}


void TriView::render()
{
	if (mesh) {
		if (renderMesh)
			rs->renderTriangleSoup(*mesh);
		if (renderEdges)
			rs->renderLines(*lines);
	}
	if (data) {
		rs->renderSpacePoints(*data, rHelper);
	}
}

void TriView::onKeyCallback(KeyCode key, Action action, Modifier mods)
{
	static std::string filename = "E:\\projects\\coursach\\SpaceReader\\data\\output.stl";
	if (action == Action::Press) {
		switch (key) {
		case KeyCode::Key_T:
			mesh.reset(Triangulation::ballPivotingTriangulation(*data));
			setLines();
			break;
		case KeyCode::Key_S:
			if (!mesh) {
				std::cout << "There is no mesh data to write in file" << std::endl;
				break;
			}
			Application::writeToSTL(*mesh, filename);
			std::cout << "Mesh was written to file" << std::endl;
			break;
		case KeyCode::Key_M:
			renderMesh = !renderMesh;
			break;
		case KeyCode::Key_L:
			renderEdges = !renderEdges;
			break;
		default:
			break;
		}
	}
}

void TriView::setLines() {
	if (!mesh)
		return;

	lines.reset(new std::vector<Vertex>);
	for (size_t triIndex = 0;  (triIndex + 2) < mesh->size(); triIndex += 3) {
		lines->push_back(mesh->at(triIndex));
		lines->push_back(mesh->at(triIndex + 1));
		lines->push_back(mesh->at(triIndex + 1));
		lines->push_back(mesh->at(triIndex + 2));
		lines->push_back(mesh->at(triIndex + 2));
		lines->push_back(mesh->at(triIndex));
	}
}
