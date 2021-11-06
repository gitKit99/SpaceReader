#include "KinectView.h"
#include "Application.h"

KinectView::KinectView(GLRenderSystem& inRs, const std::string& title, Application& app)
	: View(inRs, title, W_WIDTH, W_HEIGHT, app)
{
	kinect.initKinect();
}

void KinectView::render()
{
	rs->renderSpacePoints(kinect.getKinectData(), rHelper);
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
		case KeyCode::Key_T:
			if (static_cast<DisplayStyle>(rHelper.displayStyle) == DisplayStyle::CHESS_ORDER) {
				parentApp->createTriView(kinect.getKinectData(), W_WIDTH, W_HEIGHT);
			}
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
