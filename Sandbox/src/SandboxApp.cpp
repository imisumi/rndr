#include "Rndr.h"
#include <iostream>

// namespace Rndr {

// 	void Print();
// }


class Sandbox : public Rndr::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}

};

Rndr::Application* Rndr::CreateApplication()
{
	return new Sandbox();
}

// int main()
// {
// 	std::cout << "Hello, World!" << std::endl;

// 	#ifdef RNDR_BUILD_DLL
// 		std::cout << "DLL BUILD" << std::endl;
// 	#else
// 		std::cout << "NOT DLL BUILD" << std::endl;
// 	#endif

// 	Sandbox* sandbox = new Sandbox();
// 	sandbox->Run();
// 	delete sandbox;

// 	return 0;
// }