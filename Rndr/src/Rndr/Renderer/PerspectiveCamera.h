#pragma once


#include "Rndr/Core/Timestep.h"


#include "Rndr/Events/ApplicationEvent.h"
#include "Rndr/Events/MouseEvent.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


namespace Rndr
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(float aspectRatio, float fov, float nearClip, float farClip);

		// void Set


		glm::vec3 GetPosition() const { return m_Position; }



		const glm::mat4& GetProjection() const { return m_Projection; }


		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	

		// void UpdateCameraVectors();

		glm::vec3 CalculateDirectionFromEulerAngles(const glm::vec3& rotation);
		void RecalculateCameraView();




	private:
		float m_AspectRatio;
		float m_FOV;
		float m_NearClip;
		float m_FarClip;

		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_InverseProjection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);
		glm::mat4 m_InverseView = glm::mat4(1.0f);

		glm::mat4 m_ViewProjection = glm::mat4(1.0f);

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };


		glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };


		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 1.0f;
		bool m_FirstMouseClick = true;
		glm::vec2 m_CursorPos;
	};
}