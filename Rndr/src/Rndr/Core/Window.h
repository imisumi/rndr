#pragma once

#include "Base.h"
#include "Rndr/Events/Event.h"

namespace Rndr
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "OpenGL", uint32_t width = 1280, uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class  Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		// virtual void SetTitle(const std::string& title) = 0;
		// virtual void SetCursorMode(int mode) = 0;
		// virtual void SetCursorPos(double xpos, double ypos) = 0;
		// virtual void GetCursorPos(double& xpos, double& ypos) = 0;
		// virtual void SetIcon(const std::string& filepath) = 0;


		static Window* Create(const WindowProps& props = WindowProps());
	};
}