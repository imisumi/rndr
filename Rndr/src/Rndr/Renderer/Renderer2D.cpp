
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
	};

	struct Renderer2DData
	{
		static const uint32_t MaxTries = 101;
		static const uint32_t MaxVertices = MaxTries * 3;
		static const uint32_t MaxIndices = MaxTries * 3;

		Ref<VertexArray> CubeVertexArray;
		Ref<VertexBuffer> CubeVertexBuffer;
		Ref<Shader> TextureShader;

		uint32_t VertexIndexCount = 0;
		Vertex* VertexBufferBase = nullptr;
		uint32_t VertexBufferIndex = 0;
		Vertex* VertexBufferPtr = nullptr;

		glm::vec4 VertexPositions[3];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		// HZ_PROFILE_FUNCTION();

		s_Data.CubeVertexArray = VertexArray::Create();

		s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(Vertex));
		s_Data.CubeVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
		});
		s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

		s_Data.VertexBufferBase = new Vertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 3)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			// RNDR_INFO("{0}, {1}, {2}", quadIndices[i + 0], quadIndices[i + 1], quadIndices[i + 2]);

			offset += 3;
			// if (offset == 30)
			// 	break ;
		}
		// exit(1);

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.CubeVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		s_Data.TextureShader = Shader::Create("Editor/assets/shaders/CubeShader.glsl");
		s_Data.TextureShader->Bind();


		s_Data.VertexPositions[0] = { -0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.VertexPositions[1] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.VertexPositions[2] = {  0.5f, -0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		// HZ_PROFILE_FUNCTION();

		delete[] s_Data.VertexBufferBase;
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		// HZ_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

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
		// RNDR_CORE_INFO("Renderer2D::Flush: dataSize = {0}", dataSize);
		
		// Bind textures
		// for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		// 	s_Data.TextureSlots[i]->Bind(i);
		
		RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.VertexIndexCount);
		// RNDR_CORE_INFO("Renderer2D::Flush: s_Data.VertexIndexCount = {0}", s_Data.VertexIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		// HZ_PROFILE_FUNCTION();
		constexpr size_t VertexCount = 3;

		if (s_Data.VertexIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		for (size_t i = 0; i < VertexCount; i++)
		{
			s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[i];
			// std::cout << s_Data.VertexBufferPtr->Position.x << " " << s_Data.VertexBufferPtr->Position.y << " " << s_Data.VertexBufferPtr->Position.z << std::endl;
			s_Data.VertexBufferPtr++;
		}
		// exit(1);

		s_Data.VertexIndexCount += 3;

		s_Data.Stats.QuadCount++;

		// PrintVertexData();
		// exit(1);
	}

	void Renderer2D::PrintVertexData()
	{
		for (size_t i = 0; i < s_Data.VertexIndexCount; i++)
		{
			std::cout << "Vertex " << i << ": (" << s_Data.VertexBufferBase[i].Position.x << ", "
					<< s_Data.VertexBufferBase[i].Position.y << ", "
					<< s_Data.VertexBufferBase[i].Position.z << ")\n";
		}
		exit(1);
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
