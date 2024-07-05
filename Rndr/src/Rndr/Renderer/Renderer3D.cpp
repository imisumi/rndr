#include "Renderer3D.h"

#include "Rndr/Renderer/VertexArray.h"
#include "Rndr/Renderer/Shader.h"
#include "Rndr/Renderer/Texture.h"
#include "Rndr/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

#include <array>

namespace Rndr
{
	struct CubeVertex
	{
		glm::vec3 Position;
		// glm::vec3 Normal;
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		// glm::vec2 TexCoord = { 0.0f, 0.0f };
		// float TexIndex = 0.0f;
		// float TilingFactor = 1.0f;
	};

	struct Renderer3DData
	{
		static const uint32_t MaxCubes = 100;
		static const uint32_t MaxVertices = MaxCubes * 8;
		static const uint32_t MaxIndices = MaxCubes * 36;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t CubeIndexCount = 0;
		CubeVertex* CubeVertexBufferBase = nullptr;
		CubeVertex* CubeVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 CubeVertexPositions[8] = {
			{ -1.0f, -1.0f, 1.0f, 1.0f },
			{ -1.0f, 1.0f, 1.0f, 1.0f },
			{ -1.0f, -1.0f, 1.0f, 1.0f },
			{ -1.0f, 1.0f, -1.0f, 1.0f },
			{ 1.0f, -1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{ 1.0f, -1.0f, -1.0f, 1.0f },
			{ 1.0f, 1.0f, -1.0f, 1.0f }
		};
		glm::vec3 CubeVertexNormals[6] = {
			{ -1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, -1.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 0.0f, -1.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f }
		};

		Renderer3D::Statistics Stats;
	};

	static Renderer3DData s_Data;

