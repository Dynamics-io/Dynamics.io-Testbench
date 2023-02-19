#include "ComputeProgram_OCL.h"

#include "ComputeBuffer_OCL.h"

using namespace Dynamics_IO_Testbench::Compute;
using namespace Dynamics_IO_Testbench::Compute::OCL;

void ComputeProgram_OCL::Init(std::string name)
{
	m_program_name = name;

	m_builder = new ProgramBuilder(m_context, m_program_name);

	m_cur_state = ProgramBuildState::Inited;
}

int ComputeProgram_OCL::GetKernelID(std::string name)
{
	if (m_kernel_name_to_id.count(name) <= 0) {
		printf("Kernel not found: %s\n", name.c_str());
		return -1;
	}

	return m_kernel_name_to_id[name];
}

int ComputeProgram_OCL::KernelAddBuffer(std::string k_name, IComputeBuffer* buffer)
{
	if (m_kernel_name_to_id.count(k_name) <= 0) {
		printf("Kernel not found: %s\n", k_name.c_str());
		return -1;
	}

	int K_ID = GetKernelID(k_name);

	int res = KernelSetBuffer(k_name, buffer, m_kernel_entries[K_ID].args);
	m_kernel_entries[K_ID].args++;
	return res;
}

int ComputeProgram_OCL::KernelSetBuffer(std::string k_name, IComputeBuffer* buffer, int arg)
{
	if (m_kernel_name_to_id.count(k_name) <= 0) {
		printf("Kernel not found: %s\n", k_name.c_str());
		return -1;
	}

	int K_ID = GetKernelID(k_name);

	// TODO: handle setting kernel entry arg if larger.

	ComputeBuffer* bfr = ((ComputeBuffer_OCL*)buffer)->GetBuffer();
	return BindKernel(bfr, m_kernel_entries[K_ID].kernel, arg);
}

int ComputeProgram_OCL::RunKernel(std::string k_name, int size_x, int size_y, int size_z)
{
	if (m_kernel_name_to_id.count(k_name) <= 0) {
		printf("Kernel not found: %s\n", k_name.c_str());
		return -1;
	}

	int K_ID = GetKernelID(k_name);

	return RunKernel(K_ID, size_x, size_y, size_z);
}

int ComputeProgram_OCL::RunKernel(int kernel_id, int size_x, int size_y, int size_z)
{
	if (kernel_id >= m_num_kernels)
	{
		printf("Kernel ID not found: %i\n", kernel_id);
		return -1;
	}

	size_t global[] = { std::max(size_x, 1) , std::max(size_y, 1) , std::max(size_z, 1) };
	return m_kernel_entries[kernel_id].kernel->Execute(0, 3, global);
}

void* ComputeProgram_OCL::GetKernelFunction(int kernel_id)
{
	return nullptr;
}



// Non-interface methods:

ComputeProgram_OCL::ComputeProgram_OCL(ComputeContext* context)
{
	m_context = context;
}

IComputeProgram::ProgramBuildState ComputeProgram_OCL::ConstructSourceProgram()
{
	if (m_cur_state == ProgramBuildState::Inited)
	{
		default_shaders.Load();

		for (auto s_name : default_shaders.m_source)
		{
			std::string source = default_shaders.GetCode(s_name);
			m_builder->AppendSource(source + "\n");
		}
		// TODO: Event for additional source code

		std::vector<std::string> f_defs = default_shaders.GetFunctionsDefinitions();
		// TODO: Event for additional function definitions
		for (auto def : f_defs)
		{
			m_builder->AppendSource(def);
		}
		m_builder->AppendSource("\n");

		for (auto f_name : default_shaders.m_functions)
		{
			m_builder->AddFunction(f_name, default_shaders.GetCode(f_name) + "\n");
		}
		// TODO: Event for additional functions

		for (auto k_name : default_shaders.m_kernels)
		{
			m_builder->AddKernel(k_name, default_shaders.GetCode(k_name) + "\n");
		}
		// TODO: Event for additional kernels

		default_shaders.Clear();
		m_cur_state = ProgramBuildState::Constructed;
	}

	return m_cur_state;
}

IComputeProgram::ProgramBuildState ComputeProgram_OCL::BuildProgramFromBinary(std::string file_path, std::vector<std::string> kernels)
{
	m_builder->AddKernels(kernels);

	m_cl_build_res = m_builder->BuildFromBinary(file_path);

	if (m_cl_build_res == 0)
	{
		printf("Program Built successfully!\n");

		// get kernels
		InitKernelEntries();

		m_builder->Clear();
		m_cur_state = ProgramBuildState::Built;
	}
	else
	{
		m_cur_state = ProgramBuildState::BuildError;
		m_build_error = m_builder->GetError();
		printf("Got non-zero result from BuildFromBinary: %i\n", m_cl_build_res);
		//printf("Build Error: %s\n", m_builder->GetError().c_str());
	}

	return m_cur_state;
}

