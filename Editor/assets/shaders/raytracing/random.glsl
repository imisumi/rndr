float RandomFloat(inout uint seed)
{
	seed = seed * 747796405 + 2891336453;
	uint result = ((seed >> ((seed >> 28) + 4)) ^ seed) * 277803737;
	result = (result >> 22) ^ result;
	return float(result) / float(4294967295.0);
}

float RandomValueNormalDistribution(inout uint seed)
{
	float theta = 2.0 * 3.14159265359 * RandomFloat(seed);
	float rho = sqrt(-2.0 * log(RandomFloat(seed)));
	return rho * cos(theta);
}

vec3 RandomDirection(inout uint seed)
{
	float x = RandomValueNormalDistribution(seed);
	float y = RandomValueNormalDistribution(seed);
	float z = RandomValueNormalDistribution(seed);
	return normalize(vec3(x, y, z));
}

vec3 RandomHemisphereDirection(inout uint seed, vec3 normal)
{
	vec3 direction = RandomDirection(seed);
	if (dot(direction, normal) < 0.0)
	{
		direction = -direction;
	}
	return direction;
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