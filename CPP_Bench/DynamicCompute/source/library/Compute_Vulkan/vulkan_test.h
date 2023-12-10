#pragma once

#include "stdafx.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

//#include "vulkan/vulkan.h"
//#include "vulkan-hpp/vulkan.hpp"
//#include "glm/glm.hpp"

struct GLFWwindow;

namespace DynamicCompute {
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
                    std::optional<uint32_t> transferFamily;

                    bool isComplete() {
                        return graphicsFamily.has_value() &&
                               presentFamily.has_value() &&
                               transferFamily.has_value();
                    }
                };

                struct SwapChainSUpportDetails {
                    VkSurfaceCapabilitiesKHR capabilities;
                    std::vector<VkSurfaceFormatKHR> formats;
                    std::vector<VkPresentModeKHR> presentModes;
                };

                struct Vertex {
                    glm::vec3 pos;
                    glm::vec3 color;
                    glm::vec2 texCoord;

                    static VkVertexInputBindingDescription getBindingDescription() {
                        VkVertexInputBindingDescription bindingDescription{};
                        bindingDescription.binding = 0;
                        bindingDescription.stride = sizeof(Vertex);
                        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                        return bindingDescription;
                    }

                    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
                        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

                        attributeDescriptions[0].binding = 0;
                        attributeDescriptions[0].location = 0;
                        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                        attributeDescriptions[0].offset = offsetof(Vertex, pos);

                        attributeDescriptions[1].binding = 0;
                        attributeDescriptions[1].location = 1;
                        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                        attributeDescriptions[1].offset = offsetof(Vertex, color);

                        attributeDescriptions[2].binding = 0;
                        attributeDescriptions[2].location = 2;
                        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
                        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

                        return attributeDescriptions;
                    }
                };

                struct UniformBufferObject {
                    alignas(16) glm::mat4 model;
                    alignas(16) glm::mat4 view;
                    alignas(16) glm::mat4 proj;
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

                QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

                void createLogicalDevice();


                void createSwapChain();

                SwapChainSUpportDetails querySwapChainSupport(VkPhysicalDevice device);

                VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

                VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

                VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

                
                void createImageViews();


                void createRenderPass();


                void createDescriptorSetLayout();


                void createGraphicsPipeline();



                void createFramebuffers();


                void createCommandPool();

                void createDepthResources();
                VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
                VkFormat findDepthFormat();
                bool hasStencilComponent(VkFormat format) {
                    return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
                        format == VK_FORMAT_D24_UNORM_S8_UINT;
                }

                void createTextureImage();

                void createTextureImageView();

                void createTextureSampler();

                void createVertexBuffer();

                void createIndexBuffer();

                void createUniformBuffers();

                void createDescriptorPool();

                void createDescriptorSets();

                //uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

                void createCommandTransferBuffer();

                void createCommandBuffer();

                void createSyncObjects();

                void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);


                void drawFrame();

                void updateUniformBuffer(uint32_t currentImage);

                void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);


                static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                    void* pUserData);
                
                

                GLFWwindow* window;
                VkInstance instance;
                VkDebugUtilsMessengerEXT debugMessenger;
                VkSurfaceKHR surface;
                VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
                VkDevice device;
                VkQueue graphicsQueue;
                VkQueue presentQueue;
                VkQueue transferQueue;
                VkSwapchainKHR swapChain;
                std::vector<VkImage> swapChainImages;
                VkFormat swapChainImageFormat;
                VkExtent2D swapChainExtent;
                std::vector<VkImageView> swapChainImageViews;
                VkRenderPass renderPass;
                VkDescriptorSetLayout descriptorSetLayout;
                VkPipelineLayout pipelineLayout;
                VkPipeline graphicsPipeline;
                std::vector<VkFramebuffer> swapChainFrameBuffers;
                VkCommandPool commandPool;
                VkCommandPool tramsferCmdPool;
                VkCommandBuffer commandBuffer;
                VkCommandBuffer commandTransferBuffer;
                VkSemaphore imageAvailableSemaphore;
                VkSemaphore renderFinishedSemaphore;
                VkFence inFlightFence;
                VkBuffer vertexBuffer;
                VkDeviceMemory vertexBufferMemory;
                VkBuffer indexBuffer;
                VkDeviceMemory indexBufferMemory;
                VkDescriptorPool descriptorPool;
                std::vector<VkDescriptorSet> descriptorSets;
                VkImage textureImage;
                VkDeviceMemory textureImageMemory;
                VkImageView textureImageView;
                VkSampler textureSampler;
                VkImage depthImage;
                VkDeviceMemory depthImageMemory;
                VkImageView depthImageView;

                std::vector<VkBuffer> uniformBuffers;
                std::vector<VkDeviceMemory> uniformBuffersMemory;
                std::vector<void*> uniformBuffersMapped;


                const int MAX_FRAMES_IN_FLIGHT = 2;
                const uint32_t WIDTH = 800;
                const uint32_t HEIGHT = 600;

                uint32_t currentFrame = 0;

                const std::vector<const char*> validationLayers = {
                    "VK_LAYER_KHRONOS_validation"
                };

                std::vector<const char*> deviceExtensions = {
                    VK_KHR_SWAPCHAIN_EXTENSION_NAME
                };

                const std::vector<Vertex> vertices = {
                    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

                    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
                };

                const std::vector<uint16_t> vert_indices = {
                    0, 1, 2, 2, 3, 0,
                    4, 5, 6, 6, 7, 4
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

