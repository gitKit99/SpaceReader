#include "Application.h"
#include "KinectView.h"
#include "TriView.h"

#include <iostream>

Application::Application()
{
	rs.reset(new GLRenderSystem());
}

void Application::createKinectView()
{
	std::string title = "Kinect scene";
	views[0].reset(new KinectView(*rs, title, *this));
}

void Application::createTriView(const std::vector<Vertex>& points,
	unsigned int width, unsigned int height)
{
	std::string title = "Triangulation scene";
	std::cout << "Points count " << points.size() << std::endl;
	views[1].reset(new TriView(*rs, points, title, width, height, *this));
}

void Application::detachView(View& viewForDetach)
{
	for (std::unique_ptr<View>& view : views) {
		if (view.get() == &viewForDetach) {
			delete view.release();
			break;
		}
	}
}

void Application::run() {
	while (1) {
		if (views[0] == nullptr && views[1] == nullptr)
			break;

		for (std::unique_ptr<View>& view : views)
			if (view != nullptr)
				view->update();
	}
}


