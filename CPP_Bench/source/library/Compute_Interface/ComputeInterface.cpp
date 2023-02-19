#include "ComputeInterface.h"

#include "Compute_OCL/ComputeController_OCL.h"

using namespace Dynamics_IO_Testbench::Compute;
using namespace Dynamics_IO_Testbench::Compute::OCL;

#pragma comment(lib, "OpenCL.lib")

cl_uint ComputeInterface::num_of_platforms = 0;
cl_uint ComputeInterface::num_of_devices = 0;
cl_device_id ComputeInterface::device_ids[MAX_OCL_DEVICES] = { 0 };

IComputeController* ComputeInterface::GetComputeController(ComputeInterface::Compute_SDK implementation, ControllerInfo info)
{
	switch (implementation)
	{
	case ComputeInterface::Compute_SDK::OpenCL:
		return GetComputeController_OCL(info);

	case ComputeInterface::Compute_SDK::CUDA:
		return GetComputeController_CUDA(info);

	case ComputeInterface::Compute_SDK::VULKAN:
		return GetComputeController_Vulkan(info);

	case ComputeInterface::Compute_SDK::DIRECTX:
		return GetComputeController_DirectX(info);

	default:
		// This should never happen.
		return nullptr;
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
	return nullptr;
}

IComputeController* ComputeInterface::GetComputeController_DirectX(ControllerInfo info)
{
	return nullptr;
}

std::vector<Platform> ComputeInterface::GetSupportedPlatforms()
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

std::vector<Device> ComputeInterface::GetSupportedDevices(Platform pltfrm)
{
    std::vector<Device> res;

    num_of_devices = 0;



    // try to get a supported GPU device
    //ComputeEngine::device_ids = new cl_device_id[num_of_devices];
    cl_int get_res = clGetDeviceIDs(pltfrm.platform, CL_DEVICE_TYPE_ALL, MAX_OCL_DEVICES, device_ids, &num_of_devices);
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
        Device dev;
        ZeroMemory(&dev, sizeof(Device));

        dev.device = device_ids[i];

        ZeroMemory(Info, INFO_SIZE);
        clGetDeviceInfo(device_ids[i], CL_DEVICE_VENDOR, INFO_SIZE, Info, &n_size);
        strcpy(dev.vendor, Info);
        dev.vendor_size = n_size;

        ZeroMemory(Info, INFO_SIZE);
        clGetDeviceInfo(device_ids[i], CL_DEVICE_NAME, INFO_SIZE, Info, &n_size);
        strcpy(dev.name, Info);
        dev.name_size = n_size;

        cl_uint freq;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &freq, &n_size);
        dev.clock_frequency = freq;

        cl_uint nm_unts;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &nm_unts, &n_size);
        dev.num_compute_units = nm_unts;

        cl_int num_dim;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &num_dim, &n_size);

        size_t* sizes = new size_t[num_dim];
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * num_dim, sizes, &n_size);
        dev.max_work_size = 1;
        for (int w = 0; w < 2; w++)
        {
            dev.max_work_size *= static_cast<unsigned int>(sizes[w]);
        }
        delete[] sizes;

        size_t work_g_size;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &work_g_size, &n_size);
        dev.group_size = work_g_size;

        clGetDeviceInfo(device_ids[i], CL_DEVICE_TYPE, sizeof(cl_device_type), &type, &n_size);
        dev.is_type_default = (type & CL_DEVICE_TYPE_DEFAULT) == CL_DEVICE_TYPE_DEFAULT;
        dev.is_type_CPU = (type & CL_DEVICE_TYPE_CPU) == CL_DEVICE_TYPE_CPU;
        dev.is_type_GPU = (type & CL_DEVICE_TYPE_GPU) == CL_DEVICE_TYPE_GPU;
        dev.is_type_Accelerator = (type & CL_DEVICE_TYPE_ACCELERATOR) == CL_DEVICE_TYPE_ACCELERATOR;

        res.push_back(dev);
    }

    return res;
}