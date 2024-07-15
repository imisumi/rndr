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

#include "Rndr/Renderer/Mesh.h"


namespace Rndr
{
	class Entity;

	struct AABB
	{
		glm::vec3 Min = glm::vec3(INFINITY);
		glm::vec3 Max = glm::vec3(-INFINITY);
	};

	class BVHNode
	{
	public:
		BVHNode() = default;
		// BVHNode(const AABB& aabb, const Ref<Mesh>& mesh, uint32_t start, uint32_t end)
		// 	: AABB(aabb), Mesh(mesh), Start(start), End(end) {}

		AABB AABB;
		std::vector<Mesh::Triangle> Triangles;
		Ref<BVHNode> ChildA, ChildB;
	};

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


		void EnableBVH(bool enable) { m_EnableBVH = enable; }
		bool GetBVHEnabled() { return m_EnableBVH; }
		void SetBVHDepth(uint32_t depth) { m_BVHDepth = depth; }
		uint32_t GetBVHDepth() { return m_BVHDepth; }


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

		Ref<Material> m_MeshMaterial;
		Mesh m_Mesh;

		std::vector<float> m_SkyPixels;


		BVHNode m_BVHTree;

		bool m_EnableBVH = false;
		uint32_t m_BVHDepth = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}