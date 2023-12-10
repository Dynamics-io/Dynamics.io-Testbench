#include "Compute_DirectX/ComputeController_DX.h"

#include "Compute_DirectX/ComputeEngine.h"
#include "Compute_DirectX/ComputeProgram_DX.h"
#include "Compute_DirectX/ComputeBuffer_DX.h"

using namespace DynamicCompute::Compute;
using namespace DynamicCompute::Compute::DX;

std::vector<ComputeController_DX*> ComputeController_DX::m_controllers;

void ComputeController_DX::Init(Platform platform, void* device, std::string program_dir)
{
	m_directory = program_dir + "/"; // TODO: check end of program_dir string for '/'

	if (!ComputeEngine::IsInitialized()) {
		int res = ComputeEngine::Init(m_directory);

		if (res != 0) {
			printf("Failed to initialize DirectX\n");
		}
	}

	DirectX_Device_Info info = *(DirectX_Device_Info*)device;

	m_context = ComputeEngine::GetNewContext(info);
}

IComputeProgram_private* ComputeController_DX::AddProgram(IComputeProgram_private::ProgramInfo info)
{
	std::string name = info.Name();
	std::vector<std::string> kernels = info.Kernels();

	m_programs[name] = new ComputeProgram_DX(m_context);

	m_programs[name]->Init(name);
	m_programs[name]->SetKernelNames(kernels);


	// --- BUILD MOVED TO program->Build()
	// directory is contained and constructed completely inside ComputeEngine for DirectX. 
	// TODO: Currently ProgramInfo's FileType is ignored. Add way to support choosing between 
	// binary files and text files, instead of binary. Also requires getting DirectX's
	// compiler to actually work.
	//m_programs[name]->BuildProgramFromDirectory(kernels);

	return m_programs[name];
}

IComputeProgram_private* ComputeController_DX::GetProgram(std::string name)
{
	if (m_programs.count(name) <= 0) {
		printf("Program not found: %s\n", name.c_str());
		return nullptr;
	}

	return m_programs[name];
}

IComputeBuffer_private* ComputeController_DX::NewReadBuffer(size_t numElements, size_t stride)
{
	ComputeBuffer* bf = ComputeController_DX::NewBuffer((uint32_t)ComputeBuffer::Buffer_Type::READ, numElements, stride);
	return new ComputeBuffer_DX(bf);
}

IComputeBuffer_private* ComputeController_DX::NewWriteBuffer(size_t numElements, size_t stride)
{
	ComputeBuffer* bf = ComputeController_DX::NewBuffer((uint32_t)ComputeBuffer::Buffer_Type::Write, numElements, stride);
	return new ComputeBuffer_DX(bf);
}

IComputeBuffer_private* ComputeController_DX::NewReadWriteBuffer(size_t numElements, size_t stride)
{
	ComputeBuffer* bf = ComputeController_DX::NewBuffer((uint32_t)ComputeBuffer::Buffer_Type::Read_Write, numElements, stride);
	return new ComputeBuffer_DX(bf);
}

void ComputeController_DX::Dispose()
{
	if (mDestroyed)
		return;

	for (auto& [key, value] : m_programs) {
		delete value;
	}

	m_programs.clear();
	m_context->Dispose();

	mDestroyed = true;
}

ComputeBuffer* ComputeController_DX::NewBuffer(uint32_t type, size_t numElements, size_t stride)
{
	return m_context->CreateBuffer((ComputeBuffer::Buffer_Type)type, numElements, stride);
}

ComputeController_DX::~ComputeController_DX()
{
	Dispose();
}

IComputeController_private* ComputeController_DX::New()
{
	int index = m_controllers.size();
	m_controllers.resize(index + 1);
	m_controllers[index] = new ComputeController_DX();
	return m_controllers[index];
}

void ComputeController_DX::DisposePlatform()
{
	if (ComputeEngine::IsInitialized()) {
		ComputeEngine::Dispose();
	}
}

void ComputeController_DX::Close()
{
	m_controllers.clear();
	DisposePlatform();
}