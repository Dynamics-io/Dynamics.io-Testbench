#include "dll_main.h"

#include <time.h>
#include <chrono>

#include "utils/QuadraticSolvers.h"

#define DATA_SIZE 32


using namespace Dynamics_IO_Testbench::Compute;
using namespace Dynamics_IO_Testbench::Util;

int Vulkan_test();
int OpenCL_test();
int DirectX_test();

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
	//Vulkan_test();
	//DirectX_test();
	OpenCL_test();

	return 0;
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

int DirectX_test() {

	//ComputeInterface::ControllerInfo test_controllerInfo;
	//ComputeInterface::GetComputeController(ComputeInterface::DIRECTX, test_controllerInfo);


	std::vector<DirectX_Device_Info> devices = ComputeInterface::GetSupportedDevices_DirectX();

	printf("DirectX Devices: %u\n", (uint32_t)devices.size());
	for (DirectX_Device_Info info : devices) {
		//DirectX_Device_Info info = dev.DirectX_Info;

		printf("\t %s (%s): %i\n", info.Name, info.GetTypeName().c_str(), info.Device_ID);
	}

	DirectX_Device_Info device = devices[0];

	ComputeInterface::ControllerInfo controllerInfo{};
	controllerInfo.device = &device;
	controllerInfo.SetProgramDir("C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/DirectX");
	IComputeController* controller = ComputeInterface::GetComputeController(ComputeInterface::DIRECTX, controllerInfo);

	std::string kernel_name = "CSMain";

	IComputeProgram::ProgramInfo p_info("Test", IComputeProgram::FileType::Binary);
	p_info.AddKernel(kernel_name);

	IComputeProgram* program = controller->AddProgram(p_info);

	float Data[DATA_SIZE] = { 0 };
	for (int i = 0; i < DATA_SIZE; i++)
		Data[i] = i + 1;

	IComputeBuffer* in_Buffer = controller->NewReadBuffer(DATA_SIZE, sizeof(float));
	IComputeBuffer* out_Buffer = controller->NewWriteBuffer(DATA_SIZE, sizeof(float));

	IComputeProgram::BindIndex ind{};

	ind.RegisterIndex = 0;
	program->KernelSetBuffer(kernel_name, in_Buffer, ind);

	ind.RegisterIndex = 0;
	program->KernelSetBuffer(kernel_name, out_Buffer, ind);

	program->FinishBuild();

	in_Buffer->SetData(Data);

	program->RunKernel(kernel_name, DATA_SIZE, 0, 0);

	out_Buffer->GetData(Data);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		printf("res '%i': %f\n", i, Data[i]);
	}

	ComputeInterface::DisposePlatform(ComputeInterface::DIRECTX);
	printf("DirectX Disposed.");

	return 0;
}

int Vulkan_test()
{
	//ComputeInterface::ControllerInfo test_controllerInfo;
	//ComputeInterface::GetComputeController(ComputeInterface::VULKAN, test_controllerInfo);

	//return 0;
	//testConstructor();


	std::vector<Vulkan_Device_Info> devices = ComputeInterface::GetSupportedDevices_Vulkan();

	printf("Vulkan Devices: %u\n", (uint32_t)devices.size());
	for (Vulkan_Device_Info info : devices) {
		//Vulkan_Device_Info info = dev.Vulkan_Info;

		printf("\t %s (%s): %i, %s\n", info.Name, info.GetTypeName().c_str(), info.Device_ID, info.GetUUID().c_str());
	}

	//return 0;

	Vulkan_Device_Info device = devices[0];

	ComputeInterface::ControllerInfo controllerInfo{};
	controllerInfo.device = &device;
	controllerInfo.SetProgramDir("C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/Vulkan");
	IComputeController* controller = ComputeInterface::GetComputeController(ComputeInterface::VULKAN, controllerInfo);

	std::string kernel_name1 = "work";
	std::string kernel_name2 = "work2";

	IComputeProgram::ProgramInfo p_info("test_cl_spv", IComputeProgram::FileType::Binary);
	p_info.AddKernel(kernel_name1);
	p_info.AddKernel(kernel_name2);

	IComputeProgram* program = controller->AddProgram(p_info);

	int Data[DATA_SIZE] = { 0 };
	for (int i = 0; i < DATA_SIZE; i++)
		Data[i] = i + 1;

	IComputeBuffer* in_Buffer = controller->NewReadBuffer(DATA_SIZE, sizeof(int));
	IComputeBuffer* out_Buffer = controller->NewWriteBuffer(DATA_SIZE, sizeof(int));

	IComputeProgram::BindIndex ind{};

	ind.GlobalIndex = 0;
	//program->KernelSetBuffer(kernel_name, in_Buffer, ind);

	ind.GlobalIndex = 0;
	program->KernelSetBuffer(kernel_name1, out_Buffer, ind);
	program->KernelSetBuffer(kernel_name2, out_Buffer, ind);

	program->FinishBuild();

	in_Buffer->SetData(Data);


	program->RunKernel(kernel_name1, DATA_SIZE, 0, 0);

	out_Buffer->GetData(Data);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		printf("res '%i': %i\n", i, Data[i]);
	}

	program->RunKernel(kernel_name2, DATA_SIZE, 0, 0);

	out_Buffer->GetData(Data);

	for (int i = 0; i < DATA_SIZE; i++)
	{
		printf("res '%i': %i\n", i, Data[i]);
	}

	ComputeInterface::DisposePlatform(ComputeInterface::VULKAN);
	printf("Vulkan Disposed.");

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
	printf("OpenCL_Device_Info size: %i\n", sizeof(OpenCL_Device_Info::mem_size));

	//return 0;

	std::vector<Platform> platforms = ComputeInterface::GetSupportedPlatforms_OpenCL();

	for (const auto& p : platforms)
	{
		printf("Platform %s:\n", p.name);

		std::vector<OpenCL_Device_Info> devices = ComputeInterface::GetSupportedDevices_OpenCL(p);
		for (auto info : devices)
		{
			//OpenCL_Device_Info info = d.OpenCL_Info;
			//printf("\t%s - %s: Frequency: %u, threads: %u, Memory: %lu, Work Size: %u \n", info.vendor, info.name, info.clock_frequency, info.num_compute_units * info.group_size, info.mem_size, info.max_work_size);
			printf("\t%s - %s\n", info.vendor, info.name);
		}
	}

	Platform platf = platforms[1];
	OpenCL_Device_Info dev = ComputeInterface::GetSupportedDevices_OpenCL(platf)[0];

	ComputeInterface::ControllerInfo controllerInfo;
	controllerInfo.platform = platf;
	controllerInfo.device = &dev;
	controllerInfo.SetProgramDir("C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/OpenCL/Tree");

	IComputeController* controller = ComputeInterface::GetComputeController(ComputeInterface::OpenCL, controllerInfo);

	std::string kernel_name = "Init";

	IComputeProgram::ProgramInfo p_info("Tree", IComputeProgram::FileType::Text);
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

	return 0;

	int Data[DATA_SIZE] = { 0 };

	IComputeBuffer* compBuffer = controller->NewReadWriteBuffer(DATA_SIZE, sizeof(int));

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