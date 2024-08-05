#pragma once

#include <glm/glm.hpp>

namespace Rndr
{
	//? Bounding Volume Hierarchy
	struct bvhNode
	{

		int ChildIndex = 0;
		int TriangleIndex = 0;
		int TriangleCount = 0;
		int padding3;

	
		glm::vec3 Min = glm::vec3(INFINITY);
		float padding1;

		glm::vec3 Max = glm::vec4(-INFINITY);
		float padding2;

	};
	static_assert(sizeof(bvhNode) == 48, "bvh struct size should be 48 bytes.");



	//? Bottom Level Acceleration Structure
	struct alignas(16) BLAS
	{
		uint32_t m_IndexOffset;
		uint32_t TriangleOffset;
		uint32_t TriangleCount;
		alignas(16) glm::mat4 m_Transform;
	};
	// static_assert(sizeof(BLAS) == 80, "BLAS struct size should be 80 bytes.");

	//? Top Level Acceleration Structure
	struct TLAS
	{
		// uint32_t m_InstanceID;
		uint32_t BLASIndex;
		uint32_t ChildIndex;

		glm::vec3 Min = glm::vec3(std::numeric_limits<float>::infinity());
		glm::vec3 Max = glm::vec3(-std::numeric_limits<float>::infinity());
	
		glm::mat4 m_Transform;
	};
}