#include "window.h"

#include <stdexcept>

#include <GLFW/glfw3.h>

Window::Window( int width, int height ) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose( window );
}

void Window::poll() {
    glfwPollEvents();
}