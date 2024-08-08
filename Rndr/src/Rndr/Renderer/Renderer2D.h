#pragma once

#include "Rndr/Renderer/OrthographicCamera.h"

#include "Rndr/Renderer/opengl/Texture.h"

#include "Rndr/Renderer/Camera.h"

#include "Rndr/Renderer/EditorCamera.h"

#include "Rndr/Scene/Components.h"


#include "Rndr/Renderer/Material.h"

namespace Rndr {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const EditorCamera& camera, const Ref<Material>& material);

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();

		// static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		// static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));


		//TODO: rename to tri
		// static void DrawQuad(const glm::mat4& transform, QuadComponent& tri, int entityID = -1);
		static void DrawCube(const glm::mat4& transform, CubeComponent& cube, 
			DefaultMaterialComponent& material, int entityID = -1);

		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, int entityID = -1);

		// static void DrawCube(const glm::mat4& transform, CubeComponent& cube, const Ref<Material> material, int entityID = -1);

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};

}
