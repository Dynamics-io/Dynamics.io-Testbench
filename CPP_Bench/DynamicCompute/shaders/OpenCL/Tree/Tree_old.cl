#define NEW_INTERNAL 0
#define TRAVERSE 1
#define MAX_NODES 2
#define MAX_LEAVES 2

struct tree_counts {
	int NodeCount;
	int LeafCount;
	int padding1;
	int padding2;
};

struct metanode {
	int Parent;
	int IndexInParent;
	int RefineFlag;
	int padding;
};

struct BoundingBox{
	float4 Min;
	float4 Max;
};

struct InsertionChoice{
	int Choice;
	struct BoundingBox MergedCandidate;
	float CostChange;
};

//global struct tree_counts* g_counts;
//global struct metanode* g_metanodes;

//global float4* g_NodeChild_Min;
//global float4* g_NodeChild_Max;
//global int* g_NodeChild_Index;
//global int* g_NodeChild_LeafCount;

//global unsigned int* g_leaves;

__local struct tree_counts* p_counts;

__local float4* p_NodeChild_Min;
__local float4* p_NodeChild_Max;
__local int* p_NodeChild_Index;
__local int* p_NodeChild_LeafCount;

__local uint* p_leaves;

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

uint Leaf_New(int nodeIndex, int childIndex)
{
	return ((unsigned int)nodeIndex & 0x7FFFFFFF) | ((unsigned int)childIndex << 31);
}

int Leaf_NodeIndex(uint leaf)
{
	return (int)(leaf & 0x7FFFFFFF); 
}

int Leaf_ChildIndex(unsigned int leaf)
{
	return (int)((leaf & 0x80000000) >> 31); 
}

int AllocateNode()
{
	struct tree_counts tmp = g_counts[0];
	int count = tmp.NodeCount;
	tmp.NodeCount = count + 1;
	g_counts[0] = tmp;
	return count;
}

int IncrementLeaves()
{
	struct tree_counts cnts = g_counts[0];
	int count = cnts.LeafCount;
	cnts.LeafCount = count + 1;
	g_counts[0] = cnts;
	return count;
}

int AddLeaf(int nodeIndex, int childIndex)
{
	struct tree_counts cnts = g_counts[0];
	int count = cnts.LeafCount;
	p_leaves[count] = Leaf_New(nodeIndex, childIndex);
	cnts.LeafCount = count + 1;
	g_counts[0] = cnts;
	return count;
}

__attribute__((reqd_work_group_size(1, 1, 1)))
__kernel void Init(__global struct tree_counts* counts, __global struct metanode* metanodes){
	g_counts = counts;
	g_metanodes = metanodes;
	
	g_counts[0].NodeCount = 1;
	g_counts[0].LeafCount = 0;
	
	g_metanodes[0].Parent = -1;
	g_metanodes[0].IndexInParent = -1;
}



int InsertLeafIntoEmptySlot(
	struct BoundingBox leafBox, 
	int nodeIndex, 
	int childIndex
)
{
	
	int leafIndex = AddLeaf(nodeIndex, childIndex);
	int child = Node_GetChildIndex(nodeIndex, childIndex);
	
	p_NodeChild_Min[child] = leafBox.Min;
	p_NodeChild_Max[child] = leafBox.Max;
	p_NodeChild_Index[child] = Encode(leafIndex);
	p_NodeChild_LeafCount[child] = 1;
	
	return leafIndex;

}

float ComputeBoundsMetric_2(float4 min, float4 max)
{
	// compute volume.
    float4 offset = max - min;
    return (offset.x * offset.y) + (offset.y * offset.z) + (offset.x * offset.z);
}

float ComputeBoundsMetric_1(struct BoundingBox bounds)
{
	return ComputeBoundsMetric_2(bounds.Min, bounds.Max);
}

struct BoundingBox CreateMerged(float4 minA, float4 maxA, float4 minB, float4 maxB)
{
	struct BoundingBox merged;
	
	float4 min_ = (float4)(0,0,0,0);
	float4 max_ = (float4)(0,0,0,0);
	
