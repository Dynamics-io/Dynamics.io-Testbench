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

char error_str[ERROR_SIZE];


int main()
{
	std::vector<ComputeEngine::Platform> platforms = ComputeEngine::GetSupportedPlatforms();

	for (const auto p : platforms)
	{
		printf("Platform %s:\n", p.name);

		std::vector<ComputeEngine::Device> devices = ComputeEngine::GetSupportedDevices(p);
		for (auto d : devices)
		{
			printf("\t%s - %s: Frequency: %u, threads: %u, Memory: %lu, Work Size: %u \n", d.vendor, d.name, d.clock_frequency, d.num_compute_units * d.group_size, d.mem_size, d.max_work_size);
		}
	}

	ComputeEngine::Platform platf = platforms[0];
	ComputeEngine::Device dev = ComputeEngine::GetSupportedDevices(platf)[0];

	ComputeController* controller = new ComputeController(platf, dev, "");

	std::vector<std::string> kernels;
	kernels.push_back("bar");

	std::string file_path = "";


	controller->BuildProgramFromBinary(file_path, kernels);

	if (controller->GetState() == ComputeController::ComputeState::BuildError)
	{
		//printf("%s\n\n -------------- \n\n", controller->GetProgramBuilder()->GetFullSource().c_str());
		printf("BUILD ERROR: %s\n", controller->GetProgramBuilder()->GetError().c_str());
		return -1;
	}

	printf("Program built successfully!!!");

	return 0;
}