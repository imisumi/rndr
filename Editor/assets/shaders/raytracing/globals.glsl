const float PI = 3.14159265359;
// const float EPSILON = 0.0001;
const float EPSILON = 1e-6;
// const float EPSILON = 0.0;

// #define INFINITY (1.0 / 0.0)
#define INFINITY 1e20
#define NEGATIVE_INFINITY (-1.0 / 0.0)


struct BLAS
{
	int BufferIndex;
	int TriangleOffset;
	int TriangleCount;
	mat4 Transform;
};

struct TLAS
{
	uint BLASIndex;
	uint ChildIndex;
	mat4 Transform;
	vec3 Min;
	vec3 Max;
};



struct Triangle
{
	vec3 posA, posB, posC;
	vec3 normal;
	// Material material;
};

struct AABB
{
	vec3 Min;
	vec3 Max;
};

//TODO: compress this struct for better memory alignment
struct bvh
{
	int Left;
	int TriangleIndex;
	int TriangleCount;

	int padding1;

	vec3 Min;
	vec3 Max;
};
// struct bvh
// {
// 	vec4 Min; // xyz for min and w for triangle count
// 	vec4 Max; // xyz for max and w for index

// 	int TriangleIndex; // only needs to be know for leaf node and becuase a leaf node cant have any childeren so we can use this as triangle index
// 	// is a leaf if trianglecount is greater than 0 //TODO set to 0 in bvh construction of not a leaf
// };


struct Material
{
	vec3 albedo;
	vec3 emission;
	float emissionStrength;
};

struct HitInfo
{
	bool hit;
	vec3 position;
	vec3 normal;
	float distance;
	Material material;
};

struct Ray
{
	vec3 origin;
	vec3 dir;
	vec3 invDir;
};

struct Sphere
{
	vec3 position;
	float radius;
	Material material;
};