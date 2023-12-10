#pragma once

#include "Compute_Interface/InterfaceIncludes_private.h"

namespace DynamicCompute {
	namespace Compute {
		namespace DX {

			class ComputeContext;
			class ComputeEngine;
			class ComputeKernel;
			class ComputeBuffer;
			class ComputeProgram;

			class ComputeBuffer_DX;
			class ComputeController_DX;

			class ComputeProgram_DX : public IComputeProgram_private {
				friend class ComputeController_DX;

			public:
				// Interface methods:

				void Init(std::string name);

				// Only loading pre-compiled binaries is supported at this time. when
				// support for building for source is added, this will be implemented.
				void AddIncludeDirectory(std::string directory) {}

				void AddDefine(std::string name, std::string value) {}

				int Build();

				int FinishBuild();

				int GetKernelID(std::string name);

				void KernelSetWorkGroupSize(std::string k_name, int size);

				int KernelSetBuffer(std::string k_name, IComputeBuffer_private* buffer, BindIndex arg);

				int RunKernel(std::string k_name, int size_x, int size_y, int size_z);

				int RunKernel(int kernel_id, int size_x, int size_y, int size_z);

				void* GetKernelFunction(int kernel_id);

				ProgramBuildState GetState() { return m_cur_state; }

				int GetBuildResultCode() { return m_dx_build_res; }

				std::string GetBuildErrorMessage() { return m_build_error; }

				std::string GetProgramName() { return m_program_name; }

				void Dispose();


				// Non-interface methods:

				void SetKernelNames(std::vector<std::string> kernel_names) {
					m_kernel_names = kernel_names;
				}

				// As the name suggests, it uses the kernel name vector to find the individual 
				// binary files, each named with their entry point.
				ProgramBuildState BuildProgramFromDirectory(std::vector<std::string> kernels);

			private:

				struct kernelEnt {
					std::string name;
					ComputeKernel* kernel;
					//int args;
				};

				ComputeProgram_DX(ComputeContext* context);

				int BindKernel(ComputeBuffer* buffer, ComputeKernel* kernel, int arg);

				int InitKernelEntries();

				ComputeContext* m_context{ nullptr };

				std::string m_program_name{ "" };
				std::vector<std::string> m_kernel_names; // set before build

				ProgramBuildState m_cur_state{ ProgramBuildState::None };
				int m_dx_build_res{ 0 };
				std::string m_build_error{ "" };

				size_t m_num_kernels{ 0 };
				kernelEnt* m_kernel_entries{ nullptr };
				std::map<std::string, int> m_kernel_name_to_id;

				// program building
				std::vector<std::string> m_kernels; // set after build
				ComputeProgram* m_program{ nullptr };

				bool mDestroyed{ false };
			};

		}
	}
}