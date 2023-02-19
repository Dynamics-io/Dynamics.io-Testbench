#include "ComputeEngine.h"

using namespace Dynamics_IO_Testbench::Compute::OCL;

#pragma comment(lib, "OpenCL.lib")

cl_platform_id ComputeEngine::platform_id = 0;
cl_uint ComputeEngine::num_of_platforms = 0;
cl_context_properties ComputeEngine::properties[3] = {0};
cl_device_id ComputeEngine::device_ids[MAX_DEVICES] = { 0 };
cl_device_id ComputeEngine::cur_device_id = 0;
cl_uint ComputeEngine::num_of_devices = 0;
std::string  ComputeEngine::app_dir;

/*std::vector<ComputeEngine::Platform> ComputeEngine::GetSupportedPlatforms()
{
    std::vector<ComputeEngine::Platform> res;

    num_of_platforms = 0;

    cl_platform_id all_platforms[10];

    // retreives a list of platforms available
    if (clGetPlatformIDs(10, all_platforms, &num_of_platforms) != CL_SUCCESS)
    {
        return res;
    }

    const int INFO_SIZE = 1000;

    char Info[INFO_SIZE];
    size_t n_size = 0;
    for (int i = 0; i < num_of_platforms; i++)
    {
        ComputeEngine::Platform platform;
        ZeroMemory(&platform, sizeof(ComputeEngine));

        platform.platform = all_platforms[i];

        ZeroMemory(Info, INFO_SIZE);
        clGetPlatformInfo(all_platforms[i], CL_PLATFORM_NAME, INFO_SIZE, Info, &n_size);
        strcpy(platform.name, Info);
        platform.name_size = n_size;

        ZeroMemory(Info, INFO_SIZE);
        clGetPlatformInfo(all_platforms[i], CL_PLATFORM_VENDOR, INFO_SIZE, Info, &n_size);
        strcpy(platform.vendor, Info);
        platform.vendor_size = n_size;

        ZeroMemory(Info, INFO_SIZE);
        clGetPlatformInfo(all_platforms[i], CL_PLATFORM_VERSION, INFO_SIZE, Info, &n_size);
        strcpy(platform.version, Info);
        platform.version_size = n_size;

        res.push_back(platform);
    }

    return res;
}

std::vector<ComputeEngine::Device> ComputeEngine::GetSupportedDevices(Platform pltfrm)
{
    std::vector<Device> res;

    num_of_devices = 0;

    

    // try to get a supported GPU device
    //ComputeEngine::device_ids = new cl_device_id[num_of_devices];
    cl_int get_res = clGetDeviceIDs(pltfrm.platform, CL_DEVICE_TYPE_ALL, MAX_DEVICES, device_ids, &num_of_devices);
    if (get_res != CL_SUCCESS)
    {
        printf("Failed to get devices: %i\n", get_res);
        return res;
    }

    const int INFO_SIZE = 1000;

    char Info[INFO_SIZE];
    cl_device_type type;
    size_t n_size = 0;
    for (int i = 0; i < num_of_devices; i++)
    {
        // CL_DEVICE_MAX_WORK_ITEM_SIZES
        Device dev;
        ZeroMemory(&dev, sizeof(Device));

        dev.device = device_ids[i];

        ZeroMemory(Info, INFO_SIZE);
        clGetDeviceInfo(device_ids[i], CL_DEVICE_VENDOR, INFO_SIZE, Info, &n_size);
        strcpy(dev.vendor, Info);
        dev.vendor_size = n_size;

        ZeroMemory(Info, INFO_SIZE);
        clGetDeviceInfo(device_ids[i], CL_DEVICE_NAME, INFO_SIZE, Info, &n_size);
        strcpy(dev.name, Info);
        dev.name_size = n_size;

        cl_uint freq;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &freq, &n_size);
        dev.clock_frequency = freq;

        cl_uint nm_unts;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &nm_unts, &n_size);
        dev.num_compute_units = nm_unts;

        cl_int num_dim;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &num_dim, &n_size);
        
        size_t* sizes = new size_t[num_dim];
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * num_dim, sizes, &n_size);
        dev.max_work_size = 1;
        for (int w = 0; w < 2; w++)
        {
            dev.max_work_size *= static_cast<unsigned int>(sizes[w]);
        }
        delete[] sizes;

        size_t work_g_size;
        clGetDeviceInfo(device_ids[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &work_g_size, &n_size);
        dev.group_size = work_g_size;
        
        clGetDeviceInfo(device_ids[i], CL_DEVICE_TYPE, sizeof(cl_device_type), &type, &n_size);
        dev.is_type_default = (type & CL_DEVICE_TYPE_DEFAULT) == CL_DEVICE_TYPE_DEFAULT;
        dev.is_type_CPU = (type & CL_DEVICE_TYPE_CPU) == CL_DEVICE_TYPE_CPU;
        dev.is_type_GPU = (type & CL_DEVICE_TYPE_GPU) == CL_DEVICE_TYPE_GPU;
        dev.is_type_Accelerator = (type & CL_DEVICE_TYPE_ACCELERATOR) == CL_DEVICE_TYPE_ACCELERATOR;
        
        res.push_back(dev);
    }
    
    return res;
}*/

