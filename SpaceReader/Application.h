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
	void createTriView(const std::string& filepath);
	void detachView(View& view);
	static void writeToSTL(const std::vector<Vertex>& mesh, const std::string& filepath);

protected:
	std::unique_ptr<GLRenderSystem> rs;
	std::array<std::unique_ptr<View>, 2> views;

	void readData(const std::string& filepath);
	std::vector<Vertex> points;
};

