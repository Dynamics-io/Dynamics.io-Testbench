#pragma once

#include "ForwardDeclarations.h"
#include "PlatformStructures.h"
#include "IComputeController.h"
#include "CL/cl.h"

#define MAX_OCL_DEVICES  10

namespace Dynamics_IO_Testbench {
	namespace Compute {

		class ComputeInterface {
		public:
			enum Compute_SDK {
				OpenCL,
				CUDA,
				VULKAN,
				DIRECTX
			};

			struct ControllerInfo {
			public:
				Platform platform;
				Device device;
				std::string program_dir;
			};

			static IComputeController* GetComputeController(Compute_SDK implementation, ControllerInfo info);

            static std::vector<Platform> GetSupportedPlatforms_OpenCL();
            static std::vector<Device> GetSupportedDevices_OpenCL(Platform pltfrm);

			static std::vector<Device> GetSupportedDevices_Vulkan();
        private:

            static cl_uint num_of_platforms;
            static cl_uint num_of_devices;
            static cl_device_id device_ids[MAX_OCL_DEVICES];

			static IComputeController* GetComputeController_OCL(ControllerInfo info);

			static IComputeController* GetComputeController_CUDA(ControllerInfo info);

			static IComputeController* GetComputeController_Vulkan(ControllerInfo info);

			static IComputeController* GetComputeController_DirectX(ControllerInfo info);

			static bool isDeviceSuitable(VkPhysicalDevice device);
		};

	}
}