int ComputeEngine::Init(cl_platform_id pltform, cl_device_id device, std::string dir)
{
   
    platform_id = pltform;
    cur_device_id = device;
   

   // context properties list - must be terminated with 0
   properties[0] = CL_CONTEXT_PLATFORM;
   properties[1] = (cl_context_properties)platform_id;
   properties[2] = 0;

   app_dir = dir;

   return 0;
}

ComputeContext* ComputeEngine::GetNewContext() {
   return new ComputeContext(properties, device_ids);
}

std::string ComputeEngine::Get_CL_Version()
{
    char platformInfo[1000];
    size_t size = 0;

    cl_int status = clGetPlatformInfo(ComputeEngine::GetPlatform(), CL_PLATFORM_VERSION, 1000, platformInfo, &size);
    

    platformInfo[size] = '\0';
    std::string res = platformInfo;

    return res;
}

ComputeContext::ComputeContext(cl_context_properties properties[3], cl_device_id* device_ids)
{
    cl_int err;
    numContexts = 0;

    //context = new cl_context[ComputeEngine::GetNumDevices()];
    //command_queue = new cl_command_queue[ComputeEngine::GetNumDevices()];

    cl_device_id device = ComputeEngine::GetDevice();

    context = clCreateContext(properties, 1, &device, NULL, NULL, &err);

    command_queue[0] = clCreateCommandQueue(context, device, 0, &err);
    numContexts = 1;

    /*for (int i = 0; i < ComputeEngine::GetNumDevices(); i++)
    {
       command_queue[i] = clCreateCommandQueue(context, device_ids[i], 0, &err);
       numContexts++;
    }*/


}

/*int ComputeContext::SetProgram(const char* source)
{
   cl_int err;

   //program = new cl_program[numContexts];

   // create a program from the kernel source code
   for (int i = 0; i < numContexts; i++)
   {
      program[i] = clCreateProgramWithSource(context, 1, (const char **)&source, NULL, &err);
      // compile the program
      if (clBuildProgram(program[i], 0, NULL, NULL, NULL, NULL) != CL_SUCCESS)
      {
         //printf("Error building program\n");
         return 1;
      }
   }

   return 0;
   
}*/

ComputeProgram* ComputeContext::Add_Program(std::string name)
{
    ComputeProgram* program = new ComputeProgram(context, command_queue, numContexts);
    programs[name] = program;
    return program;
}

ComputeProgram* ComputeContext::Add_Program_Source(std::string name, const char* source)
{
   ComputeProgram* program = new ComputeProgram(context, command_queue, numContexts);
   int res = program->Set_Source(source);
   programs[name] = program;
   return program;
}

ComputeProgram* ComputeContext::Add_Program_SPIRV(std::string name, const void* binary, size_t length)
{
    ComputeProgram* program = new ComputeProgram(context, command_queue, numContexts);
    int res = program->Set_Binary(binary, length);
    programs[name] = program;
    return program;
}

ComputeProgram* ComputeContext::Add_Program_SPIRV_File(std::string name, std::string file_path)
{
    ComputeProgram* program = new ComputeProgram(context, command_queue, numContexts);

    int res = program->Set_Binary_File(file_path);
    programs[name] = program;
    return program;
}

ComputeProgram* ComputeContext::Programs(std::string name)
{
   return programs[name];
}

ComputeKernel* ComputeContext::GetKernel(std::string p_name, std::string name)
{
   return Programs(p_name)->GetKernel(name);
}

ComputeProgram::ComputeProgram(cl_context context, cl_command_queue queue[MAX_DEVICES], int numDevs)
{
   numDevices = numDevs;
   m_context = context;

   for (int i = 0; i < numDevices; i++)
   {
      command_queue[i] = queue[i];
   }
}

int ComputeProgram::Set_Source(const char* source)
{
   cl_int err;
   program = clCreateProgramWithSource(m_context, 1, (const char **)&source, NULL, &err);
   return err;
}

int ComputeProgram::Set_Binary(const void* binary, size_t length)
{
    cl_int err;
    program = clCreateProgramWithIL(m_context, binary, length, &err);
    return err;
}

int ComputeProgram::Set_Binary_File(std::string file_path)
{
    int res = -1;
    //open file
    std::ifstream infile(file_path, std::ios::binary);
    char* buffer;

    //get length of file
    infile.seekg(0, infile.end);
    size_t length = infile.tellg();
    infile.seekg(0, infile.beg);

    //read file
    if (length > 0) {

        buffer = new char[length];
        infile.read(buffer, length);

        //buffer.resize(length);
        //buffer.insert(buffer.begin(), std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());
        //infile.read(&buffer[0], length);
        //const void* binary = static_cast<void*>(buffer.data());

        printf("ComputeProgram.Set_Binary_File: Reading binary file of length %i\n", (int)length);
        res =Set_Binary(buffer, length);
    }
    else
    {
        printf("ComputeProgram.Set_Binary_File: File has length of zero.");
        res = -2;
    }

    

    return res;
}

