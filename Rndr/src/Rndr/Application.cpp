#include "Application.h"

#include "Rndr/Log.h"

#include <iostream>

#include <glad/glad.h>

#include "Input.h"

namespace Rndr
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		RNDR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// uint32_t id;
		// glGenVertexArrays(1, &id);
	}

	Application::~Application()
	{
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		// RNDR_TRACE("{0}", e);
		RNDR_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		// Rndr::Log::Init();
		// WindowResizeEvent e(1280, 720);
		// if (e.IsInCategory(EventCategoryApplication))
		// {
		// 	RNDR_TRACE(e);
		// }
		// if (e.IsInCategory(EventCategoryInput))
		// {
		// 	RNDR_TRACE(e);
		// }


		while (m_Running)
		{
			glClearColor(1.0f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);


			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();


			// auto [x, y] = Input::GetMousePosition();
			// RNDR_CORE_TRACE("Mouse position: {0}, {1}", x, y);

			m_Window->OnUpdate();
		}
	}

}