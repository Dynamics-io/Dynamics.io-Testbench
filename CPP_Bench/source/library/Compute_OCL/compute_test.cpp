#include "compute_test.h"

using namespace Dynamics_IO_Testbench::Compute::OCL;

//#pragma comment(lib, "OpenCL.lib")

static cl_uint getDeviceOpenCLVersion(
    const cl::Device& device)
{
    cl_uint major = 0;
    cl_uint minor = 0;

    std::string version = device.getInfo<CL_DEVICE_VERSION>();

    // The device version string has the form:
    //   OpenCL <Major>.<Minor> <Vendor Specific Info>
    const std::string prefix{ "OpenCL " };
    if (!version.compare(0, prefix.length(), prefix)) {
        const char* check = version.c_str() + prefix.length();
        while (isdigit(check[0])) {
            major *= 10;
            major += check[0] - '0';
            ++check;
        }
        if (check[0] == '.') {
            ++check;
        }
        while (isdigit(check[0])) {
            minor *= 10;
            minor += check[0] - '0';
            ++check;
        }
    }

    return (major << 16) | minor;
}

static bool checkDeviceForExtension(
    const cl::Device& device,
    const char* extensionName)
{
    bool supported = false;

    if (extensionName && !strchr(extensionName, ' ')) {
        std::string deviceExtensions = device.getInfo<CL_DEVICE_EXTENSIONS>();

        const char* start = deviceExtensions.c_str();
        while (true) {
            const char* where = strstr(start, extensionName);
            if (!where) {
                break;
            }
            const char* terminator = where + strlen(extensionName);
            if (where == start || *(where - 1) == ' ') {
                if (*terminator == ' ' || *terminator == '\0') {
                    supported = true;
                    break;
                }
            }
            start = terminator;
        }
    }

    return supported;
}

static std::vector<cl_uchar> readSPIRVFromFile(
    const std::string& filename)
{
    std::ifstream is(filename, std::ios::binary);
    std::vector<cl_uchar> ret;
    if (!is.good()) {
        printf("Couldn't open file '%s'!\n", filename.c_str());
        return ret;
    }

    size_t filesize = 0;
    is.seekg(0, std::ios::end);
    filesize = (size_t)is.tellg();
    is.seekg(0, std::ios::beg);

    ret.reserve(filesize);
    ret.insert(
        ret.begin(),
        std::istreambuf_iterator<char>(is),
        std::istreambuf_iterator<char>());

    return ret;
}

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

    getDeviceOpenCLVersion()

	cl_device_id deviceID = (cl_device_id)device.cl_device;

	cl_int err;

	cl_context context = clCreateContext(properties, 1, &deviceID, NULL, NULL, &err);
	cl_command_queue command_queue = clCreateCommandQueue(context, deviceID, 0, &err);


    //open file
    /*std::ifstream infile(file_path, std::ios::binary);
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
    infile.read(buffer, length);*/

    std::vector<cl_uchar> buffer = readSPIRVFromFile(file_path);
    size_t length = buffer.size();
    cl_uchar* buffer_ptr = buffer.data();



    //cl_program program = clCreateProgramWithIL(context, buffer, length, &err);
    //printf("CreateProgramWithIL: res %i\n", err);

    cl_int binary_status = 0;
    cl_program program = clCreateProgramWithBinary(context, 1, &deviceID, &length, (const unsigned char**)&buffer_ptr, &binary_status, &err);
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
