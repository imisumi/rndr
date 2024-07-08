#ifdef _TYPE_VERT_SHADER
#version 430 core

layout (location = 0) in vec3 aPos;

void main() {
	gl_Position = vec4(aPos, 1.0);
}
#endif

#ifdef _TYPE_FRAG_SHADER
out vec4 FragColor;

void main() {
	FragColor = vec4(0.2, 0.2, 0.2, 1.0); // Grid line color (dark gray)
}
#endif