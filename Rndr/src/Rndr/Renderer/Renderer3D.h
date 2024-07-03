
#pragma once


#include "Camera3D.h"

#include "Rndr/Renderer/OrthographicCamera.h"

namespace Rndr
{
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();
		// static void Init();
		// static void OnWindowResize(uint32_t width, uint32_t height);
		
		// static void BeginScene();
		// static void BeginScene(Camera3D& camera);
		// static void EndScene();

		// static void Submit(const Ref<Shader>& shader, 
		// 	const Ref<VertexArray>& vertexArray, 
		// 	const glm::mat4& transform = glm::mat4(1.0f));
		// inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		// struct SceneData
		// {
		// 	glm::mat4 ViewProjectionMatrix;
		// };

		// static SceneData* s_SceneData;
	};
}