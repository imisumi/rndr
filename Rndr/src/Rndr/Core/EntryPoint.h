#pragma once

#include <iostream>

#include "Rndr/Debug/Instrumentor.h"
#include "Rndr/Core/Application.h"

// Entry point for the application

#define RNDR_PLATFORM_WINDOWS 1

#ifdef RNDR_PLATFORM_LINUX

extern Rndr::Application* Rndr::CreateApplication();

int main(int argc, char** argv)
{
	Rndr::Log::Init();
	RNDR_CORE_WARN("Initialized Log!");
	int a = 5;
	RNDR_INFO("Hello! Var={0}", a);

	std::cout << "Welcome to Rndr engine" << std::endl;
	auto app = Rndr::CreateApplication();
	app->Run();
	delete app;
}

#elif RNDR_PLATFORM_WINDOWS

extern Rndr::Application* Rndr::CreateApplication();

int main(int argc, char** argv)
{
	Rndr::Log::Init();
	RNDR_CORE_WARN("Initialized Log! Windows");


	RNDR_PROFILE_BEGIN_SESSION("Startup", "RndrProfile-Startup.json");
	auto app = Rndr::CreateApplication();
	RNDR_PROFILE_END_SESSION();

	RNDR_PROFILE_BEGIN_SESSION("Runtime", "RndrProfile-Runtime.json");
	app->Run();
	RNDR_PROFILE_END_SESSION();

	RNDR_PROFILE_BEGIN_SESSION("Shutdown", "RndrProfile-Shutdown.json");
	delete app;
	RNDR_PROFILE_END_SESSION();
}


#endif