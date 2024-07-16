// #version 450 core
// layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
// layout(binding = 0, rgba32f) writeonly uniform image2D img_output;
// layout(binding = 1, rgba32f) readonly uniform image2D u_SkyTexture;

// layout(std430, binding = 2) buffer vertexPositions {
// 	vec3 data[]; // send as glm::vec4 but becuase of memory alignment, I just use vec3
// } positionBuffer;

// layout(std430, binding = 3) buffer vertexIndices {
// 	uint data[]; // send as glm::vec4 but becuase of memory alignment, I just use vec3
// } indexBuffer;

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


// uniform int u_ScreenWidth;
// uniform int u_ScreenHeight;

// uniform int u_SkyTextureWidth;
// uniform int u_SkyTextureHeight;

// uniform mat4 u_InvProj;
// uniform mat4 u_InvView;

// uniform vec3 u_CameraPos;


// uniform uint u_MaxIndices;

// uniform uint u_ShowDepth;


// const float PI = 3.14159265359;
// const float EPSILON = 0.0001;

// #define INFINITY (1.0 / 0.0)
// #define NEGATIVE_INFINITY (-1.0 / 0.0)

// struct Material
// {
// 	vec3 albedo;
// };

// struct HitInfo
// {
// 	bool hit;
// 	vec3 position;
// 	vec3 normal;
// 	float distance;
// 	Material material;
// };

// struct Ray
// {
// 	vec3 origin;
// 	vec3 dir;
// 	vec3 invDir;
// };

// struct Sphere
// {
// 	vec3 position;
// 	float radius;
// 	Material material;
// };

// struct Triangle
// {
// 	vec3 posA, posB, posC;
// 	vec3 normal;
// 	Material material;
// };




// // vec3 linearToSRGB(vec3 linearRGB, float exposure) {
// //     // Apply exposure adjustment
// //     linearRGB *= exposure;
    
// //     // Convert linear RGB to sRGB
// //     vec3 sRGB = mix(12.92 * linearRGB, 1.055 * pow(linearRGB, vec3(1.0 / 2.4)) - 0.055, step(0.0031308, linearRGB));
    
// //     return sRGB;
// // }

// vec2 SampleUVFromDirection(vec3 direction) {
//     // Normalize direction vector
//     direction = normalize(direction);

//     // Convert direction to spherical coordinates (latitude and longitude)
//     float longitude = atan(direction.z, direction.x);
//     float latitude = acos(direction.y);

//     // Map spherical coordinates to UV coordinates
//     float u = (longitude + PI) / (2.0 * PI);
//     float v = latitude / PI;

//     return vec2(u, v);
// }

// vec4 SampleSkybox(vec3 direction)
// {
// 	vec2 uv = SampleUVFromDirection(direction);
// 	vec4 sky = imageLoad(u_SkyTexture, ivec2(uv * vec2(u_SkyTextureWidth, u_SkyTextureHeight)));
// 	// return vec4(uv, 0.0, 1.0);
// 	return sky;
// 	// return texture(skyBox, uv);
// }

// uint generateSeed()
// {
// 	ivec2 pixelID = ivec2(gl_GlobalInvocationID.xy);
// 	vec2 uv = vec2(float(pixelID.x) / float(u_ScreenWidth - 1), float(pixelID.y) / float(u_ScreenHeight - 1));

// 	ivec2 numPixels = ivec2(u_ScreenWidth, u_ScreenHeight);
// 	ivec2 pixelCoord = ivec2(uv * vec2(numPixels - 1));

// 	uint seed = pixelCoord.y * numPixels.x + pixelCoord.x; // use this as color output for visulizing each random pixel
// 	return seed;
// }


// float RandomFloat(inout uint seed)
// {
// 	seed = seed * 747796405 + 2891336453;
// 	uint result = ((seed >> ((seed >> 28) + 4)) ^ seed) * 277803737;
// 	result = (result >> 22) ^ result;
// 	return float(result) / float(4294967295.0);
// }

// HitInfo RaySphere(Ray ray, Sphere sphere, HitInfo hitInfo)
// {
// 	vec3 offsetRayOrigin = ray.origin - sphere.position;

// 	float a = dot(ray.dir, ray.dir);
// 	float b = 2.0 * dot(ray.dir, offsetRayOrigin);
// 	float c = dot(offsetRayOrigin, offsetRayOrigin) - sphere.radius * sphere.radius;

// 	float discriminant = b * b - 4.0 * a * c;

// 	if (discriminant > 0.0)
// 	{
// 		float dist = (-b - sqrt(discriminant)) / (2.0 * a);
// 		if (dist > 0.0 && dist < hitInfo.distance)
// 		{
// 			hitInfo.hit = true;
// 			hitInfo.position = ray.origin + ray.dir * dist;
// 			hitInfo.normal = normalize(hitInfo.position - sphere.position);
// 			hitInfo.distance = dist;
// 			hitInfo.material = sphere.material;
// 		}
// 	}
// 	return hitInfo;
// }

