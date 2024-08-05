#ifdef _TYPE_VERT_SHADER
#version 460 core

out vec2 TexCoords;

void main()
{
    const vec2 pos[4] = vec2[4](
        vec2(-1.0, -1.0),
        vec2( 1.0, -1.0),
        vec2(-1.0,  1.0),
        vec2( 1.0,  1.0)
    );
    
    TexCoords = (pos[gl_VertexID] + 1.0) / 2.0;
    gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);
	// gl_Position = vec4(1.0, 1.0, 0.0, 1.0);
}
#endif

#ifdef _TYPE_FRAG_SHADER
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D u_Texture;

uniform uint u_FrameCount;



float Luminance(vec3 c)
{
    return 0.212671 * c.x + 0.715160 * c.y + 0.072169 * c.z;
}

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
mat3 ACESInputMat = mat3
(
    vec3(0.59719, 0.35458, 0.04823),
    vec3(0.07600, 0.90834, 0.01566),
    vec3(0.02840, 0.13383, 0.83777)
);

// ODT_SAT => XYZ => D60_2_D65 => sRGB
mat3 ACESOutputMat = mat3
(
    vec3(1.60475, -0.53108, -0.07367),
    vec3(-0.10208, 1.10813, -0.00605),
    vec3(-0.00327, -0.07276, 1.07602)
);

vec3 RRTAndODTFit(vec3 v)
{
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

vec3 ACESFitted(vec3 color)
{
    color = color * ACESInputMat;

    // Apply RRT and ODT
    color = RRTAndODTFit(color);

    color = color * ACESOutputMat;

    // Clamp to [0, 1]
    color = clamp(color, 0.0, 1.0);

    return color;
}

vec3 ACES(in vec3 c)
{
    float a = 2.51f;
    float b = 0.03f;
    float y = 2.43f;
    float d = 0.59f;
    float e = 0.14f;

    return clamp((c * (a * c + b)) / (c * (y * c + d) + e), 0.0, 1.0);
}

vec3 Tonemap(in vec3 c, float limit)
{
    return c * 1.0 / (1.0 + Luminance(c) / limit);
}


void main()
{
    // FragColor = vec4(TexCoords, 0.0, 1.0); // Example output, can be modified based on your needs
	// FragColor = vec4(0.3216, 0.2627, 0.5843, 1.0);
	vec4 accumulatedBuffer = texture(u_Texture, TexCoords);
	vec4 averagedColor = accumulatedBuffer / float(u_FrameCount);


	vec3 color = averagedColor.rgb;
	// color = ACES(color);


	// color = ACESFitted(color);
	// color = pow(color, vec3(1.0 / 2.2));

	// averagedColor = aces_film(averagedColor);
	// vec3 color = averagedColor.rgb;
	// color = LinearToSRGB(color);

	FragColor = vec4(color, 1.0);
}
#endif

