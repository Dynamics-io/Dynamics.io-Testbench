#include "ComputeController_OCL.h"

#include "ComputeBuffer_OCL.h"

#include <boost/algorithm/string.hpp>

using namespace Dynamics_IO_Testbench::Compute;
using namespace Dynamics_IO_Testbench::Compute::OCL;

std::vector<ComputeController_OCL*> ComputeController_OCL::m_controllers;

#define DEFAULT_BINARY_FILE_TYPE "spv"
#define DEFAULT_TEXT_FILE_TYPE "cl"

void ComputeController_OCL::Init(Platform platform, Device device, std::string program_dir)
{
	m_directory = program_dir + "/"; // TODO: check end of program_dir string for '/'

	
	ComputeEngine::Init(platform, m_directory + "include");

	// TODO: Add ability to add OpenGL shared context
	m_context = ComputeEngine::GetNewContext(device);
}

IComputeProgram* ComputeController_OCL::AddProgram(IComputeProgram::ProgramInfo info)
{
	std::string name = info.Name();
	std::vector<std::string> kernels = info.Kernels();

	ComputeProgram_OCL* program = new ComputeProgram_OCL(m_context);

	program->Init(name);

	
	printf("ComputeController_OCL: Reading program from directory: %s\n", m_directory.c_str());
	
	switch (info.Type()) {
		case IComputeProgram::FileType::Binary:
		{
			std::string full_file_path = m_directory + name + "." + DEFAULT_BINARY_FILE_TYPE;
			printf("ComputeController_OCL: Adding program binary file: %s\n", full_file_path.c_str());
			program->BuildProgramFromBinary(full_file_path, kernels);
			break;
		}
		case IComputeProgram::FileType::Text:
		{
			std::string full_file_path = m_directory + name + "." + DEFAULT_TEXT_FILE_TYPE;
			printf("ComputeController_OCL: Adding program source file: %s\n", full_file_path.c_str());
			program->BuildProgramFromSourceFile(full_file_path, kernels);
			break;
		}
	}

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

IComputeBuffer* ComputeController_OCL::NewReadBuffer(size_t numElements, size_t stride)
{
	return new ComputeBuffer_OCL(ComputeController_OCL::NewBuffer(ComputeBuffer::Buffer_Type::READ, numElements * stride));
}

IComputeBuffer* ComputeController_OCL::NewWriteBuffer(size_t numElements, size_t stride)
{
	return new ComputeBuffer_OCL(ComputeController_OCL::NewBuffer(ComputeBuffer::Buffer_Type::Write, numElements * stride));
}

IComputeBuffer* ComputeController_OCL::NewReadWriteBuffer(size_t numElements, size_t stride)
{
	return new ComputeBuffer_OCL(ComputeController_OCL::NewBuffer(ComputeBuffer::Buffer_Type::Read_Write, numElements * stride));
}


// Non-interface methods:

ComputeBuffer* ComputeController_OCL::NewBuffer(ComputeBuffer::Buffer_Type type, size_t length)
{
	return m_context->GetBuffer(type, length);
}

IComputeController* Dynamics_IO_Testbench::Compute::OCL::ComputeController_OCL::New()
{
	int index = m_controllers.size();
	m_controllers.resize(index + 1);
	m_controllers[index] = new ComputeController_OCL();
	return m_controllers[index];
}
