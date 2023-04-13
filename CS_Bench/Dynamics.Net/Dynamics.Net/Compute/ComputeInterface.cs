using System.Runtime.InteropServices;
using System.Xml.Linq;

namespace Dynamics.Compute
{
    public static class ComputeInterface
    {
        [DllImport(Settings.DLL)]
        private static extern IntPtr ComputeInterface_GetComputeController(Compute_SDK implementation, ControllerInfo info);

        [DllImport(Settings.DLL)]
        private static extern void ComputeInterface_DisposePlatform(Compute_SDK implementation);

        [DllImport(Settings.DLL)]
        private static extern int ComputeInterface_GetSupportedDevicesSize_Vulkan();

        [DllImport(Settings.DLL)]
        private static extern int ComputeInterface_GetSupportedDevices_Vulkan([In, Out] Vulkan_Device_Info[] out_devices);

        [DllImport(Settings.DLL)]
        private static extern int ComputeInterface_GetSupportedPlatformsSize_OpenCL();
        [DllImport(Settings.DLL)]
        private static extern int ComputeInterface_GetSupportedPlatforms_OpenCL([In, Out] Platform[] out_platforms);

        [DllImport(Settings.DLL)]
        private static extern void ComputeInterface_GetSupportedDevices_SetPlatform_OpenCL(Platform platform);
        [DllImport(Settings.DLL)]
        private static extern int ComputeInterface_GetSupportedDevicesSize_OpenCL();
        [DllImport(Settings.DLL)]
        private static extern int ComputeInterface_GetSupportedDevices_OpenCL([In, Out] OpenCL_Device_Info[] out_devices);

        [DllImport(Settings.DLL)]
        private static extern int ComputeInterface_GetSupportedDevicesSize_DirectX();
        [DllImport(Settings.DLL)]
        private static extern int ComputeInterface_GetSupportedDevices_DirectX([In, Out] DirectX_Device_Info[] out_devices);

        public enum Compute_SDK
        {
            OpenCL,
            VULKAN,
            DIRECTX,
            CUDA
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
        public struct ControllerInfo
        {
			public Platform platform;
            public IntPtr device;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1000)]
            public char[] program_dir;
            public int program_dir_size;

            public object Device
            {
                set
                {
                    device = StructHelpers.StructToPtr(value);
                }
            }

            public void SetProgramDir(string dir)
            {
                program_dir_size = dir.Length;
                program_dir = new char[1000];
                dir.ToCharArray().CopyTo(program_dir, 0);
                program_dir[dir.Length] = '\0';

                //program_dir = dir.ToCharArray();// new char[1000];
                //dir.ToCharArray().CopyTo(program_dir, 0);
            }

            public string ProgramDir { get { return new string(program_dir); } }
        };

        public static ComputeController GetComputeController(Compute_SDK implementation, ControllerInfo info)
        {
            ComputeController res = new ComputeController(ComputeInterface_GetComputeController(implementation, info));
            Marshal.FreeHGlobal(info.device);
            return res;
        }

        public static void DisposePlatform(Compute_SDK implementation)
        {
            ComputeInterface_DisposePlatform(implementation);
        }

        public static Vulkan_Device_Info[] Get_Supported_Devices_Vulkan()
        {
            int num = ComputeInterface_GetSupportedDevicesSize_Vulkan();

            Vulkan_Device_Info[] devices = new Vulkan_Device_Info[num];
            ComputeInterface_GetSupportedDevices_Vulkan(devices);

            return devices;
        }

        public static Platform[] Get_Supported_Platforms_OpenCL()
        {
            int num = ComputeInterface_GetSupportedPlatformsSize_OpenCL();

            Platform[] platforms = new Platform[num];
            ComputeInterface_GetSupportedPlatforms_OpenCL(platforms);

            return platforms;
        }

        public static OpenCL_Device_Info[] Get_Supported_Devices_OpenCL(Platform platform)
        {
            ComputeInterface_GetSupportedDevices_SetPlatform_OpenCL(platform);
            int num = ComputeInterface_GetSupportedDevicesSize_OpenCL();

            OpenCL_Device_Info[] devices = new OpenCL_Device_Info[num];
            ComputeInterface_GetSupportedDevices_OpenCL(devices);

            return devices;
        }

        public static DirectX_Device_Info[] Get_Supported_Devices_DirectX()
        {
            int num = ComputeInterface_GetSupportedDevicesSize_DirectX();

            DirectX_Device_Info[] devices = new DirectX_Device_Info[num];
            ComputeInterface_GetSupportedDevices_DirectX(devices);

            return devices;
        }
    }
}