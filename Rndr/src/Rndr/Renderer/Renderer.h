#pragma once

#include "RenderCommand.h"

#include "Rndr/Renderer/OrthographicCamera.h"
#include "Rndr/Renderer/Camera.h"
#include "Rndr/Renderer/Camera3D.h"
#include "Rndr/Renderer/PerspectiveCamera.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Rndr
{
	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);
		
		static void BeginScene();
		static void BeginScene(OrthographicCamera& camera);
		static void BeginScene(Camera3D& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, 
			const Ref<VertexArray>& vertexArray, 
			const glm::mat4& transform = glm::mat4(1.0f));
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}