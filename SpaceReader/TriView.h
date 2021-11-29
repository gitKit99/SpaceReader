#pragma once

#include "View.h"

class TriView : public View
{
public:
	TriView(GLRenderSystem& inRs, const std::vector<Vertex>& points, const std::string& title, 
		unsigned int width, unsigned int height, Application& app);
	virtual void render() override;
	virtual void onKeyCallback(KeyCode, Action, Modifier) override;

protected:
	void setLines();
	std::unique_ptr<std::vector<Vertex>> mesh;
	std::unique_ptr<std::vector<Vertex>> lines;
	RenderHelper mrHelper;
	bool renderMesh = true;
	bool renderEdges = true;
};