	min_.x = min(minA.x, minB.x);
	min_.y = min(minA.y, minB.y);
	min_.z = min(minA.z, minB.z);
	
	max_.x = max(maxA.x, maxB.x);
	max_.y = max(maxA.y, maxB.y);
	max_.z = max(maxA.z, maxB.z);
	
	merged.Min = min_;
	merged.Max = max_;
	
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
	int signed_ = i;//i == 0 ? 1 : i;
	if (i == 0)
		signed_ = 1;
	return (int)(32 - LeadingZeros(signed_ - 1));

}

struct InsertionChoice ComputeBestInsertionChoice(
	struct BoundingBox bounds,
	float newLeafCost,
	
	float4 NodeChild_Min,
	float4 NodeChild_Max,
	int NodeChild_Index,
	int NodeChild_LeafCount
)
{
	struct InsertionChoice result;
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
	struct BoundingBox newLeafBounds, 
	int parentIndex, 
	int indexInParent, 
	struct BoundingBox merged
)
{
	//It's a leaf node.
    //Create a new internal node with the new leaf and the old leaf as children.
    //this is the only place where a new level could potentially be created.
	
	int newNodeIndex = AllocateNode();
	int newNode = newNodeIndex;
	
	g_metanodes[newNodeIndex].Parent = parentIndex;
	g_metanodes[newNodeIndex].IndexInParent = indexInParent;
	g_metanodes[newNodeIndex].RefineFlag = 0;
	
	//The first child of the new node is the old leaf. Insert its bounding box.
	int parentNode = parentIndex;
	int childInParent = Node_GetChildIndex(parentNode, indexInParent);
	p_NodeChild_Min[Node_A(newNode)] = p_NodeChild_Min[childInParent];
	p_NodeChild_Max[Node_A(newNode)] = p_NodeChild_Max[childInParent];
	p_NodeChild_Index[Node_A(newNode)] = p_NodeChild_Index[childInParent];
	p_NodeChild_LeafCount[Node_A(newNode)] = p_NodeChild_LeafCount[childInParent];
	
	// Insert the new leaf into the second child slot
	int b = Node_GetChildIndex(newNode, 1);
	int leafIndex = AddLeaf(newNodeIndex, 1);
	p_NodeChild_Index[b] = Encode(leafIndex);
	p_NodeChild_Min[b] = newLeafBounds.Min;
	p_NodeChild_Max[b] = newLeafBounds.Max;
	p_NodeChild_LeafCount[b] = 1;
	
	//Update the old leaf node with the new index information.
	int new_a = Node_GetChildIndex(newNode, 0);
	int oldLeafIndex = Encode(p_NodeChild_Index[new_a]);
	p_leaves[oldLeafIndex] = Leaf_New(newNodeIndex, 0);
	
	//Update the original node's child pointer and bounding box.
	p_NodeChild_Index[childInParent] = newNodeIndex;
	p_NodeChild_Min[childInParent] = merged.Min;
	p_NodeChild_Max[childInParent] = merged.Max;
	p_NodeChild_LeafCount[childInParent] = 2;
	
	return leafIndex;
}

int DoAdd(struct BoundingBox bounds)
{
	struct tree_counts counts = g_counts[0];
	int LeafCount = counts.LeafCount;
	int NodeCount = counts.NodeCount;
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
		
		struct InsertionChoice choiceA = ComputeBestInsertionChoice(
			bounds,
			newLeafCost,
			p_NodeChild_Min[child_a],
			p_NodeChild_Max[child_a],
			p_NodeChild_Index[child_a],
			p_NodeChild_LeafCount[child_a]
		);
		
		
		struct InsertionChoice choiceB = ComputeBestInsertionChoice(
			bounds,
			newLeafCost,
			p_NodeChild_Min[child_b],
			p_NodeChild_Max[child_b],
			p_NodeChild_Index[child_b],
			p_NodeChild_LeafCount[child_b]
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
				p_NodeChild_Min[child_a] = choiceA.MergedCandidate.Min;
				p_NodeChild_Max[child_a] = choiceA.MergedCandidate.Max;
				nodeIndex = p_NodeChild_Index[child_a];
				p_NodeChild_LeafCount[child_a] = p_NodeChild_LeafCount[child_a] + 1;
				
				//if (depth >= 99)
				//	return nodeIndex;
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
				p_NodeChild_Min[child_b] = choiceB.MergedCandidate.Min;
				p_NodeChild_Max[child_b] = choiceB.MergedCandidate.Max;
				nodeIndex = p_NodeChild_Index[child_b];
				p_NodeChild_LeafCount[child_b] = p_NodeChild_LeafCount[child_b] + 1;
				
				//if (depth >= 99)
				//	return -300 + -depth;
			}
		}
		
	}
}

