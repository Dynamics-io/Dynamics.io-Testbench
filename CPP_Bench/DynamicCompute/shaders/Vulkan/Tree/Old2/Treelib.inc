struct BoundingBox{
	vec4 Min;
	vec4 Max;
};

struct InsertionChoice{
	int Choice;
	BoundingBox MergedCandidate;
	float CostChange;
};

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
	uint pck_index = uint(nodeIndex) & 0x7FFFFFFF;
	uint pck_child = uint(childIndex) << 31;
	return pck_index | pck_child;
}

int Leaf_NodeIndex(uint leaf)
{
	int res = int(leaf & 0x7FFFFFFF);
	return res;
}

int Leaf_ChildIndex(uint leaf)
{
	int res = int((leaf & 0x80000000) >> 31);
	return res; 
}

int GetNodeCount()
{
	return g_counts[0].x;
}

int GetLeafCount()
{
	return g_counts[0].y;
}

void SetNodeCount(int cnt)
{
	g_counts[0].x = cnt;
}

void SetLeafCount(int cnt)
{
	g_counts[0].y = cnt;
}

void SetLeaf(int index, uint leaf)
{
	g_leaves[index / 4][index % 4] = leaf;
}

uint GetLeaf(int index)
{
	return g_leaves[index / 4][index % 4];
}

int Get_Metanode_Parent(int index)
{
	return g_metanodes[index].x;
}

int Get_Metanode_IndexInParent(int index)
{
	return g_metanodes[index].y;
}

int Get_Metanode_RefineFlag(int index)
{
	return g_metanodes[index].z;
}

void Set_Metanode_Parent(int index, int value)
{
	g_metanodes[index].x = value;
}

void Set_Metanode_IndexInParent(int index, int value)
{
	g_metanodes[index].y = value;
}

void Set_Metanode_RefineFlag(int index, int value)
{
	g_metanodes[index].z = value;
}

void Set_NodeChild_Min(int c_index, vec4 val)
{
	g_NodeChild_Min[c_index] = val;
}

void Set_NodeChild_Max(int c_index, vec4 val)
{
	g_NodeChild_Max[c_index] = val;
}

vec4 Get_NodeChild_Min(int c_index)
{
	return g_NodeChild_Min[c_index];
}

vec4 Get_NodeChild_Max(int c_index)
{
	return g_NodeChild_Max[c_index];
}

void Set_NodeChild_Index(int c_index, int val)
{
	g_NodeChild_Index[c_index / 4][c_index % 4] = val;
}

void Set_NodeChild_LeafCount(int c_index, int val)
{
	g_NodeChild_LeafCount[c_index / 4][c_index % 4] = val;
}

int Get_NodeChild_Index(int c_index)
{
	return g_NodeChild_Index[c_index / 4][c_index % 4];
}

int Get_NodeChild_LeafCount(int c_index)
{
	return g_NodeChild_LeafCount[c_index / 4][c_index % 4];
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



