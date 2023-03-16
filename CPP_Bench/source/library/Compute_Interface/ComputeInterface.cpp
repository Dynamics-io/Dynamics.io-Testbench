#include "ComputeInterface.h"

#include "Compute_OCL/ComputeController_OCL.h"
#pragma comment(lib, "OpenCL.lib")

#include "Compute_Vulkan/ComputeController_VK.h"
#include "Compute_Vulkan/vulkan_utils.h"
//#include "Compute_Vulkan/vulkan_test.h"
//#include "Compute_Vulkan/vulkan_compute_test.h"

#ifdef WINDOWS_PLATFROM
#include "Compute_DirectX/SystemClass_test.h"
#include "Compute_DirectX/compute_test.h"

#endif

using namespace Dynamics_IO_Testbench::Compute;
using namespace Dynamics_IO_Testbench::Compute::OCL;
using namespace Dynamics_IO_Testbench::Compute::VK;

#ifdef WINDOWS_PLATFROM
using namespace Dynamics_IO_Testbench::Compute::DX;
#endif



cl_uint ComputeInterface::num_of_platforms = 0;
cl_uint ComputeInterface::num_of_devices = 0;
cl_device_id ComputeInterface::device_ids[MAX_OCL_DEVICES] = { 0 };

IComputeController* ComputeInterface::GetComputeController(ComputeInterface::Compute_SDK implementation, ControllerInfo info)
{
	switch (implementation)
	{
	case ComputeInterface::Compute_SDK::OpenCL:
		return GetComputeController_OCL(info);

	case ComputeInterface::Compute_SDK::VULKAN:
		return GetComputeController_Vulkan(info);

#ifdef WINDOWS_PLATFROM
	case ComputeInterface::Compute_SDK::DIRECTX:
		return GetComputeController_DirectX(info);

    case ComputeInterface::Compute_SDK::CUDA:
        return GetComputeController_CUDA(info);
#endif

	default:
		// This should never happen.
		return nullptr;
	}
}

void Dynamics_IO_Testbench::Compute::ComputeInterface::DisposePlatform(Compute_SDK implementation)
{
    switch (implementation) {
    case Compute_SDK::VULKAN:
        ComputeController_VK::DisposePlatform();
        break;
    }
}

IComputeController* ComputeInterface::GetComputeController_OCL(ControllerInfo info)
{
    IComputeController* controller = ComputeController_OCL::New();

    controller->Init(info.platform, info.device, info.program_dir);

	return controller;
}

IComputeController* ComputeInterface::GetComputeController_CUDA(ControllerInfo info)
{
	return nullptr;
}

IComputeController* ComputeInterface::GetComputeController_Vulkan(ControllerInfo info)
{
    //Vulkan_compute_test test;
    //test.Run();

    //return nullptr;
    
    IComputeController* controller = ComputeController_VK::New();

    controller->Init(info.platform, info.device, info.program_dir);

    return controller;
}

IComputeController* ComputeInterface::GetComputeController_DirectX(ControllerInfo info)
{
    IComputeController* res = nullptr;

#ifdef WINDOWS_PLATFROM

    printf("Running DirectX test app.\n");
    
    compute_test test;

    test.Run();


#endif

	return res;
}

std::vector<Platform> ComputeInterface::GetSupportedPlatforms_OpenCL()
{
    std::vector<Platform> res;

    num_of_platforms = 0;

    cl_platform_id all_platforms[10];

    // retreives a list of platforms available
    if (clGetPlatformIDs(10, all_platforms, &num_of_platforms) != CL_SUCCESS)
    {
        return res;
    }

    const int INFO_SIZE = 1000;

    char Info[INFO_SIZE];
    size_t n_size = 0;
    for (int i = 0; i < num_of_platforms; i++)
    {
        Platform platform;
        ZeroMemory(&platform, sizeof(Platform));

        platform.platform = all_platforms[i];

        ZeroMemory(Info, INFO_SIZE);
        clGetPlatformInfo(all_platforms[i], CL_PLATFORM_NAME, INFO_SIZE, Info, &n_size);
        strcpy(platform.name, Info);
        platform.name_size = n_size;

        ZeroMemory(Info, INFO_SIZE);
        clGetPlatformInfo(all_platforms[i], CL_PLATFORM_VENDOR, INFO_SIZE, Info, &n_size);
        strcpy(platform.vendor, Info);
        platform.vendor_size = n_size;

        ZeroMemory(Info, INFO_SIZE);
        clGetPlatformInfo(all_platforms[i], CL_PLATFORM_VERSION, INFO_SIZE, Info, &n_size);
        strcpy(platform.version, Info);
        platform.version_size = n_size;

        res.push_back(platform);
    }

    return res;
}

