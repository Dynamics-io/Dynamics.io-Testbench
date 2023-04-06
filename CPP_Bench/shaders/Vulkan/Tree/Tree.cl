#define NEW_INTERNAL 0
#define TRAVERSE 1

struct tree_counts {
	int NodeCount;
	int LeafCount;
} tree_counts;

struct metanode {
	int Parent;
	int IndexInParent;
	int RefineFlag;
	float LocalCostChange;
} metanode;

struct BoundingBox{
	float4 Min;
	float4 Max;
} BoundingBox;

struct InsertionChoice{
	int Choice,
	struct BoundingBox MergedCandidate;
	float CostChange;
} InsertionChoice;

int Node_GetChildIndex(int index, int child)
{
	return index * 2 + child;
}

int Encode(int index)
{
	return -1 - index;
}

unsigned int Leaf_New(int nodeIndex, int childIndex)
{
	return ((unsigned int)nodeIndex & 0x7FFF_FFFF) | ((unsigned int)childIndex << 31);
}

int Leaf_NodeIndex(unsigned int leaf)
{
	return (int)(leaf & 0x7FFFFFFF); 
}

int Leaf_ChildIndex(unsigned int leaf)
{
	return (int)((leaf & 0x80000000) >> 31); 
}

int AllocateNode(struct tree_counts* counts)
{
	struct tree_counts = counts[0];
	int count = tree_counts.NodeCount;
	tree_counts.NodeCount = count + 1;
	counts[0] = tree_counts;
	return count;
}

int IncrementLeaves(struct tree_counts* counts)
{
	struct tree_counts cnts = counts[0];
	int count = cnts.LeafCount;
	cnts.LeafCount = count + 1;
	counts[0] = cnts;
	return count;
}

int AddLeaf(int nodeIndex, int childIndex, struct tree_counts* counts, unsigned int* leaves)
{
	struct tree_counts = counts[0];
	int count = cnts.LeafCount;
	leaves[count] = Leaf_New(nodeIndex, childIndex);
	cnts.LeafCount = count + 1;
	counts[0] = cnts;
	return count;
}

__attribute__((reqd_work_group_size(1, 1, 1)))
kernel void Init(global struct tree_counts* counts, global metanode* metanodes){
	counts[0].NodeCount = 1;
	counts[0].LeafCount = 0;
	
	metanode[0].Parent = -1;
	metanode[0].IndexInParent = -1;
}



int InsertLeafIntoEmptySlot(
	struct BoundingBox leafBox, 
	int nodeIndex, 
	int childIndex, 
	
	float4* NodeChild_Min,
	float4* NodeChild_Max,
	int* NodeChild_Index,
	int* NodeChild_LeafCount
)
{
	int leafIndex = AddLeaf(nodeIndex, childIndex);
	int child = Node_GetChildIndex(nodeIndex, childIndex);
	
	NodeChild_Min[child] = leafBox.Min;
	NodeChild_Max[child] = leafBox.Max;
	NodeChild_Index[child] = Encode(leafIndex);
	NodeChild_LeafCount[child] = 1;
	
	return leafIndex;
}

float ComputeBoundsMetric_1(struct BoundingBox bounds)
{
	return ComputeBoundsMetric_2(bounds.Min, bounds.Max);
}

float ComputeBoundsMetric_2(float4 min, float4 max)
{
	// compute volume.
    float4 offset = max - min;
    return (offset.x * offset.y) + (offset.y * offset.z) + (offset.x * offset.z);
}

struct BoundingBox CreateMerged(float4 minA, float4 maxA, float4 minB, float4 maxB)
{
	struct BoundingBox merged;
	merged.Min = min(minA, minB);
	merged.Max = max(maxA, maxB);
	return merged;
}

int LeadingZeros(int x)
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

int GetContainingPowerOf2(int i)
{
	int signed_ = i == 0 ? 1 : i;
	return (int)(32 - LeadingZeros(signed_ - 1));

}

