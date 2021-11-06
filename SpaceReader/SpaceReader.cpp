#include <iostream>
#include <limits>
#include <iterator>
#include <array>

#include <glm/gtc/matrix_transform.hpp>

#include "GLWindow.h"
#include "GLRenderSystem.h"
#include "Viewport.h"
#include "KinectSensor.h"
#include "Application.h"

//#define INF std::numeric_limits<float>::infinity()
//
//static GLRenderSystem* rs;
Viewport* viewport;
//
//static glm::vec3 cubeMin{ INF, INF, INF }, cubeMax{ -INF, -INF, -INF };
//
//static bool setEncCube = false;
//const float L = 1.0f;
//const glm::vec3 defNormal{ 0.0f, 0.0f, 0.0f };
//const glm::vec3 defColor{ 1.f, 1.f, 1.f };
//
//unsigned int style = 0;
//
//const std::vector<Vertex> CUBE_VERTICES = 
//{
//    Vertex{glm::vec3(-L / 2, -L / 2, L / 2), defColor},
//    Vertex{glm::vec3(L / 2, -L / 2, L / 2), defColor},
//    Vertex{glm::vec3(L / 2, -L / 2, -L / 2), defColor},
//    Vertex{glm::vec3(-L / 2, -L / 2, -L / 2), defColor},
//
//
//    Vertex{glm::vec3(-L / 2, L / 2, L / 2), defColor},
//    Vertex{glm::vec3(L / 2, L / 2, L / 2), defColor},
//    Vertex{glm::vec3(L / 2, L / 2, -L / 2), defColor},
//    Vertex{glm::vec3(-L / 2, L / 2, -L / 2), defColor}
//};
//
//KinectSensor kinectSensor;
//
//bool drawWithColor = true;
//
//void renderScene(GLRenderSystem& rs)
//{
//    rs.renderSpacePoints(kinectSensor.getKinectData(), drawWithColor);
//}
//
//void moveCube(GLRenderSystem& rs, glm::vec3 offset)
//{
//    offset /= 10; 
//
//    rs.setWorldMatrix(glm::translate(rs.getWorldMatrix(), offset));
//}
//
//void moveCamera(Camera& camera, glm::vec3 offset)
//{
//    offset /= 10;
//    camera.translate(offset);
//}
//
//void onKeyCallback(KeyCode key, Action action, Modifier mods)
//{
//    if (key == KeyCode::Up)
//        kinectSensor.increaseCameraAngle();
//
//    if (key == KeyCode::Down)
//        kinectSensor.decreaseCameraAngle();
//
//    if (key == KeyCode::Left)
//        moveCube(*rs, glm::vec3(-1.0f, 0.0f, 0.0f));
//
//    if (key == KeyCode::Right)
//        moveCube(*rs, glm::vec3(1.0f, 0.0f, 0.0f));
//
//    if (key == KeyCode::Key_W)
//        moveCamera(viewport->getCamera(), viewport->getCamera().calcForward());
//
//    if (key == KeyCode::Key_A)
//        moveCamera(viewport->getCamera(), -viewport->getCamera().calcRight());
//
//    if (key == KeyCode::Key_S && action == Action::Press) {
//        style = (style + 1) % 4;
//        kinectSensor.setDisplayStyle(static_cast<DisplayStyle>(style));
//    }
//
//    if (key == KeyCode::Key_D)
//        moveCamera(viewport->getCamera(), viewport->getCamera().calcRight());
//
//    if (key == KeyCode::Key_C && action == Action::Release)
//        drawWithColor = !drawWithColor;
//
//    // F1 ... F7
//    if (static_cast<int>(key) >= 290 && static_cast<int>(key) <= 298 &&
//        action == Action::Press)
//    {
//        switch (key)
//        {
//        case KeyCode::F1:
//            viewport->getCamera().setFrontView();
//            break;
//        case KeyCode::F2:
//            viewport->getCamera().setTopView();
//            break;
//        case KeyCode::F3:
//            viewport->getCamera().setRearView();
//            break;
//        case KeyCode::F4:
//            viewport->getCamera().setRightView();
//            break;
//        case KeyCode::F5:
//            viewport->getCamera().setLeftView();
//            break;
//        case KeyCode::F6:
//            viewport->getCamera().setBottomView();
//            break;
//        case KeyCode::F7:
//            viewport->getCamera().setIsoView();
//            break;
//        case KeyCode::F8:
//            if (!viewport->isParallelProjection())
//                viewport->setParallelProjection(true);
//            else viewport->setParallelProjection(false);
//            break;
//        case KeyCode::F9:
//            std::cout << "Zoom to fit\n";
//            viewport->getCamera().zoomToFit(cubeMin, cubeMax);
//            break;
//        default:
//            break;
//        }
//    }
//}
//
//void scrollCallback(double xoffset, double yoffset)
//{
//    viewport->getCamera().zoom(yoffset / 10);
//}
//
//static int mouseButton = 0;
//static double u0, v0;
//static glm::vec3 a, b;
//
//double map(double var, double begin, double end,
//    double targetBegin, double targetEnd)
//{
//    double result = targetBegin + ((var - begin) * (targetEnd - targetBegin))
//        / (end - begin);
//    return result;
//}
//
//glm::vec3 setPointOnSphere(float xInTargetPlane, float yInTargetPlane)
//{
//    glm::vec3 pointOnSphere{ xInTargetPlane, yInTargetPlane, 0.0f };
//
//    // value outside sphere (R = 1), project it onto sphere
//    if (glm::length(pointOnSphere) >= 1.0f)
//        pointOnSphere = glm::normalize(pointOnSphere);
//
//    else  // point inside the sphere
//        pointOnSphere.z = std::sqrt(1.0f - std::pow(pointOnSphere.x, 2)
//            - std::pow(pointOnSphere.y, 2));
//
//    return pointOnSphere;
//}
//
//void onMouseButtonCallback(ButtonCode button, Action action, Modifier mods,
//    double xpos, double ypos)
//{
//    if (button == ButtonCode::Left)
//    {
//        if (action == Action::Press)
//        {
//            double dx = viewport->calcTargetPlaneWidth() / 2.0;
//            double dy = viewport->calcTargetPlaneHeight() / 2.0;
//            u0 = map(xpos,
//                0.0,
//                viewport->getWidth(),
//                -dx,
//                dx);
//
//            v0 = map(ypos,
//                0.0,
//                viewport->getHeight(),
//                dy,
//                -dy);
//
//            mouseButton = 1;
//        }
//        else if (action == Action::Release)
//        {
//            mouseButton = 0;
//        }
//    }
//
//    else if (button == ButtonCode::Right)
//    {
//        if (action == Action::Press)
//        {
//            double xOnTarget = map(xpos, 0.0, viewport->getWidth(),
//                -viewport->calcAspectRatio(), viewport->calcAspectRatio());
//            double yOnTarget = map(ypos, 0.0, viewport->getHeight(),
//                1.0, -1.0);
//
//            a = setPointOnSphere(static_cast<float>(xOnTarget),
//                static_cast<float>(yOnTarget));
//
//            mouseButton = 2;
//        }
//        else if (action == Action::Release)
//        {
//            mouseButton = 0;
//        }
//    }
//}
//
//void cursorPosCallback(double xpos, double ypos)
//{
//    if (mouseButton == 1)
//    {
//        double dx = viewport->calcTargetPlaneWidth() / 2.0;
//        double dy = viewport->calcTargetPlaneHeight() / 2.0;
//
//        double u = map(xpos, 0.0, viewport->getWidth(), -dx, dx);
//        double v = map(ypos, 0.0, viewport->getHeight(), dy, -dy);
//
//        double du = u0 - u;
//        double dv = v0 - v;
//
//        viewport->getCamera().pan(du, dv);
//
//        u0 = u;
//        v0 = v;
//    }
//    else if (mouseButton == 2)
//    {
//        double xOnTarget = map(xpos, 0.0, viewport->getWidth(),
//            -viewport->calcAspectRatio(), viewport->calcAspectRatio());
//        double yOnTarget = map(ypos, 0.0, viewport->getHeight(), 1, -1);
//
//        glm::vec3 b = setPointOnSphere(static_cast<float>(xOnTarget),
//            static_cast<float>(yOnTarget));
//
//        viewport->getCamera().orbit(b, a);
//        a = b;
//    }
//}


