#include "Compute_Vulkan/ComputeController_VK.h"

#include "Compute_Vulkan/ComputeEngine.h"
#include "Compute_Vulkan/ComputeProgram_VK.h"
#include "Compute_Vulkan/ComputeBuffer_VK.h"

using namespace Dynamics_IO_Testbench::Compute;
using namespace Dynamics_IO_Testbench::Compute::VK;

std::vector<ComputeController_VK*> ComputeController_VK::m_controllers;

void ComputeController_VK::Init(Platform platform, Device device, std::string program_dir)
{
	m_directory = program_dir + "/"; // TODO: check end of program_dir string for '/'

	if (!ComputeEngine::IsInitialized()) {
		int res = ComputeEngine::Init(m_directory);

		if (res != 0) {
			printf("Failed to initialize Vulkan\n");
		}
	}

	m_context = ComputeEngine::GetNewContext(device);
}

IComputeProgram* ComputeController_VK::AddProgram(IComputeProgram::ProgramInfo info)
{
	std::string name = info.Name();
	std::vector<std::string> kernels = info.Kernels();

	m_programs[name] = new ComputeProgram_VK(m_context);

	m_programs[name]->Init(name);

	std::string full_file_path = m_directory + name;

	printf("ComputeController_VK: Reading program from directory: %s\n", m_directory.c_str());

	printf("ComputeController_OCL: Adding program binary file: %s\n", full_file_path.c_str());
	m_programs[name]->BuildProgramFromBinary(full_file_path, kernels);

	return m_programs[name];
}

IComputeProgram* ComputeController_VK::GetProgram(std::string name)
{
	if (m_programs.count(name) <= 0) {
		printf("Program not found: %s\n", name.c_str());
		return nullptr;
	}

	return m_programs[name];
}

IComputeBuffer* ComputeController_VK::NewReadBuffer(size_t length)
{
	ComputeBuffer* bf = ComputeController_VK::NewBuffer((uint32_t)ComputeBuffer::Buffer_Type::READ, length);
	ComputeBuffer_VK* bf_vk = new ComputeBuffer_VK(bf);
	return bf_vk;
}

IComputeBuffer* ComputeController_VK::NewWriteBuffer(size_t length)
{
	ComputeBuffer* bf = ComputeController_VK::NewBuffer((uint32_t)ComputeBuffer::Buffer_Type::Write, length);
	return new ComputeBuffer_VK(bf);
}

IComputeBuffer* ComputeController_VK::NewReadWriteBuffer(size_t length)
{
	return new ComputeBuffer_VK(ComputeController_VK::NewBuffer((uint32_t)ComputeBuffer::Buffer_Type::Read_Write, length));
}

ComputeBuffer* ComputeController_VK::NewBuffer(uint32_t type, size_t length)
{
	return m_context->CreateBuffer((ComputeBuffer::Buffer_Type)type, length);
}

void ComputeController_VK::Dispose()
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

ComputeController_VK::~ComputeController_VK()
{
	Dispose();
}

IComputeController* ComputeController_VK::New()
{
	int index = m_controllers.size();
	m_controllers.resize(index + 1);
	m_controllers[index] = new ComputeController_VK();
	return m_controllers[index];
}

void ComputeController_VK::DisposePlatform() {
	if (ComputeEngine::IsInitialized()) {
		ComputeEngine::Dispose();
	}
}

void ComputeController_VK::Close()
{
	m_controllers.clear();
	DisposePlatform();
}
