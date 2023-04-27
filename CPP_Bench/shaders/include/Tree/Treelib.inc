#ifndef _TREELIB_INC_
#define _TREELIB_INC_

#include "Types.inc"

struct BoundingBox{
	fvector4 Min;
	fvector4 Max;
};

struct InsertionChoice{
	int Choice;
	STRUCT BoundingBox MergedCandidate;
	float CostChange;
};

#if !defined(VULKAN)
struct tree_counts {
	int NodeCount;
	int LeafCount;
	
	int padding1_;
	int padding2_;
};

struct metanode {
	int Parent;
	int IndexInParent;
	int RefineFlag;
	
	int padding_;
};
#endif

#if defined(VULKAN)

layout(std140, binding = 0) buffer g_counts_InOut{
	ivec4 g_counts[ ];
};

layout(std140, binding = 1) buffer g_metanodes_InOut{
	ivec4 g_metanodes[ ];
};

layout(std140, binding = 2) buffer g_leaves_InOut{
	uvec4 g_leaves[ ];
};

layout(std140, binding = 3) buffer g_NodeChild_Min_InOut{
	vec4 g_NodeChild_Min[ ];
};
layout(std140, binding = 4) buffer g_NodeChild_Max_InOut{
	vec4 g_NodeChild_Max[ ];
};
layout(std140, binding = 5) buffer g_NodeChild_Index_InOut{
	ivec4 g_NodeChild_Index[ ];
};
layout(std140, binding = 6) buffer g_NodeChild_LeafCount_InOut{
	ivec4 g_NodeChild_LeafCount[ ];
};

#elif defined(OPENCL)

__global struct tree_counts* g_counts;
__global struct metanode* g_metanodes;

__global float4* g_NodeChild_Min;
__global float4* g_NodeChild_Max;
__global int* g_NodeChild_Index;
__global int* g_NodeChild_LeafCount;

__global unsigned int* g_leaves;



__local struct tree_counts* p_counts;

__local float4* p_NodeChild_Min;
__local float4* p_NodeChild_Max;
__local int* p_NodeChild_Index;
__local int* p_NodeChild_LeafCount;

__local uint* p_leaves;

#elif defined(DIRECTX)

// DIRECTX buffers

#endif


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
#if defined(VULKAN)
	uint pck_index = uint(nodeIndex) & 0x7FFFFFFF;
	uint pck_child = uint(childIndex) << 31;
	return pck_index | pck_child;
#elif defined(OPENCL)
	return ((uint)nodeIndex & 0x7FFFFFFF) | ((uint)childIndex << 31);
#elif defined(DIRECTX)
	return ((uint)nodeIndex & 0x7FFFFFFF) | ((uint)childIndex << 31);
#endif
}

int Leaf_NodeIndex(uint leaf)
{
#if defined(VULKAN)
	return int(leaf & 0x7FFFFFFF);
#elif defined(OPENCL)
	return (int)(leaf & 0x7FFFFFFF);
#elif defined(DIRECTX)
	return (int)(leaf & 0x7FFFFFFF);
#endif
}

int Leaf_ChildIndex(uint leaf)
{
#if defined(VULKAN)
	return int((leaf & 0x80000000) >> 31);
#elif defined(OPENCL)
	return (int)((leaf & 0x80000000) >> 31); 
#elif defined(DIRECTX)
	return (int)((leaf & 0x80000000) >> 31); 
#endif
}

int GetNodeCount()
{
#if defined(VULKAN)
	return g_counts[0].x;
#else
	return g_counts[0].NodeCount;
#endif
}

int GetLeafCount()
{
#if defined(VULKAN)
	return g_counts[0].y;
#else
	return g_counts[0].LeafCount;
#endif
}

void SetNodeCount(int cnt)
{
#if defined(VULKAN)
	g_counts[0].x = cnt;
#else
	g_counts[0].NodeCount = cnt;
#endif
}

void SetLeafCount(int cnt)
{
#if defined(VULKAN)
	g_counts[0].y = cnt;
#else
	g_counts[0].LeafCount = cnt;
#endif
}

void SetLeaf(int index, uint leaf)
{
#if defined(VULKAN)
	g_leaves[index / 4][index % 4] = leaf;
#else
	g_leaves[index] = leaf;
#endif
}

uint GetLeaf(int index)
{
#if defined(VULKAN)
	return g_leaves[index / 4][index % 4];
#else
	return g_leaves[index];
#endif
}

int Get_Metanode_Parent(int index)
{
#if defined(VULKAN)
	return g_metanodes[index].x;
#else
	return g_metanodes[index].Parent;
#endif
}

int Get_Metanode_IndexInParent(int index)
{
#if defined(VULKAN)
	return g_metanodes[index].y;
#else
	return g_metanodes[index].IndexInParent;
#endif
}

int Get_Metanode_RefineFlag(int index)
{
#if defined(VULKAN)
	return g_metanodes[index].z;
#else
	return g_metanodes[index].RefineFlag;
#endif
}

void Set_Metanode_Parent(int index, int value)
{
#if defined(VULKAN)
	g_metanodes[index].x = value;
#else
	g_metanodes[index].Parent = value;
#endif
}

void Set_Metanode_IndexInParent(int index, int value)
{
#if defined(VULKAN)
	g_metanodes[index].y = value;
#else
	g_metanodes[index].IndexInParent = value;
#endif
}

void Set_Metanode_RefineFlag(int index, int value)
{
#if defined(VULKAN)
	g_metanodes[index].z = value;
#else
	g_metanodes[index].RefineFlag = value;
#endif
}

void Set_NodeChild_Min(int c_index, fvector4 val)
{
	g_NodeChild_Min[c_index] = val;
}

void Set_NodeChild_Max(int c_index, fvector4 val)
{
	g_NodeChild_Max[c_index] = val;
}

fvector4 Get_NodeChild_Min(int c_index)
{
	return g_NodeChild_Min[c_index];
}

fvector4 Get_NodeChild_Max(int c_index)
{
	return g_NodeChild_Max[c_index];
}

void Set_NodeChild_Index(int c_index, int val)
{
#if defined(VULKAN)
	g_NodeChild_Index[c_index / 4][c_index % 4] = val;
#else
	g_NodeChild_Index[c_index] = val;
#endif
}

void Set_NodeChild_LeafCount(int c_index, int val)
{
#if defined(VULKAN)
	g_NodeChild_LeafCount[c_index / 4][c_index % 4] = val;
#else
	g_NodeChild_LeafCount[c_index] = val;
#endif
}

int Get_NodeChild_Index(int c_index)
{
#if defined(VULKAN)
	return g_NodeChild_Index[c_index / 4][c_index % 4];
#else
	return g_NodeChild_Index[c_index];
#endif
}

int Get_NodeChild_LeafCount(int c_index)
{
#if defined(VULKAN)
	return g_NodeChild_LeafCount[c_index / 4][c_index % 4];
#else
	return g_NodeChild_LeafCount[c_index];
#endif
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
	SetLeaf(count, Leaf_New(nodeIndex, childIndex));
	SetLeafCount(count + 1);
	return count;
}


#endif //__TREELIB_INC__
