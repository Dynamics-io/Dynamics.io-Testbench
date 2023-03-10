#pragma once

#include "CL/cl.h"
#include "vulkan/vulkan.h"

namespace Dynamics_IO_Testbench {
    namespace Compute {

        enum class DeviceType {
            DEVICE_TYPE_CPU = 0,
            DEVICE_TYPE_INTEGRATED_GPU = 1,
            DEVICE_TYPE_DISCRETE_GPU = 2,
            DEVICE_TYPE_VIRTUAL_GPU = 3,
            DEVICE_TYPE_ACCELERATOR = 4,
            DEVICE_TYPE_OTHER = 5,
        };

        struct OpenCL_Device_Info {
            char vendor[1000];
            unsigned short vendor_size;

            char name[1000];
            unsigned short name_size;

            unsigned int clock_frequency;
            unsigned int num_compute_units;
            unsigned long mem_size;
            unsigned int max_work_size;
            unsigned int group_size;
            bool is_type_default;
            bool is_type_CPU;
            bool is_type_GPU;
            bool is_type_Accelerator;
        };

        struct Vulkan_Device_Info {
            std::string Name;
            uint32_t Device_ID;
            uint8_t DeviceUUID[16];
            DeviceType Type;

            std::string GetTypeName() {
                switch (Type) {
                case DeviceType::DEVICE_TYPE_CPU:
                    return "CPU";
                case DeviceType::DEVICE_TYPE_INTEGRATED_GPU:
                    return "INTEGRATED_GPU";
                case DeviceType::DEVICE_TYPE_DISCRETE_GPU:
                    return "DISCRETE_GPU";
                case DeviceType::DEVICE_TYPE_VIRTUAL_GPU:
                    return "VIRTUAL_GPU";
                case DeviceType::DEVICE_TYPE_ACCELERATOR:
                    return "ACCELERATOR";
                case DeviceType::DEVICE_TYPE_OTHER:
                    return "OTHER";
                default:
                    return "UNKNOWN";
                }
            }

            std::string GetUUID() {
                std::string res = uint8_to_hex_string(DeviceUUID, 16);
                return res;
            }

        private:
            // TODO: Move this to some Utilities class.
            std::string uint8_to_hex_string(const uint8_t* v, const size_t s) {
                std::stringstream ss;

                ss << std::hex << std::setfill('0');

                for (int i = 0; i < s; i++) {
                    ss << std::hex << std::setw(2) << static_cast<int>(v[i]);
                }

                return ss.str();
            }
        };

        struct Platform {
            void* platform;

            char name[1000];
            unsigned short name_size;

            char vendor[1000];
            unsigned short vendor_size;

            char version[1000];
            unsigned short version_size;
        };

        struct Device {
            void* cl_device;
            void* vk_device;

            OpenCL_Device_Info OpenCL_Info;
            Vulkan_Device_Info Vulkan_Info;
        };

    }
}