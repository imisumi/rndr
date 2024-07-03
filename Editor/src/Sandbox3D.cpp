#include "Sandbox3D.h"


#include <iostream>

// #include "Platform/OpenGL/OpenGLShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"

#include "Rndr/Core/Input.h"


namespace Rndr
{
	Sandbox3D::Sandbox3D()
		: Layer("Sandbox3D"), m_CameraController(1280.0f / 720.0f)
	{
		// m_Camera3D(glm::perspective(glm::radians(45.0f), 991.0f / 617.0f, 0.1f, 1000.0f)),
		// m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
		// m_PerspectiveCamera = PerspectiveCamera(45.0f, 991.0f / 617.0f, 0.1f, 1000.0f);
	}

	Sandbox3D::~Sandbox3D()
	{
		
	}

	void Sandbox3D::OnAttach()
	{

	}

	void Sandbox3D::OnDetach()
	{

	}

	void Sandbox3D::OnUpdate(Timestep ts)
	{
		// m_PerspectiveCamera.OnUpdate(ts);
		m_CameraController.OnUpdate(ts);

		RenderCommand::SetClearColor(glm::vec4(1.0f, 0.1f, 1.0f, 1));
		RenderCommand::Clear();


		Renderer2D::BeginScene(m_CameraController.GetCamera());
		Renderer2D::DrawQuad({ 0.0f, 0.0f}, { 0.5f, 0.5f }, m_SquareColor);
		Renderer2D::DrawQuad({ 1.0f, 1.0f}, { 0.5f, 0.5f }, m_SquareColor);
		Renderer2D::EndScene();
	}

	void Sandbox3D::OnRender()
	{
	}

	void Sandbox3D::OnImGuiRender()
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		glm::vec3 pos = m_PerspectiveCamera.GetPosition();
		ImGui::Text("Camera Position: (%f, %f, %f)", pos.x, pos.y, pos.z);

		ImGui::End();
	}

	void Sandbox3D::OnEvent(Event& e)
	{
		//TODO: camera controller
		m_CameraController.OnEvent(e);
		// m_PerspectiveCamera.OnEvent(e);
	}
}