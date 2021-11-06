#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Viewport.h"
#include "GLRenderSystem.h"
#include "GLWindow.h"

struct MouseState {
	ButtonCode pressedButton = ButtonCode::Undefined;
	double u0 = 0.f;
	double v0 = 0.f;
};

struct EventHelper {
	glm::vec3 vect_a{ 0.f, 0.f, 0.f };
};

class Application;

class View
{
public:
	View(GLRenderSystem& inRs, const std::string& title, unsigned int width, unsigned int height, Application& app);

	virtual void update();
	void setData(const std::vector<Vertex>& mesh);
	const std::vector<Vertex>* getData() const;

	const Viewport& getViewport() const;

protected:
	virtual void render() = 0;
	virtual void additionalRenderSetup(){ }

	virtual void onKeyCallback(KeyCode, Action, Modifier);
	virtual void scrollCallback(double, double);
	virtual void cursorPosCallback(double, double);
	virtual void onMouseButtonCallback(ButtonCode, Action, Modifier, double, double);

	Viewport viewport;
	std::unique_ptr<std::vector<Vertex>> data ;
	GLRenderSystem* rs = nullptr;
	std::unique_ptr<GLWindow> window;
	Application* parentApp = nullptr;

private:
	MouseState mouseState;
	EventHelper eHelper;
};