// vec3 ACESFilm(vec3 x)
// {
//     const float a = 2.51;
//     const float b = 0.03;
//     const float c = 2.43;
//     const float d = 0.59;
//     const float e = 0.14;
//     return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
// }

// vec3 LinearToSRGB(vec3 color)
// {
//     return mix(pow(color, vec3(1.0 / 2.4)) * 1.055 - 0.055, color * 12.92, lessThanEqual(color, vec3(0.0031308)));
// }

// vec3 ApplyExposure(vec3 color, float exposure) {
//     return color * pow(2.0, exposure);
// }

// HitInfo RayTriangle(Ray ray, Triangle tri)
// {
//     vec3 edgeAB = tri.posB - tri.posA;
//     vec3 edgeAC = tri.posC - tri.posA;
// 	vec3 normalVector = cross(edgeAB, edgeAC);
//     vec3 ao = ray.origin - tri.posA;
//     vec3 dao = cross(ao, ray.dir);

//     float determinant = -dot(ray.dir, normalVector);
//     float invDet = 1 / determinant;

//     // Calculate distance to triangle & barycentric coordinates of intersection point
//     float dst = dot(ao, normalVector) * invDet;
//     float u = dot(edgeAC, dao) * invDet;
//     float v = -dot(edgeAB, dao) * invDet;
//     float w = 1 - u - v;

//     // Initialize hit info
//     HitInfo hitInfo;
//     hitInfo.hit = abs(determinant) >= EPSILON && dst >= 0 && u >= 0 && v >= 0 && w >= 0;
// 	// hitInfo.hit = determinant >= EPSILON && dst >= 0 && u >= 0 && v >= 0 && w >= 0;  // Note the use of fabs
//     hitInfo.position = ray.origin + ray.dir * dst;
//     hitInfo.normal = normalize(normalVector);
//     hitInfo.distance = dst;
//     // hitInfo.material = tri.material;

// 	hitInfo.material.albedo = (hitInfo.normal * 0.5) + 0.5;
// 	// hitInfo.material.albedo = 0.5 * (normalVector + vec3(1.0));
// 	// hitInfo.material.albedo = vec3(u, v, w);
//     return hitInfo;
// }

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



// struct StackEntry
// {
// 	tempBVH node;
// 	int depth;
// };

// // HitInfo PreviewBVH(Ray ray, int showDepth)
// // {
// // 	StackEntry stack[32];
// // 	int stackIndex = 0;

// // 	stack[stackIndex].node = bvhNodeBuffer.data[0];
// // 	stack[stackIndex].depth = 0;
// // 	stackIndex++;


// // 	HitInfo hitInfo;
// // 	hitInfo.hit = false;
// // 	hitInfo.distance = INFINITY;

// // 	showDepth = int(u_ShowDepth);

// // 	vec3 colors[16];
// // 	colors[0]  = vec3(1.0, 0.0, 0.0); // Red
// // 	colors[1]  = vec3(0.0, 1.0, 0.0); // Green
// // 	colors[2]  = vec3(0.0, 0.0, 1.0); // Blue
// // 	colors[3]  = vec3(1.0, 1.0, 0.0); // Yellow
// // 	colors[4]  = vec3(1.0, 0.0, 1.0); // Magenta
// // 	colors[5]  = vec3(0.0, 1.0, 1.0); // Cyan
// // 	colors[6]  = vec3(0.5, 0.0, 0.0); // Dark Red
// // 	colors[7]  = vec3(0.0, 0.5, 0.0); // Dark Green
// // 	colors[8]  = vec3(0.0, 0.0, 0.5); // Dark Blue
// // 	colors[9]  = vec3(0.5, 0.5, 0.0); // Olive
// // 	colors[10] = vec3(0.5, 0.0, 0.5); // Purple
// // 	colors[11] = vec3(0.0, 0.5, 0.5); // Teal
// // 	colors[12] = vec3(1.0, 0.5, 0.0); // Orange
// // 	colors[13] = vec3(0.5, 1.0, 0.0); // Lime
// // 	colors[14] = vec3(0.0, 1.0, 0.5); // Spring Green
// // 	colors[15] = vec3(0.5, 0.0, 1.0); // Violet


// // 	int index = 0;
// // 	while (stackIndex > 0)
// // 	{
// // 		StackEntry entry = stack[--stackIndex];
// // 		tempBVH node = entry.node;
// // 		int depth = entry.depth;

// // 		vec3 min = node.min.xyz;
// // 		vec3 max = node.max.xyz;

