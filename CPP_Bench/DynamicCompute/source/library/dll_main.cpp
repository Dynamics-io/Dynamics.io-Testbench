#include "dll_main.h"
#include "Compute_OCL/ComputeEngine.h"



#define PHYSICS_DT 0.0005f // fixed dt of 0.5 ms

std::vector<Platform> platforms;
Platform current_platform;
std::vector<Vulkan_Device_Info> vk_devices;
std::vector<OpenCL_Device_Info> ocl_devices;
std::vector<DirectX_Device_Info> dx_devices;


EXPORTED void* ComputeInterface_GetComputeController(ComputeInterface_private::Compute_SDK implementation, ComputeInterface_private::ControllerInfo info)
{
	return ComputeInterface_private::GetComputeController(implementation, info);
}

EXPORTED void ComputeInterface_DisposePlatform(ComputeInterface_private::Compute_SDK implementation)
{
	return ComputeInterface_private::DisposePlatform(implementation);
}

EXPORTED int ComputeInterface_GetSupportedDevicesSize_Vulkan()
{
	vk_devices = ComputeInterface_private::GetSupportedDevices_Vulkan();
	return vk_devices.size();
}

EXPORTED int ComputeInterface_GetSupportedDevices_Vulkan(Vulkan_Device_Info* out_devices)
{
	for (int i = 0; i < vk_devices.size(); i++) {
		out_devices[i] = vk_devices[i];
	}
	return vk_devices.size();

	//out_devices = devices.data();
	//return devices.size();
}

EXPORTED int ComputeInterface_GetSupportedPlatformsSize_OpenCL()
{
	platforms = ComputeInterface_private::GetSupportedPlatforms_OpenCL();
	return platforms.size();
}

EXPORTED int ComputeInterface_GetSupportedPlatforms_OpenCL(Platform* out_platforms)
{
	for (int i = 0; i < platforms.size(); i++) {
		out_platforms[i] = platforms[i];
	}
	return platforms.size();
}

EXPORTED void ComputeInterface_GetSupportedDevices_SetPlatform_OpenCL(Platform platform)
{
	current_platform = platform;
}

EXPORTED int ComputeInterface_GetSupportedDevicesSize_OpenCL()
{
	ocl_devices = ComputeInterface_private::GetSupportedDevices_OpenCL(current_platform);
	return ocl_devices.size();
}

EXPORTED int ComputeInterface_GetSupportedDevices_OpenCL(OpenCL_Device_Info* out_devices)
{
	for (int i = 0; i < ocl_devices.size(); i++) {
		out_devices[i] = ocl_devices[i];
	}
	return ocl_devices.size();
}

EXPORTED int ComputeInterface_GetSupportedDevicesSize_DirectX()
{
	dx_devices = ComputeInterface_private::GetSupportedDevices_DirectX();
	return dx_devices.size();
}

EXPORTED int ComputeInterface_GetSupportedDevices_DirectX(DirectX_Device_Info* out_devices)
{
	for (int i = 0; i < dx_devices.size(); i++) {
		out_devices[i] = dx_devices[i];
	}
	return dx_devices.size();
}




EXPORTED void* IComputeController_AddProgram(void* handle, IComputeProgram_private::ProgramInfo info)
{
	IComputeController_private* instance = (IComputeController_private*)handle;
	return instance->AddProgram(info);
}

EXPORTED void* IComputeController_AddProgram_ref(void* handle, IComputeProgram_private::ProgramInfo* info)
{
	IComputeController_private* instance = (IComputeController_private*)handle;
	return instance->AddProgram(*info);
	delete info;
}

EXPORTED void* IComputeController_GetProgram(void* handle, char* name, int size)
{
	IComputeController_private* instance = (IComputeController_private*)handle;
	return instance->GetProgram(std::string(name, size));
}

EXPORTED void* IComputeController_NewReadBuffer(void* handle, int numElements, int stride)
{
	IComputeController_private* instance = (IComputeController_private*)handle;
	return instance->NewReadBuffer(numElements, stride);
}

EXPORTED void* IComputeController_NewWriteBuffer(void* handle, int numElements, int stride)
{
	IComputeController_private* instance = (IComputeController_private*)handle;
	return instance->NewWriteBuffer(numElements, stride);
}

EXPORTED void* IComputeController_NewReadWriteBuffer(void* handle, int numElements, int stride)
{
	IComputeController_private* instance = (IComputeController_private*)handle;
	return instance->NewReadWriteBuffer(numElements, stride);
}

EXPORTED void IComputeController_Dispose(void* handle)
{
	IComputeController_private* instance = (IComputeController_private*)handle;
	instance->Dispose();
}


/*EXPORTED void IComputeProgram_Init(void* handle, char* name, int size)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	instance->Init(std::string(name, size));
}*/

