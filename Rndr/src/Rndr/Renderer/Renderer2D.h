#pragma once


#include "Rndr/Renderer/OrthographicCamera.h"

namespace Rndr
{
	//? full static no storage
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();


		//? Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	};
}