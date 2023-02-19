#include "ComputeController_OCL.h"

#include "ComputeBuffer_OCL.h"

using namespace Dynamics_IO_Testbench::Compute;
using namespace Dynamics_IO_Testbench::Compute::OCL;


void ComputeController_OCL::Init(Platform platform, Device device, std::string program_dir)
{
	m_directory = program_dir + "/"; // TODO: check end of program_dir string for '/'

	ComputeEngine::Init(platform.platform, device.device, m_directory + "include");

	// TODO: Add ability to add OpenGL shared context
	m_context = ComputeEngine::GetNewContext();
}

IComputeProgram* ComputeController_OCL::AddProgram(IComputeProgram::ProgramInfo info)
{
	ComputeProgram_OCL* program = new ComputeProgram_OCL(m_context);

	std::string name = info.Name();
	std::vector<std::string> kernels = info.Kernels();

	program->Init(name);

	std::string full_file_path = m_directory + name + ".spv";

	printf("ComputeController_OCL: Reading program from directory: %s\n", m_directory.c_str());
	printf("ComputeController_OCL: Adding program binary file: %s\n", full_file_path.c_str());

	program->BuildProgramFromBinary(full_file_path, kernels);

	m_programs[name] = program;

	return (IComputeProgram*)program;
}

IComputeProgram* ComputeController_OCL::GetProgram(std::string name)
{
	if (m_programs.count(name) <= 0) {
		printf("Program not found: %s\n", name.c_str());
		return nullptr;
	}

	return m_programs[name];
}

IComputeBuffer* ComputeController_OCL::NewReadBuffer(size_t length)
{
	return new ComputeBuffer_OCL(ComputeController_OCL::NewBuffer(ComputeBuffer::Buffer_Type::READ, length));
}

IComputeBuffer* ComputeController_OCL::NewWriteBuffer(size_t length)
{
	return new ComputeBuffer_OCL(ComputeController_OCL::NewBuffer(ComputeBuffer::Buffer_Type::Write, length));
}

IComputeBuffer* ComputeController_OCL::NewReadWriteBuffer(size_t length)
{
	return new ComputeBuffer_OCL(ComputeController_OCL::NewBuffer(ComputeBuffer::Buffer_Type::Read_Write, length));
}


// Non-interface methods:

ComputeBuffer* ComputeController_OCL::NewBuffer(ComputeBuffer::Buffer_Type type, size_t length)
{
	return m_context->GetBuffer(type, length);
}