void ComputeProgram::AddConstant(std::string name, std::string value)
{
   args += "-D" + name + "=" + value + " ";
}

int ComputeProgram::Build(char* errorStr, size_t e_size)
{
    if (ComputeEngine::GetAppDir() != "")
    {
        std::string inc_dir = ComputeEngine::GetAppDir();
        args += "-I " + inc_dir;
    }
   int build_res = clBuildProgram(program, 0, NULL, args.c_str(), NULL, NULL);

   size_t ret_e_size;
   int res = clGetProgramBuildInfo(program, ComputeEngine::GetDevice(), CL_PROGRAM_BUILD_LOG, e_size, errorStr, &ret_e_size);

   return build_res;
}

ComputeKernel* ComputeProgram::GetKernel(std::string k_name)
{
   if (kernels.count(k_name) > 0)
   {
      return kernels[k_name];
   }

   ComputeKernel* new_kern = new ComputeKernel((char*)k_name.c_str(), command_queue, program, numDevices);
   kernels[k_name] = new_kern;
   return new_kern;
}

void ComputeProgram::Dispose()
{
    for (const auto& [key, value] : kernels) {
        value->Dispose();
    }

    clReleaseProgram(program);
}

ComputeBuffer* ComputeContext::GetBuffer(ComputeBuffer::Buffer_Type type, size_t size)
{
   cl_mem_flags flags;

   switch (type)
   {
   case ComputeBuffer::Buffer_Type::READ:
      flags = CL_MEM_READ_ONLY;
      break;

   case ComputeBuffer::Buffer_Type::Write:
      flags = CL_MEM_WRITE_ONLY;
      break;

   case ComputeBuffer::Buffer_Type::Read_Write:
      flags = CL_MEM_READ_WRITE;
      break;

   default:
      return NULL;
   }

   return new ComputeBuffer(context, command_queue, numContexts, flags, size);
}

void ComputeContext::Dispose()
{
    for (const auto& [key, value] : programs) {
        value->Dispose();
    }

    for (int i = 0; i < numContexts; i++)
    {
        clReleaseCommandQueue(command_queue[i]);
    }

    clReleaseContext(context);
}

ComputeKernel::ComputeKernel(char* name, cl_command_queue queue[MAX_DEVICES], cl_program program, int numPrograms)
{
   cl_int err;
   //numKernels = numPrograms;
   m_program = program;

   //kernels = new cl_kernel[numKernels];
   //command_queue = new cl_command_queue[numKernels];

   for (int i = 0; i < numPrograms; i++)
   {
      command_queue[i] = queue[i];
   }
   
   printf("ComputeKernel(): Create kernel %s\n", name);
   kernel = clCreateKernel(m_program, name, &err);

   if (err != 0)
   {
       printf("ComputeKernel(): Failed to create kernel: %i\n", err);

   }

   status = err;

}

int ComputeKernel::SetBuffer(int device, ComputeBuffer* buffer, int arg)
{
   int res = clSetKernelArg(kernel, arg, sizeof(cl_mem), (void*)buffer->Get_CL_Mem());
   return res;
}

int ComputeKernel::Execute(int device, cl_uint work_dim, size_t* global_work_size)
{
   cl_command_queue c_q = command_queue[device];
   int res = clEnqueueNDRangeKernel(c_q, kernel, work_dim, NULL, global_work_size, NULL, 0, NULL, NULL);

   if (res != 0)
   {
       printf("ComputeKernel.Execute: Failed to enqueue Kernel: %i\n", res);
   }

   res = clFinish(command_queue[device]);

   if (res != 0)
   {
       printf("ComputeKernel.Execute: clFinish failed after kernel enqueue: %i\n", res);
   }

   return res;
}

void ComputeKernel::Dispose()
{
    clReleaseKernel(kernel);
}

ComputeBuffer::ComputeBuffer(cl_context contexts, cl_command_queue queues[MAX_DEVICES], int numContext, cl_mem_flags type, size_t length)
{
   num = numContext;
   //buffer = new cl_mem[numContext];
   //command_queue = new cl_command_queue[numContext];
   size = length;

   context = contexts;

   cl_int err;
   for (int i = 0; i < numContext; i++)
   {
      command_queue[i] = queues[i];
      
   }

   buffer = clCreateBuffer(context, type, length, NULL, &err);
}

int ComputeBuffer::SetData(void* data)
{
   int res = clEnqueueWriteBuffer(command_queue[0], buffer, CL_TRUE, 0, size, data, 0, NULL, NULL);
   return res;
}

int ComputeBuffer::GetData(void* outData)
{
   int res = clEnqueueReadBuffer(command_queue[0], buffer, CL_TRUE, 0, size, outData, 0, NULL, NULL);
   return res;
}

void ComputeBuffer::Dispose()
{
    clReleaseMemObject(buffer);
}