IComputeProgram::ProgramBuildState ComputeProgram_OCL::BuildProgramFromBinary(const void* binary, size_t length, std::vector<std::string> kernels)
{
	m_builder->AddKernels(kernels);

	m_cl_build_res = m_builder->BuildFromBinary(binary, length);

	if (m_cl_build_res == 0)
	{
		printf("Program Built successfully!\n");

		// get kernels
		InitKernelEntries();

		m_builder->Clear();
		m_cur_state = ProgramBuildState::Built;
	}
	else
	{
		m_cur_state = ProgramBuildState::BuildError;
		m_build_error = m_builder->GetError();
		printf("Got non-zero result from BuildFromBinary: %i\n", m_cl_build_res);
		//printf("Build Error: %s\n", m_builder->GetError().c_str());
	}

	return m_cur_state;
}

IComputeProgram::ProgramBuildState ComputeProgram_OCL::BuildProgramFromSource(std::string content, std::vector<std::string> kernels)
{
	m_builder->AddKernels(kernels);
	m_builder->AppendSource(content);

	m_cl_build_res = m_builder->BuildFromSource();

	if (m_cl_build_res == 0)
	{
		printf("Program Built successfully!\n");

		// get kernels
		InitKernelEntries();


		m_builder->Clear();
		m_cur_state = ProgramBuildState::Built;
	}
	else
	{
		m_cur_state = ProgramBuildState::BuildError;
		m_build_error = m_builder->GetError();
		printf("Got non-zero result from BuildFromBinary: %i\n", m_cl_build_res);
		//printf("Build Error: %s\n", m_builder->GetError().c_str());
	}

	return m_cur_state;
}

IComputeProgram::ProgramBuildState ComputeProgram_OCL::BuildProgramFromSourceFile(std::string file_path, std::vector<std::string> kernels)
{
	m_builder->AddKernels(kernels);

	//open file
	std::ifstream infile(file_path);
	std::vector<char> buffer;

	//get length of file
	infile.seekg(0, infile.end);
	size_t length = infile.tellg();
	infile.seekg(0, infile.beg);

	//read file
	if (length > 0) {
		buffer.resize(length);
		//buffer.insert(buffer.begin(), std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());
		infile.read(&buffer[0], length);
	}

	std::string file_res(buffer.begin(), buffer.end());

	m_builder->AppendSource(file_res);

	m_cl_build_res = m_builder->BuildFromSource();

	if (m_cl_build_res == 0)
	{
		printf("Program Built successfully!\n");

		// get kernels
		InitKernelEntries();


		m_builder->Clear();
		m_cur_state = ProgramBuildState::Built;
	}
	else
	{
		m_cur_state = ProgramBuildState::BuildError;
		m_build_error = m_builder->GetError();
		printf("Got non-zero result from BuildFromBinary: %i\n", m_cl_build_res);
		//printf("Build Error: %s\n", m_builder->GetError().c_str());
	}

	return m_cur_state;
}

IComputeProgram::ProgramBuildState ComputeProgram_OCL::BuildProgramFromInternalDepo()
{
	if (m_cur_state == ProgramBuildState::Constructed)
	{
		m_cl_build_res = m_builder->BuildFromSource();

		if (m_cl_build_res == 0)
		{
			printf("Program Built successfully!\n");

			// get kernels
			InitKernelEntries();


			m_builder->Clear();
			m_cur_state = ProgramBuildState::Built;
		}
		else
		{
			m_cur_state = ProgramBuildState::BuildError;
			m_build_error = m_builder->GetError();
			printf("Got non-zero result from BuildFromBinary: %i\n", m_cl_build_res);
			//printf("Build Error: %s\n", m_builder->GetError().c_str());
		}
	}

	return m_cur_state;
}



// private functions:

int ComputeProgram_OCL::BindKernel(ComputeBuffer* buffer, ComputeKernel* kernel, int arg)
{
	printf("Bind buffer to kernel: %i\n", arg);
	return kernel->SetBuffer(0, buffer, arg);
}

void ComputeProgram_OCL::InitKernelEntries()
{
	std::vector<std::string> kernls = m_builder->GetKernels();

	m_num_kernels = kernls.size();

	if (m_kernel_entries != nullptr)
		delete[] m_kernel_entries;

	m_kernel_entries = new kernelEnt[m_num_kernels];

	for (int i = 0; i < m_num_kernels; i++)
	{
		std::string k_name = kernls[i];

		kernelEnt k_ent;
		k_ent.name = k_name;
		k_ent.kernel = m_builder->GetKernel(k_name);
		k_ent.args = 0;

		m_kernel_name_to_id[k_ent.name] = i;
		m_kernel_entries[i] = k_ent;
	}

	// established kernels.
	// someKnownKernel = m_kernel_entries["coolKernel"];
}