// // 		float dst = RayBoundingBoxDst(ray, min, max);
// // 		// if ((dst != INFINITY))
// // 		// {
// // 			if (depth == showDepth && node.ChildIndex != 0)
// // 			{
// // 				if (dst < hitInfo.distance)
// // 				{
// // 					hitInfo.distance = dst;
// // 					hitInfo.material.albedo = colors[index % 16];
// // 					hitInfo.hit = true;
// // 				}
// // 			}
// // 			else if (depth < showDepth && node.ChildIndex != 0)
// // 			{
// // 				int childDepth = depth + 1;
// // 				if (bool(RayBoundingBoxDst(ray, bvhNodeBuffer.data[node.ChildIndex].min.xyz, bvhNodeBuffer.data[node.ChildIndex].max.xyz)))
// // 				{
// // 					stack[stackIndex].node = bvhNodeBuffer.data[node.ChildIndex];
// // 					stack[stackIndex].depth = childDepth;
// // 					stackIndex++;
// // 				}
// // 				if (bool(RayBoundingBoxDst(ray, bvhNodeBuffer.data[node.ChildIndex + 1].min.xyz, bvhNodeBuffer.data[node.ChildIndex + 1].max.xyz)))
// // 				{
// // 					stack[stackIndex].node = bvhNodeBuffer.data[node.ChildIndex + 1];
// // 					stack[stackIndex].depth = childDepth;
// // 					stackIndex++;
// // 				}
// // 				// stack[stackIndex].node = bvhNodeBuffer.data[node.ChildIndex];
// // 				// stack[stackIndex].depth = childDepth;
// // 				// stackIndex++;
// // 				// stack[stackIndex].node = bvhNodeBuffer.data[node.ChildIndex + 1];
// // 				// stack[stackIndex].depth = childDepth;
// // 				// stackIndex++;
// // 			}
// // 		// }
// // 		index++;
// // 	}
// // 	return hitInfo;
// // }


// HitInfo PreviewBVH(Ray ray, int showDepth)
// {
// 	vec3 colors[16];
// 	colors[0]  = vec3(1.0, 0.0, 0.0); // Red
// 	colors[1]  = vec3(0.0, 1.0, 0.0); // Green
// 	colors[2]  = vec3(0.0, 0.0, 1.0); // Blue
// 	colors[3]  = vec3(1.0, 1.0, 0.0); // Yellow
// 	colors[4]  = vec3(1.0, 0.0, 1.0); // Magenta
// 	colors[5]  = vec3(0.0, 1.0, 1.0); // Cyan
// 	colors[6]  = vec3(0.5, 0.0, 0.0); // Dark Red
// 	colors[7]  = vec3(0.0, 0.5, 0.0); // Dark Green
// 	colors[8]  = vec3(0.0, 0.0, 0.5); // Dark Blue
// 	colors[9]  = vec3(0.5, 0.5, 0.0); // Olive
// 	colors[10] = vec3(0.5, 0.0, 0.5); // Purple
// 	colors[11] = vec3(0.0, 0.5, 0.5); // Teal
// 	colors[12] = vec3(1.0, 0.5, 0.0); // Orange
// 	colors[13] = vec3(0.5, 1.0, 0.0); // Lime
// 	colors[14] = vec3(0.0, 1.0, 0.5); // Spring Green
// 	colors[15] = vec3(0.5, 0.0, 1.0); // Violet


// 	// StackEntry stack[32];
// 	// int stackIndex = 0;

// 	// stack[stackIndex].node = bvhNodeBuffer.data[0];
// 	// stack[stackIndex].depth = 0;
// 	// stackIndex++;


// 	// HitInfo hitInfo;
// 	// hitInfo.hit = false;
// 	// hitInfo.distance = INFINITY;

// 	// showDepth = int(u_ShowDepth);


// 	tempBVH nodeStack[16];
// 	int stackIndex = 0;
// 	nodeStack[stackIndex++] = bvhNodeBuffer.data[0];

// 	HitInfo hitInfo;
// 	hitInfo.hit = false;
// 	hitInfo.distance = INFINITY;

// 	int index = 0;
// 	while (stackIndex > 0)
// 	{
// 		tempBVH node = nodeStack[--stackIndex];
// 		vec3 min = node.min.xyz;
// 		vec3 max = node.max.xyz;

// 		float dst = RayBoundingBoxDst(ray, min, max);
// 		if (bool(dst))
// 		{
// 			if (node.ChildIndex == 0)
// 			{
// 				if (dst < hitInfo.distance)
// 				{
// 					hitInfo.distance = dst;
// 					hitInfo.material.albedo = colors[index % 16];
// 					hitInfo.hit = true;
// 				}
// 			}
// 			else
// 			{
// 				nodeStack[stackIndex++] = bvhNodeBuffer.data[node.ChildIndex + 0];
// 				nodeStack[stackIndex++] = bvhNodeBuffer.data[node.ChildIndex + 1];
// 			}
// 		}
// 		index++;
// 	}
// 	return hitInfo;
// }

// vec4 TraceRay(Ray ray)
// {
// 	Sphere sphere;
// 	sphere.position = vec3(0.0, 0.0, 0.0);
// 	sphere.radius = 0.5;
// 	sphere.material.albedo = vec3(0.6784, 0.6784, 0.6784);

