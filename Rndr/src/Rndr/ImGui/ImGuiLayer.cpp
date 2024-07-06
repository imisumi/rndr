#include "ImGuiLayer.h"

// #include "imgui.h"
// #include "backends/imgui_impl_opengl3.h"
// #include "backends/imgui_impl_glfw.h"

// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"
// #include "Engine/Platform/OpenGL/ImGuiOpenGLRenderer.h"

//! TEMPORARY
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

// #define IMGUI_IMPL_OPENGL_LOADER_GLAD
// #include <examples/imgui_impl_opengl3.cpp>
// #include <examples/imgui_impl_glfw.cpp>

#include "Rndr/Core/Application.h"

#include "ImGuizmo.h"

// #include <examples/imgui_impl_glfw.h>
// #include <examples/imgui_impl_opengl3.h>

namespace Rndr
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	
	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		// io.ConfigViewportsNoAutoMerge = true;
		// io.ConfigViewportsNoTaskBarIcon = true;


		// io.Fonts->AddFontFromFileTTF("editor/assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f);
		io.Fonts->AddFontFromFileTTF("editor/assets/fonts/opensans/OpenSans-ExtraBold.ttf", 18.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("editor/assets/fonts/opensans/OpenSans-SemiBold.ttf", 18.0f);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}


		//? ImGui Color Theme
		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

	}

	void ImGuiLayer::OnDetach()
	{
		// HZ_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		// HZ_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}
	void ImGuiLayer::End()
	{
		// HZ_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}



	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		// colors[ImGuiCol_TabSelectedOverline] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		// colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}


	// void ImGuiLayer::OnImGuiRender()
	// {
	// 	static bool show = true;
	// 	ImGui::ShowDemoWindow(&show);
	// }





// 	void ImGuiLayer::ImGuiBeginDockspace()
// 	{
// 		static bool dockSpaceOpen = true;
// 		static bool opt_fullscreen = true;
// 		static bool opt_padding = false;
// 		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

// 		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
// 		// because it would be confusing to have two docking targets within each others.
// 		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
// 		if (opt_fullscreen)
// 		{
// 			const ImGuiViewport* viewport = ImGui::GetMainViewport();
// 			ImGui::SetNextWindowPos(viewport->WorkPos);
// 			ImGui::SetNextWindowSize(viewport->WorkSize);
// 			ImGui::SetNextWindowViewport(viewport->ID);
// 			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
// 			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
// 			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
// 			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
// 		}
// 		else
// 		{
// 			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
// 		}

// 		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
// 		// and handle the pass-thru hole, so we ask Begin() to not render a background.
// 		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
// 			window_flags |= ImGuiWindowFlags_NoBackground;

// 		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
// 		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
// 		// all active windows docked into it will lose their parent and become undocked.
// 		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
// 		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
// 		if (!opt_padding)
// 			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
// 		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
// 		if (!opt_padding)
// 			ImGui::PopStyleVar();

// 		if (opt_fullscreen)
// 			ImGui::PopStyleVar(2);

// 		// Submit the DockSpace
// 		ImGuiIO& io = ImGui::GetIO();

// 		//TODO: may find a better way to do this
// 		//? Set window constraints
// 		ImGuiStyle& style = ImGui::GetStyle();
// 		style.WindowMinSize.x = 370.0f;
// 		// style.WindowMinSize.y = 200.0f;


// 		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
// 		{
// 			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
// 			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
// 		}

// 		style.WindowMinSize.x = 32.0f;

// 		// if (ImGui::BeginMenuBar())
// 		// {
// 		// 	if (ImGui::BeginMenu("File"))
// 		// 	{
			


// 		// 		if (ImGui::MenuItem("Exit")) Application::Get().Close();
// 		// 		ImGui::EndMenu();
// 		// 	}
// 		// 	ImGui::EndMenuBar();
// 		// }
// 	}
	
// 	void ImGuiLayer::ImGuiEndDockspace()
// 	{
// 		ImGui::End(); // Dockspace
// 	}
}