#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

class Window {
private:
    GLFWwindow* window;
    
public:
    Window( int width, int height );
    virtual ~Window();

    bool shouldClose();
    void poll();
};

#endif