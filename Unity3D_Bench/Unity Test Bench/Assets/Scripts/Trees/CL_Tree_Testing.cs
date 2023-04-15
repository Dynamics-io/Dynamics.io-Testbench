using System.Collections;
using System.Collections.Generic;
using Dynamics.Compute;
using ComputeBuffer = Dynamics.Compute.ComputeBuffer;
using UnityEngine;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System;
using System.Security.Cryptography;

public class CL_Tree_Testing : MonoBehaviour
{
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public struct tree_counts
    {
        public int NodeCount;
        public int LeafCount;

        public int padding1;
        public int padding2;

        public static int Size()
        {
            return Marshal.SizeOf(typeof(tree_counts));
        }

        public override string ToString()
        {
            return String.Format("tree_counts:(NodeCount: {0}, LeafCount: {1})", NodeCount, LeafCount);
        }
    }


    /// <summary>
    /// Buffer of nodes in the tree.
    /// </summary>
    public Node[] Nodes;

    /// <summary>
    /// Buffer of metanodes in the tree. Metanodes contain metadata that aren't read during most query operations but are useful for bookkeeping.
    /// </summary>
    public Metanode[] Metanodes;

    /// <summary>
    /// Buffer of leaves in the tree.
    /// </summary>
    public Leaf[] Leaves;


    public int[] Results;

    /// <summary>
    /// Number of nodes in the tree.
    /// </summary>
    public int NodeCount;

    /// <summary>
    /// Number of leaves in the tree.
    /// </summary>
    public int LeafCount;

    public int BoundsToProcess;
    public Transform[] Cubes;

    ComputeController controller;
    ComputeProgram program;

    ComputeBuffer counts_buffer;
    ComputeBuffer metanodes_buffer;

    ComputeBuffer NodeChild_Min_buffer;
    ComputeBuffer NodeChild_Max_buffer;
    ComputeBuffer NodeChild_Index_buffer;
    ComputeBuffer NodeChild_LeafCount_buffer;

    ComputeBuffer leaves_buffer;

    ComputeBuffer boundingBox_add_buffer;
    ComputeBuffer numBoundsAdds;

    ComputeBuffer result_buffer;


    ComputeBuffer p_NodeChild_Min_buffer;
    ComputeBuffer p_NodeChild_Max_buffer;
    ComputeBuffer p_NodeChild_Index_buffer;
    ComputeBuffer p_NodeChild_LeafCount_buffer;

    ComputeBuffer p_leaves_buffer;


    string kernel_tree_init = "Init";
    string kernel_tree_add = "Add";

