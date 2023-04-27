using Dynamics.Compute;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using ComputeBuffer = Dynamics.Compute.ComputeBuffer;
using UnityEngine;
using System.Net.Http.Headers;
using System.Drawing;

[Serializable]
public partial class ComputeTree
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

    /// <summary>
    /// Number of nodes in the tree.
    /// </summary>
    public int NodeCount;

    /// <summary>
    /// Number of leaves in the tree.
    /// </summary>
    public int LeafCount;


    tree_counts[] cl_counts;

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

    Vector4[] NodeChild_Min;
    Vector4[] NodeChild_Max;
    int[] NodeChild_Index;
    int[] NodeChild_LeafCount;

    int m_size;
    bool m_use_kernels;

    public ComputeTree(int size, bool useKernels)
    {
        m_use_kernels = useKernels;
        Init(size);
    }

    public void Init(int size)
    {
        m_size = size;
        Init_CL();
        Init_Buffers(size);
        InitTree();
    }

    public void ExtractInfo()
    {
        Extract_Counts();
        Extract_Nodes();
        Extract_Metanodes();
        Extract_Leaves();
    }

    public void FlushAll(bool skipMetanodes)
    {
        Flush_Counts();
        Flush_Nodes();
        Flush_Leaves();

        if (!skipMetanodes)
            Flush_Metanodes();
    }

    public int GetLocalMemorySizeEstimate()
    {
        int targetLeafSlotCount = m_size;

        int leafCapacityForTarget = Math.Max(LeafCount, targetLeafSlotCount);
        int nodeCapacityForTarget = Math.Max(NodeCount, leafCapacityForTarget - 1);
        int metanodeCapacityForTarget = Math.Max(NodeCount, leafCapacityForTarget - 1);

        int sum = 0;

        sum += nodeCapacityForTarget * 2 * sizeof(float) * 4; // p_NodeChild_Min_buffer
        sum += nodeCapacityForTarget * 2 * sizeof(float) * 4; // p_NodeChild_Max_buffer
        sum += nodeCapacityForTarget * 2 * sizeof(int); // p_NodeChild_Index_buffer
        sum += nodeCapacityForTarget * 2 * sizeof(int); // p_NodeChild_LeafCount_buffer
        sum += leafCapacityForTarget * sizeof(uint); // p_leaves_buffer

        return sum;
    }

    public void SyncNodes()
    {
        for (int i = 0; i < NodeCount; i++)
        {
            NodeChild A = default;
            NodeChild B = default;

            int a_index = i * 2;
            int b_index = i * 2 + 1;

            //Debug.LogFormat("A Index: {0}", a_index);
            //Debug.LogFormat("B Index: {0}", b_index);

            A.Min = NodeChild_Min[a_index];
            A.Max = NodeChild_Max[a_index];
            A.Index = NodeChild_Index[a_index];
            A.LeafCount = NodeChild_LeafCount[a_index];

            B.Min = NodeChild_Min[b_index];
            B.Max = NodeChild_Max[b_index];
            B.Index = NodeChild_Index[b_index];
            B.LeafCount = NodeChild_LeafCount[b_index];

            Nodes[i].A = A;
            Nodes[i].B = B;
        }
    }

    private void Init_CL()
    {
        Debug.LogFormat("starting...");

        object[] device_info = GetDevice_OpenCL();
        //object[] device_info = GetDevice_Vulkan();

        ComputeInterface.ControllerInfo controllerInfo = default;
        controllerInfo.platform = device_info[0] == null ? default : (Platform)device_info[0];
        controllerInfo.Device = device_info[1];
        controllerInfo.SetProgramDir("D:\\Users\\jon\\source\\repos\\Dynamics.io-Testbench\\CPP_Bench\\shaders\\OpenCL\\Tree");
        controller = ComputeInterface.GetComputeController(ComputeInterface.Compute_SDK.OpenCL, controllerInfo);

        ComputeProgram.ProgramInfo p_info = new ComputeProgram.ProgramInfo("Tree", ComputeProgram.FileType.Text);
        p_info.AddKernel(kernel_tree_init);
        p_info.AddKernel(kernel_tree_add);


        program = controller.AddProgram(p_info);
        program.AddIncludeDirectory("D:\\Users\\jon\\source\\repos\\Dynamics.io-Testbench\\CPP_Bench\\shaders\\include");
        program.AddDefine("OPENCL", "");
        program.Build();

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

    private void Init_Buffers(int targetLeafSlotCount)
    {
        cl_counts = new tree_counts[1];

        int leafCapacityForTarget = Math.Max(LeafCount, targetLeafSlotCount);
        int nodeCapacityForTarget = Math.Max(NodeCount, leafCapacityForTarget - 1);
        int metanodeCapacityForTarget = Math.Max(NodeCount, leafCapacityForTarget - 1);

        Nodes = new Node[nodeCapacityForTarget];
        Metanodes = new Metanode[metanodeCapacityForTarget];
        Leaves = new Leaf[leafCapacityForTarget];

        NodeChild_Min = new Vector4[nodeCapacityForTarget * 2];
        NodeChild_Max = new Vector4[nodeCapacityForTarget * 2];
        NodeChild_Index = new int[Mathf.Max(nodeCapacityForTarget * 2, 4)];
        NodeChild_LeafCount = new int[Mathf.Max(nodeCapacityForTarget * 2, 4)];

        counts_buffer = controller.NewReadWriteBuffer(1, tree_counts.Size());
        metanodes_buffer = controller.NewReadWriteBuffer(metanodeCapacityForTarget, Metanode.Size());
        leaves_buffer = controller.NewReadWriteBuffer(Mathf.Max(leafCapacityForTarget, 4), sizeof(uint));

        NodeChild_Min_buffer = controller.NewReadWriteBuffer(nodeCapacityForTarget * 2, sizeof(float) * 4);
        NodeChild_Max_buffer = controller.NewReadWriteBuffer(nodeCapacityForTarget * 2, sizeof(float) * 4);
        NodeChild_Index_buffer = controller.NewReadWriteBuffer(Mathf.Max(nodeCapacityForTarget * 2, 4), sizeof(int));
        NodeChild_LeafCount_buffer = controller.NewReadWriteBuffer(Mathf.Max(nodeCapacityForTarget * 2, 4), sizeof(int));

        boundingBox_add_buffer = controller.NewReadWriteBuffer(targetLeafSlotCount, BoundingBox_CL.Size());
        numBoundsAdds = controller.NewReadWriteBuffer(4, sizeof(int));

        result_buffer = controller.NewReadWriteBuffer(Mathf.Max(targetLeafSlotCount, 4), sizeof(int));


        // Private buffers.
        //p_NodeChild_Min_buffer = controller.NewReadWriteBuffer(num * 2, sizeof(float) * 4);
        //p_NodeChild_Max_buffer = controller.NewReadWriteBuffer(num * 2, sizeof(float) * 4);
        //p_NodeChild_Index_buffer = controller.NewReadWriteBuffer(Mathf.Max(num * 2), sizeof(int));
        //p_NodeChild_LeafCount_buffer = controller.NewReadWriteBuffer(Mathf.Max(num * 2, 4), sizeof(int));
        //p_leaves_buffer = controller.NewReadWriteBuffer(Mathf.Max(num, 4), sizeof(uint));


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

    private void InitTree()
    {
        if (m_use_kernels)
        {
            program.RunKernel(kernel_tree_init, 1, 0, 0);
        }
        else
        {
            NodeCount = 1;
            LeafCount = 0;

            Metanodes[0].Parent = -1;
            Metanodes[0].IndexInParent = -1;

            Flush_Counts();
            Flush_Metanodes();
        }
    }

    private void Flush_Counts()
    {
        cl_counts[0].NodeCount = NodeCount;
        cl_counts[0].LeafCount = LeafCount;

        counts_buffer.SetData(cl_counts);
    }

    private void Extract_Counts()
    {
        counts_buffer.GetData(cl_counts);

        NodeCount = cl_counts[0].NodeCount;
        LeafCount = cl_counts[0].LeafCount;
    }

    private void Flush_Metanodes()
    {
        metanodes_buffer.SetData(Metanodes);
    }

    private void Extract_Metanodes()
    {
        metanodes_buffer.GetData(Metanodes);
    }

    private void Flush_Nodes()
    {
        NodeChild_Min_buffer.SetData(NodeChild_Min);
        NodeChild_Max_buffer.SetData(NodeChild_Max);
        NodeChild_Index_buffer.SetData(NodeChild_Index);
        NodeChild_LeafCount_buffer.SetData(NodeChild_LeafCount);
    }

    private void Extract_Nodes(bool syncNodes = false)
    {
        NodeChild_Min_buffer.GetData(NodeChild_Min);
        NodeChild_Max_buffer.GetData(NodeChild_Max);
        NodeChild_Index_buffer.GetData(NodeChild_Index);
        NodeChild_LeafCount_buffer.GetData(NodeChild_LeafCount);

        if (syncNodes)
        {
            SyncNodes();
        }
    }

    private void Flush_Leaves()
    {
        //leaves_buffer.SetData(Leaves); // TODO: Switch to this when debug variables are removed from Leaf class

        uint[] leaves_tmp = new uint[LeafCount];
        for (int i = 0; i < LeafCount; i++)
        {
            leaves_tmp[i] = Leaves[i].Packed;
        }

        leaves_buffer.SetData(leaves_tmp);
    }

    private void Extract_Leaves()
    {
        //leaves_buffer.GetData(Leaves); // TODO: Switch to this when debug variables are removed from Leaf class

        uint[] leaves_tmp = new uint[LeafCount];
        Leaves = new Leaf[LeafCount];
        leaves_buffer.GetData(leaves_tmp);

        for (int i = 0; i < LeafCount; i++)
        {
            Leaves[i] = new Leaf(leaves_tmp[i]);
        }
    }

    private object[] GetDevice_Vulkan()
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

    private object[] GetDevice_OpenCL()
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

        return new object[] { platf, dev };
    }


    int Node_GetChildIndex(int index, int child)
    {
        return index * 2 + child;
    }

    int Node_A(int index)
    {
        return index * 2 + 0;
    }

    int Node_B(int index)
    {
        return index * 2 + 1;
    }

    int Encode(int index)
    {
        return -1 - index;
    }

    uint Leaf_New(uint nodeIndex, uint childIndex)
    {
        return ((uint)nodeIndex & 0x7FFFFFFF) | ((uint)childIndex << 31);
    }

    int Leaf_NodeIndex(uint leaf)
    {
        return (int)(leaf & 0x7FFFFFFF);
    }

    int Leaf_ChildIndex(uint leaf)
    {
        return (int)((leaf & 0x80000000) >> 31);
    }

    int GetNodeCount()
    {
        return cl_counts[0].NodeCount;
    }

    int GetLeafCount()
    {
        return cl_counts[0].LeafCount;
    }

    void SetNodeCount(int cnt)
    {
        cl_counts[0].NodeCount = cnt;
    }

    void SetLeafCount(int cnt)
    {
        cl_counts[0].LeafCount = cnt;
    }

    void SetLeaf(int index, uint leaf)
    {
        Leaves[index] = new Leaf(leaf);
    }

    uint GetLeaf(int index)
    {
        return Leaves[index].Packed;
    }

    int Get_Metanode_Parent(int index)
    {
        return Metanodes[index].Parent;
    }

    int Get_Metanode_IndexInParent(int index)
    {
        return Metanodes[index].IndexInParent;
    }

    int Get_Metanode_RefineFlag(int index)
    {
        return Metanodes[index].RefineFlag;
    }

    void Set_Metanode_Parent(int index, int value)
    {
        Metanodes[index].Parent = value;
    }

    void Set_Metanode_IndexInParent(int index, int value)
    {
        Metanodes[index].IndexInParent = value;
    }

    void Set_Metanode_RefineFlag(int index, int value)
    {
        Metanodes[index].RefineFlag = value;
    }

    void Set_NodeChild_Min(int c_index, Vector4 val)
    {
        NodeChild_Min[c_index] = val;
    }

    void Set_NodeChild_Max(int c_index, Vector4 val)
    {
        NodeChild_Max[c_index] = val;
    }

    Vector4 Get_NodeChild_Min(int c_index)
    {
        return NodeChild_Min[c_index];
    }

    Vector4 Get_NodeChild_Max(int c_index)
    {
        return NodeChild_Max[c_index];
    }

    void Set_NodeChild_Index(int c_index, int val)
    {
        NodeChild_Index[c_index] = val;
    }

    void Set_NodeChild_LeafCount(int c_index, int val)
    {
        NodeChild_LeafCount[c_index] = val;
    }

    int Get_NodeChild_Index(int c_index)
    {
        return NodeChild_Index[c_index];
    }

    int Get_NodeChild_LeafCount(int c_index)
    {
        return NodeChild_LeafCount[c_index];
    }

    int AllocateNode()
    {
        int count = GetNodeCount() + 1;
        SetNodeCount(count);
        return count;
    }

    int IncrementLeaves()
    {
        int count = GetLeafCount() + 1;
        SetLeafCount(count);
        return count;
    }

    int AddLeaf(int nodeIndex, int childIndex)
    {
        int count = GetLeafCount();
        SetLeaf(count, Leaf_New((uint)nodeIndex, (uint)childIndex));
        SetLeafCount(count + 1);
        return count;
    }
}
