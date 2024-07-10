#include "Scene.h"

#include "glm/glm.hpp"

#include "Rndr/Scene/Components.h"

#include "Entity.h"

#include "Rndr/Renderer/Renderer2D.h"
#include "Rndr/Renderer/Renderer3D.h"

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
		Entity entity = { m_Registry.create(), this };
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
		// Render 2D
		// Renderer2D::BeginScene(camera);

		// auto group = m_Registry.group<TransformComponent>(entt::get<QuadComponent>);
		// for (auto entity : group)
		// {
		// 	auto [transform, quad] = group.get<TransformComponent, QuadComponent>(entity);

		// 	Renderer2D::DrawQuad(transform.GetTransform(), quad, (int)entity);
		// 	// Renderer2D::PrintVertexData();
		// }



		// ? Cube
		// auto group = m_Registry.group<TransformComponent>(entt::get<CubeComponent>);
		// for (auto entity : group)
		// {
		// 	auto [transform, cube] = group.get<TransformComponent, CubeComponent>(entity);
		// 	Renderer2D::DrawCube(transform.GetTransform(), cube, (int)entity);
		// }

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
					Renderer2D::DrawCube(transform, cube, (int)entity);
				}
			}
			Renderer2D::EndScene();	
		}

		// auto group = m_Registry.group<TransformComponent>(entt::get<DefaultMaterialComponent>);
		// auto defaultMaterial = group.get<DefaultMaterialComponent>(group.front());
		// Renderer2D::BeginScene(camera, defaultMaterial.Material);
		// for (auto entity : group)
		// {
		// 	Entity ent = { entity, this };
		// 	if (ent.HasComponent<CubeComponent>())
		// 	{
		// 		glm::mat4 transform = ent.GetComponent<TransformComponent>().GetTransform();
		// 		CubeComponent cube = ent.GetComponent<CubeComponent>();
		// 		Renderer2D::DrawCube(transform, cube, (int)entity);
		// 	}
		// }

		// Renderer2D::EndScene();

	
	}



	void Scene::OnUpdateRuntime(Timestep ts)
	{
		//? use view for one component only else use group

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto view = m_Registry.view<TransformComponent,QuadComponent>();
			for (auto entity : view)
			{
				auto [transform, quad] = view.get<TransformComponent, QuadComponent>(entity);

				// Renderer2D::DrawQuad(transform.GetTransform(), quad.Color);
				Renderer2D::DrawQuad(transform.GetTransform(), quad, (int)entity);
			}

			Renderer2D::EndScene();




			// Renderer3D::BeginScene(*mainCamera, cameraTransform);

			// auto view3D = m_Registry.view<TransformComponent, CubeComponent>();
			// for (auto entity : view3D)
			// {
			// 	auto& [transform, cube] = view3D.get<TransformComponent, CubeComponent>(entity);

			// 	Renderer3D::DrawCube(transform.GetTransform(), cube.Color);
			// }

			// Renderer3D::EndScene();

		}
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