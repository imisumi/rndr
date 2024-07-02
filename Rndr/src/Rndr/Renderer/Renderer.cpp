#include "Renderer.h"

#include <iostream>

#include "Platform/OpenGL/OpenGLShader.h"

#include <iostream>
#include <memory>

namespace Rndr
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene()
	{
		
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	void Renderer::BeginScene(Camera3D& camera)
	{
		auto viewProjection = camera.GetProjectionMatrix() * camera.GetViewMatrix();
		s_SceneData->ViewProjectionMatrix = viewProjection;
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, 
		const Ref<VertexArray>& vertexArray, 
		const glm::mat4& transform)
	{
		// std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		// shader->Bind();
		// shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		// shader->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}