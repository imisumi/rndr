#ifdef _TYPE_VERT_SHADER
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_Texcoord;
layout(location = 4) in float a_TexIndex;
layout(location = 5) in float a_TilingFactor;
layout(location = 6) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec3 v_Color;
out vec3 v_Normal;
out vec2 v_TexCoord;
out flat float v_TexIndex;
out flat float v_TilingFactor;
out flat int v_EntityID;

void main()
{
	v_Color = a_Color;
	v_Normal = a_Normal;
	v_TexCoord = a_Texcoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection  * vec4(a_Position, 1.0);
}
#endif

#ifdef _TYPE_FRAG_SHADER
layout(location = 0) out vec4 color;
layout(location = 1) out int id;

in vec3 v_Color;
in vec3 v_Normal;
in vec2 v_TexCoord;
in flat float v_TexIndex;
in flat float v_TilingFactor;
in flat int v_EntityID;

uniform sampler2D u_Textures[16];

// void main ()
// {
// 	vec4 
// 	color = vec4(v_Color, 1.0);
// 	vec3 col = v_Normal * 0.5 + 0.5;
// 	// vec3 col = v_Normal;
// 	color = vec4(col, 1.0);
// 	color = vec4(v_Color, 1.0);
// 	id = v_EntityID;
// }

void main()
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




	// Original color based on v_Color
	// vec4 baseColor = vec4(v_Color, 1.0);
	// // Compute a slight tint color based on the normal
	// vec3 normalTint = v_Normal * 0.1 + 0.0;  // Adjust the multiplier to change tint strength
		
	// // Combine the base color with the tint
	// vec3 tintedColor = baseColor.rgb + normalTint;
		
	// // Ensure the color values are within the valid range [0, 1]
	// tintedColor = clamp(tintedColor, 0.0, 1.0);
		
	// // Set the final color output
	// color = vec4(tintedColor, 1.0);
	// color = l_Texture * vec4(v_Color, 1.0);
	// color = l_Texture;
	color = l_Texture * vec4(v_Color, 1.0);
	// color = vec4(l_Texture.rgb * v_Color.rgb, l_Texture.a);
	// color = vec4(v_TexCoord, 0.0, 1.0);
		

	// for object picking
	id = v_EntityID;
}
#endif