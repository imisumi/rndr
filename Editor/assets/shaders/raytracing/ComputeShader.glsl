#version 450 core

#include "globals.glsl"
#include "uniforms.glsl"

#include "raygen.glsl"
#include "intersection.glsl"
#include "miss.glsl"
#include "closesthit.glsl"
#include "random.glsl"


//TODO: maybe transform the ray to object space instead of transforming every aabb and triangle
AABB RotateAABB(AABB aabb, mat4 transform)
{
	vec3 newMin = vec3(INFINITY);
	vec3 newMax = vec3(-INFINITY);

	// Precompute the transformed corners
	const vec3 corners[8] = {
		vec3(aabb.Min.x, aabb.Min.y, aabb.Min.z),
		vec3(aabb.Min.x, aabb.Min.y, aabb.Max.z),
		vec3(aabb.Min.x, aabb.Max.y, aabb.Min.z),
		vec3(aabb.Min.x, aabb.Max.y, aabb.Max.z),
		vec3(aabb.Max.x, aabb.Min.y, aabb.Min.z),
		vec3(aabb.Max.x, aabb.Min.y, aabb.Max.z),
		vec3(aabb.Max.x, aabb.Max.y, aabb.Min.z),
		vec3(aabb.Max.x, aabb.Max.y, aabb.Max.z)
	};

	// Transform each corner and update newMin/newMax
	for (int i = 0; i < 8; ++i) {
		vec3 newCorner = vec3(transform * vec4(corners[i], 1.0));
		newMin = min(newMin, newCorner);
		newMax = max(newMax, newCorner);
	}

	return AABB(newMin, newMax);
}

HitInfo BLASIntersection(Ray ray, BLAS blas)
{
	// bvh nodeStack[64];
	int nodeStack[32];
	int stackIndex = 0;
	nodeStack[stackIndex++] = blas.BufferIndex + 0;

	HitInfo hitInfo;
	hitInfo.hit = false;
	hitInfo.distance = INFINITY;

	mat4 invTransform = inverse(blas.Transform);


	ray.origin = vec3(invTransform * vec4(ray.origin, 1.0));
	ray.dir = vec3(invTransform * vec4(ray.dir, 0.0));
	ray.dir = normalize(ray.dir);
	ray.invDir = 1.0 / ray.dir;

	while (stackIndex > 0)
	{
		bvh node = bvhData.data[nodeStack[--stackIndex]];
		{
			if (node.Left == 0) //? if child/left == 0 then it's a leaf node
			{
				int triOffset = node.TriangleIndex + blas.TriangleOffset;
				int triCount = node.TriangleCount;
				for (int i = triOffset; i < triOffset + triCount; i++)
				{
					Triangle tri = triBuffer.data[i];
					HitInfo triHit = RayTriangle(ray, tri);
					if (triHit.hit && triHit.distance < hitInfo.distance)
					{
						hitInfo = triHit;
						hitInfo.position = vec3(blas.Transform * vec4(hitInfo.position, 1.0));
						hitInfo.normal = vec3(blas.Transform * vec4(hitInfo.normal, 0.0));
						//TODO; uv coords as well
					}
				}
			}
			else
			{
				int leftIndex = node.Left + blas.BufferIndex;
				int rightIndex = node.Left + blas.BufferIndex + 1;

				AABB leftAABB = AABB(bvhData.data[leftIndex].Min, bvhData.data[leftIndex].Max);
				AABB rightAABB = AABB(bvhData.data[rightIndex].Min, bvhData.data[rightIndex].Max);

				float leftDst = RayBoundingBoxDst(ray, leftAABB);
				float rightDst = RayBoundingBoxDst(ray, rightAABB);

				//? Working
				bool isNearestLeft = leftDst <= rightDst;
				float nearestDst = isNearestLeft ? leftDst : rightDst;
				float farthestDst = isNearestLeft ? rightDst : leftDst;
				int childIndexNear = isNearestLeft ? leftIndex : rightIndex;
				int childIndexFar = isNearestLeft ? rightIndex : leftIndex;

				if (farthestDst < hitInfo.distance)
				{
					nodeStack[stackIndex++] = childIndexFar;
				}
				if (nearestDst < hitInfo.distance)
				{
					nodeStack[stackIndex++] = childIndexNear;
				}
			}
		}
	}
	return hitInfo;
}

vec4 TraceRay(Ray ray, inout uint seed)
{
	Sphere sphere;
	sphere.position = vec3(0.0, -500.0, 0.0);
	sphere.radius = 500.0;
	sphere.material.albedo = vec3(0.8549, 0.3451, 0.6706);
	sphere.material.emission = vec3(0.0);
	sphere.material.emissionStrength = 0.0;

	Sphere sphere2;
	sphere2.position = vec3(0.0, 1.0, 0.0);
	sphere2.radius = 1.0;
	sphere2.material.albedo = vec3(0.8549, 0.3451, 0.6706);
	sphere2.material.emission = vec3(0.0);
	sphere2.material.emissionStrength = 0.0;



	vec3 incomingLight  = vec3(0.0);
	vec3 rayColor = vec3(1.0);

	int maxBounces = 5;
	for (int b = 0; b <= maxBounces; b++)
	{
		HitInfo hitinfo;
		hitinfo.hit = false;
		hitinfo.distance = INFINITY;
		for (int i = 0; i < u_BLASCount; i++)
		{
			HitInfo blasHit = BLASIntersection(ray, blasData.data[i]);
			if (blasHit.hit && blasHit.distance < hitinfo.distance)
				hitinfo = blasHit;
		}
		// hitinfo = RaySphere(ray, sphere, hitinfo);
		// hitinfo = RaySphere(ray, sphere2, hitinfo);
		if (!hitinfo.hit)
		{
			vec4 sky = vec4(simpleSky(ray.dir), 1.0);
			// vec4 sky = SampleSkybox(ray.dir);
			incomingLight += (sky.xyz * rayColor);
			break ;
		}
		
		// ray.origin = hitinfo.position;
		ray.origin = hitinfo.position + (hitinfo.normal * 0.001);
		ray.dir = RandomHemisphereDirection(seed, hitinfo.normal);
		ray.invDir = 1.0 / ray.dir;
		// ray.dir = normalize(hitinfo.normal + RandomDirection(seed));

		Material material = hitinfo.material;
		// vec3 color = hitinfo.normal * 0.5 + 0.5;
		vec3 color = vec3(0.7216, 0.7216, 0.7216);
		rayColor *= color;
	}
	return vec4(incomingLight, 1.0);

}


void main()
{
	ivec2 pixelID = ivec2(gl_GlobalInvocationID.xy);

	Ray ray = RayGen();

	uint seed = generateSeed() * 719393 * u_FrameCount;

	// vec4 finalColor = TraceRay(ray, seed);
	vec4 finalColor = vec4(0.0f);

	int samples = 1;
	for (int i = 0; i < samples; i++)
	{
		finalColor += TraceRay(ray, seed);
	}

	finalColor /= samples;
	finalColor.a = 1.0;

	// get vec4 of current pixel
	// vec4 accumulatedBuffer = imageLoad(img_output, pixelID);

	// imageStore(img_output, pixelID, finalColor);

	// imageStore(img_output, pixelID, finalColor);
	if (u_FrameCount == 1)
	{
		imageStore(img_output, pixelID, finalColor);
	}
	else
	{
		vec4 accumulatedBuffer = imageLoad(img_output, pixelID);
		imageStore(img_output, pixelID, finalColor + accumulatedBuffer);
	}
	return;
}