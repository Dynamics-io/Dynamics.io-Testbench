using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using Unity.Collections.LowLevel.Unsafe;
using UnityEngine;
using System;

partial struct TestTree
{
    enum BestInsertionChoice
    {
        NewInternal,
        Traverse
    }

    unsafe int InsertLeafIntoEmptySlot(BoundingBox leafBox, int nodeIndex, int childIndex, ref Node node)
    {
        var leafIndex = AddLeaf(nodeIndex, childIndex);
        ref var child = ref node.A;
        if (childIndex == 1)
        {
            child = ref node.B;
        }
        child.Min = leafBox.Min;
        child.Index = Encode(leafIndex);
        child.Max = leafBox.Max;
        child.LeafCount = 1;
        return leafIndex;
    }

    static void CreateMerged(Vector3 minA, Vector3 maxA, Vector3 minB, Vector3 maxB, out BoundingBox merged)
    {
        merged.Min = Vector3.Min(minA, minB);
        merged.Max = Vector3.Max(maxA, maxB);
    }

    private static unsafe BestInsertionChoice ComputeBestInsertionChoice(BoundingBox bounds, float newLeafCost, NodeChild child, out BoundingBox mergedCandidate, out float costChange)
    {
        CreateMerged(child.Min, child.Max, bounds.Min, bounds.Max, out mergedCandidate);
        var newCost = ComputeBoundsMetric(mergedCandidate);
        if (child.Index >= 0)
        {
            //Estimate the cost of child node expansions as max(SAH(newLeafBounds), costChange) * log2(child.LeafCount).
            //We're assuming that the remaining tree is balanced and that each level will expand by at least SAH(newLeafBounds). 
            //This might not be anywhere close to correct, but it's not a bad estimate.
            costChange = newCost - ComputeBoundsMetric(child.Min, child.Max);
            costChange += GetContainingPowerOf2(child.LeafCount) * Math.Max(newLeafCost, costChange);
            return BestInsertionChoice.Traverse;
        }
        else
        {
            costChange = newCost;
            return BestInsertionChoice.NewInternal;
        }
    }

    public unsafe int Add(BoundingBox bounds)
    {
        //The rest of the function assumes we have sufficient room. We don't want to deal with invalidated pointers mid-add.
        if (Leaves.Length == LeafCount)
        {
            //Note that, while we add 1, the underlying pool will request the next higher power of 2 in bytes that can hold it.
            //Since we're already at capacity, that will be ~double the size.
            Resize(LeafCount + 1);
        }


        //This is a binary tree, so the only time a node can have less than full children is when it's the root node.
        //By convention, an empty tree still has a root node with no children, so we do have to handle this case.
        if (LeafCount < 2)
        {
            return InsertLeafIntoEmptySlot(bounds, 0, LeafCount, ref Nodes[0]);
        }

        //Assumption: Index 0 is always the root if it exists, and an empty tree will have a 'root' with a child count of 0.
        int nodeIndex = 0;
        var newLeafCost = ComputeBoundsMetric(bounds);
        while (true)
        {
            //Which child should the leaf belong to?
            //Give the leaf to whichever node had the least cost change. 
            
            ref var node = ref Nodes[nodeIndex];

            ref var a = ref node.A;
            ref var b = ref node.B;

            var choiceA = ComputeBestInsertionChoice(bounds, newLeafCost, a, out var mergedA, out var costChangeA);
            var choiceB = ComputeBestInsertionChoice(bounds, newLeafCost, b, out var mergedB, out var costChangeB);

            if (costChangeA <= costChangeB)
            {
                //Debug.LogFormat("Choice A: {0} - {1}, {2}", choiceA.ToString(), costChangeA, mergedA.ToString());
                //Debug.DrawLine(mergedA.Min, mergedA.Max, Color.red, 5000);
                if (choiceA == BestInsertionChoice.NewInternal)
                {
                    //Debug.Log("MergeLeafNodes on A");
                    return MergeLeafNodes(bounds, nodeIndex, 0, mergedA);
                }
                else
                {
                    a.Min = mergedA.Min;
                    a.Max = mergedA.Max;
                    nodeIndex = a.Index;
                    ++a.LeafCount;
                }
            }
            else
            {
                //Debug.LogFormat("Choice B: {0} - {1}, {2}", choiceB.ToString(), costChangeB, mergedB.ToString());
                //Debug.DrawLine(mergedB.Min, mergedB.Max, Color.blue, 5000);
                if (choiceB == BestInsertionChoice.NewInternal)
                {
                    //Debug.Log("MergeLeafNodes on B");
                    return MergeLeafNodes(bounds, nodeIndex, 1, mergedB);
                }
                else
                {
                    //Debug.LogFormat("Traverse B");
                    b.Min = mergedB.Min;
                    b.Max = mergedB.Max;
                    nodeIndex = b.Index;
                    ++b.LeafCount;
                }
            }
            
        }
    }