// 	Triangle tri;
// 	tri.posA = vec3(-10.0, 0.0, -10.0);
// 	tri.posB = vec3(0.0, 0.0, 10.0);
// 	tri.posC = vec3(10.0, 0.0, -10.0);
// 	tri.posA = positionBuffer.data[indexBuffer.data[0]];
// 	tri.posB = positionBuffer.data[indexBuffer.data[1]];
// 	tri.posC = positionBuffer.data[indexBuffer.data[2]];
// 	tri.normal = vec3(0.0, 1.0, 0.0);
// 	tri.material.albedo = vec3(0.5922, 0.3098, 0.5451);



// 	HitInfo hitInfo;
// 	hitInfo.hit = false;
// 	hitInfo.distance = INFINITY;
// 	// for (uint i = 0; i < u_MaxIndices; i += 3)
// 	// {
// 	// 	tri.posA = positionBuffer.data[indexBuffer.data[i]];
// 	// 	tri.posB = positionBuffer.data[indexBuffer.data[i + 1]];
// 	// 	tri.posC = positionBuffer.data[indexBuffer.data[i + 2]];

// 	// 	HitInfo triHit = RayTriangle(ray, tri);
// 	// 	if (triHit.hit && triHit.distance < hitInfo.distance)
// 	// 	{
// 	// 		hitInfo = triHit;
// 	// 	}
// 	// }
// 	vec3 color;

// 	// if (RayBoundingBoxDst(ray, vec3(-3.0, -3.0, -3.0), vec3(3.0, 3.0, 3.0)) < hitInfo.distance)
// 	// {
// 	// 	color = vec3(0.2706, 0.6745, 0.4863);
// 	// }
// 	// else
// 	// {
// 	// 	color = SampleSkybox(ray.dir).xyz;
// 	// }
// 	vec3 min = bvhNodeBuffer.data[2].min.xyz;
// 	vec3 max = bvhNodeBuffer.data[2].max.xyz;

// 	// color = SampleSkybox(ray.dir).xyz;
// 	// if (hitInfo.hit)
// 	// {
// 	// 	color = hitInfo.material.albedo;
// 	// }
// 	// // if (RayBoundingBoxDst(ray, vec3(-3.0, -3.0, -3.0), vec3(3.0, 3.0, 3.0)) < hitInfo.distance)
// 	// if (RayBoundingBoxDst(ray, min, max) < hitInfo.distance)
// 	// {
// 	// 	color = vec3(0.2706, 0.6745, 0.4863);
// 	// }
// 	// else
// 	// {
// 	// 	color = SampleSkybox(ray.dir).xyz;
// 	// }

// 	hitInfo = PreviewBVH(ray, 1);
// 	if (hitInfo.hit)
// 	{
// 		color = hitInfo.material.albedo;
// 	}
// 	else
// 	{
// 		// color = SampleSkybox(ray.dir).xyz;
// 		color = vec3(0.0, 0.0, 0.0);
// 	}



// 	// HitInfo hitInfo;
// 	// hitInfo.hit = false;
// 	// hitInfo.distance = INFINITY;

// 	// // hitInfo = RaySphere(ray, sphere, hitInfo);
// 	// hitInfo = RayTriangle(ray, tri);
// 	// vec3 color;
// 	// if (hitInfo.hit)
// 	// {
// 	// 	color = hitInfo.material.albedo;
// 	// }
// 	// else
// 	// {
// 	// 	color = SampleSkybox(ray.dir).xyz;
// 	// }



// 	// color = ACESFilm(color);
// 	// color = ApplyExposure(color, -0.7);
// 	// color = LinearToSRGB(color);


// 	// color = bvhNodeBuffer.data[1].max.xyz;

// 	vec4 finalColor = vec4(color, 1.0);

// 	return finalColor;
// }




// void main()
// {
// 	ivec2 pixelID = ivec2(gl_GlobalInvocationID.xy);

// 	vec2 coord = vec2(float(pixelID.x) / float(u_ScreenWidth - 1), float(pixelID.y) / float(u_ScreenHeight - 1));
// 	coord = coord * 2.0 - 1.0;



// 	vec4 target = u_InvProj * vec4(coord.x, coord.y, 1.0, 1.0);
// 	vec3 rayDirection = vec3(u_InvView * vec4(normalize(vec3(target.xyz) / target.w), 0.0)).xyz; // Convert to world space
// 	rayDirection = normalize(rayDirection);

// 	Ray ray;
// 	ray.origin = u_CameraPos;
// 	ray.dir = rayDirection;
// 	ray.invDir = 1.0 / rayDirection;


// 	vec4 finalColor = TraceRay(ray);


// 	// vec4 finalColor = SampleSkybox(rayDirection);
// 	// vec3 srgb = linearToSRGB(finalColor.rgb, 0.5);
// 	// finalColor = vec4(srgb, 1.0);
// 	imageStore(img_output, pixelID, finalColor);
// 	return;



