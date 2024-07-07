#include "Sandbox3D.h"


#include <iostream>

// #include "Platform/OpenGL/OpenGLShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"

#include "Rndr/Core/Input.h"

#include "Rndr/Scene/SceneSerializer.h"

#include "Rndr/Utils/PlatformUtils.h"


// #include "ImGuizmo.h"
#include "Rndr/ImGui/ImGuizmo.h"



#include "Rndr/Math/Math.h"

namespace Rndr
{
	Sandbox3D::Sandbox3D()
		: Layer("Sandbox3D") 
	{
	}

	void Sandbox3D::OnAttach()
	{
		FrameBufferSpecification frameBufferSpec;
		frameBufferSpec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::Depth };
		frameBufferSpec.Width = 1280;
		frameBufferSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);
		m_FrameBuffer->Resize(1000, 1000);
		// m_FrameBuffer->~FrameBuffer();
		// m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);



		m_ActiveScene = CreateRef<Scene>();


		// auto square = m_ActiveScene->CreateEntity("Quad Entity");
		// square.AddComponent<QuadComponent>(m_SquareColor);

		// auto camera = m_ActiveScene->CreateEntity("Camera Entity");
		// auto cam = camera.AddComponent<CameraComponent>().Primary = true;
		// cam.Camera.SetViewportSize(1280, 720);


		// auto tri = m_ActiveScene->CreateEntity("Triangle");
		// tri.AddComponent<QuadComponent>(glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);


		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
	}

	void Sandbox3D::OnDetach()
	{

	}

	void Sandbox3D::OnUpdate(Timestep ts)
	{
		FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			FrameBufferSpecification spec;
			spec.Width = m_ViewportSize.x;
			spec.Height = m_ViewportSize.y;
			// m_FrameBuffer = FrameBuffer::Create(spec);
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			// m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		if (m_ViewportFocused)
		{
			// m_CameraController.OnUpdate(ts);
			m_EditorCamera.OnUpdate(ts);
		}

		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.2f, 0.3f, 1));
		RenderCommand::Clear();

		// m_ActiveScene->OnUpdateRuntime(ts);
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		m_FrameBuffer->Unbind();
	}


	void Sandbox3D::OnImGuiRender()
	{
		ImGuiLayer::ImGuiBeginDockspace();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		ImGui::Text("Viewport Size: %.0fx%.0f", m_ViewportSize.x, m_ViewportSize.y);

		ImGui::Text("Framebuffer Size: %dx%d", m_FrameBuffer->GetWidth(), m_FrameBuffer->GetHeight());
		// auto stats = Renderer2D::GetStats();
		// ImGui::Text("Renderer2D Stats:");
		// ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		// ImGui::Text("Quads: %d", stats.QuadCount);
		// ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		// ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t depthTextureID = m_FrameBuffer->GetDepthAttachmentRendererID();
		uint64_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::Image((void*)depthTextureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(RNDR_KEY_LEFT_CONTROL);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}


		ImGui::End();
		ImGui::PopStyleVar();

		// ImGui::End();
		ImGuiLayer::ImGuiEndDockspace();
	}




	void Sandbox3D::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);
		//TODO: camera controller
	
		// RNDR_CORE_INFO("{0}", e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(RNDR_BIND_EVENT_FN(Sandbox3D::OnKeyPressed));
	}

	bool Sandbox3D::OnKeyPressed(KeyPressedEvent& e)
	{
		// RNDR_CORE_INFO("Key Pressed: {0}", e.GetKeyCode());
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(RNDR_KEY_LEFT_CONTROL) || Input::IsKeyPressed(RNDR_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(RNDR_KEY_LEFT_SHIFT) || Input::IsKeyPressed(RNDR_KEY_RIGHT_SHIFT);
		switch (e.GetKeyCode())
		{
			case RNDR_KEY_S:
			{
				if (control && shift)
					SaveSceneAs();
				break;
			}
			case RNDR_KEY_O:
			{
				if (control)
					OpenScene();
				break;
			}
			case RNDR_KEY_N:
			{
				if (control)
					NewScene();
				break;
			}


			// Gizmos
			case RNDR_KEY_Q:
				m_GizmoType = -1;
				break;
			case RNDR_KEY_W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case RNDR_KEY_E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case RNDR_KEY_R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;

		}

		return false;
	}

	void Sandbox3D::NewScene()
	{
		RNDR_CORE_INFO("New Scene");
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void Sandbox3D::OpenScene()
	{
		RNDR_CORE_INFO("Open Scene");
		std::string filepath = FileDialogs::OpenFile("Rndr Scene (*.yaml)\0*.yaml\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void Sandbox3D::SaveSceneAs()
	{
		RNDR_CORE_INFO("Save Scene As");
		std::string filepath = FileDialogs::SaveFile("Rndr Scene (*.yaml)\0*.yaml\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}
}