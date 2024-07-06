#pragma once

#include "entt/entt.hpp"

#include "Rndr/Core/Timestep.h"

#include <glm/glm.hpp>

#include "Components.h"

#include <type_traits>

#include "Rndr/Core/Log.h"

namespace Rndr
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		// Entity CreateEntity(const std::string& name, const glm::vec3& translation);

		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);


		Entity GetPrimaryCameraEntity();

		// Entity CreateEntity();

		//? TEMP
		// entt::registry& Reg() { return m_Registry; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component)
		{
			if constexpr (std::is_same<T, CameraComponent>::value)
			{
				RNDR_CORE_INFO("Camera Component Added");
				component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
			}

			// if constexpr (std::is_same<T, QuadComponent>::value)
			// {
			// 	component.Color = glm::vec4(0.8f, 0.2f, 0.3f, 1.0f);
			// }
		}

		// template<>
		// void OnComponentAdded<TagComponent>(Entity entity, TagComponent& component);

		// template<>
		// void OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component);

		// template<>
		// void OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
		// {
		// 	component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		// }

		// template<>
		// void OnComponentAdded<TagComponent>(Entity entity, TagComponent& component);



		// void OnComponentAdded(Entity entity, CameraComponent& component);
		// void OnComponentAdded(Entity entity, TagComponent& component);
		// void OnComponentAdded(Entity entity, TransformComponent& component);
		// void OnComponentAdded(Entity entity, TagComponent& component);

		
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}