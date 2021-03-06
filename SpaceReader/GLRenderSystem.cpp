#include "GLRenderSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>


void GLRenderSystem::init()
{
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void GLRenderSystem::clearDisplay(float r, float g, float b)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(r, g, b, 1.0f);
}

void GLRenderSystem::setViewport(double x, double y, double width, double height)
{
    glViewport(static_cast<GLint>(x), static_cast<GLint>(y),
        static_cast<GLint>(width), static_cast<GLint>(height));
}

void GLRenderSystem::renderTriangleSoup(const std::vector<Vertex>& vertices)
{
    glm::mat4 modelView = this->viewMatrix * this->worldMatrix;
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(modelView));

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(this->projMatrix));

    glBegin(GL_TRIANGLES);
    for (std::vector<Vertex>::const_iterator it = vertices.cbegin();
        it != vertices.cend(); it++)
    {
        glNormal3f(it->normal.x, it->normal.y, it->normal.z);
        glVertex3f(it->position.x, it->position.y, it->position.z);
    }
    glEnd();

    this->renderMesh = vertices;
}

void GLRenderSystem::renderSpacePoints(const std::vector<SpacePoint>& points, bool drawWithColor)
{
    glm::mat4 modelView = this->viewMatrix * this->worldMatrix;
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(modelView));

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(this->projMatrix));

    glBegin(GL_POINTS);
    glPointSize(1.f);
    for (std::vector<SpacePoint>::const_iterator it = points.cbegin(); it != points.cend(); it++)
    {
        glVertex3f(it->pos.x, it->pos.y, it->pos.z);

        if (drawWithColor)
            glColor3f(it->color.x, it->color.y, it->color.z);
        else
            glColor3f(1.f, 1.f, 1.f);
    }
    /*for (size_t j = 0; j < points.size(); j++) {
        for (size_t i = 0; i < points[j].size(); i++) {
            glVertex3f(points[j][i].pos.x, points[j][i].pos.y, points[j][i].pos.z);

            if (drawWithColor)
                glColor3f(points[j][i].color.x, points[j][i].color.y, points[j][i].color.z);
            else
                glColor3f(1.f, 1.f, 1.f);
        }
    }*/
    glEnd();
}

void GLRenderSystem::setupLight(uint32_t index, glm::vec3 position,
    glm::vec3 Ia, glm::vec3 Id, glm::vec3 Is)
{  
    this->lightIndex = index;
    this->lightPos = position;

    this->iAmbient = Ia;
    this->iDiffuse = Id;
    this->iSpecular = Is;

    this->isLightEnable = false;

    glLightfv(GL_LIGHT0, GL_AMBIENT, value_ptr(glm::vec4(this->iAmbient, 1.0f)));
    glLightfv(GL_LIGHT0, GL_DIFFUSE, value_ptr(glm::vec4(this->iDiffuse, 1.0f)));
    glLightfv(GL_LIGHT0, GL_SPECULAR, value_ptr(glm::vec4(this->iSpecular,
        1.0f)));

    glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(glm::vec4(this->lightPos,
        1.0f)));

    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}

void GLRenderSystem::turnLight(uint32_t index, bool enable)
{
    if (index == this->lightIndex)
    {
        this->isLightEnable = enable;

        if (enable)
            glEnable(GL_LIGHT0);
        else glDisable(GL_LIGHT0);
    }

}

void GLRenderSystem::setWorldMatrix(const glm::mat4& matrix)
{
    this->worldMatrix = matrix;
}

const glm::mat4& GLRenderSystem::getWorldMatrix() const
{
    return this->worldMatrix;
}

void GLRenderSystem::setViewMatrix(const glm::mat4& matrix)
{
    this->viewMatrix = matrix;
}

const glm::mat4& GLRenderSystem::getViewMatrix() const
{
    return this->viewMatrix;
}

void GLRenderSystem::setProjMatrix(const glm::mat4& matrix)
{
    this->projMatrix = matrix;
}

const glm::mat4& GLRenderSystem::getProjMatrix() const
{
    return this->projMatrix;
}

const std::vector<Vertex>& GLRenderSystem::getRenderMesh()
{
    return this->renderMesh;
}