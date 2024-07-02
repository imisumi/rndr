#pragma once

#include "Rndr/Core/Base.h"

// #include 
// header for std::pair
#include <iostream>
#include <utility>

namespace Rndr
{
	class  Input
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);

		static std::pair<float, float> GetMousePosition();

		static float GetMouseX();
		static float GetMouseY();
	};
}