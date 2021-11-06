#include "KinectView.h"

KinectView::KinectView(GLRenderSystem& inRs, const std::string& title, Application& app)
	: View(inRs, title, W_WIDTH, W_HEIGHT, app)
{
	kinect.initKinect();
}

void KinectView::render()
{
	rs->renderSpacePoints(kinect.getKinectData(), rHelper.drawWithColor);
}

void KinectView::onKeyCallback(KeyCode keyCode, Action action, Modifier mods) {
	if (action == Action::Press) {
		switch (keyCode) {
		case KeyCode::Key_C:
			rHelper.drawWithColor = !rHelper.drawWithColor;
			break;
		case KeyCode::Key_S:
			rHelper.displayStyle = (rHelper.displayStyle + 1) % 4;
			kinect.setDisplayStyle(static_cast<DisplayStyle>(rHelper.displayStyle));
			break;
		default:
			break;
		}
	}

	if (keyCode == KeyCode::Up)
		kinect.increaseCameraAngle();
	if (keyCode == KeyCode::Down)
		kinect.decreaseCameraAngle();
}
