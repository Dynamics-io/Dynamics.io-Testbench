#exclude

__kernel void testTransform(__global struct v3 *input, __global float4 *output)
{
	size_t id = get_global_id(0);
	
	float3 a = (float3)(input[0]);
	float3 b = (float3)(input[1]);
	float3 c = (float3)(input[2]);
	float3 point = (float3)(input[3]);

	struct Triangle t = Triangle_new(a, b, c);
	struct TriangleCoord tc = CartesianToTriangleTransform(t, point);
	
	//output[id] = (float4)(t.normal, 0);
	output[id] = (float4)(tc.barycentric.alpha, tc.barycentric.beta, tc.barycentric.gamma, tc.distance);
}