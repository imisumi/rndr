
// #include "Rndr/Renderer/Renderer2D.h"

// #include "Rndr/Renderer/VertexArray.h"
// #include "Rndr/Renderer/Shader.h"
// #include "Rndr/Renderer/RenderCommand.h"

// #include <glm/gtc/matrix_transform.hpp>

// #include <array>

// namespace Rndr {
// 	struct Vertex
// 	{
// 		glm::vec3 Position;
// 		glm::vec3 Color;
// 		glm::vec2 TexCoord;
// 		int EntityID;
// 	};

// 	struct Renderer2DData
// 	{
// 		static const uint32_t MaxTries = 101;
// 		static const uint32_t MaxVertices = MaxTries * 3;
// 		static const uint32_t MaxIndices = MaxTries * 3;

// 		Ref<VertexArray> CubeVertexArray;
// 		Ref<VertexBuffer> CubeVertexBuffer;
// 		Ref<Shader> TextureShader;

// 		uint32_t VertexIndexCount = 0;
// 		Vertex* VertexBufferBase = nullptr;
// 		uint32_t VertexBufferIndex = 0;
// 		Vertex* VertexBufferPtr = nullptr;

// 		glm::vec4 VertexPositions[3];

// 		Renderer2D::Statistics Stats;

// 		// Ref<ViewportGrid> grid;

		
// 	};

// 	static Renderer2DData s_Data;

// 	void Renderer2D::Init()
// 	{

// 		// HZ_PROFILE_FUNCTION();

// 		s_Data.CubeVertexArray = VertexArray::Create();

// 		s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(Vertex));
// 		s_Data.CubeVertexBuffer->SetLayout({
// 			{ ShaderDataType::Float3, "a_Position" },
// 			{ ShaderDataType::Float3, "a_Color"},
// 			{ ShaderDataType::Float2, "a_TexCoord" },
// 			{ ShaderDataType::Int, "a_EntityID" }
// 		});
// 		s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

// 		s_Data.VertexBufferBase = new Vertex[s_Data.MaxVertices];

// 		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

// 		uint32_t offset = 0;
// 		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 3)
// 		{
// 			quadIndices[i + 0] = offset + 0;
// 			quadIndices[i + 1] = offset + 1;
// 			quadIndices[i + 2] = offset + 2;

// 			// RNDR_INFO("{0}, {1}, {2}", quadIndices[i + 0], quadIndices[i + 1], quadIndices[i + 2]);

// 			offset += 3;
// 			// if (offset == 30)
// 			// 	break ;
// 		}
// 		// exit(1);

// 		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
// 		s_Data.CubeVertexArray->SetIndexBuffer(quadIB);
// 		delete[] quadIndices;


// 		s_Data.TextureShader = Shader::Create("Editor/assets/shaders/CubeShader.glsl");
// 		s_Data.TextureShader->Bind();


// 		s_Data.VertexPositions[0] = { -0.5f,  0.5f, 0.0f, 1.0f };
// 		s_Data.VertexPositions[1] = { -0.5f, -0.5f, 0.0f, 1.0f };
// 		s_Data.VertexPositions[2] = {  0.5f, -0.5f, 0.0f, 1.0f };
// 	}

// 	void Renderer2D::Shutdown()
// 	{
// 		// HZ_PROFILE_FUNCTION();

// 		delete[] s_Data.VertexBufferBase;
// 	}

// 	void Renderer2D::BeginScene(const EditorCamera& camera)
// 	{
// 		// HZ_PROFILE_FUNCTION();

// 		s_Data.TextureShader->Bind();
// 		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

// 		StartBatch();
// 	}

// 	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
// 	{
// 		// HZ_PROFILE_FUNCTION();

// 		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

// 		s_Data.TextureShader->Bind();
// 		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

// 		StartBatch();
// 	}

// 	void Renderer2D::EndScene()
// 	{
// 		// HZ_PROFILE_FUNCTION();

// 		Flush();
// 	}

// 	void Renderer2D::StartBatch()
// 	{
// 		s_Data.VertexIndexCount = 0;
// 		s_Data.VertexBufferIndex = 0;
// 		s_Data.VertexBufferPtr = s_Data.VertexBufferBase;

// 		// s_Data.TextureSlotIndex = 1;
// 	}

