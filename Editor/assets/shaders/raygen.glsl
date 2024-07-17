Ray RayGen()
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
	return ray;
}