#pragma once

#include "stdafx.h"
#include "vulkan/vulkan.h"

#define DEFAULT_WORK_GROUP_SIZE 16

namespace Dynamics_IO_Testbench {
    namespace Compute {
        namespace VK {

            class ComputeContext;
            class ComputeEngine;
            class ComputeKernel;
            class ComputeBuffer;
            class ComputeProgram;


            class ComputeBuffer {
                friend class ComputeContext;

            public:
                enum class Buffer_Type
                {
                    READ = 0,
                    Write = 1,
                    Read_Write = 2
                };

            private:

                ComputeContext* mContext;
                Buffer_Type mType;
                VkBuffer mBuffer;
                VkDeviceMemory mBufferMemory;
                VkDeviceSize mSize;

                VkPhysicalDevice* mPhysicalDevice;
                VkDevice* mLogicalDevice;

                VkQueue* mTransferQueue;
                VkCommandBuffer* mTransferCmdBuffer;

                bool mSeperateTransferQueue;
                std::vector<uint32_t> mAllQueueFamilies;

            public:
                ~ComputeBuffer();

                int SetData(void* data);

                int GetData(void* outData);

                VkBuffer* Get_VK_Mem()
                {
                    return &mBuffer;
                }

                VkDeviceSize GetSize() { return mSize; };

            private:
                ComputeBuffer(ComputeContext* context, Buffer_Type type, VkDeviceSize size);

                void getAllQueueFamilies();

            };

            class ComputeKernel {
                friend class ComputeProgram;
            
                struct BoundBuffer {
                    ComputeBuffer* Buffer;
                    uint32_t BindIndex;
                };

                std::string mName;
                ComputeProgram* mProgram;
                VkShaderModule* mProgramModule;

                VkDevice* mDevice;
                VkDescriptorSetLayout mComputeDescriptorSetLayout;
                VkDescriptorSet mComputeDescriptorSet;
                VkDescriptorPool mDescriptorPool;
                VkPipelineLayout mComputePipelineLayout;
                VkPipeline mComputePipeline;

                VkQueue* mComputeQueue;
                VkQueue* mTransferQueue;
                VkCommandBuffer* mComputeCmdBuffer;
                VkCommandBuffer* mTransferCmdBuffer;

                uint32_t mWorkGroupSize;

                std::vector<BoundBuffer> mBoundBuffers;

            public:
                ~ComputeKernel();
                
                int SetBuffer(ComputeBuffer* buffer, int arg);

                VkResult BuildKernel();

                int Execute(uint32_t x, uint32_t y, uint32_t z);

            private:
                ComputeKernel(std::string name, ComputeProgram* program);

                VkResult createDescriptorSetLayout();

                VkResult createComputePipeline();

                VkResult createDescriptorPool();

                VkResult createDescriptorSets();

            };

            class ComputeProgram {
                friend class ComputeContext;

                std::string mName;
                ComputeContext* mContext;
                VkDevice* mDevice;
                VkShaderModule mProgramModule;

                std::map<std::string, ComputeKernel*> kernels;

            public:
                ~ComputeProgram();


                std::string GetName() {
                    return mName;
                }

                ComputeContext* GetContext() {
                    return mContext;
                }

                VkDevice* GetLogicalDevice() {
                    return mDevice;
                }

                VkShaderModule* GetProgramModule() {
                    return &mProgramModule;
                }

                int Set_Source(const char* source);

                int Set_Binary(const void* binary, size_t length);

                int Set_Binary_File(std::string file_path);

                ComputeKernel* GetKernel(std::string name);

            private:
                ComputeProgram(std::string name, ComputeContext* context);

            };

            class ComputeContext
            {
                friend class ComputeEngine;
            public:
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

            private:

                const bool IncludeGraphics = false;
                
                std::map<std::string, ComputeProgram*> programs;

                VkInstance* mInstance;
                VkPhysicalDevice mPhysicalDevice;
                QueueFamilyIndices mIndices;
                VkDevice mDevice;
                VkQueue mGraphicsQueue;
                VkQueue mComputeQueue;
                VkQueue mTransferQueue;
                VkCommandPool mGraphicsCmdPool;
                VkCommandPool mComputeCmdPool;
                VkCommandPool mTransferCmdPool;
                VkCommandBuffer mGraphicsCmdBuffer;
                VkCommandBuffer mComputeCmdBuffer;
                VkCommandBuffer mTransferCmdBuffer;

                const std::vector<const char*> DeviceExtensions = {
                    
                };

            public:
                ~ComputeContext();


                VkQueue* GetPreferedComputeQueue();
                VkQueue* GetPreferedTransferQueue();
                VkCommandBuffer* GetPreferedComputeCmdBuffer();
                VkCommandBuffer* GetPreferedTransferCmdBuffer();

                VkInstance* GetInstance() {
                    return mInstance;
                }

                QueueFamilyIndices getQueueFamilyIndices() {
                    return mIndices;
                }

                VkPhysicalDevice* GetPhysicalDevice() {
                    return &mPhysicalDevice;
                }

                VkDevice* GetLogicalDevice() {
                    return &mDevice;
                }

                ComputeProgram* Add_Program(std::string name);
                ComputeProgram* Add_Program_Source(std::string name, const char* source);
                ComputeProgram* Add_Program_SPIRV(std::string name, const void* binary, size_t length);
                ComputeProgram* Add_Program_SPIRV_File(std::string name, std::string file_path);
                ComputeProgram* Programs(std::string name);

            private:
                ComputeContext(VkInstance* instance, VkPhysicalDevice device);

                QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

                VkResult createLogicalDevice();

                void getQueueInstances();

                bool createCommandPools();

                void createCommandBuffers();
            };


            class ComputeEngine
            {
                static std::vector<VkExtensionProperties> mExtensions;
                static VkInstance mInstance;
                static VkDebugUtilsMessengerEXT mDebugMessenger;
                static bool mEnableValidationLayers;

                static std::string mApp_dir;

                static std::vector<ComputeContext> mContexts;

            public:
                static int Init(std::string dir);

                static ComputeContext* GetNewContext(VkPhysicalDevice device);

                static std::string GetAppDir()
                {
                    return mApp_dir;
                }

                static std::string Get_VK_Version();

                static bool ValidationEnabled() {
                    return mEnableValidationLayers;
                }

                static std::vector<const char*> GetValidationLayers() {
                    return validationLayers;
                }

                static void Dispose();
                
            private:
                static const std::vector<const char*> validationLayers;

                static VkResult createInstance();

                static void setupDebugMessenger();

                static std::vector<const char*> getRequiredExtensions();

                static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

                static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
                    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                    void* pUserData);

            };



        }
    }
}