struct InsertionChoice ComputeBestInsertionChoice(
	struct BoundingBox bounds,
	float newLeafCost,
	
	float4 NodeChild_Min,
	float4 NodeChild_Max,
	int NodeChild_Index,
	int NodeChild_LeafCount,
)
{
	struct InsertionChoice result;
	result.MergedCandidate = CreateMerged(NodeChild_Min, NodeChild_Max, bounds.Min, bounds.Max);
	if (NodeChild_Index >= 0)
	{
		//Estimate the cost of child node expansions as max(SAH(newLeafBounds), costChange) * log2(child.LeafCount).
        //We're assuming that the remaining tree is balanced and that each level will expand by at least SAH(newLeafBounds). 
        //This might not be anywhere close to correct, but it's not a bad estimate.
		result.CostChange = newCost - ComputeBoundsMetric(NodeChild_Min, NodeChild_Max);
		result.CostChange = result.CostChange + GetContainingPowerOf2(NodeChild_LeafCount) * max(newLeafCost, costChange);
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
	struct BoundingBox newLeafBounds, 
	int parentIndex, 
	int indexInParent, 
	struct BoundingBox merged,
	struct tree_counts* counts,
	struct metanode* metanodes,
	
	float4* NodeChild_Min,
	float4* NodeChild_Max,
	int* NodeChild_Index,
	int* NodeChild_LeafCount,
	
	unsigned int* leaves
)
{
	//It's a leaf node.
    //Create a new internal node with the new leaf and the old leaf as children.
    //this is the only place where a new level could potentially be created.
	
	int newNodeIndex = AllocateNode(counts);
	int newNode = newNodeIndex;
	
	metanodes[newNodeIndex].Parent = parentIndex;
	metanodes[newNodeIndex].IndexInParent = indexInParent;
	metanodes[newNodeIndex].RefineFlag = 0;
	
	//The first child of the new node is the old leaf. Insert its bounding box.
	int parentNode = parentIndex;
	int childInParent = Node_GetChildIndex(parentNode, indexInParent);
	NodeChild_Min[newNode] = NodeChild_Min[childInParent];
	NodeChild_Max[newNode] = NodeChild_Max[childInParent];
	NodeChild_Index[newNode] = NodeChild_Index[childInParent];
	NodeChild_LeafCount[newNode] = NodeChild_LeafCount[childInParent];
	
	// Insert the new leaf into the second child slot
	int b = Node_GetChildIndex(newNode, 1);
	int leafIndex = AddLeaf(newNodeIndex, 1);
	NodeChild_Index[b] = Encode(leafIndex);
	NodeChild_Min[b] = newLeafBounds.Min;
	NodeChild_Max[b] = newLeafBounds.Max;
	NodeChild_LeafCount[b] = 1;
	
	//Update the old leaf node with the new index information.
	int new_a = Node_GetChildIndex(newNode, 0);
	int oldLeafIndex = Encode(NodeChild_Index[new_a]);
	leaves[oldLeafIndex] = Leaf_New(newNodeIndex, 0);
	
	//Update the original node's child pointer and bounding box.
	NodeChild_Index[childInParent] = newNodeIndex;
	NodeChild_Min[childInParent] = merged.Min;
	NodeChild_Max[childInParent] = merged.Max;
	NodeChild_LeafCount[childInParent] = 2;
	
	return leafIndex;
}

__attribute__((reqd_work_group_size(1, 1, 1)))
kernel void Add(
	global struct tree_counts* counts_ref,
	global struct BoundingBox* bounds_arr,
	global int* numBounds,
	global float4* NodeChild_Min,
	global float4* NodeChild_Max,
	global int* NodeChild_Index,
	global int* NodeChild_LeafCount,
	global unsigned int* leaves,
	global int* result
)
{
	int boundsCount = numBounds[0];
	for (int i = 0; i < boundsCount; i++)
	{
		struct tree_counts counts = counts_ref[0];
		int LeafCount = counts.LeafCount;
		int NodeCount = counts.NodeCount;
		struct BoundingBox bounds = bounds_arr[i];
		
		//This is a binary tree, so the only time a node can have less than full children is when it's the root node.
		//By convention, an empty tree still has a root node with no children, so we do have to handle this case.
		if (LeafCount < 2)
		{
			result[i] = InsertLeafIntoEmptySlot(
				bounds, 
				0, 
				LeafCount, 
				NodeChild_Min,
				NodeChild_Max,
				NodeChild_Index,
				NodeChild_LeafCount
			);
			continue;
		}
		
		//Assumption: Index 0 is always the root if it exists, and an empty tree will have a 'root' with a child count of 0.
		int nodeIndex = 0;
		float newLeafCost = ComputeBoundsMetric(bounds);
		while(true)
		{
			//Which child should the leaf belong to?
            //Give the leaf to whichever node had the least cost change. 
			
			int child_a = Node_GetChildIndex(nodeIndex, 0);
			int child_b = Node_GetChildIndex(nodeIndex, 1);
			
			struct InsertionChoice choiceA = ComputeBestInsertionChoice(
				bounds,
				newLeafCost,
				NodeChild_Min[child_a],
				NodeChild_Max[child_a],
				NodeChild_Index[child_a],
				NodeChild_LeafCount[child_a],
			);
			
			
			struct InsertionChoice choiceB = ComputeBestInsertionChoice(
				bounds,
				newLeafCost,
				NodeChild_Min[child_b],
				NodeChild_Max[child_b],
				NodeChild_Index[child_b],
				NodeChild_LeafCount[child_b],
			);
			
			if (choiceA.CostChange <= choiceB.CostChange)
			{
				if (choiceA.Choice == NEW_INTERNAL)
				{
					result[i] = MergeLeafNodes(
						bounds,
						nodeIndex,
						0,
						choiceA.MergedCandidate,
						NodeChild_Min,
						NodeChild_Max,
						NodeChild_Index,
						NodeChild_LeafCount,
						leaves
					);
					break;
				}
				else
				{
					NodeChild_Min[child_a] = choiceA.MergedCandidate.Min;
					NodeChild_Max[child_a] = choiceA.MergedCandidate.Max;
					nodeIndex = NodeChild_Index[child_a];
					NodeChild_LeafCount[child_a] = NodeChild_LeafCount[child_a] + 1;
				}
			}
			else
			{
				if (choiceA.Choice == NEW_INTERNAL)
				{
					result[i] = MergeLeafNodes(
						bounds,
						nodeIndex,
						1,
						choiceB.MergedCandidate,
						NodeChild_Min,
						NodeChild_Max,
						NodeChild_Index,
						NodeChild_LeafCount,
						leaves
					);
					break;
				}
				else
				{
					NodeChild_Min[child_b] = choiceB.MergedCandidate.Min;
					NodeChild_Max[child_b] = choiceB.MergedCandidate.Max;
					nodeIndex = NodeChild_Index[child_b];
					NodeChild_LeafCount[child_b] = NodeChild_LeafCount[child_b] + 1;
				}
			}
			
		}
	}
	
}