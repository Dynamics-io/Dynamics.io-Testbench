#pragma once

#include "OCL_forwardDeclarations.h"
#include "CL/cl.h"


//#include "ComputeContext.h"
namespace Dynamics_IO_Testbench {
    namespace Compute {
        namespace OCL {

            //class ComputeContext;
            //class ComputeEngine;
            //class ComputeKernel;
            //class ComputeBuffer;

#define MAX_DEVICES  10

            class ComputeBuffer
            {
                cl_context context;
                cl_mem buffer;
                cl_command_queue command_queue[MAX_DEVICES];
                int num;
                size_t size;

            public:
                enum class Buffer_Type
                {
                    READ = 0,
                    Write = 1,
                    Read_Write = 2
                };


                ComputeBuffer(cl_context context, cl_command_queue queus[MAX_DEVICES], int numContext, cl_mem_flags type, size_t length);

                cl_mem* Get_CL_Mem()
                {
                    return &buffer;
                }

                int SetData(void* data);

                int GetData(void* outData);

                size_t GetSize() { return size; };

                void Dispose();
            };


            class ComputeKernel
            {
                cl_program m_program;
                cl_kernel kernel;
                cl_command_queue command_queue[MAX_DEVICES];
                int status;

                //int numKernels;

            public:
                ComputeKernel(char* name, cl_command_queue command_queue[MAX_DEVICES], cl_program program, int numPrograms);

                int GetStatus() { return status; }

                int SetBuffer(int device, ComputeBuffer* buffer, int arg);

                int Execute(int device, cl_uint work_dim, size_t* global_work_size);

                void Dispose();
            };

            class ComputeProgram {
                std::map<std::string, ComputeKernel*> kernels;
                cl_program program;
                cl_context m_context;
                cl_command_queue command_queue[MAX_DEVICES];
                int numDevices;


                std::string args;

            public:
                ComputeProgram(cl_context context, cl_command_queue queue[MAX_DEVICES], int numDevs);

                int Set_Source(const char* source);

                int Set_Binary(const void* binary, size_t length);

                int Set_Binary_File(std::string file_path);

                void AddConstant(std::string name, std::string value);

                int Build(char* errorStr, size_t e_size);

                ComputeKernel* GetKernel(std::string name);

                void Dispose();
            };

            class ComputeContext {
                std::map<std::string, ComputeProgram*> programs;
                cl_context context;
                cl_command_queue command_queue[MAX_DEVICES];
                int numContexts;

            public:
                ComputeContext(cl_context_properties properties[3], cl_device_id* device_ids);

                //int SetProgram(const char* source);
                ComputeProgram* Add_Program(std::string name);
                ComputeProgram* Add_Program_Source(std::string name, const char* source);
                ComputeProgram* Add_Program_SPIRV(std::string name, const void* binary, size_t length);
                ComputeProgram* Add_Program_SPIRV_File(std::string name, std::string file_path);
                ComputeProgram* Programs(std::string name);

                ComputeKernel* GetKernel(std::string p_name, std::string name);
                ComputeBuffer* GetBuffer(ComputeBuffer::Buffer_Type type, size_t length);

                void Dispose();
            };


            class ComputeEngine
            {
                static cl_platform_id platform_id;
                static cl_uint num_of_platforms;
                static cl_context_properties properties[3];
                static cl_device_id device_ids[MAX_DEVICES];
                static cl_device_id cur_device_id;
                static cl_uint num_of_devices;

                static std::string app_dir;

            public:
                /*struct Platform {
                    cl_platform_id platform;

                    char name[1000];
                    unsigned short name_size;

                    char vendor[1000];
                    unsigned short vendor_size;

                    char version[1000];
                    unsigned short version_size;
                };

                struct Device {
                    cl_device_id device;

                    char vendor[1000];
                    unsigned short vendor_size;

                    char name[1000];
                    unsigned short name_size;

                    unsigned int clock_frequency;
                    unsigned int num_compute_units;
                    unsigned long mem_size;
                    unsigned int max_work_size;
                    unsigned int group_size;
                    bool is_type_default;
                    bool is_type_CPU;
                    bool is_type_GPU;
                    bool is_type_Accelerator;
                };*/

                //static std::vector<Platform> GetSupportedPlatforms();
                //static std::vector<Device> GetSupportedDevices(Platform pltfrm);
                static int Init(cl_platform_id platform, cl_device_id device, std::string dir);
                static ComputeContext* GetNewContext();

                static cl_platform_id GetPlatform()
                {
                    return platform_id;
                }

                static int GetNumDevices() {
                    return num_of_devices;
                }

                static std::string GetAppDir()
                {
                    return app_dir;
                }

                static cl_device_id GetDevice()
                {
                    return cur_device_id;
                }

                static std::string Get_CL_Version();
            };

        }
    }
}