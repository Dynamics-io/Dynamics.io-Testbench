#include "Compute_Vulkan/ComputeEngine.h"

#include "vulkan/vulkan.h"
#include "vulkan_utils.h"
#include "vk_mem_alloc.h"

using namespace Dynamics_IO_Testbench::Compute::VK;

std::vector<VkExtensionProperties> ComputeEngine::mExtensions;
VkInstance ComputeEngine::mInstance;
VkDebugUtilsMessengerEXT ComputeEngine::mDebugMessenger;
bool ComputeEngine::mEnableValidationLayers;

std::string ComputeEngine::mApp_dir = "";

const std::vector<const char*> ComputeEngine::validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

// ComputeEngine

int ComputeEngine::Init(std::string dir)
{
#ifdef NDEBUG
	mEnableValidationLayers = false;
#else
	mEnableValidationLayers = true;
#endif

	mApp_dir = dir;

	VkResult res = createInstance();

	if (res == VK_SUCCESS)
		setupDebugMessenger();

	return (int)res;
}

ComputeContext* ComputeEngine::GetNewContext(VkPhysicalDevice device)
{
	mContexts.push_back(ComputeContext(&mInstance, device));
	return &mContexts[mContexts.size() - 1];
}

std::string ComputeEngine::Get_VK_Version()
{
	return "1.3";
}

