#pragma once

#include "entt/entt.hpp"

#include "Rndr/Core/Timestep.h"
namespace Rndr
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);

		Entity CreateEntity();

		//? TEMP
		entt::registry& Reg() { return m_Registry; }
	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}