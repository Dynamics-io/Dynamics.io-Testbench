#define NEW_INTERNAL 0
#define TRAVERSE 1

struct BoundingBox{
	float4 Min;
	float4 Max;
};

struct InsertionChoice{
	int Choice;
	struct BoundingBox MergedCandidate;
	float CostChange;
};

struct BoundingBox CreateMerged(float4 minA, float4 maxA, float4 minB, float4 maxB)
{
	struct BoundingBox merged;
	merged.Min = min(minA, minB);
	merged.Max = max(maxA, maxB);
	return merged;
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
	//float newCost = ComputeBoundsMetric_1(result.MergedCandidate);
	if (NodeChild_Index >= 0)
	{
		//Estimate the cost of child node expansions as max(SAH(newLeafBounds), costChange) * log2(child.LeafCount).
        //We're assuming that the remaining tree is balanced and that each level will expand by at least SAH(newLeafBounds). 
        //This might not be anywhere close to correct, but it's not a bad estimate.
		//result.CostChange = newCost - ComputeBoundsMetric_2(NodeChild_Min, NodeChild_Max);
		//result.CostChange = result.CostChange + GetContainingPowerOf2(NodeChild_LeafCount) * max(newLeafCost, result.CostChange);
		result.Choice = TRAVERSE;
	}
	else
	{
		//result.CostChange = newCost;
		result.Choice = NEW_INTERNAL;
	}
	
	return result;
}

__attribute__((reqd_work_group_size(1, 1, 1)))
kernel void Test(global struct BoundingBox* in_bounds, global int* out_int)
{	
	float4 child_min = (float4)(0, 0, 0, 0);
	float4 child_max = (float4)(1, 0, 0, 0);
	int NodeChild_Index = 0;
	int NodeChild_LeafCount = 0;

	out_int[0] = ComputeBestInsertionChoice(
		in_bounds[0],
		1,
		child_min,
		child_max,
		NodeChild_Index,
		NodeChild_LeafCount
	).Choice;
}
