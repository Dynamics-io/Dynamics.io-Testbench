#include "dll_main.h"

#include <time.h>
#include <chrono>

#include "utils/QuadraticSolvers.h"

#define DATA_SIZE 16


using namespace Dynamics_IO_Testbench::Compute;
using namespace Dynamics_IO_Testbench::Util;

int Vulkan_test();
int OpenCL_test();

class testClass {
public:
	testClass() {
		printf("testClass constructor.\n");
	}
	~testClass() {
		printf("testClass destructor.\n");
	}
};


int main()
{
	return Vulkan_test();
	//return OpenCL_test();
}

void testConstructor_insert(std::map<std::string, testClass>& test_map) {
	//test_map.insert("test", test_map);
}

void testConstructor() {
	std::map<std::string, testClass> test_map;
	testConstructor_insert(test_map);
}

#define TEST_SIZE 1000 * 8000

float result[TEST_SIZE];

void quadratic_tests() {
	static auto startTime = std::chrono::high_resolution_clock::now();

	float neg = 1;
	for (int i = 0; i < TEST_SIZE; i++)
	{

		result[i] = QuadraticSolvers::QuadraticArcLength(1 + (i / TEST_SIZE / 100.0f) * neg, (i / TEST_SIZE / 100.0f), (i / TEST_SIZE / 100.0f), 0, 1);
		neg = -neg;
	}

	auto currentTime = std::chrono::high_resolution_clock::now();

	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	std::cout << "Test time: " << std::to_string(time) << "\n";


	//printf("Arc Length Integral: %f\n", arcLengh);

	return;
}

int Vulkan_test()
{
	//ComputeInterface::ControllerInfo controllerInfo;
	//ComputeInterface::GetComputeController(ComputeInterface::VULKAN, controllerInfo);

	//testConstructor();

	std::vector<Device> devices = ComputeInterface::GetSupportedDevices_Vulkan();

	printf("Devices: %u\n", (uint32_t)devices.size());
	for (Device dev : devices) {
		Vulkan_Device_Info info = dev.Vulkan_Info;

		printf("%s (%s): %i, %s\n", info.Name.c_str(), info.GetTypeName().c_str(), info.Device_ID, info.GetUUID().c_str());
	}

	Device device = devices[0];

	ComputeInterface::ControllerInfo controllerInfo{};
	controllerInfo.device = device;
	controllerInfo.program_dir = "C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/Vulkan";
	IComputeController* controller = ComputeInterface::GetComputeController(ComputeInterface::VULKAN, controllerInfo);

	std::string kernel_name = "main";

	IComputeProgram::ProgramInfo p_info("compute.spv");
	p_info.AddKernel(kernel_name);

	IComputeProgram* program = controller->AddProgram(p_info);

	float Data[DATA_SIZE] = { 0 };
	for (int i = 0; i < DATA_SIZE; i++)
		Data[i] = i + 1;

	IComputeBuffer* in_Buffer = controller->NewReadBuffer(DATA_SIZE * sizeof(int));
	IComputeBuffer* out_Buffer = controller->NewWriteBuffer(DATA_SIZE * sizeof(int));

	IComputeProgram::BindIndex ind{};

	ind.GlobalIndex = 0;
	program->KernelSetBuffer(kernel_name, in_Buffer, ind);

	ind.GlobalIndex = 1;
	program->KernelSetBuffer(kernel_name, in_Buffer, ind);

	program->FinishBuild();


	program->RunKernel(kernel_name, DATA_SIZE, 0, 0);

	out_Buffer->GetData(Data);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		printf("res '%i': %i\n", i, Data[i]);
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

	for (const auto& p : platforms)
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