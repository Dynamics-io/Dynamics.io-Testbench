using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices.WindowsRuntime;
using Dynamics.Compute;
using ComputeBuffer = Dynamics.Compute.ComputeBuffer;
using UnityEngine;

public class Tree_Tests : MonoBehaviour
{
    public int DATA_SIZE = 32;
    public TestTree tree;

    public int BoundsToProcess;
    public Transform[] Cubes;

    // Start is called before the first frame update
    void Start()
    {
        //DoComputeTest();

        //DoTreeInsertTest1();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void Init(int num)
    {
        tree = new TestTree(num);
    }

    BoundingBox GetCubeBounds(Transform obj)
    {
        Vector3 pos = obj.position;
        Vector3 scale = obj.lossyScale;

        Vector3 min = new Vector3(
            (-0.5f * scale.x) + pos.x,
            (-0.5f * scale.y) + pos.y,
            (-0.5f * scale.z) + pos.z
        );
        Vector3 max = new Vector3(
            (0.5f * scale.x) + pos.x,
            (0.5f * scale.y) + pos.y,
            (0.5f * scale.z) + pos.z
        );
        return new BoundingBox(min, max);
    }

    public void DoTreeInsertTest1()
    {
        tree = new TestTree(10);

        for (int i = 0; i < Mathf.Min(BoundsToProcess, Cubes.Length); i++)
        {
            BoundingBox bounds = GetCubeBounds(Cubes[i]);
            int leafInd = tree.Add(bounds);
            Debug.LogFormat("New Leaf: {0}", leafInd);
        }

    }

    public void DoTreeInsertTest2(BoundingBox[] bounds)
    {
        for (int i = 0; i < bounds.Length; i++)
        {
            int leafInd = tree.Add(bounds[i]);
            //Debug.LogFormat("New Leaf: {0}", leafInd);
        }

    }

    void DoComputeTest()
    {
        Debug.LogFormat("starting...");
        Vulkan_Device_Info[] devices = ComputeInterface.Get_Supported_Devices_Vulkan();

        Debug.LogFormat("Devices: {0}", devices.Length);
        foreach (Vulkan_Device_Info info in devices)
        {
            //Vulkan_Device_Info info = device.Vulkan_Info;

            Debug.LogFormat("{0} ({1}): {2}", info.Name, info.GetTypeName(), info.Device_ID);
        }
        
        Vulkan_Device_Info device = devices[0];


        ComputeInterface.ControllerInfo controllerInfo = default;
        controllerInfo.Device = device;
        controllerInfo.SetProgramDir("C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/Vulkan");
        ComputeController controller = ComputeInterface.GetComputeController(ComputeInterface.Compute_SDK.VULKAN, controllerInfo);

        Debug.Log("Created controller");

        string kernel_name1 = "work";
        string kernel_name2 = "work2";

        ComputeProgram.ProgramInfo p_info = new ComputeProgram.ProgramInfo("test_cl_spv", ComputeProgram.FileType.Binary);
        p_info.AddKernel(kernel_name1);
        p_info.AddKernel(kernel_name2);

        Debug.Log("Create Program Info.");

        ComputeProgram program = controller.AddProgram(p_info);
        Debug.LogFormat("Create Program '{0}'.", "test_cl_spv");


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
            Debug.LogFormat("res '{0}': {1}", i, Data[i]);
        }

        ComputeInterface.DisposePlatform(ComputeInterface.Compute_SDK.VULKAN);
        Debug.LogFormat("Vulkan Disposed.");
    }
}
