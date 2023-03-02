#pragma once

#include "stdafx.h"
#include "vulkan/vulkan.h"


namespace Dynamics_IO_Testbench {
    namespace Compute {
        namespace VK {
            namespace Utilities {

                // non-vulkan functions

                std::vector<char> readFile(const std::string& filename);



                // get structures

                VkApplicationInfo getApplicationInfo(const char* applicationName, uint32_t appVersion, const char* EngineName, uint32_t engineVersion, uint32_t apiVersion);

                VkInstanceCreateInfo getInstanceCreateInfo(VkApplicationInfo& appInfo, std::vector<const char*>& extensions);

                VkDeviceQueueCreateInfo getDeviceQueueCreateInfo(uint32_t queueFamilyIndex, uint32_t queueCount, float& queuePriority);

                VkPhysicalDeviceFeatures getPhysicalDeviceFeatures();

                VkDeviceCreateInfo getDeviceCreateInfo(
                    std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
                    VkPhysicalDeviceFeatures& deviceFeatures,
                    std::vector<const char*>& deviceExtensions);

                VkCommandPoolCreateInfo getCommandPoolCreateInfo(VkCommandPoolCreateFlags flag, uint32_t queueFamilyIndex);

                VkCommandBufferAllocateInfo getCommandBufferAllocateInfo(VkCommandPool commandPool, VkCommandBufferLevel level, uint32_t commandBufferCount);

                VkSemaphoreCreateInfo getSemaphoreCreateInfo();

                VkFenceCreateInfo getFenceCreateInfo(bool isSignaled);

                VkBufferCreateInfo getBufferCreateInfo(VkDeviceSize size, VkBufferUsageFlags usage, VkSharingMode sharingMode, VkBufferCreateFlags flags, std::vector<uint32_t>& queueFamilies);

                // Load Enumeration functions

                std::vector<VkExtensionProperties> EnumerateInstanceExtensionProperties();

                std::vector<VkPhysicalDevice> EnumeratePhysicalDevices(VkInstance instance);

                std::vector<VkQueueFamilyProperties> GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice device);
            
                
                // resource creation abstractions

                void CreateBuffer(
                    VkPhysicalDevice& physicalDevice,
                    VkDevice& device,
                    VkDeviceSize size,
                    VkBufferUsageFlags usage,
                    VkSharingMode sharingMode,
                    VkBufferCreateFlags flags,
                    VkMemoryPropertyFlags properties,
                    std::vector<uint32_t>& queueFamilies,
                    VkBuffer& buffer,
                    VkDeviceMemory& bufferMemory);

                void CreateBuffer(
                    VkPhysicalDevice& physicalDevice,
                    VkDevice& device,
                    size_t stride,
                    size_t num_elements,
                    VkBufferUsageFlags usage,
                    VkSharingMode sharingMode,
                    VkBufferCreateFlags flags,
                    VkMemoryPropertyFlags properties,
                    std::vector<uint32_t>& queueFamilies,
                    VkBuffer& buffer,
                    VkDeviceMemory& bufferMemory);

                uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);


                // Buffer utilities

                void CopyBuffer(VkQueue& queue, VkCommandBuffer& commandBuffer, VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size);
            }


            namespace Extensions {
                // Extension methods

                VkResult CreateDebugUtilsMessengerEXT(
                    VkInstance instance,
                    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                    const VkAllocationCallbacks* pAllocator,
                    VkDebugUtilsMessengerEXT* pDebugMessenger);

                void DestroyDebugUtilsMessengerEXT(
                    VkInstance instance,
                    VkDebugUtilsMessengerEXT debugMessenger,
                    const VkAllocationCallbacks* pAllocator);

            }
        }
    }
}