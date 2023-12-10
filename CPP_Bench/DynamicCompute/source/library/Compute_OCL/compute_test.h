#pragma once

#include "stdafx.h"

#include "Compute_Interface/PlatformStructures.h"

#include "Compute_OCL/CL_SDK/cl.h"
#include "Compute_OCL/CL_SDK/opencl.hpp"
#include "Compute_OCL/CL_SDK/Utils/Utils.hpp"
//#include ""

namespace Dynamics_IO_Testbench {
    namespace Compute {
        namespace OCL {

            class compute_test {
            public:
                void Run(Platform pltform, Device device);

            private:

            };

        }
    }
}