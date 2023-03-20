#pragma once

#include "ForwardDeclarations.h"
#include "IComputeBuffer.h"
#include "IComputeProgram.h"
#include "ComputeInterface.h"

namespace Dynamics_IO_Testbench {
	namespace Compute {

		class IComputeController {
		public:
			

			virtual void Init(Platform platform, Device device, std::string program_dir) = 0;

			virtual IComputeProgram* AddProgram(IComputeProgram::ProgramInfo info) = 0;

			virtual IComputeProgram* GetProgram(std::string name) = 0;

			virtual IComputeBuffer* NewReadBuffer(size_t numElements, size_t stride) = 0;

			virtual IComputeBuffer* NewWriteBuffer(size_t numElements, size_t stride) = 0;

			virtual IComputeBuffer* NewReadWriteBuffer(size_t numElements, size_t stride) = 0;

			virtual void Dispose() = 0;
		};

	}
}