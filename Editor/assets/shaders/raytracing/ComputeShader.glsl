#version 450 core

#include "globals.glsl"
#include "uniforms.glsl"

#include "raygen.glsl"
#include "intersection.glsl"
#include "miss.glsl"
#include "closesthit.glsl"
#include "random.glsl"



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


#if 0
vec4 TraceRay(Ray ray, inout uint seed)
{
	// vec3 color;
	// // HitInfo hitInfo = PreviewBVH(ray, 1);
	// HitInfo hitInfo;
	// hitInfo.hit = false;
	// hitInfo.distance = INFINITY;

	// hitInfo = RayTriangleBVH(ray);

	float y = -1.1;
	Triangle triOne;
	triOne.posA = vec3(-10.0, y, -10.0);
	triOne.posB = vec3(-10.0, y, 10.0);
	triOne.posC = vec3(10.0, y, -10.0);
	Triangle triTwo;
	triTwo.posA = vec3(10.0, y, -10.0);
	triTwo.posB = vec3(-10.0, y, 10.0);
	triTwo.posC = vec3(10.0, y, 10.0);


	// HitInfo triOnee = RayTriangle(ray, triOne);
	// HitInfo triTwoo = RayTriangle(ray, triTwo);

	// // if (triOnee.hit && triOnee.distance < hitInfo.distance)
	// // 	hitInfo = triOnee;
	// // if (triTwoo.hit && triTwoo.distance < hitInfo.distance)
	// // 	hitInfo = triTwoo;

	// if (hitInfo.hit)
	// {
	// 	color = hitInfo.material.albedo;
	// 	// color = vec3(1.0, 0.0, 0.0);
	// 	// color = vec3(0.0, 0.0, 0.0);
	// }
	// else
	// {
	// 	color = SampleSkybox(ray.dir).xyz;
	// 	// color = vec3(0.0, 0.0, 0.0);
	// 	// color = vec3(1.0, 0.0, 0.0);
	// }



	// HitInfo hitinfo;
	// hitinfo.hit = false;
	// hitinfo.distance = INFINITY;

	vec3 radiance = vec3(0.0);
	vec3 throughput = vec3(1.0);

	int maxBounces = 4;

	for (int i = 0; i < maxBounces; i++)
	{
		HitInfo hitinfo;
		hitinfo.hit = false;
		hitinfo.distance = INFINITY;

		hitinfo = RayTriangleBVH(ray);

		HitInfo triOnee = RayTriangle(ray, triOne);
		HitInfo triTwoo = RayTriangle(ray, triTwo);

		if (triOnee.hit && triOnee.distance < hitinfo.distance)
			hitinfo = triOnee;
		if (triTwoo.hit && triTwoo.distance < hitinfo.distance)
			hitinfo = triTwoo;
		if (hitinfo.hit)
		{
			ray.origin = hitinfo.position + hitinfo.normal * 0.001;
			ray.dir = RandomHemisphereDirection(seed, hitinfo.normal);

			radiance += hitinfo.material.albedo;
			// throughput *= hitinfo.material.albedo;
		}
		else
		{
			if (i == 0)
				return vec4(0.0f);
			vec4 sky = SampleSkybox(ray.dir);
			radiance *= vec3(sky.xyz);
			break;
		}
	}


	vec4 finalColor = vec4(radiance, 1.0);

	return finalColor;
}
#endif


// vec4 TraceRay(Ray ray, inout uint seed)
// {
// 	float y = -1.1;
// 	Triangle triOne;
// 	triOne.posA = vec3(-10.0, y, -10.0);
// 	triOne.posB = vec3(-10.0, y, 10.0);
// 	triOne.posC = vec3(10.0, y, -10.0);
// 	Triangle triTwo;
// 	triTwo.posA = vec3(10.0, y, -10.0);
// 	triTwo.posB = vec3(-10.0, y, 10.0);
// 	triTwo.posC = vec3(10.0, y, 10.0);


// 	vec3 incomingLight  = vec3(0.0);
// 	vec3 rayColor = vec3(1.0);

// 	vec3 ligtDir = normalize(vec3(-1.0, -1.0, -1.0));

// 	int maxBounces = 4;

// 	for (int i = 0; i < maxBounces; i++)
// 	{
// 		HitInfo hitinfo;
// 		hitinfo.hit = false;
// 		hitinfo.distance = INFINITY;

// 		hitinfo = RayTriangleBVH(ray);

// 		HitInfo triOnee = RayTriangle(ray, triOne);
// 		HitInfo triTwoo = RayTriangle(ray, triTwo);

// 		if (triOnee.hit && triOnee.distance < hitinfo.distance)
// 			hitinfo = triOnee;
// 		if (triTwoo.hit && triTwoo.distance < hitinfo.distance)
// 			hitinfo = triTwoo;
// 		if (hitinfo.hit)
// 		{
// 			ray.origin = hitinfo.position + hitinfo.normal * 0.001;
// 			ray.dir = RandomHemisphereDirection(seed, hitinfo.normal);
// 			// ray.dir = reflect(ray.dir, hitinfo.normal);

