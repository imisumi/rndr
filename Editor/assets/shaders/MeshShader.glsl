#ifdef _TYPE_VERT_SHADER
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Texcoord;
layout(location = 3) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec3 v_Normal;
out vec2 v_Texcoord;
out flat int v_EntityID;

uniform mat4 u_Transform;     // Declare the uniform transform matrix

void main()
{
	v_Normal = a_Normal;
	v_Texcoord = a_Texcoord;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
#endif

#ifdef _TYPE_FRAG_SHADER
layout(location = 0) out vec4 color;
layout(location = 1) out int id;


in vec3 v_Normal;
in vec2 v_Texcoord;
in flat int v_EntityID;

void main()
{
	vec3 col = v_Normal * 0.5 + 0.5;
	color = vec4(col, 1.0);
	// color = vec4(0.0, 0.0, 1.0, 1.0);
	id = v_EntityID;
}
#endif