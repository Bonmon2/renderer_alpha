#ifndef WINDOW_H
#define WINDOW_H


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window {
public:
    GLFWwindow* window;
    
public:
    Window( int width, int height );
    virtual ~Window();

    bool shouldClose();
    void poll();
};

#endif