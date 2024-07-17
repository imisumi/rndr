// #pragma once

// #include "Rndr/Core/Base.h"

// #include <vector>
// #include <glm/glm.hpp>

// #include "Buffer.h"

// #include "Rndr/Renderer/VertexArray.h"

// namespace Rndr {

// 	class Mesh
// 	{
// 	public:
// 		struct Vertex
// 		{
// 			glm::vec3 Position;
// 			glm::vec3 Normal;
// 			glm::vec3 Tangent;
// 			glm::vec3 Binormal;
// 			glm::vec2 Texcoord;
// 			int EntityID;
// 		};
// 		// static_assert(sizeof(Vertex) == 14 * sizeof(float));
// 		static const int NumAttributes = 6;

// 		struct Index
// 		{
// 			uint32_t V1, V2, V3;
// 		};
// 		static_assert(sizeof(Index) == 3 * sizeof(uint32_t));

// 		Mesh() = default;
// 		Mesh(const std::string& filename);
// 		~Mesh();

// 		void Render();

// 		inline const std::string& GetFilePath() const { return m_FilePath; }

// 		inline const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
// 	// private: //TODO: make private
// 		std::vector<Vertex> m_Vertices;
// 		std::vector<Index> m_Indices;

// 		Ref<VertexArray> m_VertexArray;

// 		Ref<VertexBuffer> m_VertexBuffer;
// 		Ref<IndexBuffer> m_IndexBuffer;

// 		std::string m_FilePath;
// 	};
// }


#pragma once

#include "Rndr/Core/Base.h"

#include <vector>
#include <glm/glm.hpp>

#include "Buffer.h"

#include "Rndr/Renderer/VertexArray.h"

namespace Rndr {

	class Mesh
	{
	public:
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 Texcoord;
			int EntityID;
		};

		struct Triangle
		{
			// Vertex V1, V2, V3;

			glm::vec3 V1;
			float padding1;
			glm::vec3 V2;
			float padding2;
			glm::vec3 V3;
			float padding3;

			glm::vec3 Normal;
			float padding4;
		};
		// static_assert(sizeof(Triangle) == 3 * sizeof(glm::vec3) + 3 * sizeof(float));
		static_assert(sizeof(Triangle) == 16 * sizeof(float));
		static const int NumAttributes = 4;

		struct Index
		{
			uint32_t V1, V2, V3;
		};
		static_assert(sizeof(Index) == 3 * sizeof(uint32_t));

		Mesh() = default;
		Mesh(const std::string& filename);
		~Mesh();

		void Render();

		inline const std::string& GetFilePath() const { return m_FilePath; }

		inline const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
	// private: //TODO: make private
		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;

		Ref<VertexArray> m_VertexArray;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		std::string m_FilePath;



		std::vector<Triangle> m_Triangles;

		std::vector<glm::vec4> m_VerticesPositions;
		std::vector<glm::vec4> m_VerticesNormals;
		std::vector<uint32_t> m_VerticesIndex;
	};
}
