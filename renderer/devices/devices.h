#ifndef DEVICES_H
#define DEVICES_H

#include <vulkan/vulkan.h>

#include <optional>
#include <stdexcept>
#include <vector>
#include <set>

const std::vector< const char* > validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector< VkSurfaceFormatKHR > formats;
    std::vector< VkPresentModeKHR > presentModes;
};

class Device{
protected:
    QueueFamilyIndices findQueueFamilies( VkPhysicalDevice device, VkSurfaceKHR surface );
};

class PhysicalDevice: Device{

public:
    VkPhysicalDevice physicalDevice;

private:
    uint32_t deviceCount;

public:
    PhysicalDevice( VkInstance instance, VkSurfaceKHR surface );

private:
    bool isSuitable( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface );
    bool checkExtensionSupport( VkPhysicalDevice physicalDevice );
    SwapChainSupportDetails querySwapChainSupport( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface );
};


class LogicalDevice: Device {
private:
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    
public:
    VkDevice device;
public:
    LogicalDevice( VkInstance instance, PhysicalDevice* physicalDevice, VkSurfaceKHR surface );
    virtual ~LogicalDevice();

private:

};

#endif