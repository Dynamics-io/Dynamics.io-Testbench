using System;
using System.Diagnostics.Tracing;
using Dynamics.Compute;


namespace Dynamics.TesterApp
{
    internal class Program
    {
        const int DATA_SIZE = 32;

        static int Main(string[] args)
        {
            Console.WriteLine("starting...");
            Vulkan_Device_Info[] devices = ComputeInterface.Get_Supported_Devices_Vulkan();

            Console.WriteLine("Devices: {0}", devices.Length);
            foreach (Vulkan_Device_Info info in devices)
            {
                //Vulkan_Device_Info info = device.Vulkan_Info;

                Console.WriteLine("{0} ({1}): {2}", info.Name, info.GetTypeName(), info.Device_ID);
            }

            Vulkan_Device_Info device = devices[0];


            ComputeInterface.ControllerInfo controllerInfo = default;
            controllerInfo.Device = device;
            controllerInfo.SetProgramDir("C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/Vulkan");
            ComputeController controller = ComputeInterface.GetComputeController(ComputeInterface.Compute_SDK.VULKAN, controllerInfo);

            string kernel_name1 = "work";
            string kernel_name2 = "work2";

            ComputeProgram.ProgramInfo p_info = new ComputeProgram.ProgramInfo("test_cl_spv", ComputeProgram.FileType.Binary);
            p_info.AddKernel(kernel_name1);
            p_info.AddKernel(kernel_name2);

            ComputeProgram program = controller.AddProgram(p_info);

            int[] Data = new int[DATA_SIZE];
            for (int i = 0; i < DATA_SIZE; i++)
                Data[i] = i + 1;

            ComputeBuffer in_Buffer = controller.NewReadBuffer(DATA_SIZE, sizeof(int));
            ComputeBuffer out_Buffer = controller.NewWriteBuffer(DATA_SIZE, sizeof(int));

            ComputeProgram.BindIndex ind = default;

            ind.GlobalIndex = 0;
            program.KernelSetBuffer(kernel_name1, out_Buffer, ind);
            program.KernelSetBuffer(kernel_name2, out_Buffer, ind);

            program.FinishBuild();

            in_Buffer.SetData(Data);

            program.RunKernel(kernel_name1, DATA_SIZE, 0, 0);

            out_Buffer.GetData(Data);

            for (int i = 0; i < DATA_SIZE; i++)
            {
                Console.WriteLine("res '{0}': {1}", i, Data[i]);
            }

            ComputeInterface.DisposePlatform(ComputeInterface.Compute_SDK.VULKAN);
            Console.WriteLine("Vulkan Disposed.");

            return 0;
        }
    }
}
