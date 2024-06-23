#pragma once

#include <iostream>

#include "Rndr/Application.h"

// Entry point for the application

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
	RNDR_CORE_WARN("Initialized Log!");
	int a = 5;
	RNDR_INFO("Hello! Var={0}", a);

	std::cout << "Welcome to Rndr engine" << std::endl;
	auto app = Rndr::CreateApplication();
	app->Run();
	delete app;
}


#endif