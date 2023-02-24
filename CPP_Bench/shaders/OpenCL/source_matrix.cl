#exclude

typedef struct mat_col { float c[3]; } mat_col;
typedef struct fmat3x3 { struct mat_col r[3]; } fmat3x3;

struct v3{
	float x;
	float y;
	float z;
};

struct fmat3x3 fmat3x3_new(
	float x0, float y0, float z0,
	float x1, float y1, float z1,
	float x2, float y2, float z2
)
{
	
	float r0[3] = {x0, y0, z0};
	float r1[3] = {x1, y1, z1};
	float r2[3] = {x2, y2, z2};
	
	struct fmat3x3 mat;
	mat.r[0].c[0] = r0[0];
	mat.r[0].c[1] = r0[1];
	mat.r[0].c[2] = r0[2];
	
	mat.r[1].c[0] = r1[0];
	mat.r[1].c[1] = r1[1];
	mat.r[1].c[2] = r1[2];
	
	mat.r[2].c[0] = r2[0];
	mat.r[2].c[1] = r2[1];
	mat.r[2].c[2] = r2[2];
	
	return mat;
}

float3 to_float3(struct v3 inp)
{
	return (float3)(inp.x, inp.y, inp.z);
}


struct fmat3x3 mul_3x3_3x3(struct fmat3x3 m1, struct fmat3x3 m2)
{
	float SrcA00 = m1.r[0].c[0];
	float SrcA01 = m1.r[0].c[1];
	float SrcA02 = m1.r[0].c[2];
	float SrcA10 = m1.r[1].c[0];
	float SrcA11 = m1.r[1].c[1];
	float SrcA12 = m1.r[1].c[2];
	float SrcA20 = m1.r[2].c[0];
	float SrcA21 = m1.r[2].c[1];
	float SrcA22 = m1.r[2].c[2];

	float SrcB00 = m2.r[0].c[0];
	float SrcB01 = m2.r[0].c[1];
	float SrcB02 = m2.r[0].c[2];
	float SrcB10 = m2.r[1].c[0];
	float SrcB11 = m2.r[1].c[1];
	float SrcB12 = m2.r[1].c[2];
	float SrcB20 = m2.r[2].c[0];
	float SrcB21 = m2.r[2].c[1];
	float SrcB22 = m2.r[2].c[2];
	
	struct fmat3x3 Result;
	Result.r[0].c[0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02;
	Result.r[0].c[1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02;
	Result.r[0].c[2] = SrcA02 * SrcB00 + SrcA12 * SrcB01 + SrcA22 * SrcB02;
	Result.r[1].c[0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12;
	Result.r[1].c[1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12;
	Result.r[1].c[2] = SrcA02 * SrcB10 + SrcA12 * SrcB11 + SrcA22 * SrcB12;
	Result.r[2].c[0] = SrcA00 * SrcB20 + SrcA10 * SrcB21 + SrcA20 * SrcB22;
	Result.r[2].c[1] = SrcA01 * SrcB20 + SrcA11 * SrcB21 + SrcA21 * SrcB22;
	Result.r[2].c[2] = SrcA02 * SrcB20 + SrcA12 * SrcB21 + SrcA22 * SrcB22;
}

float3 mul_3x3_3x1(struct fmat3x3 m, float3 v)
{
	return (float3)(
		m.r[0].c[0] * v.x + m.r[1].c[0] * v.y + m.r[2].c[0] * v.z,
		m.r[0].c[1] * v.x + m.r[1].c[1] * v.y + m.r[2].c[1] * v.z,
		m.r[0].c[2] * v.x + m.r[1].c[2] * v.y + m.r[2].c[2] * v.z);
}

struct fmat3x3 inverse(struct fmat3x3 m)
{
	float OneOverDeterminant = 1.0f / (
				+ m.r[0].c[0] * (m.r[1].c[1] * m.r[2].c[2] - m.r[2].c[1] * m.r[1].c[2])
				- m.r[1].c[0] * (m.r[0].c[1] * m.r[2].c[2] - m.r[2].c[1] * m.r[0].c[2])
				+ m.r[2].c[0] * (m.r[0].c[1] * m.r[1].c[2] - m.r[1].c[1] * m.r[0].c[2]));
				
				
	struct fmat3x3 Inverse;
	Inverse.r[0].c[0] = + (m.r[1].c[1] * m.r[2].c[2] - m.r[2].c[1] * m.r[1].c[2]) * OneOverDeterminant;
	Inverse.r[1].c[0] = - (m.r[1].c[0] * m.r[2].c[2] - m.r[2].c[0] * m.r[1].c[2]) * OneOverDeterminant;
	Inverse.r[2].c[0] = + (m.r[1].c[0] * m.r[2].c[1] - m.r[2].c[0] * m.r[1].c[1]) * OneOverDeterminant;
	Inverse.r[0].c[1] = - (m.r[0].c[1] * m.r[2].c[2] - m.r[2].c[1] * m.r[0].c[2]) * OneOverDeterminant;
	Inverse.r[1].c[1] = + (m.r[0].c[0] * m.r[2].c[2] - m.r[2].c[0] * m.r[0].c[2]) * OneOverDeterminant;
	Inverse.r[2].c[1] = - (m.r[0].c[0] * m.r[2].c[1] - m.r[2].c[0] * m.r[0].c[1]) * OneOverDeterminant;
	Inverse.r[0].c[2] = + (m.r[0].c[1] * m.r[1].c[2] - m.r[1].c[1] * m.r[0].c[2]) * OneOverDeterminant;
	Inverse.r[1].c[2] = - (m.r[0].c[0] * m.r[1].c[2] - m.r[1].c[0] * m.r[0].c[2]) * OneOverDeterminant;
	Inverse.r[2].c[2] = + (m.r[0].c[0] * m.r[1].c[1] - m.r[1].c[0] * m.r[0].c[1]) * OneOverDeterminant;
	
	return Inverse;
}

