#pragma once

#include "Core.h"

namespace Rndr
{
	class RNDR_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}