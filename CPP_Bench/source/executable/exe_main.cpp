#include "dll_main.h"

#include <time.h>
#include <chrono>

#define DATA_SIZE 10


using namespace Dynamics_IO_Testbench::Compute;

int Vulkan_test();
int OpenCL_test();


int main()
{
	return Vulkan_test();
	//return OpenCL_test();
}

int Vulkan_test()
{
	//ComputeInterface::ControllerInfo controllerInfo;
	//ComputeInterface::GetComputeController(ComputeInterface::VULKAN, controllerInfo);


	std::vector<Device> devices = ComputeInterface::GetSupportedDevices_Vulkan();

	printf("Devices: %u\n", devices.size());
	for (Device dev : devices) {
		Vulkan_Device_Info info = dev.Vulkan_Info;

		printf("%s (%s): %i, %s\n", info.Name.c_str(), info.GetTypeName().c_str(), info.Device_ID, info.GetUUID().c_str());
	}

	return 0;
}


const char* KernelSource =
"__kernel void hello(__global float *input, __global float *output)\n"\
"{\n"\
"  size_t id = get_global_id(0);\n"
"  output[id] = input[id] * input[id];\n"
"}\n"\
"\n";


int OpenCL_test()
{
	std::vector<Platform> platforms = ComputeInterface::GetSupportedPlatforms_OpenCL();

	for (const auto p : platforms)
	{
		printf("Platform %s:\n", p.name);

		std::vector<Device> devices = ComputeInterface::GetSupportedDevices_OpenCL(p);
		for (auto d : devices)
		{
			//printf("\t%s - %s: Frequency: %u, threads: %u, Memory: %lu, Work Size: %u \n", d.vendor, d.name, d.clock_frequency, d.num_compute_units * d.group_size, d.mem_size, d.max_work_size);
		}
	}

	Platform platf = platforms[1];
	Device dev = ComputeInterface::GetSupportedDevices_OpenCL(platf)[0];

	ComputeInterface::ControllerInfo controllerInfo;
	controllerInfo.platform = platf;
	controllerInfo.device = dev;
	controllerInfo.program_dir = "C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/llvm_clang_Bench/Kernels";

	IComputeController* controller = ComputeInterface::GetComputeController(ComputeInterface::OpenCL, controllerInfo);

	std::string kernel_name = "work";

	IComputeProgram::ProgramInfo p_info("test_cl_opt.spv");
	p_info.AddKernel(kernel_name);

	IComputeProgram* program = controller->AddProgram(p_info);


	if (program->GetState() == IComputeProgram::ProgramBuildState::BuildError)
	{
		if (program->GetBuildResultCode() != 0)
		{
			printf("Build failed: %i\n", program->GetBuildResultCode());
			return -1;
		}

		//printf("%s\n\n -------------- \n\n", controller->GetProgramBuilder()->GetFullSource().c_str());
		printf("BUILD ERROR: %s\n", program->GetBuildErrorMessage().c_str());
		
		return -1;
	}

	printf("Finished building program '%s'.\n", p_info.Name().c_str());

	int Data[DATA_SIZE] = { 0 };

	IComputeBuffer* compBuffer = controller->NewReadWriteBuffer(DATA_SIZE * sizeof(int));

	IComputeProgram::BindIndex ind{};
	ind.ParameterIndex = 0;
	program->KernelSetBuffer(kernel_name, compBuffer, ind);

	program->RunKernel(kernel_name, DATA_SIZE, 0, 0);

	compBuffer->GetData(Data);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		printf("res '%i': %i\n", i, Data[i]);
	}


	return 0;
}