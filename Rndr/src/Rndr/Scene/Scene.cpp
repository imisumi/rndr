#include "Scene.h"

#include "glm/glm.hpp"

#include "Rndr/Scene/Components.h"

#include "Entity.h"

#include "Rndr/Renderer/Renderer3D.h"

namespace Rndr
{
	Scene::Scene()
	{

		// struct TransformComponent
		// {
		// 	glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		// 	glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		// 	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		// };

		// entt::entity entity = m_Registry.create();
		// auto& conmp = m_Registry.emplace<TransformComponent>(entity);


		// auto view = m_Registry.view<TransformComponent>();
		// for (auto entity : view)
		// {
		// 	auto& tc = view.get<TransformComponent>(entity);
		// }

		// auto group = m_Registry.group<TransformComponent>(entt::get<TransformComponent>);
		// for (auto entity : group)
		// {
		// 	auto& [transform, sprite] = group.get<TransformComponent>(entity);
		// }
	}

	Scene::~Scene()
	{
	}


	void Scene::OnUpdate(Timestep ts)
	{
		//? use view for one component only else use group
		// m_Registry.view<CameraComponent>().each([=](auto entity, auto& cameraComponent)
		// {
		// 	auto& camera = cameraComponent.Camera;
		// 	if (cameraComponent.Primary)
		// 	{
		// 		// camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		// 	}
		// });

		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = m_Registry.view<TransformComponent,CameraComponent>();
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent,CameraComponent>(entity);
				if (camera.Primary)
				{
					// camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			//? render the scene
			// Renderer3D::BeginScene(*mainCamera, *cameraTransform);


			// Renderer3D::EndScene();
		}

		// struct TransformComponent
		// {
		// 	glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		// 	glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		// 	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		// };

		// entt::entity entity = m_Registry.create();
		// auto& conmp = m_Registry.emplace<TransformComponent>(entity);
	}


	Entity Scene::CreateEntity()
	{
		Entity entity = Entity(m_Registry.create(), this);
		// entity.AddComponent<TransformComponent>();
		// entity.AddComponent<TagComponent>("Entity");
		return entity;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		//TODO
		// entity.AddComponent<TransformComponent>();
		// auto& tag = entity.AddComponent<TagComponent>();
		// tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}
}