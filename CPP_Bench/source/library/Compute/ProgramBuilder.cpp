#include "ProgramBuilder.h"

using namespace Dynamics_IO_Testbench::Compute;


ProgramBuilder::ProgramBuilder(ComputeContext* c_context, std::string name)
{
	m_context = c_context;
	m_name = name;

	m_source = "";
	m_program = m_context->Add_Program(m_name);
}

void ProgramBuilder::AddConstant(std::string name, std::string value)
{
	m_program->AddConstant(name, value);
}

void ProgramBuilder::AddFunction(std::string name, std::string f_source)
{
	AppendSource(f_source);
}

void ProgramBuilder::AddKernel(std::string name, std::string f_source)
{
	AppendSource(f_source);
	m_kernels.push_back(name);
}

void ProgramBuilder::AddKernel(std::string name)
{
	m_kernels.push_back(name);
}

void ProgramBuilder::AddKernels(std::vector<std::string> kernels)
{
	for (int i = 0; i < kernels.size(); i++)
	{
		m_kernels.push_back(kernels[i]);
	}
}

void ProgramBuilder::AppendSource(std::string f_source)
{
	m_source += f_source + "\n";
}

int ProgramBuilder::BuildFromSource()
{
	char errorstr[ERROR_SIZE];
	m_program->Set_Source(m_source.c_str());
	int status = m_program->Build(errorstr, ERROR_SIZE);
	m_error_msg = errorstr;

	for (int i = 0; i < m_kernels.size(); i++)
	{
		m_program->GetKernel(m_kernels[i]);
	}

	return status;
}

int ProgramBuilder::BuildFromBinary(std::string file_path)
{
	char errorstr[ERROR_SIZE];
	m_program->Set_Binary_File(file_path);
	int status = m_program->Build(errorstr, ERROR_SIZE);
	m_error_msg = errorstr;

	for (int i = 0; i < m_kernels.size(); i++)
	{
		m_program->GetKernel(m_kernels[i]);
	}

	return status;
}

int ProgramBuilder::BuildFromBinary(const void* binary, size_t length)
{
	char errorstr[ERROR_SIZE];
	m_program->Set_Binary(binary, length);
	int status = m_program->Build(errorstr, ERROR_SIZE);
	m_error_msg = errorstr;

	for (int i = 0; i < m_kernels.size(); i++)
	{
		m_program->GetKernel(m_kernels[i]);
	}

	return status;
}


ComputeKernel* ProgramBuilder::GetKernel(std::string name)
{
	return m_program->GetKernel(name);
}
