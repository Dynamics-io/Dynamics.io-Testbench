using System.Collections;
using System.Collections.Generic;
using Dynamics.Compute;
using ComputeBuffer = Dynamics.Compute.ComputeBuffer;
using UnityEngine;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System;

public class CL_Tree_Testing : MonoBehaviour
{
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public struct tree_counts
    {
        public int NodeCount;
        public int LeafCount;

        public static int Size()
        {
            return Marshal.SizeOf(typeof(tree_counts));
        }

        public override string ToString()
        {
            return String.Format("tree_counts:(NodeCount: {0}, LeafCount: {1})", NodeCount, LeafCount);
        }
    }


    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public struct BoundingBox
    {
        public Vector4 Min;
        public Vector4 Max;

        public BoundingBox(Vector3 min, Vector3 max)
        {
            this.Min = min;
            this.Max = max;
        }

        public static int Size()
        {
            return Marshal.SizeOf(typeof(BoundingBox));
        }
    };

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

    /// <summary>
    /// Number of nodes in the tree.
    /// </summary>
    public int NodeCount;

    /// <summary>
    /// Number of leaves in the tree.
    /// </summary>
    public int LeafCount;

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


    string kernel_tree_init = "Init";
    string kernel_tree_add = "Add";

    // Start is called before the first frame update
    void Start()
    {
        Init_CL();
        Init_Buffers();
        InitTree();

        AddBoxes();

        ExtractInfo();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnDestroy()
    {
        ComputeInterface.DisposePlatform(ComputeInterface.Compute_SDK.VULKAN);
        Debug.Log("Vulkan Compute Disposed.");
    }

    void Init_CL()
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
        controllerInfo.SetProgramDir("C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/Vulkan/Tree");
        controller = ComputeInterface.GetComputeController(ComputeInterface.Compute_SDK.VULKAN, controllerInfo);

        ComputeProgram.ProgramInfo p_info = new ComputeProgram.ProgramInfo("Tree", ComputeProgram.FileType.Binary);
        p_info.AddKernel(kernel_tree_init);
        p_info.AddKernel(kernel_tree_add);


        program = controller.AddProgram(p_info);

        Debug.Log("Tree program initialized.");
    }

    void Init_Buffers()
    {
        counts_buffer = controller.NewReadWriteBuffer(1, tree_counts.Size());
        metanodes_buffer = controller.NewReadWriteBuffer(Cubes.Length, Metanode.Size());
        leaves_buffer = controller.NewReadWriteBuffer(Cubes.Length, sizeof(uint));

        NodeChild_Min_buffer = controller.NewReadWriteBuffer(Cubes.Length * 2, sizeof(float) * 4);
        NodeChild_Max_buffer = controller.NewReadWriteBuffer(Cubes.Length * 2, sizeof(float) * 4);
        NodeChild_Index_buffer = controller.NewReadWriteBuffer(Cubes.Length * 2, sizeof(int));
        NodeChild_LeafCount_buffer = controller.NewReadWriteBuffer(Cubes.Length * 2, sizeof(int));

        leaves_buffer = controller.NewReadWriteBuffer(Cubes.Length, sizeof(uint));

        boundingBox_add_buffer = controller.NewReadWriteBuffer(Cubes.Length, BoundingBox.Size());
        numBoundsAdds = controller.NewReadWriteBuffer(1, sizeof(int));

        result_buffer = controller.NewReadWriteBuffer(Cubes.Length, sizeof(int));

        ComputeProgram.BindIndex ind = default;

        // Bind Init kernel buffers

        ind.GlobalIndex = 0;
        program.KernelSetBuffer(kernel_tree_init, counts_buffer, ind);
        ind.GlobalIndex = 1;
        program.KernelSetBuffer(kernel_tree_init, metanodes_buffer, ind);


        // Bind Add kernel buffers

        ind.GlobalIndex = 0;
        program.KernelSetBuffer(kernel_tree_add, counts_buffer, ind);
        ind.GlobalIndex = 1;
        program.KernelSetBuffer(kernel_tree_add, boundingBox_add_buffer, ind);
        ind.GlobalIndex = 2;
        program.KernelSetBuffer(kernel_tree_add, numBoundsAdds, ind);
        ind.GlobalIndex = 3;
        program.KernelSetBuffer(kernel_tree_add, NodeChild_Min_buffer, ind);
        ind.GlobalIndex = 4;
        program.KernelSetBuffer(kernel_tree_add, NodeChild_Max_buffer, ind);
        ind.GlobalIndex = 5;
        program.KernelSetBuffer(kernel_tree_add, NodeChild_Index_buffer, ind);
        ind.GlobalIndex = 6;
        program.KernelSetBuffer(kernel_tree_add, NodeChild_LeafCount_buffer, ind);
        ind.GlobalIndex = 7;
        program.KernelSetBuffer(kernel_tree_add, leaves_buffer, ind);
        ind.GlobalIndex = 8;
        program.KernelSetBuffer(kernel_tree_add, metanodes_buffer, ind);
        ind.GlobalIndex = 9;
        program.KernelSetBuffer(kernel_tree_add, result_buffer, ind);

        program.FinishBuild();

        Debug.Log("Compute Buffers Initialized.");
    }

    void InitTree()
    {
        program.RunKernel(kernel_tree_init, 1, 0, 0);
    }

    void AddBoxes()
    {
        BoundingBox[] boxAdds = new BoundingBox[Cubes.Length];
        for (int i = 0; i < Cubes.Length; i++)
        {
            BoundingBox bounds = GetCubeBounds(Cubes[i]);
            boxAdds[i] = bounds;
        }
        int[] numAdds = { boxAdds.Length };

        boundingBox_add_buffer.SetData(boxAdds);
        numBoundsAdds.SetData(numAdds);

        program.RunKernel(kernel_tree_add, 1, 0, 0);

    }

    void ExtractInfo()
    {
        tree_counts[] counts_tmp = new tree_counts[1];
        counts_buffer.GetData(counts_tmp);
        NodeCount = counts_tmp[0].NodeCount;
        LeafCount = counts_tmp[0].LeafCount;

        Nodes = new Node[NodeCount];

        //NodeChild[] node_children_tmp = new NodeChild[NodeCount * 2];
        Debug.LogFormat("Node count: {0}", NodeCount);
        Vector4[] child_min_tmp = new Vector4[NodeCount * 2];
        Vector4[] child_max_tmp = new Vector4[NodeCount * 2];
        int[] cild_index_tmp = new int[NodeCount * 2];
        int[] child_leafcount_tmp = new int[NodeCount * 2];
        Debug.LogFormat("child_min_tmp count 1: {0}", child_min_tmp.Length);

        NodeChild_Min_buffer.GetData(child_min_tmp);
        NodeChild_Max_buffer.GetData(child_max_tmp);
        NodeChild_Index_buffer.GetData(cild_index_tmp);
        NodeChild_LeafCount_buffer.GetData(child_leafcount_tmp);
        Debug.LogFormat("child_min_tmp count 2: {0}", child_min_tmp.Length);


        for (int i = 0; i < NodeCount; i++)
        {
            NodeChild A = default;
            NodeChild B = default;

            int a_index = i * 2;
            int b_index = i * 2 + 1;

            Debug.LogFormat("A Index: {0}", a_index);
            Debug.LogFormat("B Index: {0}", b_index);

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

        Metanodes = new Metanode[Cubes.Length];
        metanodes_buffer.GetData(Metanodes);

        Leaves = new Leaf[LeafCount];
        leaves_buffer.GetData(Leaves);
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
}
