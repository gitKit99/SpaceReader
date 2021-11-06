#pragma once
#include <glm/glm.hpp>
#include "glad.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <map>


struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 color;
};

class GLRenderSystem
{
public:
    void init();

    void clearDisplay(float r, float g, float b);
    void setViewport(double x, double y, double width, double height);
    void renderTriangleSoup(const std::vector<Vertex>& vertices);
    void renderSpacePoints(const std::vector<Vertex>& points, bool drawWithColor);

    void setupLight(uint32_t index, glm::vec3 position, glm::vec3 Ia,
        glm::vec3 Id, glm::vec3 Is);
    void turnLight(uint32_t index, bool enable);

    void setWorldMatrix(const glm::mat4& matrix);
    const glm::mat4& getWorldMatrix() const;

    void setViewMatrix(const glm::mat4& matrix);
    const glm::mat4& getViewMatrix() const;

    void setProjMatrix(const glm::mat4& matrix);
    const glm::mat4& getProjMatrix() const;

    const std::vector<Vertex>& getRenderMesh();

private:
    glm::mat4 worldMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;

    uint32_t lightIndex;    
    glm::vec3 lightPos;  
    glm::vec3 iAmbient;     
    glm::vec3 iDiffuse;   
    glm::vec3 iSpecular;
    bool isLightEnable;  

    std::vector<Vertex> renderMesh;
};


