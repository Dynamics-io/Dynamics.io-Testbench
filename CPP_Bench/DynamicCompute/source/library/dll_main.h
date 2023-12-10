#pragma once

//#include "physics/Physics.h"
//#include "resources/resources.h"
//#include "terrain/terrain.h"
#include "utils/utils.h"
#include "Compute_Interface/InterfaceIncludes_private.h"
#include "Logger.h"
//#include "SimContext.h"

using namespace DynamicCompute::Compute;

EXPORTED void* ComputeInterface_GetComputeController(ComputeInterface_private::Compute_SDK implementation, ComputeInterface_private::ControllerInfo info);

EXPORTED void ComputeInterface_DisposePlatform(ComputeInterface_private::Compute_SDK implementation);

EXPORTED int ComputeInterface_GetSupportedDevicesSize_Vulkan();
EXPORTED int ComputeInterface_GetSupportedDevices_Vulkan(Vulkan_Device_Info* out_devices);

EXPORTED int ComputeInterface_GetSupportedPlatformsSize_OpenCL();
EXPORTED int ComputeInterface_GetSupportedPlatforms_OpenCL(Platform* out_platforms);

EXPORTED void ComputeInterface_GetSupportedDevices_SetPlatform_OpenCL(Platform platform);
EXPORTED int ComputeInterface_GetSupportedDevicesSize_OpenCL();
EXPORTED int ComputeInterface_GetSupportedDevices_OpenCL(OpenCL_Device_Info* out_devices);

EXPORTED int ComputeInterface_GetSupportedDevicesSize_DirectX();
EXPORTED int ComputeInterface_GetSupportedDevices_DirectX(DirectX_Device_Info* out_devices);


EXPORTED void* IComputeController_AddProgram(void* handle, IComputeProgram_private::ProgramInfo info);

EXPORTED void* IComputeController_AddProgram_ref(void* handle, IComputeProgram_private::ProgramInfo* info);
		 
EXPORTED void* IComputeController_GetProgram(void* handle, char* name, int size);
		 
EXPORTED void* IComputeController_NewReadBuffer(void* handle, int numElements, int stride);
		 
EXPORTED void* IComputeController_NewWriteBuffer(void* handle, int numElements, int stride);
		 
EXPORTED void* IComputeController_NewReadWriteBuffer(void* handle, int numElements, int stride);

EXPORTED void IComputeController_Dispose(void* handle);


//EXPORTED void IComputeProgram_Init(void* handle, char* name, int size);

EXPORTED void IComputeProgram_AddIncludeDirectory(void* handle, char* name, int size);

EXPORTED void IComputeProgram_AddDefine(void* handle, char* name, int name_size, char* value, int value_size);

EXPORTED int IComputeProgram_Build(void* handle);

EXPORTED int IComputeProgram_FinishBuild(void* handle);

EXPORTED int IComputeProgram_GetKernelID(void* handle, char* name, int size);

EXPORTED int IComputeProgram_KernelSetBuffer(void* handle, char* name, int size, IComputeBuffer_private* buffer, IComputeProgram_private::BindIndex arg);

EXPORTED int IComputeProgram_RunKernel_1(void* handle, char* name, int size, int size_x, int size_y, int size_z);

EXPORTED int IComputeProgram_RunKernel_2(void* handle, int kernel_id, int size_x, int size_y, int size_z);

//EXPORTED void* IComputeProgram_GetKernelFunction(void* handle, int kernel_id);

EXPORTED IComputeProgram_private::ProgramBuildState IComputeProgram_GetState(void* handle);

EXPORTED int IComputeProgram_GetBuildResultCode(void* handle);

// EXPORTED std::string GetBuildErrorMessage() = 0;

// EXPORTED std::string GetProgramName() = 0;

EXPORTED void IComputeProgram_Dispose(void* handle);


EXPORTED void* IComputeProgram_ProgramInfo_New(char* program_name, int size, IComputeProgram_private::FileType type);

EXPORTED void IComputeProgram_ProgramInfo_AddKernel(void* handle, char* name, int size);


EXPORTED int IComputeBuffer_SetData(void* handle, void* data);

EXPORTED int IComputeBuffer_GetData(void* handle, void* outData);

EXPORTED int IComputeBuffer_SetData_2(void* handle, void* data, int size);

EXPORTED int IComputeBuffer_GetData_2(void* handle, void* outData, int size);

EXPORTED int IComputeBuffer_GetSize(void* handle);

EXPORTED void IComputeBuffer_Dispose(void* handle);

EXPORTED void* IComputeBuffer_Get_Native_Ptr(void* handle);


/*

EXPORTED int ComputeEngine_Platforms_Init();

EXPORTED void ComputeEngine_Platforms_GetAll(ComputeEngine::Platform* out_platforms);

EXPORTED int ComputeEngine_Devices_Init(ComputeEngine::Platform);

EXPORTED void ComputeEngine_Devices_GetAll(ComputeEngine::Device* out_devices);

EXPORTED void ComputeEngine_Init(ComputeEngine::Platform platform, ComputeEngine::Device device, char* dir, int dir_len);

EXPORTED void ComputeEngine_Get_CL_Version(char* str, int size);


EXPORTED void* ComputeController_New(ComputeEngine::Platform platform, ComputeEngine::Device device, char* dir, int dir_len);

EXPORTED void* ComputeController_GetProgramBuilder(void* handle);

EXPORTED void ComputeController_BuildProgram(void* handle);

EXPORTED void* ComputeController_NewReadBuffer(void* handle, size_t length);

EXPORTED void* ComputeController_NewWriteBuffer(void* handle, size_t length);

EXPORTED void* ComputeController_NewReadWriteBuffer(void* handle, size_t length);

EXPORTED int ComputeController_KernelAddBuffer(void* handle, const char* k_name, void* buffer);

EXPORTED int ComputeController_RunKernel(void* handle, char* k_name, int size_x, int size_y, int size_z);


EXPORTED void ProgramBuilder_AddConstant(void* handle, const char* name, const char* value);

EXPORTED void ProgramBuilder_AddFunction(void* handle, const char*, const char* f_source);

EXPORTED void ProgramBuilder_AddKernel(void* handle, const char* name, const char* f_source);

EXPORTED void ProgramBuilder_AppendSource(void* handle, const char* f_source);

EXPORTED int ProgramBuilder_GetErrorSize(void* handle);

EXPORTED void ProgramBuilder_GetError(void* handle, char* out_error);

EXPORTED int ProgramBuilder_Build(void* handle);



EXPORTED int ComputeBuffer_SetData(void* handle, void* data);

EXPORTED int ComputeBuffer_GetData(void* handle, void* outData);

EXPORTED void ComputeBuffer_Dispose(void* handle);

EXPORTED void* ComputeBuffer_Get_CL_Mem(void* handle);

EXPORTED unsigned long ComputeBuffer_GetSize(void* handle);

*/