// 	void Renderer2D::Flush()
// 	{
// 		if (s_Data.VertexIndexCount == 0)
// 			return; // Nothing to draw

// 		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase);
// 		s_Data.CubeVertexBuffer->SetData(s_Data.VertexBufferBase, dataSize);
// 		// RNDR_CORE_INFO("Renderer2D::Flush: dataSize = {0}", dataSize);
		
// 		// Bind textures
// 		// for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
// 		// 	s_Data.TextureSlots[i]->Bind(i);
		
// 		RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.VertexIndexCount);
// 		// RNDR_CORE_INFO("Renderer2D::Flush: s_Data.VertexIndexCount = {0}", s_Data.VertexIndexCount);
// 		s_Data.Stats.DrawCalls++;
// 	}

// 	void Renderer2D::NextBatch()
// 	{
// 		Flush();
// 		StartBatch();
// 	}

// 	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
// 	{
// 		// HZ_PROFILE_FUNCTION();
// 		constexpr size_t VertexCount = 3;

// 		if (s_Data.VertexIndexCount >= Renderer2DData::MaxIndices)
// 			NextBatch();

// 		for (size_t i = 0; i < VertexCount; i++)
// 		{
// 			s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[i];
// 			s_Data.VertexBufferPtr->Color = color;
// 			// s_Data.VertexBufferPtr->TexCoord = s_Data.QuadTextureCoords[i];
// 			// std::cout << s_Data.VertexBufferPtr->Position.x << " " << s_Data.VertexBufferPtr->Position.y << " " << s_Data.VertexBufferPtr->Position.z << std::endl;
// 			s_Data.VertexBufferPtr++;
// 		}

// 		s_Data.VertexIndexCount += 3;

// 		s_Data.Stats.QuadCount++;
// 	}

// 	void Renderer2D::DrawQuad(const glm::mat4& transform, QuadComponent& tri, int entityID)
// 	{
// 		constexpr size_t VertexCount = 3;

// 		if (s_Data.VertexIndexCount >= Renderer2DData::MaxIndices)
// 			NextBatch();

// 		for (size_t i = 0; i < VertexCount; i++)
// 		{
// 			s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[i];
// 			s_Data.VertexBufferPtr->Color = tri.Color;
// 			s_Data.VertexBufferPtr->EntityID = entityID;
// 			s_Data.VertexBufferPtr++;
// 		}

// 		s_Data.VertexIndexCount += 3;

// 		s_Data.Stats.QuadCount++;
// 	}

// 	void Renderer2D::PrintVertexData()
// 	{
// 		for (size_t i = 0; i < s_Data.VertexIndexCount; i++)
// 		{
// 			std::cout << "Vertex " << i << ": (" << s_Data.VertexBufferBase[i].Position.x << ", "
// 					<< s_Data.VertexBufferBase[i].Position.y << ", "
// 					<< s_Data.VertexBufferBase[i].Position.z << ")\n";
// 		}
// 		exit(1);
// 	}

// 	void Renderer2D::ResetStats()
// 	{
// 		memset(&s_Data.Stats, 0, sizeof(Statistics));
// 	}

// 	Renderer2D::Statistics Renderer2D::GetStats()
// 	{
// 		return s_Data.Stats;
// 	}

// }






#include "Rndr/Renderer/Renderer2D.h"

#include "Rndr/Renderer/VertexArray.h"
#include "Rndr/Renderer/Shader.h"
#include "Rndr/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

#include <array>

namespace Rndr {
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec2 TexCoord;
		glm::vec3 Normal;
		int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxCubes = 2;
		static const uint32_t MaxVertices = MaxCubes * 24;
		static const uint32_t MaxIndices = MaxCubes * 36;

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<Shader> TextureShader;

		uint32_t VertexIndexCount = 0;
		Vertex* VertexBufferBase = nullptr;
		uint32_t VertexBufferIndex = 0;
		Vertex* VertexBufferPtr = nullptr;

		glm::vec4 VertexPositions[24];

		Renderer2D::Statistics Stats;

