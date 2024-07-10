#pragma once


#include "Rndr.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"


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
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();
	private:
		// ShaderLibrary m_ShaderLibrary;

		Ref<Scene> m_ActiveScene;

		Entity m_CameraEntity;
		Entity m_HoveredEntity = { entt::null, nullptr };
	
		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		int m_GizmoType = 7;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		glm::vec4 M_TestColor = { 0.8f, 0.2f, 0.3f, 1.0f };

		EditorCamera m_EditorCamera;


		Ref<Texture2D> m_CubeIcon;


		MaterialLibrary m_MaterialLibrary;
	};
}