#pragma once

#include "entt/entt.hpp"

namespace Rndr
{
	class Scene
	{
	public:
		Scene();
		~Scene();
	private:
		entt::registry m_Registry;
	};
}