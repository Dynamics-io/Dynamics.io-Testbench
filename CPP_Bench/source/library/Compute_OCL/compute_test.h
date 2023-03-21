#pragma once

#include "stdafx.h"

#include "Compute_Interface/PlatformStructures.h"

#include "CL/cl.h"


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