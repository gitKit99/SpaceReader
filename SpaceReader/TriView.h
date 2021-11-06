#pragma once

#include "View.h"

class TriView : public View
{
public:
	TriView(GLRenderSystem& inRs, const std::vector<Vertex>& points, const std::string& title, 
		unsigned int width, unsigned int height, Application& app);
	virtual void render() override;

protected:
	std::unique_ptr<std::vector<Vertex>> mesh;
	RenderHelper mrHelper;
};

