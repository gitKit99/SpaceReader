#include "KinectSensor.h"

#include <cmath>
#include <cstdio>
#include <iostream>


void KinectSensor::getDepthData(float* fdest)
{
	long* depth2rgb = (long*)depthToRgbMap;
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
	if (sensor->NuiImageStreamGetNextFrame(depthStream, 0, &imageFrame) < 0) return;
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0) {
		int i, j;
		const USHORT* curr = (const USHORT*)LockedRect.pBits;
		for (j = 0; j < height; j++) {
			for (i = 0; i < width; i++) {
				// Get depth of pixel in millimeters
				USHORT depth = NuiDepthPixelToDepth(*curr++);
				// Store coordinates of the point corresponding to this pixel
				Vector4 pos = NuiTransformDepthImageToSkeleton(i, j, depth << 3, NUI_IMAGE_RESOLUTION_640x480);
				kinectData[j][i].pos.x = pos.x / pos.w;
				kinectData[j][i].pos.y = pos.y / pos.w;
				kinectData[j][i].pos.z = pos.z / pos.w;
				// Store the index into the color array corresponding to this pixel
				NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
					NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_RESOLUTION_640x480, NULL,
					i, j, depth << 3, depth2rgb, depth2rgb + 1);
				depth2rgb += 2;
			}
		}
	}
	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(depthStream, &imageFrame);
}

void KinectSensor::getRgbData(float* fdest)
{
	long* depth2rgb = (long*)depthToRgbMap;
	NUI_IMAGE_FRAME imageFrame;
	NUI_LOCKED_RECT LockedRect;
	if (sensor->NuiImageStreamGetNextFrame(rgbStream, 0, &imageFrame) < 0) return;
	INuiFrameTexture* texture = imageFrame.pFrameTexture;
	texture->LockRect(0, &LockedRect, NULL, 0);
	if (LockedRect.Pitch != 0) {
		const BYTE* start = (const BYTE*)LockedRect.pBits;
		int i, j;
		for (j = 0; j < height; j++) {
			for (i = 0; i < width; i++) {
				// Determine rgb color for each depth pixel
				long x = *depth2rgb++;
				long y = *depth2rgb++;
				// If out of bounds, then don't color it at all
				if (x < 0 || y < 0 || x > width || y > height) {
					//for (int n = 0; n < 3; ++n) *(fdest++) = 0.0f;
					kinectData[j][i].color.x = 0.f;
					kinectData[j][i].color.y = 0.f;
					kinectData[j][i].color.z = 0.f;
				}
				else {
					const BYTE* curr = start + (x + width * y) * 4;
					kinectData[j][i].color.x = curr[2] / 255.0f;
					kinectData[j][i].color.y = curr[1] / 255.0f;
					kinectData[j][i].color.z = curr[0] / 255.0f;
					//for (int n = 0; n < 3; ++n) *(fdest++) = curr[2 - n] / 255.0f;
				}
			}
		}
	}
	texture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(rgbStream, &imageFrame);
}

void KinectSensor::convertToSpacePoints(Vertex** data)
{
	spacePoints.clear();
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (style == DisplayStyle::RECT_GRID && ((j % step_j != 0) && (i % step_i != 0)))
				continue;

			if (style != DisplayStyle::RECT_GRID && ((j % step_j != 0) || (i % step_i != 0)))
				continue;

			spacePoints.push_back(Vertex{ data[j][i] });
		}
	}
}

bool KinectSensor::shouldSkip(int j, int i) {
	return true;
}

KinectSensor::KinectSensor()
{
	kinectData = new Vertex* [height];
	for (size_t i = 0; i < height; i++)
		kinectData[i] = new Vertex[width];
}

KinectSensor::~KinectSensor()
{
	for (size_t i = 0; i < height; i++) {
		delete[] kinectData[i];
	}
	delete[] kinectData;
}

bool KinectSensor::initKinect()
{
	// Get a working kinect sensor
	int numSensors;
	setDisplayStyle(style);

	if ((NuiGetSensorCount(&numSensors) < 0) || (numSensors < 1)
			|| (NuiCreateSensorByIndex(0, &sensor) < 0)) {
		for (size_t i = 0; i < height; i++) {
			delete[] kinectData[i];
		}
		delete[] kinectData;
		kinectData = sensorMock.readData(width, height);
		return false;
	}

	// Initialize sensor
	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, // Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,                // Image resolution
		0,        // Image stream flags, e.g. near mode
		2,        // Number of frames to buffer
		NULL,     // Event handle
		&depthStream);
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, // Depth camera or rgb camera?
		NUI_IMAGE_RESOLUTION_640x480,                // Image resolution
		0,      // Image stream flags, e.g. near mode
		2,      // Number of frames to buffer
		NULL,   // Event handle
		&rgbStream);
	sensor->NuiCameraElevationGetAngle(&cameraAngle);
	return sensor;
}

const std::vector<Vertex>& KinectSensor::getKinectData()
{
	/*float* ptr = new float[3 * pointsCount];
	if (ptr) {
		getDepthData(ptr);
		//convertToSpacePoints(ptr, true);
	}
	ptr = new float[3 * pointsCount];
	if (ptr) {
		getRgbData(ptr);
		//convertToSpacePoints(ptr, false);
	}*/
	float* ptr = nullptr;
	if (sensor != nullptr) {
		getDepthData(ptr);
		getRgbData(ptr);
	}
	convertToSpacePoints(kinectData);
	return spacePoints;
}

void KinectSensor::setDisplayStyle(DisplayStyle inStyle)
{
	style = inStyle;

	switch (style)
	{
	case DisplayStyle::ALL_POINTS:
		step_i = 1;
		step_j = 1;
		break;
	case DisplayStyle::RECT_GRID:
		step_i = 10;
		step_j = 10;
		break;
	case DisplayStyle::LINES:
		step_i = 10;
		step_j = 1;
		break;
	case DisplayStyle::CHESS_ORDER:
		step_i = 4;
		step_j = 4;
		break;
	default:
		break;
	}
}

void KinectSensor::increaseCameraAngle()
{
	cameraAngle++;
	if (cameraAngle > 27)
		cameraAngle = 27;
	
	sensor->NuiCameraElevationSetAngle(cameraAngle);
}

void KinectSensor::decreaseCameraAngle()
{
	cameraAngle--;
	if (cameraAngle < -27)
		cameraAngle = -27;

	sensor->NuiCameraElevationSetAngle(cameraAngle);
}

void KinectSensor::writeDataToFile()
{
	sensorMock.writeData(kinectData, width, height);
}
