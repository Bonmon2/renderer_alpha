#ifndef DEVICES_H
#define DEVICES_H

#include <vulkan/vulkan.h>

#include <optional>
#include <stdexcept>
#include <vector>

const std::vector< const char* > validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

class Device{
protected:
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};

class PhysicalDevice: Device{
public:
    VkPhysicalDevice physicalDevice;
private:
    uint32_t deviceCount;
public:
    PhysicalDevice( VkInstance instance );

private:
    bool isSuitable( VkPhysicalDevice device );
};

class LogicalDevice: Device {
public:
    VkDevice device;
public:
    LogicalDevice( VkInstance instance, PhysicalDevice* physicalDevice  );
    virtual ~LogicalDevice();

private:

};

#endif