    // Start is called before the first frame update
    void Start()
    {
        //DoComputeTest();
        //return;

        //Init(Cubes.Length);

        //AddBoxes();

        //ExtractInfo(Cubes.Length);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnDestroy()
    {
        ComputeInterface.DisposePlatform(ComputeInterface.Compute_SDK.VULKAN);
        ComputeInterface.DisposePlatform(ComputeInterface.Compute_SDK.OpenCL);
        Debug.Log("Compute Disposed.");
    }

    public void Init(int num)
    {
        Init_CL();
        Init_Buffers(num);
        InitTree();
    }

    public void Init_CL()
    {
        Debug.LogFormat("starting...");

        object[] device_info = GetDevice_Vulkan();

        ComputeInterface.ControllerInfo controllerInfo = default;
        controllerInfo.platform = device_info[0] == null ? default : (Platform)device_info[0];
        controllerInfo.Device = device_info[1];
        controllerInfo.SetProgramDir("C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/Vulkan/Tree");
        controller = ComputeInterface.GetComputeController(ComputeInterface.Compute_SDK.VULKAN, controllerInfo);

        ComputeProgram.ProgramInfo p_info = new ComputeProgram.ProgramInfo("Tree", ComputeProgram.FileType.Binary);
        p_info.AddKernel(kernel_tree_init);
        p_info.AddKernel(kernel_tree_add);


        program = controller.AddProgram(p_info);

        if (program.GetState() == ComputeProgram.ProgramBuildState.BuildError)
        {
            if (program.GetBuildResultCode() != 0)
            {
                Debug.LogFormat("Build failed: {0}", program.GetBuildResultCode());
                return;
            }

            //printf("%s\n\n -------------- \n\n", controller->GetProgramBuilder()->GetFullSource().c_str());
            Debug.LogFormat("BUILD ERROR");

            //return -1;
            return;
        }

        Debug.Log("Tree program initialized.");
    }

    public int GetLocalMemorySizeEstimate(int num)
    {
        int sum = 0;

        sum += num * 2 * sizeof(float) * 4; // p_NodeChild_Min_buffer
        sum += num * 2 * sizeof(float) * 4; // p_NodeChild_Max_buffer
        sum += num * 2 * sizeof(int); // p_NodeChild_Index_buffer
        sum += num * 2 * sizeof(int); // p_NodeChild_LeafCount_buffer
        sum += num * sizeof(uint); // p_leaves_buffer

        return sum;
    }

    public void Init_Buffers(int num)
    {
        counts_buffer = controller.NewReadWriteBuffer(1, tree_counts.Size());
        metanodes_buffer = controller.NewReadWriteBuffer(num, Metanode.Size());
        leaves_buffer = controller.NewReadWriteBuffer(Mathf.Max(num, 4), sizeof(uint));

        NodeChild_Min_buffer = controller.NewReadWriteBuffer(num * 2, sizeof(float) * 4);
        NodeChild_Max_buffer = controller.NewReadWriteBuffer(num * 2, sizeof(float) * 4);
        NodeChild_Index_buffer = controller.NewReadWriteBuffer(Mathf.Max(num * 2, 4), sizeof(int));
        NodeChild_LeafCount_buffer = controller.NewReadWriteBuffer(Mathf.Max(num * 2, 4), sizeof(int));

        boundingBox_add_buffer = controller.NewReadWriteBuffer(num, BoundingBox_CL.Size());
        numBoundsAdds = controller.NewReadWriteBuffer(4, sizeof(int));

        result_buffer = controller.NewReadWriteBuffer(Mathf.Max(num, 4), sizeof(int));


        // Private buffers.
        p_NodeChild_Min_buffer = controller.NewReadWriteBuffer(num * 2, sizeof(float) * 4);
        p_NodeChild_Max_buffer = controller.NewReadWriteBuffer(num * 2, sizeof(float) * 4);
        p_NodeChild_Index_buffer = controller.NewReadWriteBuffer(Mathf.Max(num * 2), sizeof(int));
        p_NodeChild_LeafCount_buffer = controller.NewReadWriteBuffer(Mathf.Max(num * 2, 4), sizeof(int));
        p_leaves_buffer = controller.NewReadWriteBuffer(Mathf.Max(num, 4), sizeof(uint));


        ComputeProgram.BindIndex ind = default;

        // Bind Init kernel buffers

        ind.ParameterIndex = 0;
        ind.GlobalIndex = 0;
        program.KernelSetBuffer(kernel_tree_init, counts_buffer, ind);
        ind.ParameterIndex = 1;
        ind.GlobalIndex = 1;
        program.KernelSetBuffer(kernel_tree_init, metanodes_buffer, ind);


        // Bind Add kernel buffers

        ind.ParameterIndex = 0;
        ind.GlobalIndex = 0;
        program.KernelSetBuffer(kernel_tree_add, counts_buffer, ind);
        ind.ParameterIndex = 8;
        ind.GlobalIndex = 1;
        program.KernelSetBuffer(kernel_tree_add, metanodes_buffer, ind);

        ind.ParameterIndex = 3;
        ind.GlobalIndex = 3;
        program.KernelSetBuffer(kernel_tree_add, NodeChild_Min_buffer, ind);
        ind.ParameterIndex = 4;
        ind.GlobalIndex = 4;
        program.KernelSetBuffer(kernel_tree_add, NodeChild_Max_buffer, ind);
        ind.ParameterIndex = 5;
        ind.GlobalIndex = 5;
        program.KernelSetBuffer(kernel_tree_add, NodeChild_Index_buffer, ind);
        ind.ParameterIndex = 6;
        ind.GlobalIndex = 6;
        program.KernelSetBuffer(kernel_tree_add, NodeChild_LeafCount_buffer, ind);

        ind.ParameterIndex = 7;
        ind.GlobalIndex = 2;
        program.KernelSetBuffer(kernel_tree_add, leaves_buffer, ind);
        
        ind.ParameterIndex = 1;
        ind.GlobalIndex = 8;
        program.KernelSetBuffer(kernel_tree_add, boundingBox_add_buffer, ind);
        ind.ParameterIndex = 2;
        ind.GlobalIndex = 7;
        program.KernelSetBuffer(kernel_tree_add, numBoundsAdds, ind);
        ind.ParameterIndex = 9;
        ind.GlobalIndex = 9;
        program.KernelSetBuffer(kernel_tree_add, result_buffer, ind);



        /*ind.ParameterIndex = 10;
        program.KernelSetBuffer(kernel_tree_add, p_NodeChild_Min_buffer, ind);
        ind.ParameterIndex = 11;
        program.KernelSetBuffer(kernel_tree_add, p_NodeChild_Max_buffer, ind);
        ind.ParameterIndex = 12;
        program.KernelSetBuffer(kernel_tree_add, p_NodeChild_Index_buffer, ind);
        ind.ParameterIndex = 13;
        program.KernelSetBuffer(kernel_tree_add, p_NodeChild_LeafCount_buffer, ind);
        ind.ParameterIndex = 14;
        program.KernelSetBuffer(kernel_tree_add, p_leaves_buffer, ind);*/

        int res = program.FinishBuild();

        Debug.LogFormat("Compute Buffers Initialized: {0}", res);
    }

    public void InitTree()
    {
        program.RunKernel(kernel_tree_init, 1, 0, 0);
    }

    public void AddBoxes()
    {
        BoundingBox_CL[] boxAdds = new BoundingBox_CL[Mathf.Min(BoundsToProcess, Cubes.Length)];
        for (int i = 0; i < Mathf.Min(BoundsToProcess, Cubes.Length); i++)
        {
            BoundingBox_CL bounds = GetCubeBounds(Cubes[i]);
            boxAdds[i] = bounds;
            Debug.LogFormat("Added: {0}", bounds);
        }
        int[] numAdds = { boxAdds.Length };

        boundingBox_add_buffer.SetData(boxAdds);
        numBoundsAdds.SetData(numAdds);

        Debug.Log("Running Add()");
        program.RunKernel(kernel_tree_add, 1, 0, 0);

    }

    public double AddBoxes(BoundingBox_CL[] boxAdds)
    {
        int[] numAdds = { boxAdds.Length, 0, 0, 0 };

        boundingBox_add_buffer.SetData(boxAdds);
        numBoundsAdds.SetData(numAdds);

        System.Diagnostics.Stopwatch timer = new System.Diagnostics.Stopwatch();
        //Debug.Log("Running Add()");
        timer.Start();
        program.RunKernel(kernel_tree_add, 1, 0, 0);
        timer.Stop();

        Results = new int[boxAdds.Length];
        result_buffer.GetData(Results);

        return timer.Elapsed.TotalSeconds;
    }

    public void ExtractInfo(int num)
    {
        tree_counts[] counts_tmp = new tree_counts[1];
        counts_buffer.GetData(counts_tmp);
        NodeCount = counts_tmp[0].NodeCount;
        LeafCount = counts_tmp[0].LeafCount;

        Nodes = new Node[num];

        //NodeChild[] node_children_tmp = new NodeChild[NodeCount * 2];
        //Debug.LogFormat("Node count: {0}", NodeCount);
        Vector4[] child_min_tmp = new Vector4[num * 2];
        Vector4[] child_max_tmp = new Vector4[num * 2];
        int[] cild_index_tmp = new int[num * 2];
        int[] child_leafcount_tmp = new int[num * 2];
        //Debug.LogFormat("child_min_tmp count 1: {0}", child_min_tmp.Length);

        NodeChild_Min_buffer.GetData(child_min_tmp);
        NodeChild_Max_buffer.GetData(child_max_tmp);
        NodeChild_Index_buffer.GetData(cild_index_tmp);
        NodeChild_LeafCount_buffer.GetData(child_leafcount_tmp);
        //Debug.LogFormat("child_min_tmp count 2: {0}", child_min_tmp.Length);


        for (int i = 0; i < NodeCount; i++)
        {
            NodeChild A = default;
            NodeChild B = default;

            int a_index = i * 2;
            int b_index = i * 2 + 1;

            //Debug.LogFormat("A Index: {0}", a_index);
            //Debug.LogFormat("B Index: {0}", b_index);

            A.Min = child_min_tmp[a_index];
            A.Max = child_max_tmp[a_index];
            A.Index = cild_index_tmp[a_index];
            A.LeafCount = child_leafcount_tmp[a_index];

            B.Min = child_min_tmp[b_index];
            B.Max = child_max_tmp[b_index];
            B.Index = cild_index_tmp[b_index];
            B.LeafCount = child_leafcount_tmp[b_index];

            Nodes[i].A = A;
            Nodes[i].B = B;
        }

        Metanodes = new Metanode[num];
        metanodes_buffer.GetData(Metanodes);

        uint[] leaves_tmp = new uint[num];
        Leaves = new Leaf[num];
        leaves_buffer.GetData(leaves_tmp);

        for (int i = 0; i < num; i++)
        {
            Leaves[i] = new Leaf(leaves_tmp[i]);
        }
    }

    public void DoComputeTest()
    {
        Debug.LogFormat("starting...");

        object[] device_info = GetDevice_OpenCL();
        //object[] device_info = GetDevice_Vulkan();

        ComputeInterface.ControllerInfo controllerInfo = default;
        controllerInfo.platform = device_info[0] == null ? default : (Platform)device_info[0];
        controllerInfo.Device = device_info[1];
        controllerInfo.SetProgramDir("C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/Vulkan/Tree");
        controller = ComputeInterface.GetComputeController(ComputeInterface.Compute_SDK.OpenCL, controllerInfo);

        Debug.Log("Created controller");

        string kernel_csmain = "CSMain";
        string kernel_name1 = "work";
        string kernel_name2 = "work2";
        string kernel_name3 = "work3";
        string kernel_name4 = "work4";
        string kernel_name5 = "work5";
        string kernel_name6 = "work6";

        string cur_kernel = kernel_csmain;

        ComputeProgram.ProgramInfo p_info = new ComputeProgram.ProgramInfo("compute_glsl_cl", ComputeProgram.FileType.Binary);
        /*p_info.AddKernel(kernel_name1);
        p_info.AddKernel(kernel_name2);
        p_info.AddKernel(kernel_name3);
        p_info.AddKernel(kernel_name4);
        p_info.AddKernel(kernel_name5);
        p_info.AddKernel(kernel_name6);*/

        p_info.AddKernel(kernel_csmain);

        Debug.Log("Create Program Info.");

        //return;

        program = controller.AddProgram(p_info);
        Debug.LogFormat("Create Program '{0}'.", "test_cl");

        int DATA_SIZE = 32;

        int[] Data = new int[DATA_SIZE];
        for (int i = 0; i < DATA_SIZE; i++)
            Data[i] = i + 1;

        ComputeBuffer in_Buffer1 = controller.NewReadWriteBuffer(DATA_SIZE, sizeof(int));
        ComputeBuffer in_Buffer2 = controller.NewReadWriteBuffer(DATA_SIZE, sizeof(int));
        //ComputeBuffer out_Buffer = controller.NewWriteBuffer(DATA_SIZE, sizeof(int));

        ComputeBuffer p_Buffer = controller.NewReadWriteBuffer(DATA_SIZE, sizeof(int));
        ComputeBuffer p_Buffer2 = controller.NewReadWriteBuffer(DATA_SIZE, sizeof(int));

        ComputeProgram.BindIndex ind = default;

        ind.GlobalIndex = 0;
        ind.ParameterIndex = 0;
        program.KernelSetBuffer(cur_kernel, in_Buffer1, ind);
        ind.GlobalIndex = 1;
        ind.ParameterIndex = 1;
        program.KernelSetBuffer(cur_kernel, in_Buffer2, ind);
        ind.GlobalIndex = 2;
        ind.ParameterIndex = 2;
        //program.KernelSetBuffer(cur_kernel, p_Buffer, ind);
        ind.GlobalIndex = 3;
        ind.ParameterIndex = 3;
        //program.KernelSetBuffer(cur_kernel, p_Buffer2, ind);

        program.FinishBuild();

        in_Buffer1.SetData(Data);
        in_Buffer2.SetData(Data);

        program.RunKernel(cur_kernel, DATA_SIZE, 0, 0);

        in_Buffer1.GetData(Data);
        int[] Data2 = new int[DATA_SIZE];
        in_Buffer2.GetData(Data2);

        for (int i = 0; i < DATA_SIZE; i++)
        {
            Debug.LogFormat("res '{0}': {1} - {2}", i, Data[i], Data2[i]);
        }

        ComputeInterface.DisposePlatform(ComputeInterface.Compute_SDK.OpenCL);
        Debug.LogFormat("OpenCL Disposed.");
    }

    object[] GetDevice_Vulkan()
    {
        Vulkan_Device_Info[] devices = ComputeInterface.Get_Supported_Devices_Vulkan();

        Debug.LogFormat("Devices: {0}", devices.Length);
        foreach (Vulkan_Device_Info info in devices)
        {
            //Vulkan_Device_Info info = device.Vulkan_Info;

            Debug.LogFormat("{0} ({1}): {2}", info.Name, info.GetTypeName(), info.Device_ID);
        }

        Vulkan_Device_Info device = devices[0];
        return new object[] { null, device };
    }

    object[] GetDevice_OpenCL()
    {
        Platform[] platforms = ComputeInterface.Get_Supported_Platforms_OpenCL();

        foreach (Platform p in platforms)
	    {
            //printf("Platform %s:\n", p.name);
            Debug.LogFormat("Platform: {0}", p.Name);

            OpenCL_Device_Info[] devices = ComputeInterface.Get_Supported_Devices_OpenCL(p);
            foreach (OpenCL_Device_Info info in devices)
            {
                //OpenCL_Device_Info info = d.OpenCL_Info;
                //printf("\t%s - %s: Frequency: %u, threads: %u, Memory: %lu, Work Size: %u \n", info.vendor, info.name, info.clock_frequency, info.num_compute_units * info.group_size, info.mem_size, info.max_work_size);
                //printf("\t%s - %s\n", info.vendor, info.name);
                Debug.LogFormat("D3vice: {0}, Local Mem: {1}", info.Name, info.local_memory_size);
            }
        }

        Platform platf = platforms[1];
        OpenCL_Device_Info dev = ComputeInterface.Get_Supported_Devices_OpenCL(platf)[0];

        return new object[]{platf, dev};
    }

    BoundingBox_CL GetCubeBounds(Transform obj)
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
        return new BoundingBox_CL(min, max);
    }
}
