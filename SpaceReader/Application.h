#pragma once

#include "View.h"

#include <string>
#include <array>


class Application
{
public:
	Application();
	void run();
	void createKinectView();
	void createTriView(const std::vector<Vertex>& points, unsigned int width, unsigned int height);
	void detachView(View& view);

protected:
	std::unique_ptr<GLRenderSystem> rs;
	std::array<std::unique_ptr<View>, 2> views;
};

