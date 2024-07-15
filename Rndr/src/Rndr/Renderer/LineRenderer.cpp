



#include "LineRenderer.h"

#include "Rndr/Renderer/VertexArray.h"
#include "Rndr/Renderer/VertexArray.h"

#include "Rndr/Renderer/RenderCommand.h"

namespace Rndr
{
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		int EntityID;
	};

	struct LineRendererData
	{
		static const uint32_t MaxLines = 10000;
		static const uint32_t MaxVertices = MaxLines * 2;
		static const uint32_t MaxIndices = MaxLines * 2;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<LineMaterial> LineMaterial;

		float LineWidth = 1.0f;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		LineRenderer::Statistics Stats;
	};

	static LineRendererData s_Data;

	void LineRenderer::Init()
	{
		s_Data.LineVertexArray = VertexArray::Create();
		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Int, "a_EntityID" }
		});
		// return ;
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
	}

	void LineRenderer::Shutdown()
	{
		delete[] s_Data.LineVertexBufferBase;
	}

	void LineRenderer::BeginScene(const EditorCamera& camera, const Ref<LineMaterial>& material, float lineWidth)
	{
		s_Data.LineMaterial = material;
		s_Data.LineWidth = lineWidth;


		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;


		s_Data.LineMaterial->Bind();
		// s_Data.LineMaterial->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		material->GetShader()->SetMat4("u_ViewProjection", camera.GetViewProjection());
	}

	void LineRenderer::EndScene()
	{
		Flush();
	}

	void LineRenderer::Flush()
	{
		if (s_Data.LineVertexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
		s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

		// RenderCommand::SetLineWidth(s_Data.LineWidth);
		RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount, s_Data.LineWidth);
		s_Data.Stats.DrawCalls++;
	}

	void LineRenderer::StartBatch()
	{
		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
	}

	void LineRenderer::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void LineRenderer::DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID)
	{
		if (s_Data.LineVertexCount >= LineRendererData::MaxIndices)
		{
			NextBatch();
		}

		s_Data.LineVertexBufferPtr->Position = start;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = end;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
		s_Data.Stats.LineCount++;
	}

	// void LineRenderer::SetLineWidth(float width)
	// {
	// 	RenderCommand::SetLineWidth(width);
	// }

	void LineRenderer::DrawGrid(float gridSize, int divisions, const glm::vec3& origin, const glm::vec4& xAxisColor, const glm::vec4& yAxisColor, const glm::vec4& zAxisColor)
	{
		float halfSize = gridSize * divisions / 2.0f;

		// Draw grid lines along the X axis
		for (int i = -divisions; i <= divisions; ++i) {
			glm::vec3 start = origin + glm::vec3(i * gridSize, -halfSize, 0.0f);
			glm::vec3 end = origin + glm::vec3(i * gridSize, halfSize, 0.0f);
			LineRenderer::DrawLine(start, end, xAxisColor);
		}

		// Draw grid lines along the Y axis
		for (int i = -divisions; i <= divisions; ++i) {
			glm::vec3 start = origin + glm::vec3(-halfSize, i * gridSize, 0.0f);
			glm::vec3 end = origin + glm::vec3(halfSize, i * gridSize, 0.0f);
			LineRenderer::DrawLine(start, end, yAxisColor);
		}

		// Optionally, draw grid lines along the Z axis (if needed)
		// for (int i = -divisions; i <= divisions; ++i) {
		//     glm::vec3 start = origin + glm::vec3(0.0f, 0.0f, i * gridSize);
		//     glm::vec3 end = origin + glm::vec3(0.0f, 0.0f, i * gridSize);
		//     LineRenderer::DrawLine(start, end, zAxisColor);
		// }
	}


	void LineRenderer::DrawAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = min;
		glm::vec3 p1 = { min.x, min.y, max.z };
		glm::vec3 p2 = { min.x, max.y, min.z };
		glm::vec3 p3 = { min.x, max.y, max.z };
		glm::vec3 p4 = { max.x, min.y, min.z };
		glm::vec3 p5 = { max.x, min.y, max.z };
		glm::vec3 p6 = { max.x, max.y, min.z };
		glm::vec3 p7 = max;

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p3, color, entityID);
		DrawLine(p3, p2, color, entityID);
		DrawLine(p2, p0, color, entityID);

		DrawLine(p4, p5, color, entityID);
		DrawLine(p5, p7, color, entityID);
		DrawLine(p7, p6, color, entityID);
		DrawLine(p6, p4, color, entityID);

		DrawLine(p0, p4, color, entityID);
		DrawLine(p1, p5, color, entityID);
		DrawLine(p2, p6, color, entityID);
		DrawLine(p3, p7, color, entityID);
	}
}