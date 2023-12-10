#ifndef _TREELIB_ADD_INC_
#define _TREELIB_ADD_INC_

#include "Types.inc"
#include "Treelib.inc"

#define NEW_INTERNAL 0
#define TRAVERSE 1

int InsertLeafIntoEmptySlot(STRUCT BoundingBox leafBox, int nodeIndex, int childIndex)
{
	int leafIndex = AddLeaf(nodeIndex, childIndex);
	int child = Node_GetChildIndex(nodeIndex, childIndex);
	
	Set_NodeChild_Min(child, leafBox.Min);
	Set_NodeChild_Max(child, leafBox.Max);
	Set_NodeChild_Index(child, Encode(leafIndex));
	Set_NodeChild_LeafCount(child, 1);
	
	return leafIndex;
}

float ComputeBoundsMetric_2(fvector4 min, fvector4 max)
{
	// compute volume.
    fvector4 offset = max - min;
    return (offset.x * offset.y) + (offset.y * offset.z) + (offset.x * offset.z);
}

float ComputeBoundsMetric_1(STRUCT BoundingBox bounds)
{
	return ComputeBoundsMetric_2(bounds.Min, bounds.Max);
}

STRUCT BoundingBox CreateMerged(fvector4 minA, fvector4 maxA, fvector4 minB, fvector4 maxB)
{
	STRUCT BoundingBox merged;
	merged.Min = min(minA, minB);
	merged.Max = max(maxA, maxB);
	return merged;
}

