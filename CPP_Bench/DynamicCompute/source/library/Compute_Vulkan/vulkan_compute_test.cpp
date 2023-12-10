#include "vulkan_compute_test.h"

#include "vulkan_utils.h"

using namespace DynamicCompute::Compute::VK;

#define BUFFER_SIZE 16

void DynamicCompute::Compute::VK::Vulkan_compute_test::InitVK()
{
    // Essential vulkan initializations
    createInstance();
    setupDebugMessenger();
    pickPhysicalDevice();
    createLogicalDevice();
    getQueueInstances();
    createCommandPools();
    createCommandBuffers();

    // resource creation
    createBuffers();
    createDescriptorSetLayout();
    createComputePipeline();
    createDescriptorPool();
    createDescriptorSets();

    // process
    Execute();

    // output
    printResults();

    
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::RunShader()
{
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::CleanUp()
{
    if (IncludeGraphics)
        vkDestroyCommandPool(device, graphicsCmdPool, nullptr);
    vkDestroyCommandPool(device, computeCmdPool, nullptr);
    vkDestroyCommandPool(device, transferCmdPool, nullptr);

    //vkDestroyBuffer(device, outputBuffer, nullptr);
    //vkFreeMemory(device, outputBufferMemory, nullptr);

    vkDestroyBuffer(device, inputBuffer, nullptr);
    vkFreeMemory(device, inputBufferMemory, nullptr);

    vkDestroyDevice(device, nullptr);

    if (enableValidationLayers) {
        Extensions::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

    vkDestroyInstance(instance, nullptr);
}


std::vector<const char*> DynamicCompute::Compute::VK::Vulkan_compute_test::getRequiredExtensions()
{

    std::vector<const char*> extensions;

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::createInstance()
{
    if (enableValidationLayers && !Utilities::checkValidationLayerSupport(validationLayers))
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo = Utilities::getApplicationInfo("Hello Triangle", VK_MAKE_VERSION(1, 0, 0), "No Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_3);


    std::vector<const char*> requiredExtensions = getRequiredExtensions();
    VkInstanceCreateInfo createInfo = Utilities::getInstanceCreateInfo(appInfo, requiredExtensions);


    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    // TODO: Add MacOS support

    std::vector<VkExtensionProperties> extensions = Utilities::EnumerateInstanceExtensionProperties();


    std::cout << "Vulkan Extensions: " << extensions.size() << "\n";
    for (const auto& ext : extensions) {
        std::cout << '\t' << ext.extensionName << '\n';
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance!");
    }
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::setupDebugMessenger()
{
    if (!enableValidationLayers)
        return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    populateDebugMessengerCreateInfo(createInfo);

    if (Extensions::CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to set up debug messenger!");
    }
}

Vulkan_compute_test::QueueFamilyIndices DynamicCompute::Compute::VK::Vulkan_compute_test::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    std::vector<VkQueueFamilyProperties> queueFamilies = Utilities::GetPhysicalDeviceQueueFamilyProperties(device);

    int i = 0;
    for (const auto& queueFam : queueFamilies) {
        if (IncludeGraphics) {
            if ((queueFam.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                (queueFam.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
                indices.shouldIncludeGraphics = true;
                indices.graphicsFamily = i;
                indices.computeFamily = i;
                std::cout << "Compute and Graphics Queue Family: " << std::to_string(i) << std::endl;
            }
        }
        else {
            if (queueFam.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                indices.computeFamily = i;
                std::cout << "Compute Queue Family: " << std::to_string(i) << std::endl;
            }
        }

        if (queueFam.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            indices.transferFamily = i;
            std::cout << "Transfer Queue Family: " << std::to_string(i) << std::endl;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::pickPhysicalDevice()
{
    std::vector<VkPhysicalDevice> devices = Utilities::EnumeratePhysicalDevices(instance);

    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

bool DynamicCompute::Compute::VK::Vulkan_compute_test::isDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionsSupported = Utilities::checkDeviceExtensionSupport(device, deviceExtensions);

    return indices.isComplete() && extensionsSupported;
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;


    std::set<uint32_t> uniqueQueueFamilies = {
        //indices.graphicsFamily.value(),
        indices.transferFamily.value(),
        indices.computeFamily.value()
    };

    float queuePriority = 1.0f;
    for (uint32_t queueFam : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = Utilities::getDeviceQueueCreateInfo(queueFam, 1, queuePriority);
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = Utilities::getPhysicalDeviceFeatures();

    VkDeviceCreateInfo createInfo = Utilities::getDeviceCreateInfo(queueCreateInfos, deviceFeatures, deviceExtensions);

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

}

void DynamicCompute::Compute::VK::Vulkan_compute_test::getQueueInstances()
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    if (IncludeGraphics)
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.computeFamily.value(), 0, &computeQueue);
    vkGetDeviceQueue(device, indices.transferFamily.value(), 0, &transferQueue);
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::createCommandPools()
{
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo graphics_poolInfo = Utilities::getCommandPoolCreateInfo(
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        IncludeGraphics ? queueFamilyIndices.graphicsFamily.value() : 0
    );

    VkCommandPoolCreateInfo compute_poolInfo = Utilities::getCommandPoolCreateInfo(
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        queueFamilyIndices.computeFamily.value()
    );

    VkCommandPoolCreateInfo transfer_poolInfo = Utilities::getCommandPoolCreateInfo(
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        queueFamilyIndices.transferFamily.value()
    );

    if (IncludeGraphics)
    {
        if (vkCreateCommandPool(device, &graphics_poolInfo, nullptr, &graphicsCmdPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create graphics command pool!");
        }
    }

    if (vkCreateCommandPool(device, &compute_poolInfo, nullptr, &computeCmdPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create transfer command pool!");
    }

    if (vkCreateCommandPool(device, &transfer_poolInfo, nullptr, &transferCmdPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create transfer command pool!");
    }

}

void DynamicCompute::Compute::VK::Vulkan_compute_test::createCommandBuffers()
{
    if (IncludeGraphics)
        Utilities::CreateCommandBuffer(graphicsCmdPool, device, graphicsCmdBuffer);
    Utilities::CreateCommandBuffer(computeCmdPool, device, computeCmdBuffer);
    Utilities::CreateCommandBuffer(transferCmdPool, device, transferCmdBuffer);
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::createBuffers()
{
    std::vector<Data> buffer_data(BUFFER_SIZE);
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer_data[i].value.x = (float)i;
    }


    VkDeviceSize bufferSize = sizeof(Data) * buffer_data.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;


    Utilities::CreateBuffer(
        physicalDevice,
        device,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        std::vector<uint32_t>(),
        stagingBuffer,
        stagingBufferMemory
    );

    void* data;
    Utilities::FlushToBuffer(device, stagingBufferMemory, bufferSize, data, buffer_data.data(), true);

    Utilities::CreateBuffer(
        physicalDevice,
        device,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        std::vector<uint32_t>(),
        inputBuffer,
        inputBufferMemory
    );


    Utilities::CopyBuffer(computeQueue, computeCmdBuffer, stagingBuffer, inputBuffer, 0, 0, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

    Utilities::CreateBuffer(
        physicalDevice,
        device,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        std::vector<uint32_t>(),
        outputBuffer,
        outputBufferMemory
    );

}

void DynamicCompute::Compute::VK::Vulkan_compute_test::createDescriptorSetLayout()
{
    
    if (Utilities::CreateComputeDescriptorSetLayout(device, 2, computeDescriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor set layout!");
    }
    std::cout << "Created Descriptor Set Layout.\n";
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::createComputePipeline()
{
    std::string shader_directory = "C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/Vulkan/";

    auto computeShaderBin = Utilities::readFile(shader_directory + "compute.spv");

    VkShaderModule computeShaderModule = Utilities::createShaderModule(device, computeShaderBin);

    std::string k_name = "main";
    VkPipelineShaderStageCreateInfo computeShaderStageInfo = Utilities::getPipelineShaderStageCreateInfo(computeShaderModule, k_name.c_str());

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = Utilities::getPipelineLayoutCreateInfo(computeDescriptorSetLayout);

    if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &computePipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute pipeline layout");
    }

    VkComputePipelineCreateInfo pipelineInfo = Utilities::getComputePipelineCreateInfo(computePipelineLayout, computeShaderStageInfo);

    if (vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &computePipeline) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create compute pipeline!");
    }
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::createDescriptorPool()
{
    if (Utilities::createDescriptorPool(device, 2, 1, descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create descriptor pool!");
    }
    std::cout << "Created Descriptor Pool.\n";
}

void DynamicCompute::Compute::VK::Vulkan_compute_test::createDescriptorSets()
{
    std::vector<VkDescriptorSetLayout> layouts(1, computeDescriptorSetLayout);

    VkResult res =Utilities::AllocateDescriptorSets(device, layouts, descriptorPool, 1, computeDescriptorSet);

    if (res != VK_SUCCESS)
        return; 

    std::vector<Utilities::DescriptorBufferInfo> descriptorBufferInfo(2);
    descriptorBufferInfo[0] = { inputBuffer, sizeof(Data) * BUFFER_SIZE, 0};
    descriptorBufferInfo[1] = { outputBuffer, sizeof(Data) * BUFFER_SIZE, 1};

    Utilities::UpdateDescriptorSets_StorageBuffers(device, computeDescriptorSet, descriptorBufferInfo);

    std::cout << "Created Descriptor Sets\n";

}

void DynamicCompute::Compute::VK::Vulkan_compute_test::Execute()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(computeCmdBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    vkCmdBindPipeline(computeCmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);

    vkCmdBindDescriptorSets(
        computeCmdBuffer,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        computePipelineLayout,
        0, 1,
        &computeDescriptorSet,
        0, 0);

    vkCmdDispatch(computeCmdBuffer, std::max(BUFFER_SIZE / 16, 1), 1, 1);

    if (vkEndCommandBuffer(computeCmdBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &computeCmdBuffer;

    if (vkQueueSubmit(computeQueue, 1, &submitInfo, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit compute command buffer!");
    }
    vkQueueWaitIdle(computeQueue);

}

void DynamicCompute::Compute::VK::Vulkan_compute_test::printResults()
{
    VkDeviceSize bufferSize = sizeof(Data) * BUFFER_SIZE;

    VkBuffer outputHostBuffer;
    VkDeviceMemory outputHostBufferMemory;

    Utilities::CreateBuffer(
        physicalDevice,
        device,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_SHARING_MODE_EXCLUSIVE,
        0,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        std::vector<uint32_t>(),
        outputHostBuffer,
        outputHostBufferMemory
    );

    Utilities::CopyBuffer(computeQueue, computeCmdBuffer, outputBuffer, outputHostBuffer, 0, 0, bufferSize);

    std::vector<Data> result_data(BUFFER_SIZE);

    Data* maped_data;
    vkMapMemory(device, outputHostBufferMemory, 0, bufferSize, 0, (void**)& maped_data);
    memcpy(result_data.data(), (Data*)maped_data, static_cast<uint32_t>(bufferSize));
    vkUnmapMemory(device, outputHostBufferMemory);
    
    int i = 0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        std::cout <<  std::to_string(i) + ": " + std::to_string(result_data[i].value.x) + "\n";
    }
    std::cout << std::endl;

    vkDestroyBuffer(device, outputHostBuffer, nullptr);
    vkFreeMemory(device, outputHostBufferMemory, nullptr);

}







VKAPI_ATTR VkBool32 VKAPI_CALL DynamicCompute::Compute::VK::Vulkan_compute_test::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        std::cerr << "validation layer ERROR: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }
    //std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}
