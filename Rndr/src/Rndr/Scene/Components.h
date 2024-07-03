#pragma once

#include <glm/glm.hpp>

#include <string>

#include "Rndr/Renderer/Camera3D.h"

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
		glm::mat4 Transform = glm::mat4(1.0f);

		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct DefaultCubeComponent
	{
		glm::vec4 Color = { 0.8f, 0.2f, 0.3f, 1.0f };
	};

	struct CameraComponent
	{
		Camera Camera;
		bool Primary = true; // TODO: implement in scene

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			: Camera(projection) {}
	};
}