// 			// incomingLight  += hitinfo.material.albedo;
// 			// rayColor *= hitinfo.material.albedo;

// 			float d = dot(hitinfo.normal, -ligtDir);
// 			d = max(d, 0.0);

// 			// float emmitedLight = 0.0;
// 			// incomingLight  += emmitedLight * rayColor;
// 			rayColor *= hitinfo.material.albedo * d;
// 		}
// 		else
// 		{
// 			// if (i == 0)
// 				// return vec4(0.0f);
// 			vec4 sky = SampleSkybox(ray.dir);
// 			sky = vec4(0.0);
// 			// incomingLight  *= vec3(sky.xyz);
// 			incomingLight  += sky.xyz * rayColor;
// 			break;
// 		}
// 	}

// 	vec4 finalColor = vec4(incomingLight , 1.0);
// 	return finalColor;
// }


vec3 simpleSky(vec3 normal)
{
	vec3 unitDirection = normalize(normal);
	float t = 0.5 * (unitDirection.y + 1.0);
	return mix(vec3(1.0), vec3(0.5, 0.7, 1.0), t);
}

vec4 TraceRay(Ray ray, inout uint seed)
{

	// Sphere sphere;
	// sphere.position = vec3(0.0, -50.0, 0.0);
	// sphere.radius = 50.0;
	// sphere.material.albedo = vec3(0.5725, 0.6275, 0.651);
	// sphere.material.emission = vec3(0.0);
	// sphere.material.emissionStrength = 0.0
	
	Sphere sphere;
	sphere.position = vec3(0.0, -50.0, 0.0);
	sphere.radius = 50.0;
	sphere.material.albedo = vec3(0.8549, 0.3451, 0.6706);
	sphere.material.emission = vec3(0.0);
	sphere.material.emissionStrength = 0.0;

	Sphere sphere2;
	sphere2.position = vec3(0.0, 1.0, 0.0);
	sphere2.radius = 1.0;
	sphere2.material.albedo = vec3(0.8549, 0.3451, 0.6706);
	sphere2.material.emission = vec3(0.0);
	sphere2.material.emissionStrength = 0.0;

	// Sphere sphere2;
	// sphere2.position = vec3(0.0, 1.0, 0.0);
	// sphere2.radius = 1.0;
	// sphere2.material.albedo = vec3(0.698, 0.698, 0.698);
	// sphere2.material.emission = vec3(1.0, 1.0, 1.0);
	// sphere2.material.emissionStrength = 1.0;

	float y = -1.1;
	Triangle triOne;
	triOne.posA = vec3(-10.0, y, -10.0);
	triOne.posB = vec3(-10.0, y, 10.0);
	triOne.posC = vec3(10.0, y, -10.0);
	Triangle triTwo;
	triTwo.posA = vec3(10.0, y, -10.0);
	triTwo.posB = vec3(-10.0, y, 10.0);
	triTwo.posC = vec3(10.0, y, 10.0);





	vec3 incomingLight  = vec3(0.0);
	vec3 rayColor = vec3(1.0);

	int maxBounces = 1;

	HitInfo hitinfo;
	hitinfo.hit = false;
	hitinfo.distance = INFINITY;
	for (int i = 0; i <= maxBounces; i++)
	{
		hitinfo.hit = false;
		hitinfo.distance = INFINITY;
		// hitinfo = RaySphere(ray, sphere, hitinfo);

		// HitInfo hitinfo2 = RayTriangleBVH(ray);
		// if (hitinfo2.hit && hitinfo2.distance < hitinfo.distance)
		// 	hitinfo = hitinfo2;

		// HitInfo triOnee = RayTriangle(ray, triOne);
		// HitInfo triTwoo = RayTriangle(ray, triTwo);

		// if (triOnee.hit && triOnee.distance < hitinfo.distance)
		// 	hitinfo = triOnee;
		// if (triTwoo.hit && triTwoo.distance < hitinfo.distance)
		// 	hitinfo = triTwoo;

		// hitinfo = RaySphere(ray, sphere, hitinfo);
		// hitinfo = RaySphere(ray, sphere2, hitinfo);

		hitinfo = RayTriangleBVH(ray);

		if (!hitinfo.hit)
		{
			vec4 sky = SampleSkybox(ray.dir);
			// vec4 sky = vec4(simpleSky(ray.dir), 1.0);
			incomingLight += (sky.xyz * rayColor);
			break ;
		}

		ray.origin = hitinfo.position + hitinfo.normal * 0.001;
		// ray.origin = hitinfo.position;
		ray.dir = RandomHemisphereDirection(seed, hitinfo.normal);

		Material material = hitinfo.material;
		// vec3 emittedLight = material.emission * material.emissionStrength;
		// incomingLight += emittedLight * rayColor;


		material.albedo = vec3(0.7529, 0.7529, 0.7529);
		rayColor *= material.albedo;

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