    unsafe int MergeLeafNodes(BoundingBox newLeafBounds, int parentIndex, int indexInParent, BoundingBox merged)
    {
        //It's a leaf node.
        //Create a new internal node with the new leaf and the old leaf as children.
        //this is the only place where a new level could potentially be created.

        var newNodeIndex = AllocateNode();
        ref var newNode = ref Nodes[newNodeIndex];
        ref var newMetanode = ref Metanodes[newNodeIndex];
        newMetanode.Parent = parentIndex;
        newMetanode.IndexInParent = indexInParent;
        newMetanode.RefineFlag = 0;

        //The first child of the new node is the old leaf. Insert its bounding box.
        ref var parentNode = ref Nodes[parentIndex];
        ref var childInParent = ref parentNode.A;
        if (indexInParent == 1)
            childInParent = ref parentNode.B;
        newNode.A = childInParent;

        // Insert the new leaf into the second child slot
        ref var b = ref newNode.B;
        var leafIndex = AddLeaf(newNodeIndex, 1);
        b.Index = Encode(leafIndex);
        b.Min = newLeafBounds.Min;
        b.Max = newLeafBounds.Max;
        b.LeafCount = 1;

        //Update the old leaf node with the new index information.
        var oldLeafIndex = Encode(newNode.A.Index);
        Leaves[oldLeafIndex] = new Leaf(newNodeIndex, 0);

        //Update the original node's child pointer and bounding box.
        childInParent.Index = newNodeIndex;
        childInParent.Min = merged.Min;
        childInParent.Max = merged.Max;
        Debug.Assert(childInParent.LeafCount == 1);
        childInParent.LeafCount = 2;

        return leafIndex;
    }

    public static int LeadingZeros(int x)
    {
        // from https://stackoverflow.com/questions/10439242/count-leading-zeroes-in-an-int32

        const int numIntBits = sizeof(int) * 8; //compile time constant
                                                //do the smearing
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        //count the ones
        x -= x >> 1 & 0x55555555;
        x = (x >> 2 & 0x33333333) + (x & 0x33333333);
        x = (x >> 4) + x & 0x0f0f0f0f;
        x += x >> 8;
        x += x >> 16;
        return numIntBits - (x & 0x0000003f); //subtract # of 1s from 32
    }

    public static int GetContainingPowerOf2(int i)
    {
        var unsigned = i == 0 ? 1 : i;
        return (int)(32 - LeadingZeros(unsigned - 1));

    }

    internal static float ComputeBoundsMetric(BoundingBox bounds)
    {
        return ComputeBoundsMetric(bounds.Min, bounds.Max);
    }

    internal static float ComputeBoundsMetric(Vector3 min, Vector3 max)
    {
        // compute volume.
        var offset = max - min;
        return (offset.x * offset.y) + (offset.y * offset.z) + (offset.x * offset.z);
    }
}
