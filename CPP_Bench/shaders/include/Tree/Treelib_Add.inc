#ifndef __TREELIB_INC__
#define __TREELIB_INC__

#include "Treelib.inc"

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

float ComputeBoundsMetric_2(
#if defined(VULKAN)
	vec4 min, 
	vec4 max
#else
	float4 min,
	float4 max
#endif
)
{
	// compute volume.
#if defined(VULKAN)
    vec4 offset = max - min;
#else
	float4 offset = max - min;
#endif
    return (offset.x * offset.y) + (offset.y * offset.z) + (offset.x * offset.z);
}

float ComputeBoundsMetric_1(STRUCT BoundingBox bounds)
{
	return ComputeBoundsMetric_2(bounds.Min, bounds.Max);
}


#endif //__TREELIB_INC__