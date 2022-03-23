#include "window/window.h"

#define GLFW_INCLUDE_VULKAN

uint32_t WIDTH = 800, HEIGHT = 600;

class app {
private:
    Window* window;
public:
    void run() {
        window = new Window( WIDTH, HEIGHT );

        while( !window->shouldClose() ) {
            window->poll();
        }
    }
};

int main()
{
    app myApp;

    myApp.run();

    return 0;
}