__attribute__((reqd_work_group_size(1, 1, 1)))
__kernel void Add(
	__global struct tree_counts* counts_ref,
	__global struct BoundingBox* bounds_arr,
	__global int* numBounds,
	__global float4* NodeChild_Min,
	__global float4* NodeChild_Max,
	__global int* NodeChild_Index,
	__global int* NodeChild_LeafCount,
	__global uint* leaves,
	__global struct metanode* metanodes,
	__global int* result
	
	//__local struct tree_counts* pCounts_ref,
	//__local struct BoundingBox* pBounds_arr,
	//__local float4* pNodeChild_Min,
	//__local float4* pNodeChild_Max,
	//__local int* pNodeChild_Index,
	//__local int* pNodeChild_LeafCount,
	//__local uint* pLeaves
	//__local struct metanode* pMetanodes
	
)
{
	g_counts = counts_ref;
	g_metanodes = metanodes;
	
	//g_NodeChild_Min = NodeChild_Min;
	//g_NodeChild_Max = NodeChild_Max;
	//g_NodeChild_Index = NodeChild_Index;
	//g_NodeChild_LeafCount = NodeChild_LeafCount;
	
	//g_leaves = leaves;
	
	__local float4 pNodeChild_Min[MAX_NODES * 2];
	__local float4 pNodeChild_Max[MAX_NODES * 2];
	__local int pNodeChild_Index[MAX_NODES * 2];
	__local int pNodeChild_LeafCount[MAX_NODES * 2];
	__local uint pLeaves[MAX_LEAVES];
	
	p_NodeChild_Min = pNodeChild_Min;
	p_NodeChild_Max = pNodeChild_Max;
	p_NodeChild_Index = pNodeChild_Index;
	p_NodeChild_LeafCount = pNodeChild_LeafCount;
	p_leaves = pLeaves;
	
	int boundsCount = numBounds[0];
	int node_count = g_counts[0].NodeCount;
	
	event_t ev_in[5];
	ev_in[0] = async_work_group_copy (pNodeChild_Min, NodeChild_Min, MAX_NODES * 2, 0);
	ev_in[1] = async_work_group_copy (pNodeChild_Max, NodeChild_Max, MAX_NODES * 2, 0);
	ev_in[2] = async_work_group_copy (pNodeChild_Index, NodeChild_Index, MAX_NODES * 2, 0);
	ev_in[3] = async_work_group_copy (pNodeChild_LeafCount, NodeChild_LeafCount, MAX_NODES * 2, 0);
	ev_in[4] = async_work_group_copy (pLeaves, leaves, MAX_LEAVES, 0);
	wait_group_events(5, ev_in);
	
	for (int i = 0; i < boundsCount; i++)
	{		
		result[i] = DoAdd(bounds_arr[i]);
	}
	
	node_count = g_counts[0].NodeCount;
	event_t ev_out[5];
	ev_out[0] = async_work_group_copy (NodeChild_Min, pNodeChild_Min, node_count, 0);
	ev_out[1] = async_work_group_copy (NodeChild_Max, pNodeChild_Max, node_count, 0);
	ev_out[2] = async_work_group_copy (NodeChild_Index, pNodeChild_Index, node_count, 0);
	ev_out[3] = async_work_group_copy (NodeChild_LeafCount, pNodeChild_LeafCount, node_count, 0);
	ev_out[4] = async_work_group_copy (leaves, pLeaves, g_counts[0].LeafCount, 0);
	wait_group_events(5, ev_out);
	
}

