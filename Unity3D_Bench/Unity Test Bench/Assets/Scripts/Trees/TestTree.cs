using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

[Serializable]
[StructLayout(LayoutKind.Sequential)]
public unsafe partial struct TestTree
{
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

    bool rootInited;

    public unsafe TestTree(int initialLeafCapacity = 4096) : this()
    {
        rootInited = false;
        Resize(initialLeafCapacity);
    }

    int AllocateNode()
    {
        Debug.Assert(Nodes.Length > NodeCount && Metanodes.Length > NodeCount,
            "Any attempt to allocate a node should not overrun the allocated nodes. For all operations that allocate nodes, capacity should be preallocated.");
        Debug.LogFormat("New node allocated: {0}", NodeCount);
        return NodeCount++;
    }

    int AddLeaf(int nodeIndex, int childIndex)
    {
        Debug.Assert(LeafCount < Leaves.Length,
            "Any attempt to allocate a leaf should not overrun the allocated leaves. For all operations that allocate leaves, capacity should be preallocated.");
        Leaves[LeafCount] = new Leaf(nodeIndex, childIndex);
        return LeafCount++;
    }

    public static int Encode(int index)
    {
        return -1 - index;
    }

    void InitializeRoot()
    {
        NodeCount = 1;
        ref var rootMetanode = ref Metanodes[0];
        rootMetanode.Parent = -1;
        rootMetanode.IndexInParent = -1;
        rootInited = true;
        Debug.Log("Root node initialized.");
    }

    public void Resize(int targetLeafSlotCount)
    {
        int leafCapacityForTarget = Math.Max(LeafCount, targetLeafSlotCount);
        int nodeCapacityForTarget = Math.Max(NodeCount, leafCapacityForTarget - 1);
        int metanodeCapacityForTarget = Math.Max(NodeCount, leafCapacityForTarget - 1);

        if (Nodes == null)
        {
            Nodes = new Node[nodeCapacityForTarget];
            Metanodes = new Metanode[metanodeCapacityForTarget];
            Leaves = new Leaf[leafCapacityForTarget];
        }
        else
        {
            Node[] tmpNodes = new Node[NodeCount];
            Metanode[] tmpMeta = new Metanode[NodeCount];
            Leaf[] tmpLeaves = new Leaf[LeafCount];

            Array.Copy(Nodes, tmpNodes, NodeCount);
            Array.Copy(Metanodes, tmpMeta, NodeCount);
            Array.Copy(Leaves, tmpLeaves, NodeCount);

            Nodes = new Node[nodeCapacityForTarget];
            Metanodes = new Metanode[metanodeCapacityForTarget];
            Leaves = new Leaf[leafCapacityForTarget];

            Array.Copy(tmpNodes, Nodes, NodeCount);
            Array.Copy(tmpMeta, Metanodes, NodeCount);
            Array.Copy(tmpLeaves, Leaves, NodeCount);
        }

        if (!rootInited)
        {
            InitializeRoot();
        }
    }

}
