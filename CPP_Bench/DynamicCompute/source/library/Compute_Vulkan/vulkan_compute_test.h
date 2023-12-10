#pragma once

#include "stdafx.h"
#include "vulkan/vulkan.h"

namespace DynamicCompute {
    namespace Compute {
        namespace VK {

            class Vulkan_compute_test {
            public:
                void Run() {
                    InitVK();
                    RunShader();
                    CleanUp();
                }

            private:

                struct QueueFamilyIndices {
                    bool shouldIncludeGraphics = false;

                    std::optional<uint32_t> graphicsFamily;
                    std::optional<uint32_t> transferFamily;
                    std::optional<uint32_t> computeFamily;

                    bool isComplete() {
                        if (shouldIncludeGraphics) {
                            return
                                graphicsFamily.has_value() &&
                                computeFamily.has_value() &&
                                transferFamily.has_value();
                        }

                        return
                            computeFamily.has_value() &&
                            transferFamily.has_value();
                    }
                };

                struct Data {
                    glm::vec1 value;
                };

                void InitVK();

                void RunShader();

                void CleanUp();


                // Initialization Helpers

                std::vector<const char*> getRequiredExtensions();

                void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

                void createInstance();

                void setupDebugMessenger();


                QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);


                void pickPhysicalDevice();

                bool isDeviceSuitable(VkPhysicalDevice device);

                void createLogicalDevice();

                void getQueueInstances();

                void createCommandPools();

                void createCommandBuffers();


                void createBuffers();

                void createDescriptorSetLayout();

                void createComputePipeline();
                

                void createDescriptorPool();

                void createDescriptorSets();


                void Execute();


                void printResults();


                static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                    void* pUserData);


                VkInstance instance;
                VkDebugUtilsMessengerEXT debugMessenger;
                VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
                VkDevice device;
                VkQueue graphicsQueue;
                VkQueue computeQueue;
                VkQueue transferQueue;
                VkCommandPool graphicsCmdPool;
                VkCommandPool computeCmdPool;
                VkCommandPool transferCmdPool;
                VkCommandBuffer graphicsCmdBuffer;
                VkCommandBuffer computeCmdBuffer;
                VkCommandBuffer transferCmdBuffer;
                VkBuffer inputBuffer;
                VkDeviceMemory inputBufferMemory;
                VkBuffer outputBuffer;
                VkDeviceMemory outputBufferMemory;
                VkDescriptorSetLayout computeDescriptorSetLayout;
                VkDescriptorSet computeDescriptorSet;
                VkDescriptorPool descriptorPool;
                VkPipelineLayout computePipelineLayout;
                VkPipeline computePipeline;

                const std::vector<const char*> validationLayers = {
                    "VK_LAYER_KHRONOS_validation"
                };

                std::vector<const char*> deviceExtensions = {
                    
                };

                const bool IncludeGraphics = false;

#ifdef NDEBUG
                const bool enableValidationLayers = false;
#else
                const bool enableValidationLayers = true;
#endif
                
            };
        }
    }
}
