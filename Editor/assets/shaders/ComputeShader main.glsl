#version 450 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_output;

// layout(binding = 1) uniform sampler2D skyBox;
layout(binding = 1, rgb32af) uniform readonly image2D skyBox;

uniform int u_ScreenWidth;
uniform int u_ScreenHeight;

uniform mat4 u_InvProj;
uniform mat4 u_InvView;

uniform vec3 u_CameraPos;

const float PI = 3.14159265359;


struct Material
{
	vec3 albedo;
	vec3 emission;
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
	vec3 direction;
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
	vec3 normA, normB, normC;
};


HitInfo RaySphere(Ray ray, Sphere sphere, HitInfo hitInfo)
{
	vec3 offsetRayOrigin = ray.origin - sphere.position;

	float a = dot(ray.direction, ray.direction);
	float b = 2.0 * dot(ray.direction, offsetRayOrigin);
	float c = dot(offsetRayOrigin, offsetRayOrigin) - sphere.radius * sphere.radius;

	float discriminant = b * b - 4.0 * a * c;

	if (discriminant > 0.0)
	{
		float dist = (-b - sqrt(discriminant)) / (2.0 * a);
		if (dist > 0.0 && dist < hitInfo.distance)
		{
			hitInfo.hit = true;
			hitInfo.position = ray.origin + ray.direction * dist;
			hitInfo.normal = normalize(hitInfo.position - sphere.position);
			hitInfo.distance = dist;
			hitInfo.material = sphere.material;
		}
	}
	return hitInfo;
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

vec4 SampleSkybox(vec3 direction) {
    vec2 uv = SampleUVFromDirection(direction);
	// return vec4(uv, 0.0, 1.0);
    return texture(skyBox, uv);
}

void main() {
    // ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
    // vec2 uv = vec2(pixel_coords) / vec2(u_ScreenWidth - 1, u_ScreenHeight - 1);

	// uint seed = generateSeed();
	// float rngState = seed / (float(u_ScreenWidth) * float(u_ScreenHeight));
	// float r = RandomFloat(seed);
	// float g = RandomFloat(seed);
	// float b = RandomFloat(seed);
	// vec4 col = vec4(r, g, b, 1.0);

    // imageStore(img_output, pixel_coords, col);




	ivec2 pixelID = ivec2(gl_GlobalInvocationID.xy);
	// uint seed = generateSeed() + u_FrameNumber * 719393;
	uint seed = generateSeed();

	vec2 coord = vec2(float(pixelID.x) / float(u_ScreenWidth), float(pixelID.y) / float(u_ScreenHeight));
	// coord = coord * 2.0 - 1.0;
	vec4 outputColor = vec4(coord, 0.0, 1.0);
	outputColor = texture(skyBox, coord);
	imageStore(img_output, pixelID, outputColor);
	return ;


	vec4 target = u_InvProj * vec4(coord.x, coord.y, 1.0, 1.0);
	vec3 rayDirection = vec3(u_InvView * vec4(normalize(vec3(target.xyz) / target.w), 0.0)).xyz; // Convert to world space
	rayDirection = normalize(rayDirection);

	Ray ray;
	ray.origin = u_CameraPos;
	ray.direction = rayDirection;

	vec3 color = vec3(0.0, 0.0, 0.0);

	Sphere sphere;
	sphere.position = vec3(0.0, 0.0, 0.0);
	sphere.radius = 0.5;
	sphere.material.albedo = vec3(1.0, 0.0, 0.0);
	sphere.material.emission = vec3(0.0, 0.0, 0.0);

	HitInfo hitInfo;
	hitInfo.hit = false;
	hitInfo.distance = 1000000.0;

	hitInfo = RaySphere(ray, sphere, hitInfo);

	if (hitInfo.hit)
	{
		color = hitInfo.material.albedo;
	}
	else
	{
		color = SampleSkybox(rayDirection).xyz;
	}

	vec4 col = vec4(color, 1.0);
	imageStore(img_output, pixelID, col);

}
