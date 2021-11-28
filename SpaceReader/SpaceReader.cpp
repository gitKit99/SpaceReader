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

Viewport* viewport;


int main()
{
    Application app;
    const std::string path = "E:\\projects\\coursach\\SpaceReader\\data\\bunnyData.txt";
    //app.createKinectView();
    app.createTriView(path);

    app.run();
}
