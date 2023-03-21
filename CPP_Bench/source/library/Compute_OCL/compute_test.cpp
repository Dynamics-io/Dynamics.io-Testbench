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

static cl::Program createProgramWithIL(
    const cl::Context& context,
    const std::vector<cl_uchar>& il)
{
    cl_program program = nullptr;

    // Use the core clCreateProgramWithIL if a device supports OpenCL 2.1 or
    // newer and SPIR-V.
    bool useCore = false;

    // Use the extension clCreateProgramWithILKHR if a device supports
    // cl_khr_il_program.
    bool useExtension = false;

    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    for (auto device : devices) {
#ifdef CL_VERSION_2_1
        // Note: This could look for "SPIR-V" in CL_DEVICE_IL_VERSION.
        if (getDeviceOpenCLVersion(device) >= 0x00020001 &&
            !device.getInfo<CL_DEVICE_IL_VERSION>().empty()) {
            useCore = true;
        }
#endif
        if (checkDeviceForExtension(device, "cl_khr_il_program")) {
            useExtension = true;
        }
    }

#ifdef CL_VERSION_2_1
    if (useCore) {
        program = clCreateProgramWithIL(
            context(),
            il.data(),
            il.size(),
            nullptr);
    }
    else
#endif
        if (useExtension) {
            cl::Platform platform{ devices[0].getInfo<CL_DEVICE_PLATFORM>() };

            auto clCreateProgramWithILKHR_ = (clCreateProgramWithILKHR_fn)
                clGetExtensionFunctionAddressForPlatform(
                    platform(),
                    "clCreateProgramWithILKHR");

            if (clCreateProgramWithILKHR_) {
                program = clCreateProgramWithILKHR_(
                    context(),
                    il.data(),
                    il.size(),
                    nullptr);
            }
        }

    return cl::Program{ program };
}




void compute_test::Run(Platform pltform, Device device)
{
    std::string file_path = "C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/OpenCL/test_cl_spv.spv";


    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    printf("\n\n");

    for (cl::Platform plat : platforms) {
        //printf("Platform Name: %s\n", plat.getInfo<CL_PLATFORM_NAME>().c_str());
    }

    cl::Platform platform_obj = platforms[1];
    std::vector<cl::Device> devices;
    platform_obj.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    printf("Platform Name: %s\n", platform_obj.getInfo<CL_PLATFORM_NAME>().c_str());

    for (cl::Device devc : devices) {
        //printf("Device Name: %s\n", devc.getInfo<CL_DEVICE_NAME>().c_str());
    }

    cl::Device device_obj = devices[0];
    printf("Device Name: %s\n", device_obj.getInfo<CL_DEVICE_NAME>().c_str());

    bool core_support = false;

    if (getDeviceOpenCLVersion(device_obj) >= 0x00020001 &&
        !device_obj.getInfo<CL_DEVICE_IL_VERSION>().empty()) {
        printf("Core API supports IL!\n");
        core_support = true;
    }
    else {
        printf("CL Version: %X\n", getDeviceOpenCLVersion(device_obj));
        printf("CL_DEVICE_IL_VERSION: %s\n", device_obj.getInfo<CL_DEVICE_IL_VERSION>().c_str());
    }

    bool extensionSupported = false;

    if (checkDeviceForExtension(device_obj, "cl_khr_il_program")) {
        printf("IL Extension supported!\n");
        extensionSupported = true;
    }
    else {
        printf("Il Extension NOT supported!\n");
    }

    cl_platform_id platform_id = platform_obj();
    cl_device_id deviceID = device_obj();

    if (!(core_support || extensionSupported)) {
        printf("IL not supported!\n");
        return;
    }

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



	cl_int err;

	cl_context context = clCreateContext(properties, 1, &deviceID, NULL, NULL, &err);
	cl_command_queue command_queue = clCreateCommandQueue(context, deviceID, 0, &err);


    std::vector<cl_uchar> buffer = readSPIRVFromFile(file_path);
    size_t length = buffer.size();
    cl_uchar* buffer_ptr = buffer.data();

    cl_program program;
    if (extensionSupported) {
        auto clCreateProgramWithILKHR_ = (clCreateProgramWithILKHR_fn)
            clGetExtensionFunctionAddressForPlatform(
                platform_obj(),
                "clCreateProgramWithILKHR");

        program = clCreateProgramWithILKHR_(context, buffer_ptr, length, &err);
        printf("clCreateProgramWithILKHR_: res %i\n", err);
    }
    else {

        //cl_program program = clCreateProgramWithIL(context, buffer, length, &err);
        //printf("CreateProgramWithIL: res %i\n", err);

        cl_int binary_status = 0;
        program = clCreateProgramWithBinary(context, 1, &deviceID, &length, (const unsigned char**)&buffer_ptr, &binary_status, &err);
        printf("CreateProgramWithBinary: res %i, %i\n", err, binary_status);
    }

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
