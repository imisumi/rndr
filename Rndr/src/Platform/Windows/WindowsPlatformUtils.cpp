#include "Rndr/Utils/PlatformUtils.h"

#ifdef RNDR_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <commdlg.h>
	#include <glfw/glfw3.h>
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <glfw/glfw3native.h>
#endif

#include "Rndr/Core/Application.h"

#include <filesystem>

namespace Rndr
{
	std::string FileDialogs::OpenFile(const char* filter)
	{
	#ifdef RNDR_PLATFORM_WINDOWS
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;
	#elif RNDR_PLATFORM_LINUX
		char filename[1024];
		std::string command = "zenity --file-selection --file-filter='";
		command += filter;
		command += "' 2>/dev/null";
		FILE* f = popen(command.c_str(), "r");
		if (f == nullptr)
			return std::string();
		fgets(filename, sizeof(filename), f);
		pclose(f);
		// Remove the newline character at the end of the filename
		filename[strcspn(filename, "\n")] = 0;
		return std::string(filename);
	#endif
		return std::string();
	}

	std::string FileDialogs::SaveFile(const char* filter)
	{
	#ifdef RNDR_PLATFORM_WINDOWS
		OPENFILENAMEA ofn;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
			return ofn.lpstrFile;
	#elif RNDR_PLATFORM_LINUX
		char filename[1024];
		std::string command = "zenity --file-selection --save --confirm-overwrite --file-filter='";
		command += filter;
		command += "' 2>/dev/null";
		FILE* f = popen(command.c_str(), "r");
		if (f == nullptr)
			return std::string();
		fgets(filename, sizeof(filename), f);
		pclose(f);
		// Remove the newline character at the end of the filename
		filename[strcspn(filename, "\n")] = 0;
		return std::string(filename);
	#endif
		return std::string();
	}


}