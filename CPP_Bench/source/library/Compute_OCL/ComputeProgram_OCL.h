#pragma once

#include "OCL_forwardDeclarations.h"
#include "Compute_Interface/InterfaceIncludes.h"
#include "ComputeEngine.h"
#include "ProgramBuilder.h"
#include "ShaderDepository.h"

namespace Dynamics_IO_Testbench {
	namespace Compute {
		namespace OCL {

			class ComputeProgram_OCL : public IComputeProgram {
				friend class ComputeController_OCL;

			public:

				// Interface methods:

				void Init(std::string name);

				int FinishBuild() { return 0; }

				int GetKernelID(std::string name);

				int KernelAddBuffer(std::string k_name, IComputeBuffer* buffer);

				int KernelSetBuffer(std::string k_name, IComputeBuffer* buffer, BindIndex arg);

				int RunKernel(std::string k_name, int size_x, int size_y, int size_z);

				int RunKernel(int kernel_id, int size_x, int size_y, int size_z);

				void* GetKernelFunction(int kernel_id);

				ProgramBuildState GetState() { return m_cur_state; }

				int GetBuildResultCode() { return m_cl_build_res; }

				std::string GetBuildErrorMessage() { return m_build_error; }

				std::string GetProgramName() { return m_program_name; }

				void Dispose() {}


				// Non-interface methods:

				ProgramBuilder* GetProgramBuilder() { return m_builder; }

				ProgramBuildState ConstructSourceProgram();

				ProgramBuildState BuildProgramFromBinary(std::string file_path, std::vector<std::string> kernels);

				ProgramBuildState BuildProgramFromBinary(const void* binary, size_t length, std::vector<std::string> kernels);

				ProgramBuildState BuildProgramFromSource(std::string content, std::vector<std::string> kernels);

				ProgramBuildState BuildProgramFromSourceFile(std::string file_path, std::vector<std::string> kernels);

				ProgramBuildState BuildProgramFromInternalDepo();

			private:

				struct kernelEnt {
					std::string name;
					ComputeKernel* kernel;
					int args;
				};

				ComputeProgram_OCL(ComputeContext* context);

				int BindKernel(ComputeBuffer* buffer, ComputeKernel* kernel, int arg);

				int InitKernelEntries();

				std::string m_program_name;

				ComputeContext* m_context{ nullptr };
				ProgramBuilder* m_builder{ nullptr };
				ProgramBuildState m_cur_state{ ProgramBuildState::None };

				std::string m_build_error = "";
				int m_cl_build_res = { 0 };

				ShaderDepository default_shaders;

				size_t m_num_kernels;
				kernelEnt* m_kernel_entries{ nullptr };
				std::map<std::string, int> m_kernel_name_to_id;
			};
		}
	}
}