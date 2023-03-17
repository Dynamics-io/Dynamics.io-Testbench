#pragma once

#include "Compute_Interface/InterfaceIncludes.h"

namespace Dynamics_IO_Testbench {
    namespace Compute {
        namespace DX {

            // forward declarations
            class ComputeBuffer;

            class ComputeBuffer_DX : public IComputeBuffer {
                friend class ComputeController_DX;

            public:
                int SetData(void* data);

                int GetData(void* outData);

                size_t GetSize();

                void Dispose();

                void* Get_Native_Ptr();


                // Non-interface methods:

                ComputeBuffer* GetBuffer() { return m_buffer; }

            private:
                ComputeBuffer_DX(ComputeBuffer* src)
                    : m_buffer(src) {}

                ComputeBuffer* m_buffer;
            };

        }
    }
}