// 	// vec2 skyCoord = vec2(coord.x * float(u_SkyTextureWidth - 1), coord.y * float(u_SkyTextureHeight - 1));

// 	// ivec2 texCoord = ivec2(coord * vec2(u_SkyTextureWidth, u_SkyTextureHeight));

// 	// vec4 skyColor = imageLoad(u_SkyTexture, texCoord);

// 	// vec3 color = linearToSRGB(skyColor.rgb);
// 	// skyColor = vec4(color, 1.0);
//     // imageStore(img_output, pixelID, skyColor);
// }




















































































































#version 450 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(binding = 0, rgba32f) writeonly uniform image2D img_output;
layout(binding = 1, rgba32f) readonly uniform image2D u_SkyTexture;

layout(std430, binding = 2) buffer vertexPositions {
	vec3 data[]; // send as glm::vec4 but becuase of memory alignment, I just use vec3
} positionBuffer;

layout(std430, binding = 3) buffer vertexIndices {
	uint data[]; // send as glm::vec4 but becuase of memory alignment, I just use vec3
} indexBuffer;

struct tempBVH
{
	vec4 min;
	vec4 max;
	int ChildIndex;
	int padding1;
	int padding2;
	int padding3;
};

layout(std430, binding = 4) buffer bvhNodes {
	tempBVH data[];
} bvhNodeBuffer;


uniform int u_ScreenWidth;
uniform int u_ScreenHeight;

uniform int u_SkyTextureWidth;
uniform int u_SkyTextureHeight;

uniform mat4 u_InvProj;
uniform mat4 u_InvView;

uniform vec3 u_CameraPos;


uniform uint u_MaxIndices;

uniform uint u_ShowDepth;


const float PI = 3.14159265359;
const float EPSILON = 0.0001;

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

struct Triangle
{
	vec3 posA, posB, posC;
	vec3 normal;
	Material material;
};

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


// HitInfo PreviewBVH(Ray ray, int showDepth)
// {
// 	vec3 colors[16];
// 	colors[0]  = vec3(1.0, 0.0, 0.0); // Red
// 	colors[1]  = vec3(0.0, 1.0, 0.0); // Green
// 	colors[2]  = vec3(0.0, 0.0, 1.0); // Blue
// 	colors[3]  = vec3(1.0, 1.0, 0.0); // Yellow
// 	colors[4]  = vec3(1.0, 0.0, 1.0); // Magenta
// 	colors[5]  = vec3(0.0, 1.0, 1.0); // Cyan
// 	colors[6]  = vec3(0.5, 0.0, 0.0); // Dark Red
// 	colors[7]  = vec3(0.0, 0.5, 0.0); // Dark Green
// 	colors[8]  = vec3(0.0, 0.0, 0.5); // Dark Blue
// 	colors[9]  = vec3(0.5, 0.5, 0.0); // Olive
// 	colors[10] = vec3(0.5, 0.0, 0.5); // Purple
// 	colors[11] = vec3(0.0, 0.5, 0.5); // Teal
// 	colors[12] = vec3(1.0, 0.5, 0.0); // Orange
// 	colors[13] = vec3(0.5, 1.0, 0.0); // Lime
// 	colors[14] = vec3(0.0, 1.0, 0.5); // Spring Green
// 	colors[15] = vec3(0.5, 0.0, 1.0); // Violet


// 	tempBVH nodeStack[10];
// 	int stackIndex = 0;
// 	nodeStack[stackIndex++] = bvhNodeBuffer.data[0];

// 	HitInfo hitInfo;
// 	hitInfo.hit = false;
// 	hitInfo.distance = INFINITY;

// 	int index = 0;
// 	while (stackIndex > 0)
// 	{
// 		tempBVH node = nodeStack[--stackIndex];
// 		vec3 min = node.min.xyz;
// 		vec3 max = node.max.xyz;

// 		float dst = RayBoundingBoxDst(ray, min, max);
// 		// if (dst < hitInfo.distance)
// 		if (bool(dst))
// 		{
// 			if (node.ChildIndex == 0)
// 			{
// 				if (dst < hitInfo.distance)
// 				{
// 					hitInfo.distance = dst;
// 					hitInfo.material.albedo = colors[index % 16];
// 					hitInfo.hit = true;
// 				}
// 			}
// 			else
// 			{
// 				nodeStack[stackIndex++] = bvhNodeBuffer.data[node.ChildIndex + 0];
// 				nodeStack[stackIndex++] = bvhNodeBuffer.data[node.ChildIndex + 1];
// 			}
// 		}
// 		index++;
// 	}
// 	return hitInfo;
// }


