#include "Application.h"

#include "Rndr/Log.h"

#include <iostream>

// #include <glad/glad.h>

#include "Input.h"

#include "Rndr/Renderer/Renderer.h"

#include "GLFW/glfw3.h"

namespace Rndr
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	// static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	// {
	// 	switch (type)
	// 	{
	// 		case ShaderDataType::Float:    return GL_FLOAT;
	// 		case ShaderDataType::Float2:   return GL_FLOAT;
	// 		case ShaderDataType::Float3:   return GL_FLOAT;
	// 		case ShaderDataType::Float4:   return GL_FLOAT;
	// 		case ShaderDataType::Mat3:     return GL_FLOAT;
	// 		case ShaderDataType::Mat4:     return GL_FLOAT;
	// 		case ShaderDataType::Int:      return GL_INT;
	// 		case ShaderDataType::Int2:     return GL_INT;
	// 		case ShaderDataType::Int3:     return GL_INT;
	// 		case ShaderDataType::Int4:     return GL_INT;
	// 		case ShaderDataType::Bool:     return GL_BOOL;
	// 	}
	// 	RNDR_CORE_ASSERT(false, "Unknown ShaderDataType!");
	// 	return 0;
	// }

	Application::Application()
	{
		RNDR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);



		
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
		// RNDR_CORE_TRACE("{0}", e);

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

			float time = (float)glfwGetTime(); // Platform::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			


			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

}