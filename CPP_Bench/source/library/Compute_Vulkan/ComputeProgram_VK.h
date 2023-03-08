#pragma once

#include "Compute_Interface/InterfaceIncludes.h"

//#include "ProgramBuilder.h"

namespace Dynamics_IO_Testbench {
	namespace Compute {
		namespace VK {

			class ComputeContext;
			class ComputeEngine;
			class ComputeKernel;
			class ComputeBuffer;
			class ComputeProgram;

			class ComputeBuffer_VK;
			class ComputeController_VK;

			class ComputeProgram_VK : public IComputeProgram {
				friend class ComputeController_VK;

			public:
				// Interface methods:

				void Init(std::string name);

				int FinishBuild();

				int GetKernelID(std::string name);

				int KernelSetBuffer(std::string k_name, IComputeBuffer* buffer, BindIndex arg);

				int RunKernel(std::string k_name, int size_x, int size_y, int size_z);

				int RunKernel(int kernel_id, int size_x, int size_y, int size_z);

				void* GetKernelFunction(int kernel_id);

				ProgramBuildState GetState() { return m_cur_state; }

				int GetBuildResultCode() { return m_vk_build_res; }

				std::string GetBuildErrorMessage() { return m_build_error; }

				std::string GetProgramName() { return m_program_name; }

				void Dispose();


				// Non-interface methods:

				ProgramBuildState BuildProgramFromBinary(std::string file_path, std::vector<std::string> kernels);

				ProgramBuildState BuildProgramFromBinary(const void* binary, size_t length, std::vector<std::string> kernels);


				ComputeProgram_VK(){}
				~ComputeProgram_VK();

			private:

				struct kernelEnt {
					std::string name;
					ComputeKernel* kernel;
					//int args;
				};

				ComputeProgram_VK(ComputeContext* context);

				int BindKernel(ComputeBuffer* buffer, ComputeKernel* kernel, int arg);

				int InitKernelEntries();

				ComputeContext* m_context{ nullptr };

				std::string m_program_name{""};
				ProgramBuildState m_cur_state{ProgramBuildState::None};
				int m_vk_build_res { 0 };
				std::string m_build_error{""};

				size_t m_num_kernels{ 0 };
				kernelEnt* m_kernel_entries{ nullptr };
				std::map<std::string, int> m_kernel_name_to_id;

				// program building
				std::vector<std::string> m_kernels;
				ComputeProgram* m_program{ nullptr };

				bool mDestroyed{ false };
			};
		}
	}
}