#include "dll_main.h"
#include "Compute/ComputeEngine.h"



#define PHYSICS_DT 0.0005f // fixed dt of 0.5 ms

using namespace Dynamics_IO_Testbench::Compute;

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




