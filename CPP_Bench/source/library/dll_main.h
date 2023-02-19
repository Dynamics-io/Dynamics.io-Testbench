#pragma once

//#include "physics/Physics.h"
//#include "resources/resources.h"
//#include "terrain/terrain.h"
#include "utils/utils.h"
#include "Compute_Interface/InterfaceIncludes.h"
#include "Logger.h"
//#include "SimContext.h"

using namespace Dynamics_IO_Testbench::Compute;

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