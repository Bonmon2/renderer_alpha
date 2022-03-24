#include "devices.h"


const std::vector< const char* > deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

PhysicalDevice::PhysicalDevice( VkInstance instance, VkSurfaceKHR surface ) {
    physicalDevice = VK_NULL_HANDLE;
    deviceCount = 0;

    vkEnumeratePhysicalDevices( instance, &deviceCount, nullptr );
    if( deviceCount == 0 ) {
        throw std::runtime_error( "failed to find GPUs with vulkan support" );
    }

    std::vector< VkPhysicalDevice > devices( deviceCount );
    vkEnumeratePhysicalDevices( instance, &deviceCount, devices.data() );

    for( const auto& device : devices ) {
        if( isSuitable( device, surface ) ) {
            physicalDevice = device;
            break;
        }
    }

    if( physicalDevice == VK_NULL_HANDLE ) {
        throw std::runtime_error( "failed to find a suitable GPU" );
    }
}

bool PhysicalDevice::isSuitable( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface ) {
    QueueFamilyIndices indices = findQueueFamilies( physicalDevice, surface );

    bool extensionsSupported = checkExtensionSupport( physicalDevice );

    bool swapChainAdequate = false;
    if( extensionsSupported ) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport( physicalDevice, surface );
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate ;
}

bool PhysicalDevice::checkExtensionSupport( VkPhysicalDevice physicalDevice ) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties( physicalDevice, nullptr, &extensionCount, nullptr );

    std::vector< VkExtensionProperties > availableExtensions( extensionCount );
    vkEnumerateDeviceExtensionProperties( physicalDevice, nullptr, &extensionCount, availableExtensions.data() );

    std::set< std::string > requiredExtensions( deviceExtensions.begin(), deviceExtensions.end() );

    for( const auto& extension : availableExtensions ) {
        requiredExtensions.erase( extension.extensionName );
    }
    return requiredExtensions.empty();
}

QueueFamilyIndices Device::findQueueFamilies( VkPhysicalDevice device, VkSurfaceKHR surface ) {
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

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR( device, i, surface, &presentSupport );

        if( presentSupport ) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

SwapChainSupportDetails PhysicalDevice::querySwapChainSupport( VkPhysicalDevice physicalDevice, VkSurfaceKHR surface ) {
    SwapChainSupportDetails details;

    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, surface, &details.capabilities );

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, &formatCount, details.formats.data() );

    if( formatCount != 0 ) {
        details.formats.resize( formatCount );
        vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, surface, &formatCount, details.formats.data() );
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, &presentModeCount, details.presentModes.data() );

    if( presentModeCount != 0 ) {
        details.presentModes.resize( presentModeCount );
        vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, surface, &presentModeCount, details.presentModes.data() );
    }

    return details;
}

LogicalDevice::LogicalDevice( VkInstance instance, PhysicalDevice* physicalDevice, VkSurfaceKHR surface  ) {
    QueueFamilyIndices indices = findQueueFamilies( physicalDevice->physicalDevice, surface );

    std::vector< VkDeviceQueueCreateInfo > queueCreateInfos;
    std::set< uint32_t > uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for( uint32_t queueFamily : uniqueQueueFamilies ) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex    = indices.graphicsFamily.value();
        queueCreateInfo.queueCount          = 1;
        queueCreateInfo.pQueuePriorities    = &queuePriority;

        queueCreateInfos.push_back( queueCreateInfo );
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast< uint32_t > ( queueCreateInfos.size() );
    createInfo.pQueueCreateInfos    = queueCreateInfos.data();
    createInfo.pEnabledFeatures     = &deviceFeatures;
    createInfo.enabledExtensionCount    = static_cast< uint32_t > ( deviceExtensions.size() );
    createInfo.ppEnabledExtensionNames  = deviceExtensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>( validationLayers.size() );
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if( vkCreateDevice( physicalDevice->physicalDevice, &createInfo, nullptr, &device ) != VK_SUCCESS ) {
        std::runtime_error( "failed to create logical device" );
    }

    vkGetDeviceQueue( device, indices.graphicsFamily.value() , 0, &graphicsQueue );
    vkGetDeviceQueue( device, indices.presentFamily.value(), 0, &presentQueue );
}

LogicalDevice::~LogicalDevice() {
    vkDestroyDevice( device, nullptr );
}