EXPORTED void IComputeProgram_AddIncludeDirectory(void* handle, char* name, int size)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	instance->AddIncludeDirectory(std::string(name, size));
}

EXPORTED void IComputeProgram_AddDefine(void* handle, char* name, int name_size, char* value, int value_size)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	instance->AddDefine(std::string(name, name_size), std::string(value, value_size));
}

EXPORTED int IComputeProgram_Build(void* handle)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	return instance->Build();
}

EXPORTED int IComputeProgram_FinishBuild(void* handle)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	return instance->FinishBuild();
}

EXPORTED int IComputeProgram_GetKernelID(void* handle, char* name, int size)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	return instance->GetKernelID(std::string(name, size));
}

EXPORTED int IComputeProgram_KernelSetBuffer(void* handle, char* name, int size, IComputeBuffer_private* buffer, IComputeProgram_private::BindIndex arg)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	return instance->KernelSetBuffer(std::string(name, size), buffer, arg);
}

EXPORTED int IComputeProgram_RunKernel_1(void* handle, char* name, int size, int size_x, int size_y, int size_z)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	return instance->RunKernel(std::string(name, size), size_x, size_y, size_z);
}

EXPORTED int IComputeProgram_RunKernel_2(void* handle, int kernel_id, int size_x, int size_y, int size_z)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	return instance->RunKernel(kernel_id, size_x, size_y, size_z);
}

EXPORTED IComputeProgram_private::ProgramBuildState IComputeProgram_GetState(void* handle)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	return instance->GetState();
}

EXPORTED int IComputeProgram_GetBuildResultCode(void* handle)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	return instance->GetBuildResultCode();
}

EXPORTED void IComputeProgram_Dispose(void* handle)
{
	IComputeProgram_private* instance = (IComputeProgram_private*)handle;
	instance->Dispose();
}

EXPORTED void* IComputeProgram_ProgramInfo_New(char* program_name, int size, IComputeProgram_private::FileType type)
{
	char tmp[100] = { 0 };
	memcpy(tmp, program_name, size);
	std::string p_name = std::string(program_name, size);
	return new IComputeProgram_private::ProgramInfo(p_name, type);
}

EXPORTED void IComputeProgram_ProgramInfo_AddKernel(void* handle, char* name, int size)
{
	IComputeProgram_private::ProgramInfo* instance = (IComputeProgram_private::ProgramInfo*)handle;
	std::string k_name = std::string(name, size);
	instance->AddKernel(k_name);
}


EXPORTED int IComputeBuffer_SetData(void* handle, void* data)
{
	IComputeBuffer_private* instance = (IComputeBuffer_private*)handle;
	return instance->SetData(data);
}

EXPORTED int IComputeBuffer_GetData(void* handle, void* outData)
{
	IComputeBuffer_private* instance = (IComputeBuffer_private*)handle;
	return instance->GetData(outData);
}

EXPORTED int IComputeBuffer_SetData_2(void* handle, void* data, int size)
{
	IComputeBuffer_private* instance = (IComputeBuffer_private*)handle;
	return instance->SetData(data);
}

EXPORTED int IComputeBuffer_GetData_2(void* handle, void* outData, int size)
{
	IComputeBuffer_private* instance = (IComputeBuffer_private*)handle;
	return instance->GetData(outData);
}

EXPORTED int IComputeBuffer_GetSize(void* handle)
{
	IComputeBuffer_private* instance = (IComputeBuffer_private*)handle;
	return instance->GetSize();
}

EXPORTED void IComputeBuffer_Dispose(void* handle)
{
	IComputeBuffer_private* instance = (IComputeBuffer_private*)handle;
	return instance->Dispose();
}

EXPORTED void* IComputeBuffer_Get_Native_Ptr(void* handle)
{
	IComputeBuffer_private* instance = (IComputeBuffer_private*)handle;
	return instance->Get_Native_Ptr();
}



