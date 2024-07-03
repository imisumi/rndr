#pragma once

#include "Rndr/Core/Base.h"

// #include 
// header for std::pair
#include <iostream>
#include <utility>

#include "KeyCodes.h"
#include "MouseButtonCodes.h"


/*! @name Key and button actions
 *  @{ */
/*! @brief The key or mouse button was released.
 *
 *  The key or mouse button was released.
 *
 *  @ingroup input
 */
#define RNDR_RELEASE                0
/*! @brief The key or mouse button was pressed.
 *
 *  The key or mouse button was pressed.
 *
 *  @ingroup input
 */
#define RNDR_PRESS                  1
/*! @brief The key was held down until it repeated.
 *
 *  The key was held down until it repeated.
 *
 *  @ingroup input
 */
#define RNDR_REPEAT                 2
/*! @} */

namespace Rndr
{
	class  Input
	{
	public:
		static int IsKeyPressed(int keycode);

		static int IsMouseButtonPressed(int button);

		static std::pair<float, float> GetMousePosition();
		static void SetMousePosition(float x, float y);

		static void SetCursorMode(int mode);

		static float GetMouseX();
		static float GetMouseY();
	};
}