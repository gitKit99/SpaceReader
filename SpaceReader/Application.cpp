#include "Application.h"
#include "KinectView.h"
#include "TriView.h"

#include <iostream>
#include <sstream>

void Application::readData(const std::string& filepath) {

	std::ifstream file(filepath);
	std::cout << "Loading data from file" << std::endl;

	if (!file) {
		std::cout << "FIlepath is incorrect" << std::endl;
		return;
	}

	std::string buffer;
	std::vector<std::string> tokens;
	int maxTokensCount = 8;
	tokens.resize(maxTokensCount);

	while (true) {
		if (file.eof() || file.fail()) {
			//std::cout << "File wasn't read to the end" << std::endl;
			return;
		}

		std::getline(file, buffer);

		std::istringstream line(buffer);
		int tokenCount{ 0 };
		while (!(line.eof() || line.fail()
			|| (line.rdbuf()->in_avail() == 0)))
		{
			if (tokenCount >= maxTokensCount)
			{
				maxTokensCount = tokenCount + 1;
				tokens.resize(maxTokensCount);
				std::cout << "Read line was longer than expected";
			}
			line >> tokens[tokenCount];
			tokenCount++;
		}

		if (tokenCount < 8 || (tokens[0].compare("pos") != 0)
			|| (tokens[4].compare("color") != 0)) {
			std::cout << "Incorrect line" << std::endl;
			return;
		}

		glm::vec3 position;
		position.x = static_cast<float>(
			atof(tokens[1].c_str()));
		position.y = static_cast<float>(
			atof(tokens[2].c_str()));
		position.z = static_cast<float>(
			atof(tokens[3].c_str()));

		glm::vec3 color;
		color.x = static_cast<float>(
			atof(tokens[5].c_str()));
		color.y = static_cast<float>(
			atof(tokens[6].c_str()));
		color.z = static_cast<float>(
			atof(tokens[7].c_str()));

		Vertex v;
		v.pos = position;
		v.color = color;
		points.push_back(v);
	}

	std::cout << "File was succesfully read" << std::endl;
	file.close();
}

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

void Application::createTriView(const std::string& filepath)
{
	std::string title = "Triangulation scene";
	readData(filepath);
	views[1].reset(new TriView(*rs, points, title, 640, 480, *this));
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

void Application::writeToSTL(const std::vector<Vertex>& mesh,
	const std::string& filename)
{
	std::ofstream file;
	file.open(filename);

	if (!file) {
		std::cout << "Cannot open file " << filename << std::endl;
		return;
	}

	file << "solid model\n";
	for (auto it = mesh.cbegin(); it != mesh.cend(); it += 3)
	{
		glm::vec3 a{ (it + 1)->pos.x - it->pos.x,
			   (it + 1)->pos.y - it->pos.y,
			   (it + 1)->pos.z - it->pos.z };

		glm::vec3 b{ (it + 2)->pos.x - (it + 1)->pos.x,
			   (it + 2)->pos.y - (it + 1)->pos.y,
			   (it + 2)->pos.z - (it + 1)->pos.z };
		glm::vec3 facetNormal = glm::normalize(glm::cross(a, b));

		file << "\tfacet normal ";
		file << facetNormal.x << " " << facetNormal.y << " " <<
			facetNormal.z << std::endl;

		file << "\t\touter loop\n";
		for (int i = 0; i < 3; i++)
		{
			file << "\t\t\tvertex ";
			file << (*(it + i)).pos.x << " " << (*(it + i)).pos.y << " " <<
				(*(it + i)).pos.z << std::endl;
		}

		file << "\t\tendloop\n";
		file << "\tendfacet\n";
	}
	file << "endsolid model\n";
	file.close();
}


