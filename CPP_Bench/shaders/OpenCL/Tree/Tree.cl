
#include "Tree/Treelib.inc"

#define MAX_NODES 2
#define MAX_LEAVES 2


__attribute__((reqd_work_group_size(1, 1, 1)))
__kernel void Init(__global struct tree_counts* counts, __global struct metanode* metanodes){
	/*g_counts = counts;
	g_metanodes = metanodes;
	
	g_counts[0].NodeCount = 1;
	g_counts[0].LeafCount = 0;
	
	g_metanodes[0].Parent = -1;
	g_metanodes[0].IndexInParent = -1;*/
	
	SetNodeCount(1);
	SetLeafCount(0);
	
	Set_Metanode_Parent(0, -1);
	Set_Metanode_IndexInParent(0, -1);
}



#include "Tree/Treelib_Add.inc"

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
	
	g_NodeChild_Min = NodeChild_Min;
	g_NodeChild_Max = NodeChild_Max;
	g_NodeChild_Index = NodeChild_Index;
	g_NodeChild_LeafCount = NodeChild_LeafCount;
	
	g_leaves = leaves;
	
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

