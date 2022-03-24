#include "renderer.h"



Renderer::Renderer( Window* window ) {
    debug = new Debug();
    createInstance();
    debug->create( instance );

    createSurface( window );

    physicalDevice = new PhysicalDevice( instance, surface );
    logicalDevice = new LogicalDevice( instance, physicalDevice, surface );

    
}

Renderer::~Renderer() {
    logicalDevice->~LogicalDevice();

    debug->detroy( instance );

    vkDestroySurfaceKHR( instance, surface, nullptr );
    vkDestroyInstance( instance, nullptr );
}

void Renderer::createInstance() {

    VkApplicationInfo appInfo{};
    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext               = nullptr;
    appInfo.pApplicationName    = "renderer alpha";
    appInfo.pEngineName         = "no engine";
    appInfo.engineVersion       = 1;
    appInfo.apiVersion          = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo         = &appInfo;

    std::vector< const char* > extensions =  getRequiredExtensions();
    createInfo.enabledExtensionCount    = extensions.size();
    createInfo.ppEnabledExtensionNames  = extensions.data();
    
    if( enableValidationLayers ) {    
        createInfo.enabledLayerCount        = validationLayers.size();
        createInfo.ppEnabledLayerNames      = validationLayers.data();

        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug->createInfo;
    } else {
        createInfo.enabledLayerCount        = 0;
        createInfo.ppEnabledLayerNames      = nullptr;
    }


    if( vkCreateInstance( &createInfo, nullptr, &instance ) != VK_SUCCESS ) {
        throw std::runtime_error( "Failed to initialise vulkan" );
    }
}

std::vector< const char* > Renderer::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );

    std::vector< const char* > extensions( glfwExtensions, glfwExtensions + glfwExtensionCount );

    if( enableValidationLayers ) {
        extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
    }

    return extensions;
}

void Renderer::createSurface( Window* window ) {
    if( glfwCreateWindowSurface( instance, window->window, nullptr, &surface ) != VK_SUCCESS ) {
        throw std::runtime_error( "failed to create window surface" );
    }
}
