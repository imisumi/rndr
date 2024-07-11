#ifdef _TYPE_VERT_SHADER
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec3 v_Color;
out flat int v_EntityID;

void main()
{
	v_Color = a_Color;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection  * vec4(a_Position, 1.0);
}
#endif

#ifdef _TYPE_FRAG_SHADER
layout(location = 0) out vec4 color;
layout(location = 1) out int id;

in vec3 v_Color;
in flat int v_EntityID;

void main ()
{

	color = vec4(v_Color, 1.0);
	id = v_EntityID;
}
#endif