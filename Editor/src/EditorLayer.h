#pragma once

#include "Rndr.h"

#include "Rndr/Renderer/Mesh.h"
#include "Rndr/Renderer/Camera3D.h"

namespace Rndr
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnMovement(Timestep ts);
		void OnUpdate(Timestep ts) override;

		void ImGuiBeginDockspace();
		void ImGuiEndDockspace();

		void OnImGuiRender() override;

		void OnEvent(Event& event) override;


	private:
		ShaderLibrary m_ShaderLibrary;
		// Ref<Shader> m_TextureShader;
		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;
			
		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;

		Ref<Texture2D> m_Texture, m_TextureAlpha;

		// Ref<Mesh> m_Mesh;
		Mesh m_Mesh;

		Ref<FrameBuffer> m_FrameBuffer;

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec2 m_ViewPortSize;
		bool m_Resize = false;


		OrthographicCamera m_Camera;
		Camera3D m_Camera3D;


		glm::vec3 m_CameraPosition;
		float m_CameraMoveSpeed = 5.0f;

		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 45.0f;

		glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
	};
}