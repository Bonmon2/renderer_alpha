#ifndef RENDERER_H
#define RENDERER_H

#include "debug/debug.h"
#include "devices/devices.h"


#include <GLFW/glfw3.h>

#include <vector>
#include <stdexcept>




class Renderer {
private:
    VkInstance instance;
    
    Debug* debug;
    PhysicalDevice* physicalDevice;
    LogicalDevice* logicalDevice;

public:
    Renderer();
    virtual ~Renderer();

    void createInstance();
private:
    std::vector< const char* > getRequiredExtensions();


};

#endif