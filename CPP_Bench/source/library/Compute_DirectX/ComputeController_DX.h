#pragma once

#include "Compute_Interface/InterfaceIncludes.h"
#include "Compute_Interface/PlatformStructures.h"

namespace Dynamics_IO_Testbench {
	namespace Compute {
		namespace DX {

			class ComputeContext;
			class ComputeEngine;
			class ComputeKernel;
			class ComputeBuffer;
			class ComputeProgram;

			class ComputeProgram_DX;

			class ComputeController_DX : public IComputeController {
				friend class ComputeInterface;

			public:
				void Init(Platform platform, Device device, std::string program_dir);

				IComputeProgram* AddProgram(IComputeProgram::ProgramInfo info);

				IComputeProgram* GetProgram(std::string name);

				IComputeBuffer* NewReadBuffer(size_t numElements, size_t stride);

				IComputeBuffer* NewWriteBuffer(size_t numElements, size_t stride);

				IComputeBuffer* NewReadWriteBuffer(size_t numElements, size_t stride);

				void Dispose();

				static void Close();


				// Non-interface methods:

				ComputeBuffer* NewBuffer(uint32_t type, size_t numElements, size_t stride);

				~ComputeController_DX();
				ComputeController_DX() {}

			private:

				static IComputeController* New();
				static void DisposePlatform();

				ComputeContext* m_context{ nullptr };
				std::string m_directory{ "" };

				std::map<std::string, ComputeProgram_DX*> m_programs;

				bool mDestroyed{ false };

				static std::vector<ComputeController_DX*> m_controllers;
			};

		}
	}
}