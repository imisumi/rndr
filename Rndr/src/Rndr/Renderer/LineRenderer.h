#pragma once

#include "Rndr/Renderer/EditorCamera.h"
#include "Rndr/Renderer/Material.h"

namespace Rndr
{
	class LineRenderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const EditorCamera& camera, const Ref<LineMaterial>& material, float lineWidth = 1.0f);

		static void EndScene();
		static void Flush();

		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID = -1);

		static void DrawGrid(float gridSize, int divisions, const glm::vec3& origin, const glm::vec4& xAxisColor, const glm::vec4& yAxisColor, const glm::vec4& zAxisColor);

		// static void SetLineWidth(float width);
		// static void GetLineWidth();


		static void DrawAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color, int entityID = -1);


		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t LineCount = 0;

			uint32_t GetTotalVertexCount() const { return LineCount * 2; }
		};
		static void ResetStats();
		static Statistics GetStats();
	
	private:
		static void StartBatch();
		static void NextBatch();
	};
}