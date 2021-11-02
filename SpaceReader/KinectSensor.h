#pragma once

#include <vector>

#include <Windows.h>
#include <Ole2.h>

#include <NuiApi.h>
#include <NuiImageCamera.h>
#include <NuiSensor.h>

#include "GLRenderSystem.h"

enum class DisplayStyle {
	ALL_POINTS,
	RECT_GRID,
	LINES,
	CHESS_ORDER
};

class KinectSensor
{
private:
	static const int width = 640;
	static const int height = 480;

	int pointsCount = width * height;
	HANDLE depthStream;
	HANDLE rgbStream;
	INuiSensor* sensor;
	long depthToRgbMap[width * height * 2];
	std::vector<SpacePoint> spacePoints;
	DisplayStyle style = DisplayStyle::ALL_POINTS;
	int step_i = 1;
	int step_j = 1;
	int readHeight = height;
	int readWidth = width;

	SpacePoint** kinectData;

	long cameraAngle;

	void getDepthData(float* dest);
	void getRgbData(float* dest);
	void convertToSpacePoints(SpacePoint** data);
	bool shouldSkip(int j, int i);
public:
	KinectSensor();
	bool initKinect();
	const std::vector<SpacePoint>& getKinectData();
	int getWidth() { return width; }
	int getHeight() { return height; }
	DisplayStyle getDisplayStyle() { return style; }
	void setDisplayStyle(DisplayStyle inStyle);
	void increaseCameraAngle();
	void decreaseCameraAngle();
};