int main()
{
    Application app;
    app.createKinectView();

    app.run();


    /*rs = new GLRenderSystem();
    viewport = new Viewport();

    GLWindow window("myWindow", 640, 480);

    viewport->setViewportSize(640, 480);
    viewport->setFOV(60.0);
    viewport->setZNear(0.01);
    viewport->setZFar(500);

    window.setKeyCallback(onKeyCallback);
    window.setScrollCallback(scrollCallback);
    window.setCursorPosCallback(cursorPosCallback);
    window.setMouseCallback(onMouseButtonCallback);

    kinectSensor.initKinect();
    viewport->getCamera().setEyeTargetUp(glm::vec3{ 0.f, 0.f, -1.f }, glm::vec3{ 0.f, 0.f, 0.f }, glm::vec3{ 0.f, 1.f, 0.f });

    rs->init();
    rs->turnLight(0, true);

    while (!glfwWindowShouldClose(window.getGLFWHandle()))
    {
        rs->setViewport(0, 0, window.getWidth(), window.getHeight());
        rs->clearDisplay(0.1f, 0.1f, 0.1f);

        rs->setViewMatrix(viewport->getCamera().calcViewMatrix());
        rs->setProjMatrix(viewport->calcProjMatrix());

        renderScene(*rs);
        glfwSwapBuffers(window.getGLFWHandle());
        glfwWaitEvents();
    }

    glfwTerminate();*/
}
