#pragma once


#include "Rndr.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Panels/SceneHierarchyPanel.h"


#include "Rndr/Renderer/EditorCamera.h"

namespace Rndr
{
	class Sandbox3D : public Layer
	{
	public:
		Sandbox3D();
		virtual ~Sandbox3D() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		// ShaderLibrary m_ShaderLibrary;

		Ref<Scene> m_ActiveScene;

		Entity m_CameraEntity;

		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		int m_GizmoType = -1;

		SceneHierarchyPanel m_SceneHierarchyPanel;


		EditorCamera m_EditorCamera;
	};
}