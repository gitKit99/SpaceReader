#include "KinectSensorMock.h"
#include "KinectSensor.h"

#include <iostream>
#include <sstream>

void KinectSensorMock::writeData(Vertex** data, unsigned int width, 
	unsigned int height, const std::string& filepath)
{
	std::cout << "Writing data in file" << std::endl;

	if (data == nullptr) {
		std::cout << "Data was incorrect" << std::endl;
		return;
	}

	std::ofstream file(filepath);

	if (!file) {
		std::cout << "FIlepath is incorrect" << std::endl;
		return;
	}

	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			const Vertex& v = data[i][j];
			file << "pos " << v.pos.x << " " << v.pos.y << " " << v.pos.z <<
				" color " << v.color.x << " " << v.color.y << " " << v.color.z 
				<< std::endl;
		}
	}

	file.close();
	std::cout << "File was written successfully" << std::endl;
}

Vertex** KinectSensorMock::readData(unsigned int width, unsigned int height, const std::string& filepath)
{
	std::ifstream file(filepath);
	std::cout << "Loading data from file" << std::endl;

	if (!file) {
		std::cout << "FIlepath is incorrect" << std::endl;
		return nullptr;
	}

	Vertex** data = new Vertex * [height];
	for (size_t i = 0; i < height; i++)
		data[i] = new Vertex[width];

	std::string buffer;
	std::vector<std::string> tokens;
	int maxTokensCount = 0;
	tokens.resize(maxTokensCount);

	for (size_t lineIndex = 0; lineIndex < width * height; lineIndex++) {
		if (file.eof() || file.fail()) {
			std::cout << "File wasn't read to the end" << std::endl;
			return data;
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
			return data;
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

		int hIndex = lineIndex / width;
		int wIndex = lineIndex % width;

		data[hIndex][wIndex].pos = position;
		data[hIndex][wIndex].color = color;
	}

	std::cout << "File was succesfully read" << std::endl;
	file.close();

	return data;
}
