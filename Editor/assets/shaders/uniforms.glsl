layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(binding = 0, rgba32f) writeonly uniform image2D img_output;
layout(binding = 1, rgba32f) readonly uniform image2D u_SkyTexture;
layout(std430, binding = 2) buffer triangleBuffer {
	Triangle data[]; // send as glm::vec4 but becuase of memory alignment, I just use vec3
} triBuffer;

layout(std430, binding = 3) buffer bvhBuffer {
	bvh data[]; // send as glm::vec4 but becuase of memory alignment, I just use vec3
} bvhData;



uniform int u_TriangleCount;

uniform int u_ScreenWidth;
uniform int u_ScreenHeight;

uniform int u_SkyTextureWidth;
uniform int u_SkyTextureHeight;

uniform mat4 u_InvProj;
uniform mat4 u_InvView;

uniform vec3 u_CameraPos;
