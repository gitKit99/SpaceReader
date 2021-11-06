#pragma once

#include "View.h"

#include <string>
#include <array>


class Application
{
public:
	Application();
	void run();
	void createView(const std::string& title, unsigned int width, unsigned int height);
	void createKinectView();
	void detachView(View& view);

protected:
	std::unique_ptr<GLRenderSystem> rs;
	std::array<std::unique_ptr<View>, 2> views;
};

