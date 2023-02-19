#include "dll_main.h"

#include <time.h>
#include <chrono>

#define DATA_SIZE 10


using namespace Dynamics_IO_Testbench::Compute;

const char* KernelSource =
"__kernel void hello(__global float *input, __global float *output)\n"\
"{\n"\
"  size_t id = get_global_id(0);\n"
"  output[id] = input[id] * input[id];\n"
"}\n"\
"\n";


int main()
{
	std::vector<Platform> platforms = ComputeInterface::GetSupportedPlatforms();

	for (const auto p : platforms)
	{
		printf("Platform %s:\n", p.name);

		std::vector<Device> devices = ComputeInterface::GetSupportedDevices(p);
		for (auto d : devices)
		{
			printf("\t%s - %s: Frequency: %u, threads: %u, Memory: %lu, Work Size: %u \n", d.vendor, d.name, d.clock_frequency, d.num_compute_units * d.group_size, d.mem_size, d.max_work_size);
		}
	}

	Platform platf = platforms[1];
	Device dev = ComputeInterface::GetSupportedDevices(platf)[0];

	ComputeInterface::ControllerInfo controllerInfo;
	controllerInfo.platform = platf;
	controllerInfo.device = dev;
	controllerInfo.program_dir = "C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/llvm_clang_Bench/Kernels/llvm_binaries/spirv_binaries";

	IComputeController* controller = ComputeInterface::GetComputeController(ComputeInterface::OpenCL, controllerInfo);

	IComputeProgram::ProgramInfo p_info("test_source.cl");
	p_info.AddKernel("bar");

	IComputeProgram* program = controller->AddProgram(p_info);


	if (program->GetState() == IComputeProgram::ProgramBuildState::BuildError)
	{
		//printf("%s\n\n -------------- \n\n", controller->GetProgramBuilder()->GetFullSource().c_str());
		//printf("BUILD ERROR: %s\n", controller->GetProgramBuilder()->GetError().c_str());
		printf("Build failed: %i\n", program->GetBuildResultCode());
		return -1;
	}

	printf("Program built successfully!!!");

	int Data[DATA_SIZE] = { 0 };

	IComputeBuffer* compBuffer = controller->NewReadWriteBuffer(DATA_SIZE * sizeof(int));

	program->KernelAddBuffer("bar", compBuffer);

	program->RunKernel("bar", DATA_SIZE, 0, 0);

	compBuffer->GetData(Data);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		printf("res '%i': %i\n", i, Data[i]);
	}


	return 0;
}