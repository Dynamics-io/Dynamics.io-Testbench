#pragma once

#include "Compute_Interface/InterfaceIncludes_private.h"
#include "Compute_Interface/PlatformStructures_private.h"

namespace DynamicCompute {
	namespace Compute {
		namespace DX {

			class ComputeContext;
			class ComputeEngine;
			class ComputeKernel;
			class ComputeBuffer;
			class ComputeProgram;

			class ComputeProgram_DX;

			class ComputeController_DX : public IComputeController_private {
				friend class ComputeInterface_private;

			public:
				void Init(Platform platform, void* device, std::string program_dir);

				IComputeProgram_private* AddProgram(IComputeProgram_private::ProgramInfo info);

				IComputeProgram_private* GetProgram(std::string name);

				IComputeBuffer_private* NewReadBuffer(size_t numElements, size_t stride);

				IComputeBuffer_private* NewWriteBuffer(size_t numElements, size_t stride);

				IComputeBuffer_private* NewReadWriteBuffer(size_t numElements, size_t stride);

				void Dispose();

				static void Close();


				// Non-interface methods:

				ComputeBuffer* NewBuffer(uint32_t type, size_t numElements, size_t stride);

				~ComputeController_DX();
				ComputeController_DX() {}

			private:

				static IComputeController_private* New();
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