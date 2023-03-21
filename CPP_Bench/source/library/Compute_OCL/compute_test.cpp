#include "compute_test.h"

using namespace Dynamics_IO_Testbench::Compute::OCL;

#pragma comment(lib, "OpenCL.lib")

void compute_test::Run(Platform pltform, Device device)
{
    std::string file_path = "C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/OpenCL/test_cl_spv.spv";

	cl_platform_id platform_id = (cl_platform_id)pltform.platform;

	// context properties list - must be terminated with 0
	cl_context_properties properties[3];
	properties[0] = CL_CONTEXT_PLATFORM;
	properties[1] = (cl_context_properties)platform_id;
	properties[2] = 0;

    char platformInfo[1000];
    size_t size = 0;

    cl_int status = clGetPlatformInfo(platform_id, CL_PLATFORM_VERSION, 1000, platformInfo, &size);


    platformInfo[size] = '\0';
    //std::string v_str = platformInfo;
    printf("OpenCL version: %s\n", platformInfo);

	cl_device_id deviceID = (cl_device_id)device.cl_device;

	cl_int err;

	cl_context context = clCreateContext(properties, 1, &deviceID, NULL, NULL, &err);
	cl_command_queue command_queue = clCreateCommandQueue(context, deviceID, 0, &err);


    //open file
    std::ifstream infile(file_path, std::ios::binary);
    char* buffer;
    //get length of file
    infile.seekg(0, infile.end);
    size_t length = infile.tellg();
    infile.seekg(0, infile.beg);
    if (length == 0)
        return;

    //read file
    printf("Reading binary file of length %i\n", (int)length);
    buffer = new char[length];
    infile.read(buffer, length);

    //cl_program program = clCreateProgramWithIL(context, buffer, length, &err);
    //printf("CreateProgramWithIL: res %i\n", err);
    cl_int binary_status = 0;
    cl_program program = clCreateProgramWithBinary(context, 1, &deviceID, &length, (const unsigned char**)&buffer, &binary_status, &err);
    printf("CreateProgramWithBinary: res %i, %i\n", err, binary_status);

    std::string args;
    cl_int build_res = clBuildProgram(program, 1, &deviceID, args.c_str(), NULL, NULL);
    printf("BuildProgram: res %i\n", build_res);

    
    cl_kernel kernel = clCreateKernel(program, "work", &err);
    printf("Create kernel '%s': %i\n", "work", err);

    char errorStr[1000];
    size_t e_size = 0;
    int res = clGetProgramBuildInfo(program, deviceID, CL_PROGRAM_BUILD_LOG, 1000, errorStr, &e_size);
    errorStr[size] = '\0';
    printf("LOG (%i): %s\n", (int)e_size, errorStr);
}
