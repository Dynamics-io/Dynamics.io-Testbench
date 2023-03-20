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

				IComputeBuffer* NewReadBuffer(size_t numElements, size_t stride);

				IComputeBuffer* NewWriteBuffer(size_t numElements, size_t stride);

				IComputeBuffer* NewReadWriteBuffer(size_t numElements, size_t stride);

				void Dispose() {}

				// Non-interface methods:

				ComputeBuffer* NewBuffer(ComputeBuffer::Buffer_Type type, size_t length);

			private:
				ComputeController_OCL() {}

				static IComputeController* New();

				ComputeContext* m_context{ nullptr };
				std::string m_directory;

				std::map<std::string, ComputeProgram_OCL*> m_programs;

				static std::vector<ComputeController_OCL*> m_controllers;
			};

		}
	}
}