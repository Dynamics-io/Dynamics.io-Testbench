#pragma once

#include "OCL_forwardDeclarations.h"
#include "Compute_Interface/InterfaceIncludes.h"
#include "ComputeEngine.h"

namespace Dynamics_IO_Testbench {
    namespace Compute {
        namespace OCL {

            class ComputeBuffer_OCL : public IComputeBuffer{

                friend class ComputeController_OCL;
                
            public:
                int SetData(void* data);

                int GetData(void* outData);

                size_t GetSize();

                void Dispose();

                void* Get_Native_Ptr();


                // Non-interface methods:

                ComputeBuffer* GetBuffer() { return m_buffer; }

            private:
                ComputeBuffer_OCL(ComputeBuffer* src)
                    : m_buffer(src){}

                ComputeBuffer* m_buffer;
            };

        }
    }
}