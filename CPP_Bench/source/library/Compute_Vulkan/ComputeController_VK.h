#pragma once

#include "Compute_Interface/InterfaceIncludes.h"
#include "Compute_Interface/PlatformStructures.h"

namespace Dynamics_IO_Testbench {
	namespace Compute {
		namespace VK {

			class ComputeContext;
			class ComputeEngine;
			class ComputeKernel;
			class ComputeBuffer;
			class ComputeProgram;

			class ComputeProgram_VK;

			class ComputeController_VK : public IComputeController {
				friend class ComputeInterface;

			public:
				void Init(Platform platform, Device device, std::string program_dir);

				IComputeProgram* AddProgram(IComputeProgram::ProgramInfo info);

				IComputeProgram* GetProgram(std::string name);

				IComputeBuffer* NewReadBuffer(size_t length);

				IComputeBuffer* NewWriteBuffer(size_t length);

				IComputeBuffer* NewReadWriteBuffer(size_t length);

				void Dispose();

				static void Close();

				// Non-interface methods:

				ComputeBuffer* NewBuffer(uint32_t type, size_t length);

				~ComputeController_VK();
				ComputeController_VK() {}
			private:
				

				

				static IComputeController* New();

				ComputeContext* m_context{ nullptr };
				std::string m_directory{ "" };

				std::map<std::string, ComputeProgram_VK*> m_programs;

				bool mDestroyed{ false };

				static std::vector<ComputeController_VK*> m_controllers;
			};


		}
	}
}