#pragma once

#include "OCL_forwardDeclarations.h"
#include "Compute_Interface/PlatformStructures.h"
#include "ComputeProgram_OCL.h"
#include "ComputeEngine.h"


namespace Dynamics_IO_Testbench {
	namespace Compute {
		namespace OCL {
			
			class ComputeController_OCL : public IComputeController {
				friend class ComputeInterface;

			public:
				void Init(Platform platform, Device device, std::string program_dir);

				IComputeProgram* AddProgram(IComputeProgram::ProgramInfo info);

				IComputeProgram* GetProgram(std::string name);

				IComputeBuffer* NewReadBuffer(size_t length);

				IComputeBuffer* NewWriteBuffer(size_t length);

				IComputeBuffer* NewReadWriteBuffer(size_t length);

				void Dispose() {}

				// Non-interface methods:

				ComputeBuffer* NewBuffer(ComputeBuffer::Buffer_Type type, size_t length);

			private:
				ComputeController_OCL() {}

				static IComputeController* New() { return new ComputeController_OCL(); }

				ComputeContext* m_context{ nullptr };
				std::string m_directory;

				std::map<std::string, ComputeProgram_OCL*> m_programs;

			};

		}
	}
}