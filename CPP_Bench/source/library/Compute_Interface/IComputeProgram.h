#pragma once

#include "ForwardDeclarations.h"
#include "IComputeBuffer.h"

namespace Dynamics_IO_Testbench {
	namespace Compute {

		class IComputeProgram {
		public:

			enum class ProgramBuildState {
				None,
				Inited,
				Constructed,
				Built,
				BuildError
			};

			struct ProgramInfo {
			public:

				ProgramInfo(std::string name)
					: m_name(name) {}

				void AddKernel(std::string name) { m_kernels.push_back(name); }

				std::string Name() { return m_name; }

				std::vector<std::string> Kernels() { return m_kernels; }

			private:
				std::string m_name;
				std::vector<std::string> m_kernels;
			};

			virtual void Init(std::string name) = 0;

			virtual int GetKernelID(std::string name) = 0;

			virtual int KernelAddBuffer(std::string k_name, IComputeBuffer* buffer) = 0;

			virtual int KernelSetBuffer(std::string k_name, IComputeBuffer* buffer, int arg) = 0;

			virtual int RunKernel(std::string k_name, int size_x, int size_y, int size_z) = 0;

			virtual int RunKernel(int kernel_id, int size_x, int size_y, int size_z) = 0;

			virtual void* GetKernelFunction(int kernel_id) = 0;

			virtual ProgramBuildState GetState() = 0;

			virtual int GetBuildResultCode() = 0;

			virtual std::string GetBuildErrorMessage() = 0;

			virtual std::string GetProgramName() = 0;
		};
	}
}