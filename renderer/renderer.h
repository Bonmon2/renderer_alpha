#ifndef RENDERER_H
#define RENDERER_H

#include "debug/debug.h"
#include <GLFW/glfw3.h>

#include <vector>
#include <stdexcept>

const std::vector< const char* > validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};


class Renderer {
private:
    VkInstance instance;
    
    Debug* debug;

public:
    Renderer();
    virtual ~Renderer();

    void createInstance();
private:
    std::vector< const char* > getRequiredExtensions();


};

#endif