std::vector<Device> ComputeInterface::GetSupportedDevices_OpenCL(Platform pltfrm)
{
    std::vector<Device> res;

    num_of_devices = 0;



    // try to get a supported GPU device
    //ComputeEngine::device_ids = new cl_device_id[num_of_devices];
    cl_int get_res = clGetDeviceIDs((cl_platform_id)pltfrm.platform, CL_DEVICE_TYPE_ALL, MAX_OCL_DEVICES, device_ids, &num_of_devices);
    if (get_res != CL_SUCCESS)
    {
        printf("Failed to get devices: %i\n", get_res);
        return res;
    }

    const int INFO_SIZE = 1000;

    char Info[INFO_SIZE];
    cl_device_type type;
    size_t n_size = 0;
    for (int i = 0; i < num_of_devices; i++)
    {
        // CL_DEVICE_MAX_WORK_ITEM_SIZES
        Device device{};
        OpenCL_Device_Info info{};

        device.cl_device = device_ids[i];

        ZeroMemory(Info, INFO_SIZE);
        clGetDeviceInfo(device_ids[i], CL_DEVICE_VENDOR, INFO_SIZE, Info, &n_size);
        strcpy(info.vendor, Info);
        info.vendor_size = n_size;

        ZeroMemory(Info, INFO_SIZE);
        clGetDeviceInfo(device_ids[i], CL_DEVICE_NAME, INFO_SIZE, Info, &n_size);
        strcpy(info.name, Info);
        info.name_size = n_size;

        cl_uint freq;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &freq, &n_size);
        info.clock_frequency = freq;

        cl_uint nm_unts;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &nm_unts, &n_size);
        info.num_compute_units = nm_unts;

        cl_int num_dim;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &num_dim, &n_size);

        size_t* sizes = new size_t[num_dim];
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * num_dim, sizes, &n_size);
        info.max_work_size = 1;
        for (int w = 0; w < 2; w++)
        {
            info.max_work_size *= static_cast<unsigned int>(sizes[w]);
        }
        delete[] sizes;

        size_t work_g_size;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &work_g_size, &n_size);
        info.group_size = work_g_size;

        clGetDeviceInfo(device_ids[i], CL_DEVICE_TYPE, sizeof(cl_device_type), &type, &n_size);
        info.is_type_default = (type & CL_DEVICE_TYPE_DEFAULT) == CL_DEVICE_TYPE_DEFAULT;
        info.is_type_CPU = (type & CL_DEVICE_TYPE_CPU) == CL_DEVICE_TYPE_CPU;
        info.is_type_GPU = (type & CL_DEVICE_TYPE_GPU) == CL_DEVICE_TYPE_GPU;
        info.is_type_Accelerator = (type & CL_DEVICE_TYPE_ACCELERATOR) == CL_DEVICE_TYPE_ACCELERATOR;

        device.OpenCL_Info = info;

        res.push_back(device);
    }

    return res;
}

std::vector<Device> ComputeInterface::GetSupportedDevices_Vulkan()
{
    std::vector<Device> result;

    VkApplicationInfo appInfo = Utilities::getApplicationInfo(
        "Get_Devices",
        VK_MAKE_VERSION(1, 0, 0),
        "Dynamics_IO",
        VK_MAKE_VERSION(1, 0, 0),
        VK_API_VERSION_1_3);


    std::vector<const char*> requiredExtensions;
    VkInstanceCreateInfo createInfo = Utilities::getInstanceCreateInfo(appInfo, requiredExtensions);
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;

    VkResult res;
    VkInstance instance;
    res = vkCreateInstance(&createInfo, nullptr, &instance);

    if (res != VK_SUCCESS) {
        printf("ComputeInterface::GetSupportedDevices_Vulkan(): Error Initializing Vulkan Instance: %u\n", res);
        return result;
    }

    std::vector<VkPhysicalDevice> devices = Utilities::EnumeratePhysicalDevices(instance);

    printf("Total Devices: %li\n", devices.size());

    

    for (const auto& device : devices) {
        if (!isDeviceSuitable(device)) {
            continue;
        }

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        VkPhysicalDeviceFeatures deviceFeatures{};
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        /*VkPhysicalDeviceProperties2 deviceProperties2{};
        deviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES;
        vkGetPhysicalDeviceProperties2(device, &deviceProperties2);
        VkPhysicalDeviceProperties deviceProperties = deviceProperties2.properties;
        VkPhysicalDeviceIDProperties deviceIDProperties = *((VkPhysicalDeviceIDProperties*)deviceProperties2.pNext);*/
            
        Device res_device{};
        Vulkan_Device_Info info{};

        info.Name = std::string(deviceProperties.deviceName);
        info.Device_ID = deviceProperties.deviceID;

        //memcpy(info.DeviceUUID, deviceIDProperties.deviceUUID, 16);

        switch (deviceProperties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            info.Type = DeviceType::DEVICE_TYPE_OTHER;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            info.Type = DeviceType::DEVICE_TYPE_INTEGRATED_GPU;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            info.Type = DeviceType::DEVICE_TYPE_DISCRETE_GPU;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            info.Type = DeviceType::DEVICE_TYPE_VIRTUAL_GPU;
            break;

        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            info.Type = DeviceType::DEVICE_TYPE_CPU;
            break;
        }
            
        res_device.vk_device = device;
        res_device.Vulkan_Info = info;
        result.push_back(res_device);
        
    }


    vkDestroyInstance(instance, nullptr);

    return result;
}

bool ComputeInterface::isDeviceSuitable(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    Utilities::QueueFamilyIndices indices = Utilities::findQueueFamilies(device);

    std::vector<const char*> deviceExtensions;// TODO: User-provided extensions.
    bool extensionsSupported = Utilities::checkDeviceExtensionSupport(device, deviceExtensions);

    return indices.isComplete() && extensionsSupported;
}
