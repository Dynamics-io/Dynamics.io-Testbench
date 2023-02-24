#pragma once

#include "stdafx.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//#include "vulkan/vulkan.h"
//#include "vulkan-hpp/vulkan.hpp"

struct GLFWwindow;

namespace Dynamics_IO_Testbench {
    namespace Compute {
        namespace VK {

            class Vulkan_test {
            public:

                void Run() {
                    InitWindow();
                    InitVK();
                    MainLoop();
                    CleanUp();
                }

                void DoTest();

            private:

                struct QueueFamilyIndices {
                    std::optional<uint32_t> graphicsFamily;
                    std::optional<uint32_t> presentFamily;

                    bool isComplete() {
                        return graphicsFamily.has_value() &&
                               presentFamily.has_value();
                    }
                };

                struct SwapChainSUpportDetails {
                    VkSurfaceCapabilitiesKHR capabilities;
                    std::vector<VkSurfaceFormatKHR> formats;
                    std::vector<VkPresentModeKHR> presentModes;
                };

                void InitWindow();

                void InitVK();

                void MainLoop();

                void CleanUp();


                bool checkValidationLayerSupport();

                std::vector<const char*> getRequiredExtensions();

                void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

                void createInstance();

                void setupDebugMessenger();

                void createSurface();


                void pickPhysicalDevice();

                bool isDeviceSuitable(VkPhysicalDevice device);

                bool checkDeviceExtensionSupport(VkPhysicalDevice device);

                QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

                void createLogicalDevice();


                void createSwapChain();

                SwapChainSUpportDetails querySwapChainSupport(VkPhysicalDevice device);

                VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

                VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

                VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

                
                void createImageViews();


                void createRenderPass();


                void createGraphicsPipeline();

                VkShaderModule createShaderModule(const std::vector<char>& code);


                void createFramebuffers();


                void createCommandPool();

                void createCommandBuffer();

                void createSyncObjects();

                void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

                void drawFrame();

                static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                    void* pUserData);
                
                static std::vector<char> readFile(const std::string& filename);

                GLFWwindow* window;
                VkInstance instance;
                VkDebugUtilsMessengerEXT debugMessenger;
                VkSurfaceKHR surface;
                VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
                VkDevice device;
                VkQueue graphicsQueue;
                VkQueue presentQueue;
                VkSwapchainKHR swapChain;
                std::vector<VkImage> swapChainImages;
                VkFormat swapChainImageFormat;
                VkExtent2D swapChainExtent;
                std::vector<VkImageView> swapChainImageViews;
                VkRenderPass renderPass;
                VkPipelineLayout pipelineLayout;
                VkPipeline graphicsPipeline;
                std::vector<VkFramebuffer> swapChainFrameBuffers;
                VkCommandPool commandPool;
                VkCommandBuffer commandBuffer;
                VkSemaphore imageAvailableSemaphore;
                VkSemaphore renderFinishedSemaphore;
                VkFence inFlightFence;

                const uint32_t WIDTH = 800;
                const uint32_t HEIGHT = 600;

                const std::vector<const char*> validationLayers = {
                    "VK_LAYER_KHRONOS_validation"
                };

                const std::vector<const char*> deviceExtensions = {
                    VK_KHR_SWAPCHAIN_EXTENSION_NAME
                };

#ifdef NDEBUG
                const bool enableValidationLayers = false;
#else
                const bool enableValidationLayers = true;
#endif
            };

        }
    }
}

