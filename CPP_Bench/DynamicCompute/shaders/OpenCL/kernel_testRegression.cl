

__kernel void testRegression(__global int* input, __global float3 *output)
{
	size_t id = get_global_id(0);
	
	float a_term = 0;
	float b_term = 0;
	float c_term = 0;
	int i = 0;
	for (i = 0; i < input[id]; i++)
	{		
		float points[3][2] = {
			{ 0 + i / (float)10, 1 },
			{ 0.5 + i / (float)10, 0 },
			{ 1 + i / (float)10, 1 }
		};
		
		struct Variables var = regress3_preCalculate(points);
		
		a_term = regress3_aTerm(var);
		b_term = regress3_bTerm(var);
		c_term = regress3_cTerm(var);
	}
	

	output[id] = (float3)((float)i, b_term, c_term);
}