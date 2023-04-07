#pragma once

#include "ForwardDeclarations.h"
#include "PlatformStructures.h"
#include "IComputeController.h"
//#include "CL/cl.h"
#include "Compute_OCL/CL_SDK/cl.h"

namespace Dynamics_IO_Testbench {
	namespace Compute {

		class ComputeInterface {
		public:
			enum Compute_SDK {
				OpenCL,
				VULKAN,
#ifdef WINDOWS_PLATFROM
				DIRECTX,
				CUDA
#endif
			};

			struct ControllerInfo {
			public:
				Platform platform;
				void* device;
				char program_dir[1000];
				int program_dir_size;

				void SetProgramDir(std::string dir) {
					program_dir_size = dir.size();
					memcpy(program_dir, dir.data(), program_dir_size);
					program_dir[program_dir_size] = '\0';
				}
			};

			static IComputeController* GetComputeController(Compute_SDK implementation, ControllerInfo info);

			static void DisposePlatform(Compute_SDK implementation);

            static std::vector<Platform> GetSupportedPlatforms_OpenCL();
            static std::vector<OpenCL_Device_Info> GetSupportedDevices_OpenCL(Platform pltfrm);

			static std::vector<Vulkan_Device_Info> GetSupportedDevices_Vulkan();

			static std::vector<DirectX_Device_Info> GetSupportedDevices_DirectX();

        private:

            static cl_uint num_of_platforms;
            static cl_uint num_of_devices;
            static cl_device_id device_ids[MAX_OCL_DEVICES];

			static IComputeController* GetComputeController_OCL(ControllerInfo info);

			static IComputeController* GetComputeController_CUDA(ControllerInfo info);

			static IComputeController* GetComputeController_Vulkan(ControllerInfo info);

			static IComputeController* GetComputeController_DirectX(ControllerInfo info);

			static bool VK_isDeviceSuitable(VkPhysicalDevice device);
		};

	}
}