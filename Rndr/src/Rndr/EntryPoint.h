#pragma once

#include <iostream>

#include "Rndr/Application.h"

// Entry point for the application

#ifdef RNDR_PLATFORM_LINUX

extern Rndr::Application* Rndr::CreateApplication();

int main(int argc, char** argv)
{
	std::cout << "Welcome to Rndr engine" << std::endl;
	auto app = Rndr::CreateApplication();
	app->Run();
	delete app;
}

#endif