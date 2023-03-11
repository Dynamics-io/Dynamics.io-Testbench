#pragma once

#include "stdafx.h"
#include "vulkan/vulkan.h"
#include "Compute_Vulkan/vulkan_utils.h"
#include "Compute_Interface/PlatformStructures.h"

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

                ComputeContext* mContext{nullptr};
                Buffer_Type mType{};
                VkBuffer mBuffer{};
                VkDeviceMemory mBufferMemory{};
                VkBuffer stagingBuffer{};
                VkDeviceMemory stagingBufferMemory{};
                VkDeviceSize mSize{};

                VkPhysicalDevice* mPhysicalDevice{ nullptr };
                VkDevice* mLogicalDevice{ nullptr };

                VkQueue* mTransferQueue{ nullptr };
                VkCommandBuffer* mTransferCmdBuffer{ nullptr };

                bool mSeperateTransferQueue{ false };
                std::vector<uint32_t> mAllQueueFamilies;

                bool mDestroyed{ false };

            public:
                ComputeBuffer() {}
                ~ComputeBuffer();

                int SetData(void* data);

                int GetData(void* outData);

                VkBuffer* Get_VK_Mem()
                {
                    return &mBuffer;
                }

                VkDeviceSize GetSize() { return mSize; };

                void Dispose();

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

                std::string mName{ "" };
                ComputeProgram* mProgram{ nullptr };
                VkShaderModule* mProgramModule{ nullptr };

                VkDevice* mDevice{ nullptr };
                VkDescriptorSetLayout mComputeDescriptorSetLayout{};
                VkDescriptorSet mComputeDescriptorSet{};
                VkDescriptorPool mDescriptorPool{};
                VkPipelineLayout mComputePipelineLayout{};
                VkPipeline mComputePipeline{};

                VkQueue* mComputeQueue{ nullptr };
                VkQueue* mTransferQueue{ nullptr };
                VkCommandBuffer* mComputeCmdBuffer{ nullptr };
                VkCommandBuffer* mTransferCmdBuffer{ nullptr };

                uint32_t mWorkGroupSize{ DEFAULT_WORK_GROUP_SIZE };

                std::vector<BoundBuffer> mBoundBuffers;

                bool mDestroyed{ false };

            public:
                ComputeKernel() {}
                ~ComputeKernel();
                
                int SetBuffer(ComputeBuffer* buffer, int arg);

                VkResult BuildKernel();

                int Execute(uint32_t x, uint32_t y, uint32_t z);

                void Dispose();

            private:
                ComputeKernel(std::string name, ComputeProgram* program);

                VkResult createDescriptorSetLayout();

                VkResult createComputePipeline();

                VkResult createDescriptorPool();

                VkResult createDescriptorSets();

            };

            class ComputeProgram {
                friend class ComputeContext;

                std::string mName{""};
                ComputeContext* mContext{ nullptr };
                VkDevice* mDevice{ nullptr };
                VkShaderModule mProgramModule{};

                std::map<std::string, ComputeKernel> kernels;

                bool mDestroyed{ false };

            public:
                ComputeProgram(){}
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

                int Buildkernels();

                void Dispose();

            private:
                ComputeProgram(std::string name, ComputeContext* context);

            };

            class ComputeContext
            {
                friend class ComputeEngine;

            private:

                const bool IncludeGraphics = false;
                
                std::map<std::string, ComputeProgram> programs;
                std::vector<ComputeBuffer> mBuffers;

                VkInstance* mInstance{nullptr};
                VkPhysicalDevice mPhysicalDevice{};
                Utilities::QueueFamilyIndices mIndices{};
                VkDevice mDevice{};
                VkQueue mGraphicsQueue{};
                VkQueue mComputeQueue{};
                VkQueue mTransferQueue{};
                VkCommandPool mGraphicsCmdPool{};
                VkCommandPool mComputeCmdPool{};
                VkCommandPool mTransferCmdPool{};
                VkCommandBuffer mGraphicsCmdBuffer{};
                VkCommandBuffer mComputeCmdBuffer{};
                VkCommandBuffer mTransferCmdBuffer{};

                bool mDestroyed{ false };

                const std::vector<const char*> DeviceExtensions = {
                    
                };

            public:
                ComputeContext(){}
                ~ComputeContext();


                VkQueue* GetPreferedComputeQueue();
                VkQueue* GetPreferedTransferQueue();
                VkCommandBuffer* GetPreferedComputeCmdBuffer();
                VkCommandBuffer* GetPreferedTransferCmdBuffer();

                VkInstance* GetInstance() {
                    return mInstance;
                }

                Utilities::QueueFamilyIndices getQueueFamilyIndices() {
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

                ComputeKernel* GetKernel(std::string p_name, std::string name);
                ComputeBuffer* CreateBuffer(ComputeBuffer::Buffer_Type type, VkDeviceSize size);

                void Dispose();

            private:
                ComputeContext(VkInstance* instance, Device device);

                //Utilities::QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

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

                static bool mInitialized;

                static std::string mApp_dir;

                static std::vector<ComputeContext> mContexts;

            public:
                static int Init(std::string dir);

                static ComputeContext* GetNewContext(Device device);

                static std::string GetAppDir()
                {
                    return mApp_dir;
                }

                static std::string Get_VK_Version();

                static bool ValidationEnabled() {
                    return mEnableValidationLayers;
                }

                static bool IsInitialized() {
                    return mInitialized;
                }

                static const std::vector<const char*> GetValidationLayers();

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
