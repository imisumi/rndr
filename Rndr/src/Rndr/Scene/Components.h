#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

// #include "Rndr/Renderer/Camera3D.h"

#include "SceneCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Rndr
{

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		// glm::mat4 Transform{ 1.0f };
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f }; // in Radians
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), Translation);

			// glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 });
			// rotation = glm::rotate(rotation, Rotation.y, { 0, 1, 0 });
			// rotation = glm::rotate(rotation, Rotation.z, { 0, 0, 1 });

			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);

			return translation * rotation * scale;
		}
	};

	// struct DefaultCubeComponent
	// {
	// 	glm::vec4 Color = { 0.8f, 0.2f, 0.3f, 1.0f };

	// 	//TODO: Implement Material / MaterialInstance
	// 	// Material = Shader + data/uniforms
	// 	// Ref<Material> Material = nullptr;
	// };

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: implement in scene maybe?
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		// CameraComponent(const glm::mat4& projection)
			// : Camera(projection) {}
	};



	struct QuadComponent
	{
		glm::vec4 Color = { 0.8f, 0.2f, 0.3f, 1.0f };
	};

	struct CubeComponent
	{
		glm::vec4 Color = { 0.8f, 0.2f, 0.3f, 1.0f };
	};
}