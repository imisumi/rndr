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

	struct bvhNode
	{

		int ChildIndex = 0;
		int TriangleIndex = 0;
		int TriangleCount = 0;
		int padding3;

	
		glm::vec3 Min = glm::vec3(INFINITY);
		float padding1;

		glm::vec3 Max = glm::vec4(-INFINITY);
		float padding2;
	

	};
	static_assert(sizeof(bvhNode) == 48, "bvh struct size should be 48 bytes.");

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
		Entity GetEntityWithID(UUID uuid);
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		// void OnUpdateRuntime(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);


		void EnableBVH(bool enable) { m_EnableBVH = enable; }
		bool GetBVHEnabled() { return m_EnableBVH; }
		void SetBVHDepth(uint32_t depth) { m_BVHDepth = depth; }
		uint32_t GetBVHDepth() { return m_BVHDepth; }


		Entity GetPrimaryCameraEntity();

		void generateBVH();
		void splitBVH(int parentIndex, int depth);
		void splitBVH(int parentIndex, int depth, int meshIndex);
		void generateAABB(int parentIndex);
		void generateAABB(int parentIndex, int meshIndex);
		void drawBVH(bvhNode node, int depth, int visableDepth);
		Mesh m_Mesh;

		std::vector<bvhNode> m_BVH;


		const entt::registry& GetRegistry() const { return m_Registry; }

		// std::vector<tempBVH> m_TempBVH;
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

		Ref<Texture2D> m_NullObject, m_MeshIcon;

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		Ref<ViewportGrid> m_Grid;

		Ref<LineMaterial> m_LineMaterial;
		
		Ref<Material> m_MeshMaterial;

		std::vector<float> m_SkyPixels;

	

		// std::vector<BVHNodeV2> m_BVHNodes;

		// BVHNode m_BVHTree;

		bool m_EnableBVH = true;
		uint32_t m_BVHDepth = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}