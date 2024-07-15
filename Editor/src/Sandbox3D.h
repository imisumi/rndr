#pragma once


#include "Rndr.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/MaterialPanel.h"


#include "Rndr/Renderer/EditorCamera.h"

#include "Rndr/Renderer/Mesh.h"

#include "Rndr/Renderer/ComputeShader.h"

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


		void LoadMeshEntity(const std::filesystem::path& path);
	private:
		// ShaderLibrary m_ShaderLibrary;

		Ref<Scene> m_ActiveScene;

		Mesh m_Mesh;

		Entity m_CameraEntity;
		Entity m_HoveredEntity = { entt::null, nullptr };
	
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<FrameBuffer> m_ComputeFrameBuffer;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		int m_GizmoType = 7;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		MaterialPanel m_MaterialPanel;

		glm::vec4 M_TestColor = { 0.8f, 0.2f, 0.3f, 1.0f };

		EditorCamera m_EditorCamera;


		Ref<Texture2D> m_CubeIcon;

		Ref<ComputeShader> m_ComputeShader;
		uint32_t m_TempComputeTextureID;

		Ref<LineMaterial> m_LineMaterial;


		// MaterialLibrary m_MaterialLibrary;
		Ref<MaterialLibrary> m_MaterialLibrary;


		std::vector<float> m_SkyPixels;
		uint32_t m_SkyTextureID;
		uint32_t m_SkyTextureWidth, m_SkyTextureHeight;

		bool m_EnableCompute = false;
	};
}