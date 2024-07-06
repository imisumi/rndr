#include "Scene.h"

#include "glm/glm.hpp"

#include "Rndr/Scene/Components.h"

#include "Entity.h"

#include "Rndr/Renderer/Renderer2D.h"

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



	void Scene::OnUpdate(Timestep ts)
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
				auto& [transform, quad] = view.get<TransformComponent, QuadComponent>(entity);

				Renderer2D::DrawQuad(transform.GetTransform(), quad.Color);
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

}