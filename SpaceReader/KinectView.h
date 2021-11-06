#pragma once

#include "View.h"
#include "KinectSensor.h"

class KinectView : public View
{
public:
	KinectView(GLRenderSystem& inRs, const std::string& title, Application& app);
	virtual void render() override;

protected:
	static const unsigned int W_WIDTH = 640u;
	static const unsigned int W_HEIGHT = 480u;

	KinectSensor kinect;

};

