#pragma once

#include "ForwardDeclarations.h"

namespace Dynamics_IO_Testbench {
    namespace Compute {

        class IComputeBuffer {
        public:
            virtual int SetData(void* data) = 0;

            virtual int GetData(void* outData) = 0;

            virtual size_t GetSize() = 0;

            virtual void Dispose() = 0;

            virtual void* Get_Native_Ptr() = 0;
        };

    }
}