#include "Scene.h"

#include "glm/glm.hpp"

#include "Rndr/Scene/Components.h"

#include "Entity.h"

#include "Rndr/Renderer/Renderer2D.h"
#include "Rndr/Renderer/Renderer3D.h"

#include "Rndr/Renderer/LineRenderer.h"

#include "Rndr/Core/Log.h"

namespace Rndr
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		// Entity entity = { m_Registry.create(), this };
		// entity.AddComponent<IDComponent>();
		// entity.AddComponent<TransformComponent>();
		// auto& tag = entity.AddComponent<TagComponent>();
		// tag.Tag = name.empty() ? "Entity" : name;
		// return entity;
		return CreateEntityWithID(UUID(), name);
	}

	Entity Scene::CreateEntityWithID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}
	
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{


		int defaultMaterialCount = 0;
		auto group = m_Registry.group<TransformComponent>(entt::get<DefaultMaterialComponent>);
		if (group.size())
		{
			auto defaultMaterial = group.get<DefaultMaterialComponent>(group.front());
			Renderer2D::BeginScene(camera, defaultMaterial.Material);
			for (auto entity : group)
			{
				Entity ent = { entity, this };
				if (ent.HasComponent<CubeComponent>())
				{
					glm::mat4 transform = ent.GetComponent<TransformComponent>().GetTransform();
					CubeComponent cube = ent.GetComponent<CubeComponent>();
					DefaultMaterialComponent material = ent.GetComponent<DefaultMaterialComponent>();
					Renderer2D::DrawCube(transform, cube, material, (int)entity);
				}
			}
			Renderer2D::EndScene();
		}


		LineRenderer::BeginScene(camera, m_LineMaterial, 1.0f);
		float camY = camera.GetPosition().y;
		glm::vec4 gridColor = {0.28f, 0.28f, 0.28f, .0f};
		float gridSize = 25.0f;
		float gridSpacing = 1.0f;
		// float gridSpacing = 1.0f + std::floor(camY / 5.0f);
		for (int x = -gridSize; x <= gridSize; x++)
		{
			for (int z = -gridSize; z <= gridSize; z++)
			{
				// LineRenderer::DrawLine({(float)x, 0.0f, (float)z}, {(float)x + 1.0f, 0.0f, (float)z}, gridColor);
				// LineRenderer::DrawLine({(float)x, 0.0f, (float)z}, {(float)x, 0.0f, (float)z + 1.0f}, gridColor);
				LineRenderer::DrawLine({(float)x, 0.0f, (float)z}, {(float)x + gridSpacing, 0.0f, (float)z}, gridColor);
				LineRenderer::DrawLine({(float)x, 0.0f, (float)z}, {(float)x, 0.0f, (float)z + gridSpacing}, gridColor);
			}
		}
		LineRenderer::EndScene();




		LineRenderer::BeginScene(camera, m_LineMaterial, 2.0f);
		//? x
		LineRenderer::DrawLine(glm::vec3(0.0f), glm::vec3(2.0f, 0.0f, 0.0f), glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));
		//? Y
		LineRenderer::DrawLine(glm::vec3(0.0f), glm::vec3(0.0f, 2.0f, 0.0f), glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
		//? Z
		LineRenderer::DrawLine(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 2.0f), glm::vec4(0.2f, 0.2f, 0.8f, 1.0f));
		LineRenderer::EndScene();

	
	}


	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		//? Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}


	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{entity, this};
		}
		return {};
	}












	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		if constexpr (std::is_same<T, CameraComponent>::value)
		{
			RNDR_CORE_INFO("Camera Component Added");
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
		// Add more specializations as needed
	}

	// Explicit template instantiations
	template void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component);
	template void Scene::OnComponentAdded<QuadComponent>(Entity entity, QuadComponent& component);
	template void Scene::OnComponentAdded<CubeComponent>(Entity entity, CubeComponent& component);
	template void Scene::OnComponentAdded<DefaultMaterialComponent>(Entity entity, DefaultMaterialComponent& component);








}