	void Renderer3D::Init()
	{
		s_Data.CubeVertexArray = VertexArray::Create();

		s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CubeVertex));
		s_Data.CubeVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			// { ShaderDataType::Float3, "a_Normal" }
			{ ShaderDataType::Float4, "a_Color" },
			// { ShaderDataType::Float2, "a_TexCoord" },
			// { ShaderDataType::Float, "a_TexIndex" },
			// { ShaderDataType::Float, "a_TilingFactor" }
			});
		s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

		s_Data.CubeVertexBufferBase = new CubeVertex[s_Data.MaxVertices];

		uint32_t* cubeIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 36)
		{
			cubeIndices[i + 0] = offset + 2 - 1;
			cubeIndices[i + 1] = offset + 3 - 1;
			cubeIndices[i + 2] = offset + 1 - 1;

			cubeIndices[i + 3] = offset + 4 - 1;
			cubeIndices[i + 4] = offset + 7 - 1;
			cubeIndices[i + 5] = offset + 3 - 1;


			cubeIndices[i + 6] = offset + 8 - 1;
			cubeIndices[i + 7] = offset + 5 - 1;
			cubeIndices[i + 8] = offset + 7 - 1;

			cubeIndices[i + 9] = offset + 6 - 1;
			cubeIndices[i + 10] = offset + 1 - 1;
			cubeIndices[i + 11] = offset + 5 - 1;


			cubeIndices[i + 12] = offset + 7 - 1;
			cubeIndices[i + 13] = offset + 1 - 1;
			cubeIndices[i + 14] = offset + 3 - 1;

			cubeIndices[i + 15] = offset + 4 - 1;
			cubeIndices[i + 16] = offset + 6 - 1;
			cubeIndices[i + 17] = offset + 8 - 1;


			cubeIndices[i + 18] = offset + 2 - 1;
			cubeIndices[i + 19] = offset + 4 - 1;
			cubeIndices[i + 20] = offset + 3 - 1;

			cubeIndices[i + 21] = offset + 4 - 1;
			cubeIndices[i + 22] = offset + 8 - 1;
			cubeIndices[i + 23] = offset + 7 - 1;


			cubeIndices[i + 24] = offset + 8 - 1;
			cubeIndices[i + 25] = offset + 6 - 1;
			cubeIndices[i + 26] = offset + 4 - 1;

			cubeIndices[i + 27] = offset + 6 - 1;
			cubeIndices[i + 28] = offset + 2 - 1;
			cubeIndices[i + 29] = offset + 1 - 1;


			cubeIndices[i + 30] = offset + 7 - 1;
			cubeIndices[i + 31] = offset + 5 - 1;
			cubeIndices[i + 32] = offset + 1 - 1;

			cubeIndices[i + 33] = offset + 4 - 1;
			cubeIndices[i + 34] = offset + 2 - 1;
			cubeIndices[i + 35] = offset + 6 - 1;

			offset += 8;
			break ;
		}

		Ref<IndexBuffer> cubeIB = IndexBuffer::Create(cubeIndices, s_Data.MaxIndices);
		s_Data.CubeVertexArray->SetIndexBuffer(cubeIB);
		delete[] cubeIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (int32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader = Shader::Create("Editor/assets/shaders/CubeShader.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
	}

	void Renderer3D::Shutdown()
	{
		delete[] s_Data.CubeVertexBufferBase;
	}

	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
		// camera.

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer3D::EndScene()
	{
		Flush();
	}

	void Renderer3D::StartBatch()
	{
		s_Data.CubeIndexCount = 0;
		s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer3D::Flush()
	{
		if (s_Data.CubeIndexCount == 0)
			return; // Nothing to draw

		uint32_t dataSize = (uint8_t*)s_Data.CubeVertexBufferPtr - (uint8_t*)s_Data.CubeVertexBufferBase;
		s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferBase, dataSize);

		// for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		// 	s_Data.TextureSlots[i]->Bind(i);

		// s_Data.CubeVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.CubeIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer3D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer3D::DrawCube(const glm::mat4& transform, const glm::vec4& color)
	{
		constexpr size_t CubeVertexCount = 8;
		const float textureIndex = 0.0f; // White texture

		if (s_Data.CubeIndexCount >= Renderer3DData::MaxIndices)
			NextBatch();

		// for (size_t i = 0; i < CubeVertexCount; i++)
		// {
			// s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[i];
			// s_Data.CubeVertexBufferPtr->Color = color;
			// s_Data.CubeVertexBufferPtr->TexCoord = textureCoords[i];
			// s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
			// s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
		// }

		// vn -1.0000 -0.0000 -0.0000
		// vn -0.0000 -0.0000 -1.0000
		// vn 1.0000 -0.0000 -0.0000
		// vn -0.0000 -0.0000 1.0000
		// vn -0.0000 -1.0000 -0.0000
		// vn -0.0000 1.0000 -0.0000

		glm::vec4 colors[8] = {
			{ 0.2f, 0.6f, 0.8f, 1.0f },  // Light Blue
			{ 0.9f, 0.3f, 0.5f, 1.0f },  // Rose Pink
			{ 0.3f, 0.7f, 0.4f, 1.0f },  // Mint Green
			{ 0.9f, 0.8f, 0.2f, 1.0f },  // Sunny Yellow
			{ 0.6f, 0.3f, 0.9f, 1.0f },  // Lavender Purple
			{ 0.2f, 0.8f, 0.7f, 1.0f },  // Aqua
			{ 0.8f, 0.4f, 0.1f, 1.0f },  // Burnt Orange
			{ 0.4f, 0.6f, 0.9f, 1.0f }   // Sky Blue
		};

		for (size_t i = 0; i < CubeVertexCount; i++)
		{
			s_Data.CubeVertexBufferPtr->Position = transform * s_Data.CubeVertexPositions[i];
			// s_Data.CubeVertexBufferPtr->Normal = 
			s_Data.CubeVertexBufferPtr->Color = colors[i];
			// s_Data.CubeVertexBufferPtr->TexCoord = textureCoords[i % 4];
			// s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
			// s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.CubeVertexBufferPtr++;
		}

		s_Data.CubeIndexCount += 36;
		s_Data.Stats.QuadCount++;
	}








	void Renderer3D::ResetStats()
	{
		s_Data.Stats = {};
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_Data.Stats;
	}
}