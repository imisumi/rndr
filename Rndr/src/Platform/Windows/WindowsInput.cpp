#include "Rndr/Core/Input.h"

#include <GLFW/glfw3.h>

#include "Rndr/Core/Application.h"

namespace Rndr
{
	int Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		// return state == GLFW_PRESS || state == GLFW_REPEAT || state == GLFW_RELEASE;
		return state;
	}

	int Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		// return state == GLFW_PRESS;
		return state;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	void Input::SetMousePosition(float x, float y)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetCursorPos(window, x, y);
	}

	void Input::SetCursorMode(int mode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		glfwSetInputMode(window, GLFW_CURSOR, mode);
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
}