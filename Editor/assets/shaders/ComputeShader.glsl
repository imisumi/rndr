#version 450 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(binding = 0, rgba32f) writeonly uniform image2D img_output;
layout(binding = 1, rgba32f) readonly uniform image2D u_SkyTexture;


struct Triangle
{
	vec3 posA, posB, posC;
	vec3 normal;
	// Material material;
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


layout(std430, binding = 2) buffer triangleBuffer {
	Triangle data[]; // send as glm::vec4 but becuase of memory alignment, I just use vec3
} triBuffer;

layout(std430, binding = 3) buffer bvhBuffer {
	bvh data[]; // send as glm::vec4 but becuase of memory alignment, I just use vec3
} bvhData;


// struct tempBVH
// {
// 	vec4 min;
// 	vec4 max;
// 	int ChildIndex;
// 	int padding1;
// 	int padding2;
// 	int padding3;
// };

// layout(std430, binding = 4) buffer bvhNodes {
// 	tempBVH data[];
// } bvhNodeBuffer;

uniform int u_TriangleCount;

uniform int u_ScreenWidth;
uniform int u_ScreenHeight;

uniform int u_SkyTextureWidth;
uniform int u_SkyTextureHeight;

uniform mat4 u_InvProj;
uniform mat4 u_InvView;

uniform vec3 u_CameraPos;


// uniform uint u_MaxIndices;

// uniform uint u_ShowDepth;


const float PI = 3.14159265359;
// const float EPSILON = 0.0001;
const float EPSILON = 1e-5;

#define INFINITY (1.0 / 0.0)
#define NEGATIVE_INFINITY (-1.0 / 0.0)

struct Material
{
	vec3 albedo;
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

// struct Triangle
// {
// 	vec3 posA, posB, posC;
// 	vec3 normal;
// 	Material material;
// };

uint generateSeed()
{
	ivec2 pixelID = ivec2(gl_GlobalInvocationID.xy);
	vec2 uv = vec2(float(pixelID.x) / float(u_ScreenWidth - 1), float(pixelID.y) / float(u_ScreenHeight - 1));

	ivec2 numPixels = ivec2(u_ScreenWidth, u_ScreenHeight);
	ivec2 pixelCoord = ivec2(uv * vec2(numPixels - 1));

	uint seed = pixelCoord.y * numPixels.x + pixelCoord.x; // use this as color output for visulizing each random pixel
	return seed;
}


float RandomFloat(inout uint seed)
{
	seed = seed * 747796405 + 2891336453;
	uint result = ((seed >> ((seed >> 28) + 4)) ^ seed) * 277803737;
	result = (result >> 22) ^ result;
	return float(result) / float(4294967295.0);
}



// float RayBoundingBoxDst(Ray ray, vec3 boxMin, vec3 boxMax)
// {
// 	vec3 tMin = (boxMin - ray.origin) * ray.invDir;
// 	vec3 tMax = (boxMax - ray.origin) * ray.invDir;
// 	vec3 t1 = min(tMin, tMax);
// 	vec3 t2 = max(tMin, tMax);
// 	float tNear = max(max(t1.x, t1.y), t1.z);
// 	float tFar = min(min(t2.x, t2.y), t2.z);

// 	bool hit = tFar >= tNear && tFar > 0;
// 	float dst = hit ? tNear > 0 ? tNear : 0 : INFINITY;
// 	return dst;
// };

float RayBoundingBoxDst(Ray ray, vec3 boxMin, vec3 boxMax)
{
	vec3 tMin = (boxMin - ray.origin) * ray.invDir;
	vec3 tMax = (boxMax - ray.origin) * ray.invDir;
	vec3 t1 = min(tMin, tMax);
	vec3 t2 = max(tMin, tMax);
	float tNear = max(max(t1.x, t1.y), t1.z);
	float tFar = min(min(t2.x, t2.y), t2.z);

	bool hit = tFar >= tNear && tFar > 0;
	return hit ? tNear : INFINITY;
};


vec2 SampleUVFromDirection(vec3 direction) {
    // Normalize direction vector
    direction = normalize(direction);

    // Convert direction to spherical coordinates (latitude and longitude)
    float longitude = atan(direction.z, direction.x);
    float latitude = acos(direction.y);

    // Map spherical coordinates to UV coordinates
    float u = (longitude + PI) / (2.0 * PI);
    float v = latitude / PI;

    return vec2(u, v);
}

vec4 SampleSkybox(vec3 direction)
{
	vec2 uv = SampleUVFromDirection(direction);
	vec4 sky = imageLoad(u_SkyTexture, ivec2(uv * vec2(u_SkyTextureWidth, u_SkyTextureHeight)));
	// return vec4(uv, 0.0, 1.0);
	return sky;
	// return texture(skyBox, uv);
}



HitInfo RayTriangle(Ray ray, Triangle tri)
{
    vec3 edgeAB = tri.posB - tri.posA;
    vec3 edgeAC = tri.posC - tri.posA;
	vec3 normalVector = cross(edgeAB, edgeAC);
    vec3 ao = ray.origin - tri.posA;
    vec3 dao = cross(ao, ray.dir);

    float determinant = -dot(ray.dir, normalVector);
    float invDet = 1 / determinant;

    // Calculate distance to triangle & barycentric coordinates of intersection point
    float dst = dot(ao, normalVector) * invDet;
    float u = dot(edgeAC, dao) * invDet;
    float v = -dot(edgeAB, dao) * invDet;
    float w = 1 - u - v;

    // Initialize hit info
    HitInfo hitInfo;
    hitInfo.hit = abs(determinant) >= EPSILON && dst >= 0 && u >= 0 && v >= 0 && w >= 0;
	hitInfo.hit = abs(determinant) >= EPSILON && dst >= 0 && u >= 0 && v >= 0 && w >= 0;

	// hitInfo.hit = determinant >= EPSILON && dst >= 0 && u >= 0 && v >= 0 && w >= 0;  // Note the use of fabs
    hitInfo.position = ray.origin + ray.dir * dst;
    hitInfo.normal = normalize(normalVector);
    hitInfo.distance = dst;
    // hitInfo.material = tri.material;

	hitInfo.material.albedo = (hitInfo.normal * 0.5) + 0.5;
	// hitInfo.material.albedo = 0.5 * (normalVector + vec3(1.0));
	// hitInfo.material.albedo = vec3(u, v, w);
    return hitInfo;
}

HitInfo RayTriangleBVH(Ray ray)
{
	// bvh nodeStack[64];
	int nodeStack[64];
	int stackIndex = 0;
	nodeStack[stackIndex++] = 0;

	HitInfo hitInfo;
	hitInfo.hit = false;
	hitInfo.distance = INFINITY;

	while (stackIndex > 0)
	{
		// int nodeIndex = nodeStack[--stackIndex];
		bvh node = bvhData.data[nodeStack[--stackIndex]];

		// if (RayBoundingBoxDst(ray, node.Min, node.Max) < hitInfo.distance)
		{
			if (node.Left == 0)
			{
				int i = node.TriangleIndex;
				int maxTri = node.TriangleCount + i;
				while (i < maxTri)
				{
					HitInfo triHit = RayTriangle(ray, triBuffer.data[i]);
					if (triHit.hit && triHit.distance < hitInfo.distance)
						hitInfo = triHit;
					i++;
				}
			}
			else
			{
				int leftIndex = node.Left;
				int rightIndex = node.Left + 1;
				float leftDst = RayBoundingBoxDst(ray, bvhData.data[leftIndex].Min, bvhData.data[leftIndex].Max);
				float rightDst = RayBoundingBoxDst(ray, bvhData.data[rightIndex].Min, bvhData.data[rightIndex].Max);

				// bool isNearestLeft = leftDst < rightDst;
				// float nearestDst = isNearestLeft ? leftDst : rightDst;
				// float farthestDst = isNearestLeft ? rightDst : leftDst;
				// int childIndexNear = isNearestLeft ? leftIndex : rightIndex;
				// int childIndexFar = isNearestLeft ? rightIndex : leftIndex;

				// if (farthestDst < hitInfo.distance)
				// {
				// 	nodeStack[stackIndex++] = childIndexFar;
				// }
				// if (nearestDst < hitInfo.distance)
				// {
				// 	nodeStack[stackIndex++] = childIndexNear;
				// }


				if (leftDst != INFINITY && rightDst != INFINITY )
				{
					if (leftDst < rightDst && leftDst < hitInfo.distance)
					{
						nodeStack[stackIndex++] = rightIndex;
						nodeStack[stackIndex++] = leftIndex;
					}
					else if (rightDst < hitInfo.distance)
					{
						nodeStack[stackIndex++] = leftIndex;
						nodeStack[stackIndex++] = rightIndex;
					}
				}
				else if (leftDst != INFINITY && leftDst < hitInfo.distance)
				{
					nodeStack[stackIndex++] = leftIndex;
				}
				else if (rightDst != INFINITY && rightDst < hitInfo.distance)
				{
					nodeStack[stackIndex++] = rightIndex;
				}
			}
		}
	}
	return hitInfo;
}

vec4 TraceRay(Ray ray)
{
	vec3 color;
	// HitInfo hitInfo = PreviewBVH(ray, 1);
	HitInfo hitInfo;
	hitInfo.hit = false;
	hitInfo.distance = INFINITY;

	// int index = 2;
	// int i = bvhData.data[index].TriangleIndex;
	// int maxTri = bvhData.data[index].TriangleCount + i;
	// i = 4815;
	// maxTri = 3896;
	// int i = 0;
	// int maxTri = u_TriangleCount;
	// while (i < maxTri)
	// {
	// 	HitInfo triHit = RayTriangle(ray, triBuffer.data[i]);
	// 	if (triHit.hit && triHit.distance < hitInfo.distance)
	// 	if (triHit.hit)
	// 		hitInfo = triHit;
	// 	i++;
	// }

	hitInfo = RayTriangleBVH(ray);

	if (hitInfo.hit)
	{
		color = hitInfo.material.albedo;
		// color = vec3(1.0, 0.0, 0.0);
		// color = vec3(0.0, 0.0, 0.0);
	}
	else
	{
		color = SampleSkybox(ray.dir).xyz;
		color = vec3(0.0, 0.0, 0.0);
		// color = vec3(1.0, 0.0, 0.0);
	}

	// color 

	vec4 finalColor = vec4(color, 1.0);

	return finalColor;
}




void main()
{
	ivec2 pixelID = ivec2(gl_GlobalInvocationID.xy);
	// imageStore(img_output, pixelID, vec4(1.0, 0.0, 0.0, 1.0));
	// return ;

	vec2 coord = vec2(float(pixelID.x) / float(u_ScreenWidth - 1), float(pixelID.y) / float(u_ScreenHeight - 1));
	coord = coord * 2.0 - 1.0;


	vec4 target = u_InvProj * vec4(coord.x, coord.y, 1.0, 1.0);
	vec3 rayDirection = vec3(u_InvView * vec4(normalize(vec3(target.xyz) / target.w), 0.0)).xyz; // Convert to world space
	rayDirection = normalize(rayDirection);

	Ray ray;
	ray.origin = u_CameraPos;
	ray.dir = rayDirection;
	ray.invDir = 1.0 / rayDirection;

	vec4 finalColor = TraceRay(ray);
	imageStore(img_output, pixelID, finalColor);
	return;
}