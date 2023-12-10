#include "Compute_DirectX/ComputeProgram_DX.h"

#include "Compute_DirectX/ComputeEngine.h"
#include "Compute_DirectX/ComputeBuffer_DX.h"

using namespace DynamicCompute::Compute;
using namespace DynamicCompute::Compute::DX;

void ComputeProgram_DX::Init(std::string name)
{
	m_program_name = name;
	m_cur_state = ProgramBuildState::Inited;
	m_program = m_context->Add_Program(name);
}

int DynamicCompute::Compute::DX::ComputeProgram_DX::Build()
{
	// directory is contained and constructed completely inside ComputeEngine for DirectX. 
	// TODO: Currently ProgramInfo's FileType is ignored. Add way to support choosing between 
	// binary files and text files, instead of binary. Also requires getting DirectX's
	// compiler to actually work.
	BuildProgramFromDirectory(m_kernel_names);

	return 0;
}

int ComputeProgram_DX::FinishBuild()
{
	return m_program->Buildkernels();
}

int ComputeProgram_DX::GetKernelID(std::string name)
{
	if (m_kernel_name_to_id.count(name) <= 0) {
		printf("Kernel not found: %s\n", name.c_str());
		return -1;
	}

	return m_kernel_name_to_id[name];
}

void DynamicCompute::Compute::DX::ComputeProgram_DX::KernelSetWorkGroupSize(std::string k_name, int size)
{
}

int ComputeProgram_DX::KernelSetBuffer(std::string k_name, IComputeBuffer_private* buffer, BindIndex arg)
{
	if (m_kernel_name_to_id.count(k_name) <= 0) {
		printf("Kernel not found: %s\n", k_name.c_str());
		return -1;
	}

	int K_ID = GetKernelID(k_name);

	// TODO: handle setting kernel entry arg if larger.

	ComputeBuffer* bfr = ((ComputeBuffer_DX*)buffer)->GetBuffer();
	return BindKernel(bfr, m_kernel_entries[K_ID].kernel, arg.RegisterIndex);
}

int ComputeProgram_DX::RunKernel(std::string k_name, int size_x, int size_y, int size_z)
{
	if (m_kernel_name_to_id.count(k_name) <= 0) {
		printf("Kernel not found: %s\n", k_name.c_str());
		return -1;
	}

	int K_ID = GetKernelID(k_name);

	return RunKernel(K_ID, size_x, size_y, size_z);
}

int ComputeProgram_DX::RunKernel(int kernel_id, int size_x, int size_y, int size_z)
{
	if (kernel_id >= m_num_kernels)
	{
		printf("Kernel ID not found: %i\n", kernel_id);
		return -1;
	}

	return m_kernel_entries[kernel_id].kernel->Execute(size_x, size_y, size_z);
}

void* ComputeProgram_DX::GetKernelFunction(int kernel_id)
{
	return nullptr;
}

void ComputeProgram_DX::Dispose()
{
	if (mDestroyed)
		return;

	/*
	TODO: This releases all VK resources, however, other than
	the vector of ComputeKernels, it does not free the memory
	of the ComputeProgram object itself as it is still present in
	the std::vector<ComputeProgram> in the owning ComputeContext.
	If a user were to constantly create programs and destroy them
	for some reason, it would create a memory leak with an ever
	growing vector of disposed ComputePrograms.

	std::vector<ComputeProgram> in ComputeContext should be replaced
	with an std::map
	*/

	m_program->Dispose();
	m_kernels.clear();

	if (m_kernel_entries != nullptr)
		delete[] m_kernel_entries;

	mDestroyed = true;
}

ComputeProgram_DX::ProgramBuildState ComputeProgram_DX::BuildProgramFromDirectory(std::vector<std::string> kernels)
{
	for (int i = 0; i < kernels.size(); i++)
	{
		m_kernels.push_back(kernels[i]);

		// Create kernel instances inside program
		m_program->GetKernel(m_kernels[i]);
	}

	printf("Program Built successfully!\n");

	// setup kernels
	int kernel_res = InitKernelEntries();

	if (kernel_res == 0)
		m_cur_state = ProgramBuildState::Constructed;
	else
	{
		m_cur_state = ProgramBuildState::BuildError;
		m_build_error = "Kernel Error " + std::to_string(kernel_res);
	}

	return m_cur_state;
}


ComputeProgram_DX::ComputeProgram_DX(ComputeContext* context) {
	m_context = context;
}

int ComputeProgram_DX::BindKernel(ComputeBuffer* buffer, ComputeKernel* kernel, int arg)
{
	printf("Bind buffer to kernel: %i\n", arg);
	return kernel->SetBuffer(buffer, arg);
}

int ComputeProgram_DX::InitKernelEntries()
{
	m_num_kernels = m_kernels.size();

	if (m_kernel_entries != nullptr)
		delete[] m_kernel_entries;

	m_kernel_entries = new kernelEnt[m_num_kernels];

	for (int i = 0; i < m_num_kernels; i++) {
		std::string k_name = m_kernels[i];

		kernelEnt k_ent;
		k_ent.name = k_name;
		k_ent.kernel = m_program->GetKernel(k_name);
		//k_ent.args = 0;

		int k_status = 0;//k_ent.kernel->GetStatus();

		if (k_status != 0)
		{
			printf("Kernel '%s' failed with Code %i.\n", k_ent.name.c_str(), k_status);
			return k_status;
		}

		m_kernel_name_to_id[k_ent.name] = i;
		m_kernel_entries[i] = k_ent;

	}

	return 0;
}
