#include "Application.h"
#include "KinectView.h"

Application::Application()
{
	rs.reset(new GLRenderSystem());
}

void Application::createView(const std::string& title, unsigned int width, unsigned int height)
{
}

void Application::createKinectView()
{
	std::string title = "Kinect reader";
	views[0].reset(new KinectView(*rs, title, *this));
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


