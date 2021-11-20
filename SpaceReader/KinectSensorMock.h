#pragma once

#include <fstream>
#include <string>

struct Vertex;

class KinectSensorMock
{
public:
	void writeData(Vertex** data, unsigned int width, unsigned int height, 
		const std::string& filepath = getDefaultFilepath());

	Vertex** readData(unsigned int width, unsigned int height, 
		const std::string& filepath = getDefaultFilepath());
private:
	static const std::string& getDefaultFilepath() { static const std::string 
		dFilePath = "E:\\projects\\coursach\\SpaceReader\\data\\kinect_data.txt"; return dFilePath; }
};

