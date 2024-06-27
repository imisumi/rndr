#pragma once

#include "RenderCommand.h"

#include "Rndr/Renderer/Camera.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Rndr
{
	class Renderer
	{
	public:
		static void BeginScene();
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Renderer::Submit(const Ref<Shader>& shader, 
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