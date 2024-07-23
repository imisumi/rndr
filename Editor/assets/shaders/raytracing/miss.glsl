



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


// vec2 node_tex_environment_mirror_ball(vec3 co)
// {
//   vec3 nco = normalize(co);
//   nco.y -= 1.0;

//   float div = 2.0 * sqrt(max(-0.5 * nco.y, 0.0));
//   nco /= max(1e-8, div);

//   vec2 uv = 0.5 * nco.xzz + 0.5;
// }

vec4 SampleSkybox(vec3 direction)
{
	vec2 uv = SampleUVFromDirection(direction);
	vec4 sky = imageLoad(u_SkyTexture, ivec2(uv * vec2(u_SkyTextureWidth, u_SkyTextureHeight)));
	// return vec4(uv, 0.0, 1.0);
	return sky;
	// return texture(skyBox, uv);
}