// HitInfo PreviewBVH(Ray ray, int showDepth)
// {
// 	vec3 colors[16];
// 	colors[0]  = vec3(1.0, 0.0, 0.0); // Red
// 	colors[1]  = vec3(0.0, 1.0, 0.0); // Green
// 	colors[2]  = vec3(0.0, 0.0, 1.0); // Blue
// 	colors[3]  = vec3(1.0, 1.0, 0.0); // Yellow
// 	colors[4]  = vec3(1.0, 0.0, 1.0); // Magenta
// 	colors[5]  = vec3(0.0, 1.0, 1.0); // Cyan
// 	colors[6]  = vec3(0.5, 0.0, 0.0); // Dark Red
// 	colors[7]  = vec3(0.0, 0.5, 0.0); // Dark Green
// 	colors[8]  = vec3(0.0, 0.0, 0.5); // Dark Blue
// 	colors[9]  = vec3(0.5, 0.5, 0.0); // Olive
// 	colors[10] = vec3(0.5, 0.0, 0.5); // Purple
// 	colors[11] = vec3(0.0, 0.5, 0.5); // Teal
// 	colors[12] = vec3(1.0, 0.5, 0.0); // Orange
// 	colors[13] = vec3(0.5, 1.0, 0.0); // Lime
// 	colors[14] = vec3(0.0, 1.0, 0.5); // Spring Green
// 	colors[15] = vec3(0.5, 0.0, 1.0); // Violet


// 	// tempBVH nodeStack[32];
// 	int nodeStack[32];
// 	int stackIndex = 0;
// 	nodeStack[stackIndex++] = 0;

// 	HitInfo hitInfo;
// 	hitInfo.hit = false;
// 	hitInfo.distance = INFINITY;

// 	int index = 0;
// 	while (stackIndex > 0)
// 	{
// 		int nodeIndex = nodeStack[--stackIndex];
// 		tempBVH node = bvhNodeBuffer.data[nodeIndex];
// 		vec3 min = node.min.xyz;
// 		vec3 max = node.max.xyz;

// 		float dst = RayBoundingBoxDst(ray, min, max);
// 		// if (bool(dst))
// 		// if (dst < hitInfo.distance)
// 		// {
// 			if (node.ChildIndex == 0)
// 			{
// 				if (dst < hitInfo.distance)
// 				{
// 					hitInfo.distance = dst;
// 					hitInfo.material.albedo = colors[index % 16];
// 					hitInfo.hit = true;
// 				}
// 			}
// 			else
// 			{
// 				// nodeStack[stackIndex++] = bvhNodeBuffer.data[node.ChildIndex + 0];
// 				// nodeStack[stackIndex++] = bvhNodeBuffer.data[node.ChildIndex + 1];

// 				tempBVH left = bvhNodeBuffer.data[node.ChildIndex + 0];
// 				tempBVH right = bvhNodeBuffer.data[node.ChildIndex + 1];

// 				float leftDst = RayBoundingBoxDst(ray, left.min.xyz, left.max.xyz);
// 				float rightDst = RayBoundingBoxDst(ray, right.min.xyz, right.max.xyz);

// 				if (leftDst > rightDst)
// 				{
// 					// if (leftDst < hitInfo.distance) bvhNodeBuffer.data[nodeStack[stackIndex++]] = left;
// 					// if (rightDst < hitInfo.distance) bvhNodeBuffer.data[nodeStack[stackIndex++]] = right;
// 					if (leftDst < hitInfo.distance) nodeStack[stackIndex++] = node.ChildIndex + 0;
// 					if (rightDst < hitInfo.distance) nodeStack[stackIndex++] = node.ChildIndex + 1;
// 				}
// 				else
// 				{
// 					if (rightDst < hitInfo.distance) nodeStack[stackIndex++] = node.ChildIndex + 1;
// 					if (leftDst < hitInfo.distance) nodeStack[stackIndex++] = node.ChildIndex + 0;
// 				}
// 			}
// 		// }
// 		index++;
// 	}
// 	return hitInfo;
// }


// HitInfo PreviewBVH(Ray ray, int showDepth)
// {
// 	vec3 colors[16];
// 	colors[0]  = vec3(1.0, 0.0, 0.0); // Red
// 	colors[1]  = vec3(0.0, 1.0, 0.0); // Green
// 	colors[2]  = vec3(0.0, 0.0, 1.0); // Blue
// 	colors[3]  = vec3(1.0, 1.0, 0.0); // Yellow
// 	colors[4]  = vec3(1.0, 0.0, 1.0); // Magenta
// 	colors[5]  = vec3(0.0, 1.0, 1.0); // Cyan
// 	colors[6]  = vec3(0.5, 0.0, 0.0); // Dark Red
// 	colors[7]  = vec3(0.0, 0.5, 0.0); // Dark Green
// 	colors[8]  = vec3(0.0, 0.0, 0.5); // Dark Blue
// 	colors[9]  = vec3(0.5, 0.5, 0.0); // Olive
// 	colors[10] = vec3(0.5, 0.0, 0.5); // Purple
// 	colors[11] = vec3(0.0, 0.5, 0.5); // Teal
// 	colors[12] = vec3(1.0, 0.5, 0.0); // Orange
// 	colors[13] = vec3(0.5, 1.0, 0.0); // Lime
// 	colors[14] = vec3(0.0, 1.0, 0.5); // Spring Green
// 	colors[15] = vec3(0.5, 0.0, 1.0); // Violet



