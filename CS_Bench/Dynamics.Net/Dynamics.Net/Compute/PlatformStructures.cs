using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace Dynamics.Compute
{
    public class StructHelpers
    {
        public static IntPtr StructToPtr(object obj)
        {
            var ptr = Marshal.AllocHGlobal(Marshal.SizeOf(obj));
            Marshal.StructureToPtr(obj, ptr, false);
            return ptr;
        }
    }

    public enum DeviceType : int
    {
        DEVICE_TYPE_CPU = 0,
        DEVICE_TYPE_INTEGRATED_GPU = 1,
        DEVICE_TYPE_DISCRETE_GPU = 2,
        DEVICE_TYPE_VIRTUAL_GPU = 3,
        DEVICE_TYPE_ACCELERATOR = 4,
        DEVICE_TYPE_OTHER = 5,
    };

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct OpenCL_Device_Info
    {
        IntPtr cl_device;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1000)]
        char[] vendor;
        ushort vendor_size;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1000)]
        char[] name;
        ushort name_size;

        uint clock_frequency;
        uint num_compute_units;
        ulong mem_size;
        uint max_work_size;
        uint group_size;
        byte is_type_default;
        byte is_type_CPU;
        byte is_type_GPU;
        byte is_type_Accelerator;

        public string Name { get { return new string(name).Substring(0, name_size); } }

        public string Vendor { get { return new string(vendor).Substring(0, vendor_size); } }
    };

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct Vulkan_Device_Info
    {
        public IntPtr vk_device;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
        private char[] name;
        public ushort name_size;

        public UInt32 Device_ID;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public Byte[] DeviceUUID;
        public DeviceType Type;

        public string Name { get { return new string(name).Substring(0, name_size); } }

        public string GetTypeName()
        {
            switch (Type)
            {
                case DeviceType.DEVICE_TYPE_CPU:
                    return "CPU";
                case DeviceType.DEVICE_TYPE_INTEGRATED_GPU:
                    return "INTEGRATED_GPU";
                case DeviceType.DEVICE_TYPE_DISCRETE_GPU:
                    return "DISCRETE_GPU";
                case DeviceType.DEVICE_TYPE_VIRTUAL_GPU:
                    return "VIRTUAL_GPU";
                case DeviceType.DEVICE_TYPE_ACCELERATOR:
                    return "ACCELERATOR";
                case DeviceType.DEVICE_TYPE_OTHER:
                    return "OTHER";
                default:
                    return "UNKNOWN";
            }
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct DirectX_Device_Info
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)]
        char[] name;
        ushort name_size;

        int AdapterIndex;
        uint Device_ID;
        DeviceType Type;

        public string Name { get { return new string(name).Substring(0, name_size); } }

        string GetTypeName()
        {
            switch (Type)
            {
                case DeviceType.DEVICE_TYPE_CPU:
                    return "CPU";
                case DeviceType.DEVICE_TYPE_INTEGRATED_GPU:
                    return "INTEGRATED_GPU";
                case DeviceType.DEVICE_TYPE_DISCRETE_GPU:
                    return "DISCRETE_GPU";
                case DeviceType.DEVICE_TYPE_VIRTUAL_GPU:
                    return "VIRTUAL_GPU";
                case DeviceType.DEVICE_TYPE_ACCELERATOR:
                    return "ACCELERATOR";
                case DeviceType.DEVICE_TYPE_OTHER:
                    return "OTHER";
                default:
                    return "UNKNOWN";
            }
        }
    };

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct Platform
    {
        IntPtr platform;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1000)]
        char[] name;
        ushort name_size;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1000)]
        char[] vendor;
        ushort vendor_size;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1000)]
        char[] version;
        ushort version_size;

        public string Name { get { return new string(name).Substring(0, name_size); } }

        public string Vendor { get { return new string(vendor).Substring(0, vendor_size); } }

        public string Version { get { return new string(version).Substring(0, version_size); } }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct Device
    {
        public IntPtr cl_device;
        public IntPtr vk_device;

        public OpenCL_Device_Info OpenCL_Info;
        public Vulkan_Device_Info Vulkan_Info;
        public DirectX_Device_Info DirectX_Info;
    };
}
