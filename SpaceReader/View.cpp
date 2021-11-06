#include "View.h"
#include "MathUtils.h"
#include "Application.h"

#include <functional>
#include <utility>
namespace OperatorUtils {
    glm::vec3 setPointOnSphere(float xInTargetPlane, float yInTargetPlane)
    {
        glm::vec3 pointOnSphere{ xInTargetPlane, yInTargetPlane, 0.0f };

        // value outside sphere (R = 1), project it onto sphere
        if (glm::length(pointOnSphere) >= 1.0f)
            pointOnSphere = glm::normalize(pointOnSphere);

        else  // point inside the sphere
            pointOnSphere.z = std::sqrt(1.0f - std::pow(pointOnSphere.x, 2)
                - std::pow(pointOnSphere.y, 2));

        return pointOnSphere;
    }
}

View::View(GLRenderSystem& inRs, const std::string& title, unsigned int width, 
    unsigned int height, Application& app)
	: parentApp(&app), rs(&inRs)
{
	viewport.setViewportSize(width, height);
	window.reset(new GLWindow(title, width, height));

    viewport.setFOV(60.0);
    viewport.setZNear(0.01);
    viewport.setZFar(500);

    window->setKeyCallback([&](KeyCode key, Action action, Modifier mods) {
        this->onKeyCallback(key, action, mods);
        });
    window->setScrollCallback([&](double xoffset, double yoffset) {
        this->scrollCallback(xoffset, yoffset);
        });
    window->setCursorPosCallback([&](double xpos, double ypos) {
        this->cursorPosCallback(xpos, ypos);
        });
    window->setMouseCallback([&](ButtonCode button, Action action, Modifier mods, double xpos, double ypos) {
        this->onMouseButtonCallback(button, action, mods, xpos, ypos);
        });

    viewport.getCamera().setEyeTargetUp(glm::vec3{ 0.f, 0.f, -1.f }, 
        glm::vec3{ 0.f, 0.f, 0.f }, glm::vec3{ 0.f, 1.f, 0.f });

    rs->init();
    rs->turnLight(0, true);

    additionalRenderSetup();
}

void View::update() {
    if (glfwWindowShouldClose(window->getGLFWHandle())) {
        delete window.release();
        parentApp->detachView(*this);
        return;
    }

    rs->setViewport(0, 0, window->getWidth(), window->getHeight());
    rs->clearDisplay(0.5f, 0.5f, 0.5f);

    rs->setViewMatrix(viewport.getCamera().calcViewMatrix());
    rs->setProjMatrix(viewport.calcProjMatrix());

    render();

    glfwSwapBuffers(window->getGLFWHandle());
    glfwWaitEvents();
}

void View::onKeyCallback(KeyCode keyCode, Action action, Modifier mods) {
    // will be overloaded
}

void View::scrollCallback(double xoffset, double yoffset) {
    viewport.getCamera().zoom(yoffset);
}

void View::cursorPosCallback(double xpos, double ypos) {
    if (mouseState.pressedButton == ButtonCode::Left)
    {
        double dx = viewport.calcTargetPlaneWidth() / 2.0;
        double dy = viewport.calcTargetPlaneHeight() / 2.0;

        double u = MathUtils::map(xpos, 0.0, viewport.getWidth(), -dx, dx);
        double v = MathUtils::map(ypos, 0.0, viewport.getHeight(), dy, -dy);

        double du = mouseState.u0 - u;
        double dv = mouseState.v0 - v;

        viewport.getCamera().pan(du, dv);

        mouseState.u0 = u;
        mouseState.v0 = v;
    }
    else if (mouseState.pressedButton == ButtonCode::Right)
    {
        double xOnTarget = MathUtils::map(xpos, 0.0, viewport.getWidth(),
            -viewport.calcAspectRatio(), viewport.calcAspectRatio());
        double yOnTarget = MathUtils::map(ypos, 0.0, viewport.getHeight(), 1, -1);

        glm::vec3 b = OperatorUtils::setPointOnSphere(static_cast<float>(xOnTarget),
            static_cast<float>(yOnTarget));

        viewport.getCamera().orbit(b, eHelper.vect_a);
        eHelper.vect_a = b;
    }
}

void View::onMouseButtonCallback(ButtonCode button, Action action, Modifier mods,
    double xpos, double ypos)
{
    if (button == ButtonCode::Left)
    {
        if (action == Action::Press)
        {
            double dx = viewport.calcTargetPlaneWidth() / 2.0;
            double dy = viewport.calcTargetPlaneHeight() / 2.0;
            mouseState.u0 = MathUtils::map(xpos,
                0.0,
                viewport.getWidth(),
                -dx,
                dx);

            mouseState.v0 = MathUtils::map(ypos,
                0.0,
                viewport.getHeight(),
                dy,
                -dy);

            mouseState.pressedButton = button;
        }
        else if (action == Action::Release)
        {
            mouseState.pressedButton = ButtonCode::Undefined;
        }
    }

    else if (button == ButtonCode::Right)
    {
        if (action == Action::Press)
        {
            double xOnTarget = MathUtils::map(xpos, 0.0, viewport.getWidth(),
                -viewport.calcAspectRatio(), viewport.calcAspectRatio());
            double yOnTarget = MathUtils::map(ypos, 0.0, viewport.getHeight(),
                1.0, -1.0);

            eHelper.vect_a = OperatorUtils::setPointOnSphere(static_cast<float>(xOnTarget),
                static_cast<float>(yOnTarget));

            mouseState.pressedButton = button;
        }
        else if (action == Action::Release)
        {
            mouseState.pressedButton = ButtonCode::Undefined;
        }
    }
}

const Viewport& View::getViewport() const {
    return viewport;
}

void View::setData(const std::vector<Vertex>& mesh) {
    if (data != nullptr) {
        data->clear();
    }
    data.reset(new std::vector<Vertex>(mesh.size()));
    
    for (size_t i = 0; i < mesh.size(); i++) {
        data->at(i) = mesh.at(0);
    }
}

const std::vector<Vertex>* View::getData() const {
    return data.get();
}

