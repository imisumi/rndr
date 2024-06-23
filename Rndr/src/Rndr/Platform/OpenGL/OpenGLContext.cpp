#include "OpenGLContext.h"

#include "Rndr/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GL/GL.h>

namespace Rndr
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RNDR_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RNDR_CORE_ASSERT(status, "Failed to initialize Glad!");

		RNDR_CORE_INFO("OpenGL Info:");
		RNDR_CORE_INFO("  Vendor:   {0}", (const char*)glGetString(GL_VENDOR));
		RNDR_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		RNDR_CORE_INFO("  Version:  {0}", (const char*)glGetString(GL_VERSION));

		#ifdef RNDR_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		RNDR_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Rndr requires at least OpenGL version 4.5!");
		#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	// OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	// 	: m_WindowHandle(windowHandle)
	// {
	// 	HZ_CORE_ASSERT(windowHandle, "Window handle is null!")
	// }

	// void OpenGLContext::Init()
	// {
	// 	glfwMakeContextCurrent(m_WindowHandle);
	// 	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	// 	HZ_CORE_ASSERT(status, "Failed to initialize Glad!");
	// }

	// void OpenGLContext::SwapBuffers()
	// {
	// 	glfwSwapBuffers(m_WindowHandle);
	// }
}