/*

using namespace DynamicCompute::Compute;

//using namespace FlexRigLib::Compute;
std::vector<ComputeEngine::Platform> platforms;
std::vector<ComputeEngine::Device> devices;

EXPORTED int ComputeEngine_Platforms_Init()
{
	platforms = ComputeEngine::GetSupportedPlatforms();
	return platforms.size();
}

EXPORTED void ComputeEngine_Platforms_GetAll(ComputeEngine::Platform* out_platforms)
{
	for (int i = 0; i < platforms.size(); i++)
	{
		out_platforms[i] = platforms[i];
	}
}

EXPORTED int ComputeEngine_Devices_Init(ComputeEngine::Platform pltfm)
{
	devices = ComputeEngine::GetSupportedDevices(pltfm);
	return devices.size();
}

EXPORTED void ComputeEngine_Devices_GetAll(ComputeEngine::Device* out_devices)
{
	for (int i = 0; i < devices.size(); i++)
	{
		out_devices[i] = devices[i];
	}
}

EXPORTED void ComputeEngine_Init(ComputeEngine::Platform platform, ComputeEngine::Device device, char* dir, int dir_len)
{
	ComputeEngine::Init(platform, device, std::string(dir, dir_len));
}

EXPORTED void ComputeEngine_Get_CL_Version(char* str, int size)
{
	strcpy(str, ComputeEngine::Get_CL_Version().c_str());
}



EXPORTED void* ComputeController_New(ComputeEngine::Platform platform, ComputeEngine::Device device, char* dir, int dir_len)
{
	return new ComputeController(platform, device, std::string(dir, dir_len));
}

EXPORTED void* ComputeController_GetProgramBuilder(void* handle)
{
	ComputeController* c_cont = (ComputeController*)handle;
	return c_cont->GetProgramBuilder();
}

EXPORTED void ComputeController_BuildProgram(void* handle)
{
	ComputeController* c_cont = (ComputeController*)handle;
	c_cont->BuildProgramFromSource();
}

EXPORTED void* ComputeController_NewReadBuffer(void* handle, size_t length)
{
	ComputeController* c_cont = (ComputeController*)handle;
	return c_cont->NewReadBuffer(length);
}

EXPORTED void* ComputeController_NewWriteBuffer(void* handle, size_t length)
{
	ComputeController* c_cont = (ComputeController*)handle;
	return c_cont->NewWriteBuffer(length);
}

EXPORTED void* ComputeController_NewReadWriteBuffer(void* handle, size_t length)
{
	ComputeController* c_cont = (ComputeController*)handle;
	return c_cont->NewReadWriteBuffer(length);
}

EXPORTED int ComputeController_KernelAddBuffer(void* handle, const char* k_name, void* buffer)
{
	ComputeController* c_cont = (ComputeController*)handle;
	return c_cont->KernelAddBuffer(k_name, (ComputeBuffer*)buffer);
}

EXPORTED int ComputeController_RunKernel(void* handle, char* k_name, int size_x, int size_y, int size_z)
{
	ComputeController* c_cont = (ComputeController*)handle;
	return c_cont->RunKernel(k_name, size_x, size_y, size_z);
}



EXPORTED void ProgramBuilder_AddConstant(void* handle, const char* name, const char* value)
{
	ProgramBuilder* prgm = (ProgramBuilder*)handle;
	prgm->AddConstant(name, value);
}

EXPORTED void ProgramBuilder_AddFunction(void* handle, const char* name, const char* f_source)
{
	ProgramBuilder* prgm = (ProgramBuilder*)handle;
	prgm->AddFunction(name, f_source);
}

EXPORTED void ProgramBuilder_AddKernel(void* handle, const char* name, const char* f_source)
{
	ProgramBuilder* prgm = (ProgramBuilder*)handle;
	prgm->AddKernel(name, f_source);
}

EXPORTED void ProgramBuilder_AppendSource(void* handle, const char* f_source)
{
	ProgramBuilder* prgm = (ProgramBuilder*)handle;
	prgm->AppendSource(f_source);
}

EXPORTED int ProgramBuilder_GetErrorSize(void* handle)
{
	ProgramBuilder* prgm = (ProgramBuilder*)handle;
	return prgm->GetError().size();
}

EXPORTED void ProgramBuilder_GetError(void* handle, char* out_error)
{
	ProgramBuilder* prgm = (ProgramBuilder*)handle;
	strcpy(out_error, prgm->GetError().c_str());
}

EXPORTED int ProgramBuilder_Build(void* handle)
{
	ProgramBuilder* prgm = (ProgramBuilder*)handle;
	return prgm->BuildFromSource();
}



EXPORTED int ComputeBuffer_SetData(void* handle, void* data)
{
	ComputeBuffer* buf = (ComputeBuffer*)handle;
	return buf->SetData(data);
}

EXPORTED int ComputeBuffer_GetData(void* handle, void* outData)
{
	ComputeBuffer* buf = (ComputeBuffer*)handle;
	return buf->GetData(outData);
}

EXPORTED void ComputeBuffer_Dispose(void* handle)
{
	ComputeBuffer* buf = (ComputeBuffer*)handle;
	return buf->Dispose();
}

EXPORTED void* ComputeBuffer_Get_CL_Mem(void* handle)
{
	ComputeBuffer* buf = (ComputeBuffer*)handle;
	return buf->Get_CL_Mem();
}

EXPORTED unsigned long ComputeBuffer_GetSize(void* handle)
{
	ComputeBuffer* buf = (ComputeBuffer*)handle;
	return buf->GetSize();
}




*/

