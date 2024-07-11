#pragma once

#include "entt/entt.hpp"

#include "Rndr/Core/Timestep.h"

#include <glm/glm.hpp>

#include "Components.h"

#include <type_traits>

#include "Rndr/Core/Log.h"

#include "Rndr/Renderer/EditorCamera.h"


#include "Rndr/Renderer/Grid.h"


#include "Rndr/Core/UUID.h"


#include "Rndr/Renderer/LineRenderer.h"


namespace Rndr
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void SetLineMaterial(const Ref<LineMaterial>& material)
		{
			m_LineMaterial = material;
		}


		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		// void OnUpdateRuntime(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);


		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		// {
		// 	if constexpr (std::is_same<T, CameraComponent>::value)
		// 	{
		// 		RNDR_CORE_INFO("Camera Component Added");
		// 		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		// 	}
		// }

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		Ref<ViewportGrid> m_Grid;

		Ref<LineMaterial> m_LineMaterial;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}