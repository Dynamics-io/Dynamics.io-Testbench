#include "ComputeInterface.h"
#include "Compute_Interface/ComputeInterface_private.h"
#include "IComputeController.h"

using namespace DynamicCompute::Compute;

IComputeController* DynamicCompute::Compute::ComputeInterface::GetComputeController(Compute_SDK implementation, ControllerInfo info)
{
	return new IComputeController(ComputeInterface_private::GetComputeController((ComputeInterface_private::Compute_SDK)implementation, *((ComputeInterface_private::ControllerInfo*)&info)));
}

void DynamicCompute::Compute::ComputeInterface::DisposePlatform(Compute_SDK implementation)
{
	ComputeInterface_private::DisposePlatform((ComputeInterface_private::Compute_SDK)implementation);
}

std::vector<Platform> DynamicCompute::Compute::ComputeInterface::GetSupportedPlatforms_OpenCL()
{
	return ComputeInterface_private::GetSupportedPlatforms_OpenCL();
}

std::vector<OpenCL_Device_Info> DynamicCompute::Compute::ComputeInterface::GetSupportedDevices_OpenCL(Platform pltfrm)
{
	return ComputeInterface_private::GetSupportedDevices_OpenCL(pltfrm);
}

std::vector<Vulkan_Device_Info> DynamicCompute::Compute::ComputeInterface::GetSupportedDevices_Vulkan()
{
	return ComputeInterface_private::GetSupportedDevices_Vulkan();
}

std::vector<DirectX_Device_Info> DynamicCompute::Compute::ComputeInterface::GetSupportedDevices_DirectX()
{
	return ComputeInterface_private::GetSupportedDevices_DirectX();
}