int LeadingZeros(int x)
{
	// from https://stackoverflow.com/questions/10439242/count-leading-zeroes-in-an-int32
	const int int_size = 4; //sizeof(int);
	const int numIntBits = int_size * 8; //compile time constant
	
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

int GetContainingPowerOf2(int i)
{
	int signed_ = i;//i == 0 ? 1 : i;
	if (i == 0)
		signed_ = 1;
	return (32 - LeadingZeros(signed_ - 1));

}

STRUCT InsertionChoice ComputeBestInsertionChoice(
	STRUCT BoundingBox bounds,
	float newLeafCost,
	
	fvector4 NodeChild_Min,
	fvector4 NodeChild_Max,
	int NodeChild_Index,
	int NodeChild_LeafCount
)
{
	STRUCT InsertionChoice result;
	result.MergedCandidate = CreateMerged(NodeChild_Min, NodeChild_Max, bounds.Min, bounds.Max);
	float newCost = ComputeBoundsMetric_1(result.MergedCandidate);
	if (NodeChild_Index >= 0)
	{
		//Estimate the cost of child node expansions as max(SAH(newLeafBounds), costChange) * log2(child.LeafCount).
        //We're assuming that the remaining tree is balanced and that each level will expand by at least SAH(newLeafBounds). 
        //This might not be anywhere close to correct, but it's not a bad estimate.
		result.CostChange = newCost - ComputeBoundsMetric_2(NodeChild_Min, NodeChild_Max);
		result.CostChange = result.CostChange + GetContainingPowerOf2(NodeChild_LeafCount) * max(newLeafCost, result.CostChange);
		result.Choice = TRAVERSE;
	}
	else
	{
		result.CostChange = newCost;
		result.Choice = NEW_INTERNAL;
	}
	
	return result;
}

int MergeLeafNodes(
	STRUCT BoundingBox newLeafBounds, 
	int parentIndex, 
	int indexInParent, 
	STRUCT BoundingBox merged
)
{
    //It's a leaf node.
    //Create a new internal node with the new leaf and the old leaf as children.
    //this is the only place where a new level could potentially be created.
	
    int newNodeIndex = AllocateNode();
	int newNode = newNodeIndex;
    
    Set_Metanode_Parent(newNodeIndex, parentIndex);
	Set_Metanode_IndexInParent(newNodeIndex, indexInParent);
	Set_Metanode_RefineFlag(newNodeIndex, 0);
    
    //The first child of the new node is the old leaf. Insert its bounding box.
    int parentNode = parentIndex;
    int childInParent = Node_GetChildIndex(parentNode, indexInParent);
    Set_NodeChild_Min(Node_A(newNode), Get_NodeChild_Min(childInParent));
	Set_NodeChild_Max(Node_A(newNode), Get_NodeChild_Max(childInParent));
	Set_NodeChild_Index(Node_A(newNode), Get_NodeChild_Index(childInParent));
	Set_NodeChild_LeafCount(Node_A(newNode), Get_NodeChild_LeafCount(childInParent));
    
    // Insert the new leaf into the second child slot
	int b = Node_GetChildIndex(newNode, 1);
	int leafIndex = AddLeaf(newNodeIndex, 1);
	Set_NodeChild_Index(b, Encode(leafIndex));
	Set_NodeChild_Min(b, newLeafBounds.Min);
	Set_NodeChild_Max(b, newLeafBounds.Max);
	Set_NodeChild_LeafCount(b, 1);
    
    //Update the old leaf node with the new index information.
	int new_a = Node_GetChildIndex(newNode, 0);
	int oldLeafIndex = Encode(Get_NodeChild_Index(new_a));
	SetLeaf(oldLeafIndex, Leaf_New(newNodeIndex, 0));
    
    //Update the original node's child pointer and bounding box.
    Set_NodeChild_Index(childInParent, newNodeIndex);
	Set_NodeChild_Min(childInParent, merged.Min);
	Set_NodeChild_Max(childInParent, merged.Max);
	Set_NodeChild_LeafCount(childInParent, 2);
    
    return leafIndex;
}

int DoAdd(STRUCT BoundingBox bounds)
{
	int LeafCount = GetLeafCount();
	int NodeCount = GetNodeCount();
	int res = -1;
	
	//This is a binary tree, so the only time a node can have less than full children is when it's the root node.
	//By convention, an empty tree still has a root node with no children, so we do have to handle this case.
	if (LeafCount < 2)
	{
		res = InsertLeafIntoEmptySlot(bounds, 0, LeafCount);
		return res;
	}
	
	//Assumption: Index 0 is always the root if it exists, and an empty tree will have a 'root' with a child count of 0.
	int nodeIndex = 0;
	float newLeafCost = ComputeBoundsMetric_1(bounds);
	int depth = 0;
    while(true)
    {
        if (depth > 100)
			return -4;
        
        //Which child should the leaf belong to?
		//Give the leaf to whichever node had the least cost change.
        
        int child_a = Node_GetChildIndex(nodeIndex, 0);
		int child_b = Node_GetChildIndex(nodeIndex, 1);
        
        STRUCT InsertionChoice choiceA = ComputeBestInsertionChoice(
			bounds,
			newLeafCost,
			Get_NodeChild_Min(child_a),
			Get_NodeChild_Max(child_a),
			Get_NodeChild_Index(child_a),
			Get_NodeChild_LeafCount(child_a)
		);
        
        STRUCT InsertionChoice choiceB = ComputeBestInsertionChoice(
			bounds,
			newLeafCost,
			Get_NodeChild_Min(child_b),
			Get_NodeChild_Max(child_b),
			Get_NodeChild_Index(child_b),
			Get_NodeChild_LeafCount(child_b)
		);
        
        if (choiceA.CostChange <= choiceB.CostChange)
        {
            if (choiceA.Choice == NEW_INTERNAL)
            {
                res = MergeLeafNodes(
					bounds,
					nodeIndex,
					0,
					choiceA.MergedCandidate
				);
				return res;
            }
            else
            {
                depth = depth + 1;
				Set_NodeChild_Min(child_a, choiceA.MergedCandidate.Min);
				Set_NodeChild_Max(child_a, choiceA.MergedCandidate.Max);
				nodeIndex = Get_NodeChild_Index(child_a);
				Set_NodeChild_LeafCount(child_a, Get_NodeChild_LeafCount(child_a) + 1);
            }
        }
        else
        {
            if (choiceB.Choice == NEW_INTERNAL)
            {
                res = MergeLeafNodes(
					bounds,
					nodeIndex,
					1,
					choiceB.MergedCandidate
				);
				return res;
            }
            else
            {
                depth = depth + 1;
				Set_NodeChild_Min(child_b, choiceB.MergedCandidate.Min);
				Set_NodeChild_Max(child_b, choiceB.MergedCandidate.Max);
				nodeIndex = Get_NodeChild_Index(child_b);
				Set_NodeChild_LeafCount(child_b, Get_NodeChild_LeafCount(child_b) + 1);
            }
        }
        
    }
	
	return res;
}


#endif //_TREELIB_ADD_INC_