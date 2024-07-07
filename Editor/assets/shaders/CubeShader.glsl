#ifdef _TYPE_VERT_SHADER
#version 430 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;


void main()
{
	gl_Position = u_ViewProjection  * vec4(a_Position, 1.0);
}
#endif

#ifdef _TYPE_FRAG_SHADER
layout(location = 0) out vec4 color;

void main ()
{

	color = vec4(0.8196, 0.2431, 0.4627, 1.0);
}
#endif