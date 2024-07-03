#pragma once

#include "Rndr/Core/Window.h"

#include <GLFW/glfw3.h>

#include "Rndr/Core/Log.h"

// struct GLFWwindow;

#include "Rndr/Renderer/GraphicsContext.h"

namespace Rndr {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		// inline void SetTitle(const std::string& title) override { m_Data.Title = title; }
		// inline void SetCursorMode(int mode) override { glfwSetInputMode(m_Window, GLFW_CURSOR, mode); }
		// inline void SetCursorPos(double xpos, double ypos) override { glfwSetCursorPos(m_Window, xpos, ypos); }
		// inline void GetCursorPos(double& xpos, double& ypos) override { glfwGetCursorPos(m_Window, &xpos, &ypos); }

		inline virtual void* GetNativeWindow() const override { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}