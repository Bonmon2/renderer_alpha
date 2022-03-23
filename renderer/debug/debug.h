#ifndef DEBUG_H
#define DEBUG_H

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#include <vulkan/vulkan.h>

class Debug {
private:
    VkDebugUtilsMessengerEXT debugMessenger;
public:
    Debug( VkInstance instance );
    void detroy( VkInstance instance );
};

#endif