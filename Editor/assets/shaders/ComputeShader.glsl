#version 450 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(binding = 0, rgba32f) writeonly uniform image2D img_output;
layout(binding = 1, rgba32f) readonly uniform image2D u_SkyTexture;


uniform int u_ScreenWidth;
uniform int u_ScreenHeight;

uniform int u_SkyTextureWidth;
uniform int u_SkyTextureHeight;

uniform mat4 u_InvProj;
uniform mat4 u_InvView;

uniform vec3 u_CameraPos;


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


// vec3 linearToSRGB(vec3 linearRGB, float exposure) {
//     // Apply exposure adjustment
//     linearRGB *= exposure;
    
//     // Convert linear RGB to sRGB
//     vec3 sRGB = mix(12.92 * linearRGB, 1.055 * pow(linearRGB, vec3(1.0 / 2.4)) - 0.055, step(0.0031308, linearRGB));
    
//     return sRGB;
// }

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

HitInfo RaySphere(Ray ray, Sphere sphere, HitInfo hitInfo)
{
	vec3 offsetRayOrigin = ray.origin - sphere.position;

	float a = dot(ray.dir, ray.dir);
	float b = 2.0 * dot(ray.dir, offsetRayOrigin);
	float c = dot(offsetRayOrigin, offsetRayOrigin) - sphere.radius * sphere.radius;

	float discriminant = b * b - 4.0 * a * c;

	if (discriminant > 0.0)
	{
		float dist = (-b - sqrt(discriminant)) / (2.0 * a);
		if (dist > 0.0 && dist < hitInfo.distance)
		{
			hitInfo.hit = true;
			hitInfo.position = ray.origin + ray.dir * dist;
			hitInfo.normal = normalize(hitInfo.position - sphere.position);
			hitInfo.distance = dist;
			hitInfo.material = sphere.material;
		}
	}
	return hitInfo;
}

vec3 ACESFilm(vec3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec3 LinearToSRGB(vec3 color)
{
    return mix(pow(color, vec3(1.0 / 2.4)) * 1.055 - 0.055, color * 12.92, lessThanEqual(color, vec3(0.0031308)));
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
    // hitInfo.hit = abs(determinant) >= EPSILON && dst >= 0 && u >= 0 && v >= 0 && w >= 0;
	hitInfo.hit = determinant >= EPSILON && dst >= 0 && u >= 0 && v >= 0 && w >= 0;  // Note the use of fabs
    hitInfo.position = ray.origin + ray.dir * dst;
    hitInfo.normal = tri.normal;
    hitInfo.distance = dst;
    hitInfo.material = tri.material;
    return hitInfo;
}

vec4 TraceRay(Ray ray)
{
	Sphere sphere;
	sphere.position = vec3(0.0, 0.0, 0.0);
	sphere.radius = 0.5;
	sphere.material.albedo = vec3(0.6784, 0.6784, 0.6784);

	Triangle tri;
	tri.posA = vec3(-10.0, 0.0, -10.0);
	tri.posB = vec3(0.0, 0.0, 10.0);
	tri.posC = vec3(10.0, 0.0, -10.0);
	tri.normal = vec3(0.0, 1.0, 0.0);
	tri.material.albedo = vec3(0.5922, 0.3098, 0.5451);

	HitInfo hitInfo;
	hitInfo.hit = false;
	hitInfo.distance = INFINITY;

	// hitInfo = RaySphere(ray, sphere, hitInfo);
	hitInfo = RayTriangle(ray, tri);
	vec3 color;
	if (hitInfo.hit)
	{
		color = hitInfo.material.albedo;
	}
	else
	{
		color = SampleSkybox(ray.dir).xyz;
	}



	color = ACESFilm(color);
	color = LinearToSRGB(color);
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


	vec4 finalColor = TraceRay(ray);


	// vec4 finalColor = SampleSkybox(rayDirection);
	// vec3 srgb = linearToSRGB(finalColor.rgb, 0.5);
	// finalColor = vec4(srgb, 1.0);
	imageStore(img_output, pixelID, finalColor);
	return;



	// vec2 skyCoord = vec2(coord.x * float(u_SkyTextureWidth - 1), coord.y * float(u_SkyTextureHeight - 1));

	// ivec2 texCoord = ivec2(coord * vec2(u_SkyTextureWidth, u_SkyTextureHeight));

	// vec4 skyColor = imageLoad(u_SkyTexture, texCoord);

	// vec3 color = linearToSRGB(skyColor.rgb);
	// skyColor = vec4(color, 1.0);
    // imageStore(img_output, pixelID, skyColor);
}