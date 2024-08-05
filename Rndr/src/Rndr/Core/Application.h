#pragma once

#include "Base.h"
#include "Rndr/Events/Event.h"
#include "Rndr/Events/ApplicationEvent.h"

#include "Rndr/Core/Window.h"

#include "Rndr/Core/LayerStack.h"

#include "Rndr/ImGui/ImGuiLayer.h"

#include <memory>

#include "Rndr/Renderer/Shader.h"


#include "Rndr/Renderer/VertexArray.h"

#include "Rndr/Renderer/Camera.h"

#include "Rndr/Core/Timestep.h"

namespace Rndr
{
	class Application
	{
	public:
		Application(const std::string& name = "Rndr App");
		virtual ~Application();

		void Run();
		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);


		inline Window& GetWindow() { return *m_Window; }
		
		inline static Application& Get() { return *s_Instance; }


	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}