// 	int stack[64];
// 	int ptr = 0;
// 	stack[ptr++] = 0;

// 	HitInfo hitInfo;
// 	hitInfo.hit = false;
// 	hitInfo.distance = INFINITY;

// 	float leftHit = 0.0;
// 	float rightHit = 0.0;

// 	int index = 0;
// 	while (index != -1)
// 	{
// 		int leftIndex = bvhNodeBuffer.data[index].ChildIndex + 0;
// 		int rightIndex = bvhNodeBuffer.data[index].ChildIndex + 1;
// 		bool isLeaf = bvhNodeBuffer.data[index].ChildIndex == 0;



// 		if (isLeaf)
// 		{
// 			vec3 min = bvhNodeBuffer.data[index].min.xyz;
// 			vec3 max = bvhNodeBuffer.data[index].max.xyz;
// 			if (bool(RayBoundingBoxDst(ray, min, max) < hitInfo.distance))
// 			{
// 				hitInfo.distance = RayBoundingBoxDst(ray, min, max);
// 				hitInfo.material.albedo = colors[ptr % 16];
// 				hitInfo.hit = true;
// 			}
// 			index = -1;
// 		}
// 		else
// 		{
// 			leftHit = RayBoundingBoxDst(ray, bvhNodeBuffer.data[leftIndex].min.xyz, bvhNodeBuffer.data[leftIndex].max.xyz);
// 			rightHit = RayBoundingBoxDst(ray, bvhNodeBuffer.data[rightIndex].min.xyz, bvhNodeBuffer.data[rightIndex].max.xyz);
// 			if (leftHit > 0.0 && rightHit > 0.0)
// 			{
// 				int defered = 0;
// 				if (leftHit < rightHit)
// 				{
// 					index = leftIndex;
// 					defered = rightIndex;
// 				}
// 				else
// 				{
// 					index = rightIndex;
// 					defered = leftIndex;
// 				}
// 				stack[ptr++] = defered;
// 				continue;
// 			}
// 			else if (leftHit > 0.0)
// 			{
// 				ptr = leftIndex;
// 				continue;
// 			}
// 			else if (rightHit > 0.0)
// 			{
// 				ptr = rightIndex;
// 				continue;
// 			}
// 		}
// 		index = stack[--ptr];
// 	}
// 	return hitInfo;
// }

HitInfo PreviewBVH(Ray ray, int showDepth)
{
	vec3 colors[16];
	colors[0]  = vec3(1.0, 0.0, 0.0); // Red
	colors[1]  = vec3(0.0, 1.0, 0.0); // Green
	colors[2]  = vec3(0.0, 0.0, 1.0); // Blue
	colors[3]  = vec3(1.0, 1.0, 0.0); // Yellow
	colors[4]  = vec3(1.0, 0.0, 1.0); // Magenta
	colors[5]  = vec3(0.0, 1.0, 1.0); // Cyan
	colors[6]  = vec3(0.5, 0.0, 0.0); // Dark Red
	colors[7]  = vec3(0.0, 0.5, 0.0); // Dark Green
	colors[8]  = vec3(0.0, 0.0, 0.5); // Dark Blue
	colors[9]  = vec3(0.5, 0.5, 0.0); // Olive
	colors[10] = vec3(0.5, 0.0, 0.5); // Purple
	colors[11] = vec3(0.0, 0.5, 0.5); // Teal
	colors[12] = vec3(1.0, 0.5, 0.0); // Orange
	colors[13] = vec3(0.5, 1.0, 0.0); // Lime
	colors[14] = vec3(0.0, 1.0, 0.5); // Spring Green
	colors[15] = vec3(0.5, 0.0, 1.0); // Violet

	int stack[256]; // Increased stack size
	int ptr = 0;
	stack[ptr++] = 0;

	HitInfo hitInfo;
	hitInfo.hit = false;
	hitInfo.distance = INFINITY;

	int safity = 0;
	int leafcount = 0;
	while (ptr > 0)
	{
		if (safity > 20000)
			break ;
		int index = stack[--ptr];
		int leftIndex = bvhNodeBuffer.data[index].ChildIndex;
		int rightIndex = bvhNodeBuffer.data[index].ChildIndex + 1;
		bool isLeaf = bvhNodeBuffer.data[index].ChildIndex == 0; // Ensure this is correct

		if (isLeaf)
		{
			vec3 min = bvhNodeBuffer.data[index].min.xyz;
			vec3 max = bvhNodeBuffer.data[index].max.xyz;
			float dst = RayBoundingBoxDst(ray, min, max);
			if (dst > 0.0 && dst < hitInfo.distance)
			{
				hitInfo.distance = dst;
				hitInfo.material.albedo = colors[leafcount % 16];
				hitInfo.hit = true;
				leafcount++;
			}
		}
		else
		{
			float leftHit = RayBoundingBoxDst(ray, bvhNodeBuffer.data[leftIndex].min.xyz, bvhNodeBuffer.data[leftIndex].max.xyz);
			float rightHit = RayBoundingBoxDst(ray, bvhNodeBuffer.data[rightIndex].min.xyz, bvhNodeBuffer.data[rightIndex].max.xyz);
			if (leftHit > 0.0 && rightHit > 0.0)
			{
				if (leftHit < rightHit)
				{
					stack[ptr++] = rightIndex;
					stack[ptr++] = leftIndex;
				}
				else
				{
					stack[ptr++] = leftIndex;
					stack[ptr++] = rightIndex;
				}
			}
			else if (leftHit > 0.0)
			{
				stack[ptr++] = leftIndex;
			}
			else if (rightHit > 0.0)
			{
				stack[ptr++] = rightIndex;
			}
		}
		safity++;
	}
	return hitInfo;
}