VkResult ComputeEngine::createInstance()
{
	if (mEnableValidationLayers && !Utilities::checkValidationLayerSupport(validationLayers))
	{
		mEnableValidationLayers = false;
	}

	VkApplicationInfo appInfo = Utilities::getApplicationInfo(
		"Dynamics_IO_VK",
		VK_MAKE_VERSION(1, 0, 0),
		"Dynamics_IO",
		VK_MAKE_VERSION(1, 0, 0),
		VK_API_VERSION_1_3);

	std::vector<const char*> requiredExtensions = getRequiredExtensions();

	VkInstanceCreateInfo createInfo = Utilities::getInstanceCreateInfo(appInfo, requiredExtensions);

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (mEnableValidationLayers) {
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

	mExtensions = Utilities::EnumerateInstanceExtensionProperties();

	VkResult res;
	res = vkCreateInstance(&createInfo, nullptr, &mInstance);

	return res;
}

void ComputeEngine::setupDebugMessenger()
{
	if (!mEnableValidationLayers)
		return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	populateDebugMessengerCreateInfo(createInfo);

	if (Extensions::CreateDebugUtilsMessengerEXT(mInstance, &createInfo, nullptr, &mDebugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to set up debug messenger!");
	}
}

std::vector<const char*> ComputeEngine::getRequiredExtensions()
{
	std::vector<const char*> extensions;

	if (mEnableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void ComputeEngine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
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

void ComputeEngine::Dispose()
{
	mContexts.clear();

	if (mEnableValidationLayers) {
		Extensions::DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
	}

	vkDestroyInstance(mInstance, nullptr);
}

VKAPI_ATTR VkBool32 VKAPI_CALL ComputeEngine::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		std::cerr << "validation layer ERROR: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
	//std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}


// Compute Context

ComputeContext::ComputeContext(VkInstance* instance, VkPhysicalDevice device) {
	mInstance = instance;
	mPhysicalDevice = device;
	mIndices = findQueueFamilies(device);

	VkResult res = createLogicalDevice();

	getQueueInstances();

	bool success = createCommandPools();

	if (success)
		createCommandBuffers();
}

VkQueue* ComputeContext::GetPreferedComputeQueue()
{
	// TODO: Better logic to determine this
	return &mComputeQueue;
}

VkQueue* ComputeContext::GetPreferedTransferQueue()
{
	// TODO: Better logic to determine this
	return &mTransferQueue;
}

VkCommandBuffer* ComputeContext::GetPreferedComputeCmdBuffer()
{
	// TODO: Better logic to determine this
	return &mComputeCmdBuffer;
}

VkCommandBuffer* ComputeContext::GetPreferedTransferCmdBuffer()
{
	// TODO: Better logic to determine this
	return &mTransferCmdBuffer;
}

ComputeProgram* ComputeContext::Add_Program(std::string name) {
	ComputeProgram* program = new ComputeProgram(name, this);
	programs[name] = program;
	return program;
}

ComputeProgram* ComputeContext::Add_Program_Source(std::string name, const char* source) {
	if (programs.count(name) == 0)
	{
		Add_Program(name);
	}
	int res = programs[name]->Set_Source(source);
	return programs[name];
}

ComputeProgram* ComputeContext::Add_Program_SPIRV(std::string name, const void* binary, size_t length) {
	if (programs.count(name) == 0)
	{
		Add_Program(name);
	}
	int res = programs[name]->Set_Binary(binary, length);
	return programs[name];
}

ComputeProgram* ComputeContext::Add_Program_SPIRV_File(std::string name, std::string file_path)
{
	if (programs.count(name) == 0)
	{
		Add_Program(name);
	}
	int res = programs[name]->Set_Binary_File(file_path);
	return programs[name];
}

ComputeProgram* ComputeContext::Programs(std::string name) {
	if (programs.count(name) > 0) {
		return programs[name];
	}
	return nullptr;
}

ComputeContext::QueueFamilyIndices ComputeContext::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	std::vector<VkQueueFamilyProperties> queueFamilies = Utilities::GetPhysicalDeviceQueueFamilyProperties(device);

	int i = 0;
	for (const auto& queueFam : queueFamilies) {
		if ((queueFam.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
			!(queueFam.queueFlags & VK_QUEUE_TRANSFER_BIT)) {
			indices.computeFamily = i;
		}

		if (!(queueFam.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
			(queueFam.queueFlags & VK_QUEUE_TRANSFER_BIT)) {
			indices.transferFamily = i;
		}
	}

	int i = 0;
	if (!indices.computeFamily.has_value() || !indices.computeFamily.has_value()) {
		for (const auto& queueFam : queueFamilies) {
			if ((queueFam.queueFlags & VK_QUEUE_TRANSFER_BIT) &&
				(queueFam.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
				indices.shouldIncludeGraphics = true;
				indices.transferFamily = i;
				indices.computeFamily = i;
			}
		}
	}

	return indices;
}

VkResult ComputeContext::createLogicalDevice()
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;


	std::vector<uint32_t> uniqueQueueFamilies;
	if (mIndices.computeFamily.value() == mIndices.transferFamily.value())
	{
		if (IncludeGraphics)
			uniqueQueueFamilies.push_back(mIndices.graphicsFamily.value());
		uniqueQueueFamilies.push_back(mIndices.transferFamily.value());
		uniqueQueueFamilies.push_back(mIndices.computeFamily.value());
	}
	else 
	{
		if (IncludeGraphics)
			uniqueQueueFamilies.push_back(mIndices.graphicsFamily.value());
		uniqueQueueFamilies.push_back(mIndices.computeFamily.value());
	}

	float queuePriority = 1.0f;
	for (uint32_t queueFam : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = Utilities::getDeviceQueueCreateInfo(queueFam, 1, queuePriority);
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = Utilities::getPhysicalDeviceFeatures();

	VkDeviceCreateInfo createInfo = Utilities::getDeviceCreateInfo(queueCreateInfos, deviceFeatures, (std::vector<const char*>)DeviceExtensions);

	if (ComputeEngine::ValidationEnabled()) {
		std::vector<const char*> layers = ComputeEngine::GetValidationLayers();
		createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		createInfo.ppEnabledLayerNames = layers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	VkResult res;
	res = vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice);

	return res;
}

void ComputeContext::getQueueInstances()
{
	if (IncludeGraphics)
		vkGetDeviceQueue(mDevice, mIndices.graphicsFamily.value(), 0, &mGraphicsQueue);
	vkGetDeviceQueue(mDevice, mIndices.computeFamily.value(), 0, &mComputeQueue);
	vkGetDeviceQueue(mDevice, mIndices.transferFamily.value(), 0, &mTransferQueue);
}

bool ComputeContext::createCommandPools()
{
	VkCommandPoolCreateInfo graphics_poolInfo = Utilities::getCommandPoolCreateInfo(
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		IncludeGraphics ? mIndices.graphicsFamily.value() : 0
	);

	VkCommandPoolCreateInfo compute_poolInfo = Utilities::getCommandPoolCreateInfo(
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		mIndices.computeFamily.value()
	);

	VkCommandPoolCreateInfo transfer_poolInfo = Utilities::getCommandPoolCreateInfo(
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		mIndices.transferFamily.value()
	);

	if (IncludeGraphics)
	{
		if (vkCreateCommandPool(mDevice, &graphics_poolInfo, nullptr, &mGraphicsCmdPool) != VK_SUCCESS) {
			return false;
		}
	}

	if (vkCreateCommandPool(mDevice, &compute_poolInfo, nullptr, &mComputeCmdPool) != VK_SUCCESS) {
		return false;
	}

	if (vkCreateCommandPool(mDevice, &transfer_poolInfo, nullptr, &mTransferCmdPool) != VK_SUCCESS) {
		return false;
	}

	return true;
}

void ComputeContext::createCommandBuffers()
{
	if (IncludeGraphics)
		Utilities::CreateCommandBuffer(mGraphicsCmdPool, mDevice, mGraphicsCmdBuffer);
	Utilities::CreateCommandBuffer(mComputeCmdPool, mDevice, mComputeCmdBuffer);
	Utilities::CreateCommandBuffer(mTransferCmdPool, mDevice, mTransferCmdBuffer);
}

ComputeContext::~ComputeContext() {

	programs.clear();

	if (IncludeGraphics)
		vkDestroyCommandPool(mDevice, mGraphicsCmdPool, nullptr);
	vkDestroyCommandPool(mDevice, mComputeCmdPool, nullptr);
	vkDestroyCommandPool(mDevice, mTransferCmdPool, nullptr);

	vkDestroyDevice(mDevice, nullptr);
}

// Compute Program

ComputeProgram::ComputeProgram(std::string name, ComputeContext* context) {
	mName = name;
	mContext = context;
	mProgramModule = nullptr;
	mDevice = context->GetLogicalDevice();
}

int ComputeProgram::Set_Source(const char* source) {
	// TODO: implement SPIRV generation from source.
	return 0;
}

int ComputeProgram::Set_Binary(const void* binary, size_t length) {

	char* char_binary = (char*) binary;
	std::vector<char> shader_bin (char_binary, char_binary + length);

	mProgramModule = Utilities::createShaderModule(*mDevice, shader_bin);

	return 0;
}

int ComputeProgram::Set_Binary_File(std::string file_path) {
	auto computeShaderBin = Utilities::readFile(file_path);

	mProgramModule = Utilities::createShaderModule(*mDevice, computeShaderBin);

	return 0;
}

ComputeKernel* ComputeProgram::GetKernel(std::string name) {
	if (kernels.count(name) > 0)
	{
		return kernels[name];
	}

	return nullptr;
}

ComputeProgram::~ComputeProgram() {

	kernels.clear();

	vkDestroyShaderModule(*mDevice, mProgramModule, nullptr);
}


// Compute Kernel

ComputeKernel::ComputeKernel(std::string name, ComputeProgram* program) {
	mName = name;
	mProgram = program;
	mProgramModule = program->GetProgramModule();
	mDevice = program->GetLogicalDevice();

	ComputeContext* context = program->GetContext();

	mComputeQueue = context->GetPreferedComputeQueue();
	mTransferQueue = context->GetPreferedTransferQueue();

	mComputeCmdBuffer = context->GetPreferedComputeCmdBuffer();
	mTransferCmdBuffer = context->GetPreferedTransferCmdBuffer();

	mWorkGroupSize = DEFAULT_WORK_GROUP_SIZE;
}

int ComputeKernel::SetBuffer(ComputeBuffer* buffer, int arg)
{
	BoundBuffer buffer_info{};
	buffer_info.Buffer = buffer;
	buffer_info.BindIndex = arg;

	mBoundBuffers.push_back(buffer_info);

	return 0;
}

VkResult ComputeKernel::BuildKernel()
{
	VkResult res = createDescriptorSetLayout();

	if (res != VK_SUCCESS)
		return res;

	res = createComputePipeline();

	if (res != VK_SUCCESS)
		return res;

	res = createDescriptorPool();

	if (res != VK_SUCCESS)
		return res;

	res = createDescriptorSets();

	mBoundBuffers.clear();

	return res;
}

int ComputeKernel::Execute(uint32_t x, uint32_t y, uint32_t z)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;


	if (vkBeginCommandBuffer(*mComputeCmdBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("Failed to begin recording command buffer!");
	}

	vkCmdBindPipeline(*mComputeCmdBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, mComputePipeline);

	vkCmdBindDescriptorSets(
		*mComputeCmdBuffer,
		VK_PIPELINE_BIND_POINT_COMPUTE,
		mComputePipelineLayout,
		0, 1,
		&mComputeDescriptorSet,
		0, 0);

	vkCmdDispatch(
		*mComputeCmdBuffer, 
		std::max((int)x / (int)mWorkGroupSize, 1), 
		std::max((int)y / (int)mWorkGroupSize, 1), 
		std::max((int)z / (int)mWorkGroupSize, 1));

	if (vkEndCommandBuffer(*mComputeCmdBuffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to record command buffer!");
	}

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = mComputeCmdBuffer;

	if (vkQueueSubmit(*mComputeQueue, 1, &submitInfo, nullptr) != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit compute command buffer!");
	}
	vkQueueWaitIdle(*mComputeQueue);

	return 0;
}

VkResult ComputeKernel::createDescriptorSetLayout()
{
	int numBuffers = mBoundBuffers.size();

	return Utilities::CreateComputeDescriptorSetLayout(*mDevice, numBuffers, mComputeDescriptorSetLayout);
}

VkResult ComputeKernel::createComputePipeline()
{
	VkPipelineShaderStageCreateInfo computeShaderStageInfo = Utilities::getPipelineShaderStageCreateInfo(*mProgramModule, mName.c_str());

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = Utilities::getPipelineLayoutCreateInfo(mComputeDescriptorSetLayout);

	VkResult res = vkCreatePipelineLayout(*mDevice, &pipelineLayoutInfo, nullptr, &mComputePipelineLayout);

	if (res != VK_SUCCESS)
		return res;

	VkComputePipelineCreateInfo pipelineInfo = Utilities::getComputePipelineCreateInfo(mComputePipelineLayout, computeShaderStageInfo);

	res = vkCreateComputePipelines(*mDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mComputePipeline);

	return res;
}

VkResult ComputeKernel::createDescriptorPool()
{
	int numBuffers = mBoundBuffers.size();

	return Utilities::createDescriptorPool(*mDevice, numBuffers, 1, mDescriptorPool);
}

VkResult ComputeKernel::createDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(1, mComputeDescriptorSetLayout);

	VkResult res = Utilities::AllocateDescriptorSets(*mDevice, layouts, mDescriptorPool, 1, mComputeDescriptorSet);

	if (res != VK_SUCCESS)
		return;

	int numBuffers = mBoundBuffers.size();

	std::vector<Utilities::DescriptorBufferInfo> descriptorBufferInfo(numBuffers);

	for (int i = 0; i < numBuffers; i++) {
		Utilities::DescriptorBufferInfo bufferInfo{};

		BoundBuffer bufferBindInfo = mBoundBuffers[i];
		ComputeBuffer* comp_buffer = bufferBindInfo.Buffer;

		bufferInfo.buffer = *comp_buffer->Get_VK_Mem(); // TODO: populate buffer info based on bound buffers
		bufferInfo.size = comp_buffer->GetSize();
		bufferInfo.binding = bufferBindInfo.BindIndex;

		descriptorBufferInfo[i] = bufferInfo;
	}

	Utilities::UpdateDescriptorSets_StorageBuffers(*mDevice, mComputeDescriptorSet, descriptorBufferInfo);
}

ComputeKernel::~ComputeKernel() {

	vkDestroyPipeline(*mDevice, mComputePipeline, nullptr);
	vkDestroyPipelineLayout(*mDevice, mComputePipelineLayout, nullptr);
	vkDestroyDescriptorPool(*mDevice, mDescriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(*mDevice, mComputeDescriptorSetLayout, nullptr);
}


// Compute Buffer

ComputeBuffer::ComputeBuffer(ComputeContext* context, Buffer_Type type, VkDeviceSize size) {
	mContext = context;
	mType = type;
	mSize = size;

	mPhysicalDevice = context->GetPhysicalDevice();
	mLogicalDevice = context->GetLogicalDevice();

	mTransferQueue = context->GetPreferedTransferQueue();
	mTransferCmdBuffer = context->GetPreferedTransferCmdBuffer();

	getAllQueueFamilies();

	// Marks whether this is tranfering from or to the staging buffer.
	// If it Read Only, it should be the destination (DST) from the staging buffer. (Host -> Buffer_DST)
	// If it is Write Only, it should be the src of the staging buffer. (Buffer_SRC -> Host)
	// If it is Read and Write, it should be both.
	VkBufferUsageFlags transfer_flag; 
	switch (mType) {
	case Buffer_Type::READ:
		transfer_flag = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		break;

	case Buffer_Type::Write:
		transfer_flag = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		break;

	case Buffer_Type::Read_Write:
		transfer_flag = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		break;
	}

	Utilities::CreateBuffer(
		*mPhysicalDevice,
		*mLogicalDevice,
		mSize,
		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | transfer_flag,
		VK_SHARING_MODE_CONCURRENT,
		0,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		mAllQueueFamilies,
		mBuffer,
		mBufferMemory
	);
}

int ComputeBuffer::SetData(void* src_data) {
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	Utilities::CreateBuffer(
		*mPhysicalDevice,
		*mLogicalDevice,
		mSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_SHARING_MODE_CONCURRENT,
		0,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		mAllQueueFamilies,
		stagingBuffer,
		stagingBufferMemory
	);

	void* data;
	Utilities::FlushToBuffer(*mLogicalDevice, stagingBufferMemory, mSize, data, src_data, true);

	Utilities::CopyBuffer(*mTransferQueue, *mTransferCmdBuffer, stagingBuffer, mBuffer, mSize);

	vkDestroyBuffer(*mLogicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(*mLogicalDevice, stagingBufferMemory, nullptr);
}

int ComputeBuffer::GetData(void* outData) {
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	Utilities::CreateBuffer(
		*mPhysicalDevice,
		*mLogicalDevice,
		mSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_SHARING_MODE_CONCURRENT,
		0,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		mAllQueueFamilies,
		stagingBuffer,
		stagingBufferMemory
	);

	Utilities::CopyBuffer(*mTransferQueue, *mTransferCmdBuffer, mBuffer, stagingBuffer, mSize);

	void* maped_data;
	vkMapMemory(*mLogicalDevice, stagingBufferMemory, 0, mSize, 0, &maped_data);
	memcpy(outData, maped_data, static_cast<uint32_t>(mSize));
	vkUnmapMemory(*mLogicalDevice, stagingBufferMemory);

	vkDestroyBuffer(*mLogicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(*mLogicalDevice, stagingBufferMemory, nullptr);
}

void Dynamics_IO_Testbench::Compute::VK::ComputeBuffer::getAllQueueFamilies()
{
	std::vector<VkQueueFamilyProperties> queueFamilies = Utilities::GetPhysicalDeviceQueueFamilyProperties(*mPhysicalDevice);

	int i = 0;
	for (auto queueFam : queueFamilies) {
		if ((queueFam.queueFlags & VK_QUEUE_TRANSFER_BIT) ||
			(queueFam.queueFlags & VK_QUEUE_COMPUTE_BIT) ||
			(queueFam.queueFlags & VK_QUEUE_COMPUTE_BIT)) {
			
			mAllQueueFamilies.push_back(i);
		}
	}
}

ComputeBuffer::~ComputeBuffer() {
	vkDestroyBuffer(*mLogicalDevice, mBuffer, nullptr);
	vkFreeMemory(*mLogicalDevice, mBufferMemory, nullptr);
}
