#include "window/window.h"
#include "renderer/renderer.h"

#define GLFW_INCLUDE_VULKAN

uint32_t WIDTH = 800, HEIGHT = 600;

class app {
private:
    Window* window;
    Renderer* renderer;
public:
    void run() {
        window = new Window( WIDTH, HEIGHT );

        renderer = new Renderer();


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