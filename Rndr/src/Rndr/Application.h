#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Window.h"

#include "Rndr/LayerStack.h"

#include "Rndr/ImGui/ImGuiLayer.h"

#include <memory>

#include "Rndr/Renderer/Shader.h"

#include "Rndr/Renderer/Buffer.h"

#include "Rndr/Renderer/VertexArray.h"

#include "Rndr/Renderer/Camera.h"

#include "Rndr/Core/Timestep.h"

namespace Rndr
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);


		inline Window& GetWindow() { return *m_Window; }
		
		inline static Application& Get() { return *s_Instance; }


	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}