// HitInfo PreviewBVH(Ray ray, int showDepth)
// {
// 	vec3 colors[16];
// 	colors[0]  = vec3(1.0, 0.0, 0.0); // Red
// 	colors[1]  = vec3(0.0, 1.0, 0.0); // Green
// 	colors[2]  = vec3(0.0, 0.0, 1.0); // Blue
// 	colors[3]  = vec3(1.0, 1.0, 0.0); // Yellow
// 	colors[4]  = vec3(1.0, 0.0, 1.0); // Magenta
// 	colors[5]  = vec3(0.0, 1.0, 1.0); // Cyan
// 	colors[6]  = vec3(0.5, 0.0, 0.0); // Dark Red
// 	colors[7]  = vec3(0.0, 0.5, 0.0); // Dark Green
// 	colors[8]  = vec3(0.0, 0.0, 0.5); // Dark Blue
// 	colors[9]  = vec3(0.5, 0.5, 0.0); // Olive
// 	colors[10] = vec3(0.5, 0.0, 0.5); // Purple
// 	colors[11] = vec3(0.0, 0.5, 0.5); // Teal
// 	colors[12] = vec3(1.0, 0.5, 0.0); // Orange
// 	colors[13] = vec3(0.5, 1.0, 0.0); // Lime
// 	colors[14] = vec3(0.0, 1.0, 0.5); // Spring Green
// 	colors[15] = vec3(0.5, 0.0, 1.0); // Violet


// 	tempBVH nodeStack[32];
// 	int stackIndex = 0;
// 	nodeStack[stackIndex++] = bvhNodeBuffer.data[0];

// 	HitInfo hitInfo;
// 	hitInfo.hit = false;
// 	hitInfo.distance = INFINITY;

// 	int index = 0;
// 	while (stackIndex > 0)
// 	{
// 		tempBVH node = nodeStack[--stackIndex];
// 		vec3 min = node.min.xyz;
// 		vec3 max = node.max.xyz;

// 		float dst = RayBoundingBoxDst(ray, min, max);
// 		// if (bool(dst))
// 		// if (dst < hitInfo.distance)
// 		// {
// 			if (node.ChildIndex == 0)
// 			{
// 				if (dst < hitInfo.distance)
// 				{
// 					hitInfo.distance = dst;
// 					hitInfo.material.albedo = colors[index % 16];
// 					hitInfo.hit = true;
// 				}
// 			}
// 			else
// 			{
// 				// nodeStack[stackIndex++] = bvhNodeBuffer.data[node.ChildIndex + 0];
// 				// nodeStack[stackIndex++] = bvhNodeBuffer.data[node.ChildIndex + 1];

// 				tempBVH left = bvhNodeBuffer.data[node.ChildIndex + 0];
// 				tempBVH right = bvhNodeBuffer.data[node.ChildIndex + 1];

// 				float leftDst = RayBoundingBoxDst(ray, left.min.xyz, left.max.xyz);
// 				float rightDst = RayBoundingBoxDst(ray, right.min.xyz, right.max.xyz);

// 				if (leftDst > rightDst)
// 				{
// 					if (leftDst < hitInfo.distance) nodeStack[stackIndex++] = left;
// 					if (rightDst < hitInfo.distance) nodeStack[stackIndex++] = right;
// 				}
// 				else
// 				{
// 					if (rightDst < hitInfo.distance) nodeStack[stackIndex++] = right;
// 					if (leftDst < hitInfo.distance) nodeStack[stackIndex++] = left;
// 				}
// 			}
// 		// }
// 		index++;
// 	}
// 	return hitInfo;
// }

vec4 TraceRay(Ray ray)
{
	vec3 color;
	HitInfo hitInfo = PreviewBVH(ray, 1);
	if (hitInfo.hit)
	{
		color = hitInfo.material.albedo;
	}
	else
	{
		color = vec3(0.0, 0.0, 0.0);
	}

	vec4 finalColor = vec4(color, 1.0);

	return finalColor;
}




void main()
{
	ivec2 pixelID = ivec2(gl_GlobalInvocationID.xy);

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