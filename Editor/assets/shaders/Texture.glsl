#ifdef _TYPE_VERT_SHADER
#version 430 core

// layout(location = 0) in vec3 a_Position;
// layout(location = 1) in vec2 a_TexCoord;
// layout(location = 2) in vec4 a_Color;
// layout(location = 3) in float a_TexIndex;
// layout(location = 4) in float a_TilingFactor;
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjection  * vec4(a_Position, 1.0);
}
#endif

#ifdef _TYPE_FRAG_SHADER
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[16];

void main ()
{
	vec4 l_Texture;
	int l_TexIndex = int(v_TexIndex);

	switch (l_TexIndex) {
		case 0:  l_Texture = texture(u_Textures[0],  v_TexCoord * v_TilingFactor); break;
		case 1:  l_Texture = texture(u_Textures[1],  v_TexCoord * v_TilingFactor); break;
		case 2:  l_Texture = texture(u_Textures[2],  v_TexCoord * v_TilingFactor); break;
		case 3:  l_Texture = texture(u_Textures[3],  v_TexCoord * v_TilingFactor); break;
		case 4:  l_Texture = texture(u_Textures[4],  v_TexCoord * v_TilingFactor); break;
		case 5:  l_Texture = texture(u_Textures[5],  v_TexCoord * v_TilingFactor); break;
		case 6:  l_Texture = texture(u_Textures[6],  v_TexCoord * v_TilingFactor); break;
		case 7:  l_Texture = texture(u_Textures[7],  v_TexCoord * v_TilingFactor); break;
		case 8:  l_Texture = texture(u_Textures[8],  v_TexCoord * v_TilingFactor); break;
		case 9:  l_Texture = texture(u_Textures[9],  v_TexCoord * v_TilingFactor); break;
		case 10: l_Texture = texture(u_Textures[10], v_TexCoord * v_TilingFactor); break;
		case 11: l_Texture = texture(u_Textures[11], v_TexCoord * v_TilingFactor); break;
		case 12: l_Texture = texture(u_Textures[12], v_TexCoord * v_TilingFactor); break;
		case 13: l_Texture = texture(u_Textures[13], v_TexCoord * v_TilingFactor); break;
		case 14: l_Texture = texture(u_Textures[14], v_TexCoord * v_TilingFactor); break;
		case 15: l_Texture = texture(u_Textures[15], v_TexCoord * v_TilingFactor); break;
	}

	color = l_Texture * v_Color;
}
#endif