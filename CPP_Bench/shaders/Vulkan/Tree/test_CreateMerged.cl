struct BoundingBox{
	float4 Min;
	float4 Max;
};

struct BoundingBox CreateMerged(float4 minA, float4 maxA, float4 minB, float4 maxB)
{
	struct BoundingBox merged;
	merged.Min = min(minA, minB);
	merged.Max = max(maxA, maxB);
	return merged;
}

__attribute__((reqd_work_group_size(1, 1, 1)))
kernel void Test(global struct BoundingBox* in_box, global struct BoundingBox* out_box)
{	
	out_box[0] = CreateMerged(in_box[0].Min, in_box[0].Max, out_box[0].Min, out_box[0].Max);
}
