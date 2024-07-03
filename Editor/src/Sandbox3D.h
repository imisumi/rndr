#pragma once


#include "Rndr.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Rndr
{
	class Sandbox3D : public Layer
	{
	public:
		Sandbox3D();
		~Sandbox3D();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts);
		void OnRender();
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		// Camera3D m_Camera3D;
		// OrthographicCamera m_Camera;

		OrthographicCameraController m_CameraController;

		PerspectiveCamera m_PerspectiveCamera;

		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_Shader;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		ShaderLibrary m_ShaderLibrary;
	};
}