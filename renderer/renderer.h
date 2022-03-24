#ifndef RENDERER_H
#define RENDERER_H

#include "../window/window.h"

#include "debug/debug.h"
#include "devices/devices.h"

#include <GLFW/glfw3native.h>

#include <vector>
#include <stdexcept>




class Renderer {
private:
    VkInstance instance;
    VkSurfaceKHR surface;
    
    Debug* debug;
    PhysicalDevice* physicalDevice;
    LogicalDevice* logicalDevice;

public:
    Renderer( Window* window );
    virtual ~Renderer();

    void createInstance();
private:
    std::vector< const char* > getRequiredExtensions();
    void createSurface( Window* window );


};

#endif