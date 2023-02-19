#pragma once

#include "CL/cl.h"

namespace Dynamics_IO_Testbench {
    namespace Compute {

        struct Platform {
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
        };

    }
}