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


#include "ImGuizmo.h"

namespace Rndr
{
	Sandbox3D::Sandbox3D()
		: Layer("Sandbox3D") //m_CameraController(1920.0f / 1080.0f)
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
		// OnImGuiRender();

		// ImVec2 viewPortSize =  ImGui::GetContentRegionAvail();
		// RNDR_CORE_INFO("Viewport size: {0}, {1}", viewPortSize.x, viewPortSize.y);
		// ImGui::End();

		FrameBufferSpecification frameBufferSpec;
		frameBufferSpec.Width = 1280;
		frameBufferSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);


		m_Texture = Texture2D::Create("Editor/assets/textures/uv.png");
		m_PenguinTexture = Texture2D::Create("Editor/assets/textures/penguin.png");


		m_ActiveScene = CreateRef<Scene>();


		// auto square = m_ActiveScene->CreateEntity("Quad Entity");
		// square.AddComponent<QuadComponent>(m_SquareColor);

		// // glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, -0.1f));
		// // auto square2 = m_ActiveScene->CreateEntity("Quad Entity 2", glm::vec3(3.0f, 0.0f, -0.1f));
		// // square2.AddComponent<QuadComponent>(glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
		// // square2.AddComponent<QuadComponent>();



		// // auto square = m_ActiveScene->CreateEntity("Square");

		// m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
		// m_CameraEntity.AddComponent<CameraComponent>();

		// auto camera2 = m_ActiveScene->CreateEntity("Camera Entity 2");
		// camera2.AddComponent<CameraComponent>();

		// auto greenSquare = m_ActiveScene->CreateEntity("Green Square");
		// greenSquare.AddComponent<QuadComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

		// auto camera = m_ActiveScene->CreateEntity("Camera Entity");
		// auto cam = camera.AddComponent<CameraComponent>().Primary = true;
		// cam.Camera.SetViewportSize(1280, 720);
		

		// auto cube = m_ActiveScene->CreateEntity("Cube");
		// cube.AddComponent<CubeComponent>();



		m_SceneHierarchyPanel.SetContext(m_ActiveScene);



		// SceneSerializer serializer(m_ActiveScene);
		// serializer.Serialize("Editor/assets/scenes/Scene.yaml");
		// serializer.Deserialize("Editor/assets/scenes/Scene.yaml");
	}

	void Sandbox3D::OnDetach()
	{

	}

	void Sandbox3D::OnUpdate(Timestep ts)
	{
		if (m_Resize)
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			// m_Camera = OrthographicCamera(-m_ViewPortSize.x / m_ViewPortSize.y, m_ViewPortSize.x / m_ViewPortSize.y, -1.0f, 1.0f);
			// m_CameraController.OnResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_Resize = false;
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		}



		// m_PerspectiveCamera.OnUpdate(ts);

		// if (m_ViewPortFocused)
		{
			// m_CameraController.OnUpdate(ts);
			// auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
			// auto& transform = m_CameraEntity.GetComponent<TransformComponent>().Transform;
			// // glm::vec3 pos = m_CameraController.GetCamera().GetPosition();
			// // transform = glm::translate(glm::mat4(1.0f), pos);
			// transform = m_CameraController.GetTransform();
		}

		// Renderer2D::ResetStats();

		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.2f, 0.3f, 1));
		RenderCommand::Clear();


		m_ActiveScene->OnUpdate(ts);


		// Renderer2D::BeginScene(m_CameraController.GetCamera());
		// Renderer2D::DrawQuad({ 0.0f, 0.0f}, { 0.5f, 0.5f }, m_SquareColor);
		// Renderer2D::DrawQuad({ 0.5f, 0.3f}, { 0.5f, 0.8f }, glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
		// Renderer2D::DrawQuad({ -5.0f, -5.0f, -0.1}, { 10.0f, 10.0f }, m_Texture, 10.0f);
		// Renderer2D::DrawQuad({ -0.5f, -0.5f, -0.05}, { 1.0f, 1.0f }, m_PenguinTexture);
		// Renderer2D::EndScene();


		// Renderer2D::BeginScene(m_CameraController.GetCamera());
		// constexpr float step = 0.05f;
		// constexpr float size = 5.0f;
		// for (float y = -size; y < size; y += step)
		// {
		// 	for (float x = -size; x < size; x += step)
		// 	{
		// 		glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
		// 		Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
		// 	}
		// }
		// Renderer2D::EndScene();

		m_FrameBuffer->Unbind();
	}

	void Sandbox3D::OnRender()
	{
	}

	void Sandbox3D::OnImGuiRender()
	{
		ImGuiLayer::ImGuiBeginDockspace();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}
			


				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		m_SceneHierarchyPanel.OnImGuiRender();

		ImGui::Begin("Settings");

		// if (ImGui::BeginMenuBar())
		// {
		// 	if (ImGui::BeginMenu("File"))
		// 	{
		// 		if (ImGui::MenuItem("Serialize "))
		// 		{
		// 			SceneSerializer serializer(m_ActiveScene);
		// 			serializer.Serialize("Editor/assets/scenes/Scene.yaml");
		// 		}

		// 		if (ImGui::MenuItem("Deserialize "))
		// 		{
		// 			SceneSerializer serializer(m_ActiveScene);
		// 			serializer.Deserialize("Editor/assets/scenes/Scene.yaml");
		// 		}

		// 		ImGui::EndMenu();
		// 	}

		// 	ImGui::EndMenuBar();
		// }

		if (ImGui::Button("Serialize"))
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize("Editor/assets/scenes/Scene.yaml");
		}

		if (ImGui::Button("Deserialize"))
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize("Editor/assets/scenes/Scene.yaml");
		}


		// auto stats = Renderer2D::GetStats();
		// ImGui::Text("Renderer2D Stats:");
		// ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		// ImGui::Text("Quads: %d", stats.QuadCount);
		// ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		// ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
		glm::vec3 pos = m_PerspectiveCamera.GetPosition();
		ImGui::Text("Camera Position: (%f, %f, %f)", pos.x, pos.y, pos.z);


		// {
		// 	auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
		// 	float orthoSize = camera.GetOrthographicSize();
		// 	if (ImGui::DragFloat("Orthographic Size", &orthoSize))
		// 	{
		// 		camera.SetOrthographicSize(orthoSize);
		// 	}
		// }

		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");

		ImVec2 viewPortSize =  ImGui::GetContentRegionAvail();
		if (m_ViewPortSize != *(glm::vec2*)&viewPortSize && viewPortSize.x > 0 && viewPortSize.y > 0)
		{
			m_ViewPortSize =  { viewPortSize.x, viewPortSize.y };
			m_Resize = true;
			RNDR_WARN("Viewport size: {0}, {1}", viewPortSize.x, viewPortSize.y);
		}


		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentRendererID(), 
			ImVec2{ (float)m_FrameBuffer->GetWidth(), (float)m_FrameBuffer->GetHeight() }, 
			ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// Gizmos
		// Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		// if (selectedEntity)
		// {
		// 	auto tag = selectedEntity.GetComponent<TagComponent>().Tag;
		// 	RNDR_CORE_INFO("Selected Entity: {0}", tag);

		// 	ImGuizmo::SetOrthographic(false);
		// 	ImGuizmo::SetDrawlist();

		// 	float windowWidth = (float)ImGui::GetWindowWidth();
		// 	float windowHeight = (float)ImGui::GetWindowHeight();
		// 	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		// 	// Camera
		// 	auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
		// 	auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
		// 	const glm::mat4& cameraProjection = camera.GetProjection();
		// 	auto& cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

		// 	// Entity transform
		// 	auto& tc = selectedEntity.GetComponent<TransformComponent>();
		// 	glm::mat4 transform = tc.GetTransform();

		// 	// Snapping //TODO

		// 	ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
		// 		ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform));

		// 	if (ImGuizmo::IsUsing())
		// 	{
		// 		glm::vec3 translation, rotation, scale;
		// 		// Math::DecomposeTransform(transform, translation, rotation, scale);
		// 	}
		// }


		ImGui::End();
		ImGui::PopStyleVar();





		ImGuiLayer::ImGuiEndDockspace();
	}

	void Sandbox3D::OnEvent(Event& e)
	{
		//TODO: camera controller
		{
			// m_CameraController.OnEvent(e);

			// auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
			// camera.SetProjection(m_CameraController.GetCamera().GetProjectionMatrix());
		}


		// m_PerspectiveCamera.OnEvent(e);

		RNDR_CORE_INFO("{0}", e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(RNDR_BIND_EVENT_FN(Sandbox3D::OnKeyPressed));
	}

	bool Sandbox3D::OnKeyPressed(KeyPressedEvent& e)
	{
		//TODO: Check why event is not being dispatched
		RNDR_CORE_INFO("Key Pressed: {0}", e.GetKeyCode());
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

		}

		return false;
	}

	void Sandbox3D::NewScene()
	{
		RNDR_CORE_INFO("New Scene");
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void Sandbox3D::OpenScene()
	{
		RNDR_CORE_INFO("Open Scene");
		std::string filepath = FileDialogs::OpenFile("Rndr Scene (*.yaml)\0*.yaml\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
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