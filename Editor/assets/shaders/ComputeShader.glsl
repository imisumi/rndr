#version 450 core

#include "globals.glsl"
#include "uniforms.glsl"

#include "raygen.glsl"



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
	// hitInfo.hit = abs(determinant) >= EPSILON && dst >= 0 && u >= 0 && v >= 0 && w >= 0;

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

	Ray ray = RayGen();

	vec4 finalColor = TraceRay(ray);
	imageStore(img_output, pixelID, finalColor);
	return;
}