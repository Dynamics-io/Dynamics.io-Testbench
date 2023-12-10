#exclude

struct Triangle{
	float3 a;
	float3 b;
	float3 c;
	
	float3 u;
	float3 v;
	
	float3 normal;
} Triangle;

struct Barycentric {
	float alpha;
	float beta;
	float gamma;
} Barycentric;

struct TriangleCoord {
	struct Barycentric barycentric;
	float distance;
} TriangleCoord;

struct Triangle Triangle_normalize(struct Triangle t)
{
	t.normal = cross(t.u, t.v);
	t.normal = normalize(t.normal);
	
	return t;
}

struct Triangle Triangle_new(float3 a, float3 b, float3 c)
{
	struct Triangle t;
	
	t.a = a;
	t.b = b;
	t.c = c;
	
	t.u = a - c;
	t.v = b - c;
	
	t = Triangle_normalize(t);
	
	return t;
}

struct TriangleCoord CartesianToTriangleTransform(struct Triangle t, float3 p)
{
	float3 u = t.u;
	float3 v = t.v;
	float3 n = t.normal;
	
	struct fmat3x3 l_matrix = fmat3x3_new(
		u.x, u.y, u.z,
        v.x, v.y, v.z,
        n.x, n.y, n.z
	);
	
	struct fmat3x3 i_matrix = inverse(l_matrix);
	
	float3 result = mul_3x3_3x1(i_matrix, (p - t.c));
	
	struct Barycentric b;
	b.alpha = result.x;
	b.beta = result.y;
	b.gamma = (1.0f - result.x - result.y);
	
	struct TriangleCoord tc;
	tc.barycentric = b;
	tc.distance = result.z;
	
	return tc;
}

bool IsInsideTriangle(const struct TriangleCoord local, const float margin)
{
	struct Barycentric coord = local.barycentric;
	float distance = local.distance;
	return (coord.alpha >= 0) && (coord.beta >= 0) && (coord.gamma >= 0) && (abs(distance) <= margin);
}