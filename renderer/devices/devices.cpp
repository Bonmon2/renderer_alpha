#include "devices.h"


#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

PhysicalDevice::PhysicalDevice( VkInstance instance ) {
    physicalDevice = VK_NULL_HANDLE;
    deviceCount = 0;

    vkEnumeratePhysicalDevices( instance, &deviceCount, nullptr );
    if( deviceCount == 0 ) {
        throw std::runtime_error( "failed to find GPUs with vulkan support" );
    }

    std::vector< VkPhysicalDevice > devices( deviceCount );
    vkEnumeratePhysicalDevices( instance, &deviceCount, devices.data() );

    for( const auto& device : devices ) {
        if( isSuitable( device ) ) {
            physicalDevice = device;
            break;
        }
    }

    if( physicalDevice == VK_NULL_HANDLE ) {
        throw std::runtime_error( "failed to find a suitable GPU" );
    }
}

bool PhysicalDevice::isSuitable( VkPhysicalDevice device ) {
    QueueFamilyIndices indices = findQueueFamilies( device );

    return indices.isComplete();
}

QueueFamilyIndices Device::findQueueFamilies( VkPhysicalDevice device ) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, nullptr );

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, queueFamilies.data() );

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

LogicalDevice::LogicalDevice( VkInstance instance, PhysicalDevice* physicalDevice ) {
    QueueFamilyIndices indices = findQueueFamilies( physicalDevice->physicalDevice );

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex    = indices.graphicsFamily.value();
    queueCreateInfo.queueCount          = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities    = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos    = &queueCreateInfo;
    createInfo.pEnabledFeatures     = &deviceFeatures;
    createInfo.enabledExtensionCount    = 0;

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>( validationLayers.size() );
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if( vkCreateDevice( physicalDevice->physicalDevice, &createInfo, nullptr, &device ) != VK_SUCCESS ) {
        std::runtime_error( "failed to create logical device" );
    }
}

LogicalDevice::~LogicalDevice() {
    vkDestroyDevice( device, nullptr );
}
