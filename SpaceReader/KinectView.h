#pragma once

#include "View.h"
#include "KinectSensor.h"

struct RenderHelper {
	bool drawWithColor = true;
	int displayStyle = 0;
};

class KinectView : public View
{
public:
	KinectView(GLRenderSystem& inRs, const std::string& title, Application& app);
	virtual void render() override;

protected:
	static const unsigned int W_WIDTH = 640u;
	static const unsigned int W_HEIGHT = 480u;

	virtual void onKeyCallback(KeyCode, Action, Modifier) override;

	KinectSensor kinect;
	RenderHelper rHelper;
};

