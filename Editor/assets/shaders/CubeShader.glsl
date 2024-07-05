#ifdef _TYPE_VERT_SHADER
#version 430 core

// layout(location = 0) in vec3 a_Position;
// layout(location = 1) in vec2 a_TexCoord;
// layout(location = 2) in vec4 a_Color;
// layout(location = 3) in float a_TexIndex;
// layout(location = 4) in float a_TilingFactor;
layout(location = 0) in vec3 a_Position;
// layout(location = 1) in vec3 a_Normal;
layout(location = 1) in vec4 a_Color;
// layout(location = 2) in vec2 a_TexCoord;
// layout(location = 3) in float a_TexIndex;
// layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

// out vec3 v_Normal;
// out vec2 v_TexCoord;
out vec4 v_Color;
// out float v_TexIndex;
// out float v_TilingFactor;

void main()
{
	// v_Normal = a_Normal;
	// v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	// v_TexIndex = a_TexIndex;
	// v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjection  * vec4(a_Position, 1.0);
}
#endif

#ifdef _TYPE_FRAG_SHADER
layout(location = 0) out vec4 color;

// in vec2 v_TexCoord;
in vec4 v_Color;
// in float v_TexIndex;
// in float v_TilingFactor;
// in vec3 v_Normal;

uniform sampler2D u_Textures[16];

void main ()
{
	color = v_Color;
	// vec3 col = v_Normal * 0.5 + 0.5;
	// color = vec4(col, 1.0);
	// color = vec4(1.0, 0.0, 0.0, 1.0);
}
#endif