		// Ref<ViewportGrid> grid;

		
	};

	static Renderer2DData s_Data;
	static glm::mat4 s_ViewProjection;

	void Renderer2D::Init()
	{

		// HZ_PROFILE_FUNCTION();

		s_Data.CubeVertexArray = VertexArray::Create();

		s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(Vertex));
		s_Data.CubeVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Color"},
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Int, "a_EntityID" }
		});
		s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

		s_Data.VertexBufferBase = new Vertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 36)
		{
			//? Front face 1
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			//? Front face 2
			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			//? Back face 1
			quadIndices[i + 6] = offset + 4;
			quadIndices[i + 7] = offset + 5;
			quadIndices[i + 8] = offset + 6;
			//? Back face 2
			quadIndices[i + 9] = offset + 6;
			quadIndices[i + 10] = offset + 7;
			quadIndices[i + 11] = offset + 4;

			//? Right face 1
			quadIndices[i + 12] = offset + 8;
			quadIndices[i + 13] = offset + 9;
			quadIndices[i + 14] = offset + 10;
			//? Right face 2
			quadIndices[i + 15] = offset + 10;
			quadIndices[i + 16] = offset + 11;
			quadIndices[i + 17] = offset + 8;

			//? Left face 1
			quadIndices[i + 18] = offset + 12;
			quadIndices[i + 19] = offset + 13;
			quadIndices[i + 20] = offset + 14;
			//? Left face 2
			quadIndices[i + 21] = offset + 14;
			quadIndices[i + 22] = offset + 15;
			quadIndices[i + 23] = offset + 12;

			//? Top face 1
			quadIndices[i + 24] = offset + 16;
			quadIndices[i + 25] = offset + 17;
			quadIndices[i + 26] = offset + 18;
			//? Top face 2
			quadIndices[i + 27] = offset + 18;
			quadIndices[i + 28] = offset + 19;
			quadIndices[i + 29] = offset + 16;

			//? Bottom face 1
			quadIndices[i + 30] = offset + 20;
			quadIndices[i + 31] = offset + 21;
			quadIndices[i + 32] = offset + 22;
			//? Bottom face 2
			quadIndices[i + 33] = offset + 22;
			quadIndices[i + 34] = offset + 23;
			quadIndices[i + 35] = offset + 20;

			offset += 24;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.CubeVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		s_Data.TextureShader = Shader::Create("Editor/assets/shaders/CubeShader.glsl");
		s_Data.TextureShader->Bind();

		//? Front face
		s_Data.VertexPositions[0] = { -0.5f,  0.5f, 0.5f, 1.0f }; //? Top left
		s_Data.VertexPositions[1] = { -0.5f, -0.5f, 0.5f, 1.0f }; //? Bottom left
		s_Data.VertexPositions[2] = {  0.5f, -0.5f, 0.5f, 1.0f }; //? Bottom right
		s_Data.VertexPositions[3] = {  0.5f,  0.5f, 0.5f, 1.0f }; //? Top right

		//? Back face
		s_Data.VertexPositions[4] = { -0.5f,  0.5f, -0.5f, 1.0f }; //? Top left
		s_Data.VertexPositions[5] = { -0.5f, -0.5f, -0.5f, 1.0f }; //? Bottom left
		s_Data.VertexPositions[6] = {  0.5f, -0.5f, -0.5f, 1.0f }; //? Bottom right
		s_Data.VertexPositions[7] = {  0.5f,  0.5f, -0.5f, 1.0f }; //? Top right

		//? Right face
		s_Data.VertexPositions[8] = { 0.5f,  0.5f,  0.5f, 1.0f }; //? Top left
		s_Data.VertexPositions[9] = { 0.5f, -0.5f,  0.5f, 1.0f }; //? Bottom left
		s_Data.VertexPositions[10] = { 0.5f, -0.5f, -0.5f, 1.0f }; //? Bottom right
		s_Data.VertexPositions[11] = { 0.5f,  0.5f, -0.5f, 1.0f }; //? Top right

		//? Left face
		s_Data.VertexPositions[12] = { -0.5f,  0.5f,  0.5f, 1.0f }; //? Top left
		s_Data.VertexPositions[13] = { -0.5f, -0.5f,  0.5f, 1.0f }; //? Bottom left
		s_Data.VertexPositions[14] = { -0.5f, -0.5f, -0.5f, 1.0f }; //? Bottom right
		s_Data.VertexPositions[15] = { -0.5f,  0.5f, -0.5f, 1.0f }; //? Top right

		//? Top face
		s_Data.VertexPositions[16] = { -0.5f, 0.5f,  0.5f, 1.0f }; //? Top left
		s_Data.VertexPositions[17] = { -0.5f, 0.5f, -0.5f, 1.0f }; //? Bottom left
		s_Data.VertexPositions[18] = {  0.5f, 0.5f, -0.5f, 1.0f }; //? Bottom right
		s_Data.VertexPositions[19] = {  0.5f, 0.5f,  0.5f, 1.0f }; //? Top right

		//? Bottom face
		s_Data.VertexPositions[20] = { -0.5f, -0.5f,  0.5f, 1.0f }; //? Top left
		s_Data.VertexPositions[21] = { -0.5f, -0.5f, -0.5f, 1.0f }; //? Bottom left
		s_Data.VertexPositions[22] = {  0.5f, -0.5f, -0.5f, 1.0f }; //? Bottom right
		s_Data.VertexPositions[23] = {  0.5f, -0.5f,  0.5f, 1.0f }; //? Top right
	}

	void Renderer2D::Shutdown()
	{
		// HZ_PROFILE_FUNCTION();

		delete[] s_Data.VertexBufferBase;
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		// HZ_PROFILE_FUNCTION();

		s_ViewProjection = camera.GetViewProjection();

		// s_Data.TextureShader->Bind();
		// s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera, const Ref<Material>& material)
	{
		// HZ_PROFILE_FUNCTION();

		// s_Data.TextureShader->Bind();
		// s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
		material->Bind();
		material->GetShader()->SetMat4("u_ViewProjection", camera.GetViewProjection());

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		// HZ_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	

	void Renderer2D::EndScene()
	{
		// HZ_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.VertexIndexCount = 0;
		s_Data.VertexBufferIndex = 0;
		s_Data.VertexBufferPtr = s_Data.VertexBufferBase;

		// s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		if (s_Data.VertexIndexCount == 0)
			return; // Nothing to draw

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase);
		s_Data.CubeVertexBuffer->SetData(s_Data.VertexBufferBase, dataSize);
		
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.VertexIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, QuadComponent& tri, int entityID)
	{
		constexpr size_t VertexCount = 24;

		if (s_Data.VertexIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		constexpr glm::vec3 normals[] = {
			{0.0f, 0.0f, 1.0f}, // Front face
			{0.0f, 0.0f, -1.0f}, // Back face
			{1.0f, 0.0f, 0.0f}, // Right face
			{-1.0f, 0.0f, 0.0f}, // Left face
			{0.0f, 1.0f, 0.0f}, // Top face
			{0.0f, -1.0f, 0.0f} // Bottom face
		};

		uint32_t normalIndex = 0;
		for (size_t i = 0; i < VertexCount; i++)
		{
			s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[i];
			s_Data.VertexBufferPtr->Color = tri.Color;
			s_Data.VertexBufferPtr->Normal = normals[normalIndex];
			s_Data.VertexBufferPtr->EntityID = entityID;
			s_Data.VertexBufferPtr++;
			if (i % 4 == 3)
				normalIndex++;
		}

		s_Data.VertexIndexCount += 36;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawCube(const glm::mat4& transform, CubeComponent& cube, int entityID)
	{
		constexpr size_t VertexCount = 24;

		if (s_Data.VertexIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		constexpr glm::vec3 normals[] = {
			{0.0f, 0.0f, 1.0f}, // Front face
			{0.0f, 0.0f, -1.0f}, // Back face
			{1.0f, 0.0f, 0.0f}, // Right face
			{-1.0f, 0.0f, 0.0f}, // Left face
			{0.0f, 1.0f, 0.0f}, // Top face
			{0.0f, -1.0f, 0.0f} // Bottom face
		};

		uint32_t normalIndex = 0;
		for (size_t i = 0; i < VertexCount; i++)
		{
			s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[i];
			// s_Data.VertexBufferPtr->Position = s_Data.VertexPositions[i];
			s_Data.VertexBufferPtr->Color = cube.Color;
			s_Data.VertexBufferPtr->Normal = normals[normalIndex];
			s_Data.VertexBufferPtr->EntityID = entityID;
			s_Data.VertexBufferPtr++;
			if (i % 4 == 3)
				normalIndex++;
		}

		s_Data.VertexIndexCount += 36;

		s_Data.Stats.QuadCount++;
	}


	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

}
