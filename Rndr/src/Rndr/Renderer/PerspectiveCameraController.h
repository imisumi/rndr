// #pragma once

// #include "PerspectiveCamera.h"


// #include "Rndr/Core/Timestep.h"


// #include "Rndr/Events/ApplicationEvent.h"
// #include "Rndr/Events/MouseEvent.h"

// #include <glm/glm.hpp>

// namespace Rndr
// {
// 	class PerspectiveCameraController
// 	{
// 	public:
// 		PerspectiveCameraController(float aspectRatio, float fov, float nearClip, float farClip);

// 		void OnUpdate(Timestep ts);
// 		void OnEvent(Event& e);

// 	private:
// 		bool OnMouseScrolled(MouseScrolledEvent& e);
// 		bool OnWindowResized(WindowResizeEvent& e);

// 		// void UpdateCameraVectors();

// 		glm::vec3 CalculateDirectionFromEulerAngles(const glm::vec3& rotation);
// 		void RecalculateCameraView();


// 		bool OnMouseScrolled(MouseScrolledEvent& e);
// 		bool OnWindowResized(WindowResizeEvent& e);

// 	private:
// 		// float m_AspectRatio;
// 		// float m_FOV;
// 		// float m_NearClip;
// 		// float m_FarClip;
// 		PerspectiveCamera m_Camera;

// 		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
// 		float m_CameraTranslationSpeed = 1.0f, m_CameraRotationSpeed = 1.0f;


// 		// glm::mat4 m_Projection = glm::mat4(1.0f);
// 		// glm::mat4 m_Projection = glm::mat4(1.0f);
// 		// glm::mat4 m_InverseProjection = glm::mat4(1.0f);
// 		// glm::mat4 m_View = glm::mat4(1.0f);
// 		// glm::mat4 m_InverseView = glm::mat4(1.0f);

// 		// glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f };
// 		// glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
// 		// glm::vec3 m_Right = { 1.0f, 0.0f, 0.0f };
// 		// glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };

// 		bool m_FirstMouseClick = true;
// 		glm::vec2 m_CursorPos;
// 	};
// }