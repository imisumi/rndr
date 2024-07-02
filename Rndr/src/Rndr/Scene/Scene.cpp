#include "Scene.h"

#include "glm/glm.hpp"

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
}