#include "KinectView.h"

KinectView::KinectView(GLRenderSystem& inRs, const std::string& title, Application& app)
	: View(inRs, title, W_WIDTH, W_HEIGHT, app)
{
	kinect.initKinect();
}

void KinectView::render()
{
	rs->renderSpacePoints(kinect.getKinectData(), true);
}
