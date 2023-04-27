using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using UnityEngine;

[Serializable]
[StructLayout(LayoutKind.Explicit)]
public struct NodeChild : IEquatable<NodeChild>
{
    [FieldOffset(0)]
    public Vector3 Min;
    [FieldOffset(12)]
    public int Index;
    [FieldOffset(16)]
    public Vector3 Max;
    [FieldOffset(28)]
    public int LeafCount;

    public bool Equals(NodeChild other)
    {
        return 
            Index == other.Index && 
            LeafCount == other.LeafCount &&
            Vector3.Distance(Min, other.Min) <= 0.0001f &&
            Vector3.Distance(Max, other.Max) <= 0.0001f;
    }
}

//Note that the format of this node implies that we don't explicitly test against the root bounding box during normal execution.
//For almost all broad phase use cases, queries will be inside the root bounding box anyway. For non-broad phase uses, the outer bounding box will likely be stored
//elsewhere- for example, in the broad phase.

/// <summary>
/// 2-wide tree node.
/// </summary>
[Serializable]
[StructLayout(LayoutKind.Explicit)]
public unsafe struct Node : IEquatable<Node>
{
    [FieldOffset(0)]
    public NodeChild A;
    [FieldOffset(32)]
    public NodeChild B;

    public bool Equals(Node other)
    {
        return A.Equals(other.A) && B.Equals(other.B);
    }
}

//Node metadata isn't required or used during collision testing, so it is stored separately.
//This helps avoid splitting Nodes across cache lines and decreases memory bandwidth requirements during testing.
/// <summary>
/// Metadata associated with a 2-child tree node.
/// </summary>
[Serializable]
[StructLayout(LayoutKind.Explicit)]
public unsafe struct Metanode : IEquatable<Metanode>
{
    [FieldOffset(0)]
    public int Parent;
    [FieldOffset(4)]
    public int IndexInParent;
    [FieldOffset(8)]
    public int RefineFlag;
    /// <summary>
    /// Cached change in cost of the tree starting at this node since the previous frame.
    /// The local cost change is unioned with the refine flags. They're never used simultaneously.
    /// This will be overwritten right after use, so don't expect anything meaningful here outside of refinement scheduling's scope.
    /// </summary>
    //[FieldOffset(12)]
    //public float LocalCostChange;
    [FieldOffset(12)]
    public int padding;

    public static int Size()
    {
        return Marshal.SizeOf(typeof(Metanode));
    }

    public bool Equals(Metanode other)
    {
        return
            Parent == other.Parent &&
            IndexInParent == other.IndexInParent &&
            RefineFlag == other.RefineFlag;// && 
            //LocalCostChange == other.LocalCostChange;
    }

    public override string ToString()
    {
        return String.Format("Metanode:(Parent: {0}, IndexInParent: {1}, RefineFlag: {2})",
            Parent, IndexInParent, RefineFlag);
    }
}

/// <summary>
/// Pointer to a leaf's tree location.
/// </summary>
/// <remarks>The identity of a leaf is implicit in its position within the leaf array.</remarks>
[Serializable]
public struct Leaf : IEquatable<Leaf>
{
    public int Node_Index;
    public int Child_Index;

    /// <summary>
    /// Gets the index of the node that the leaf is directly held by.
    /// </summary>
    public int NodeIndex
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get { return (int)(packed & 0x7FFFFFFF); }
    }
    /// <summary>
    /// Gets which child within the owning node the leaf is in.
    /// </summary>
    public int ChildIndex
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get { return (int)((packed & 0x80000000) >> 31); }
    }

    public uint Packed
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get { return packed; }
    }

    uint packed;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Leaf(uint pcked)
    {
        Node_Index = 0;
        Child_Index = 0;
        packed = pcked;
        Node_Index = NodeIndex;
        Child_Index = ChildIndex;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Leaf(int nodeIndex, int childIndex)
    {
        Debug.Assert((childIndex & ~1) == 0, "Binary trees can't have children in slots other than 0 and 1!");
        packed = ((uint)nodeIndex & 0x7FFF_FFFF) | ((uint)childIndex << 31);
        Node_Index = nodeIndex;
        Child_Index = childIndex;
    }

    public bool Equals(Leaf other)
    {
        return packed == other.packed;
    }
}
