// #include "PerspectiveCameraController.h"

// #include "Rndr/Core/Input.h"

// //TEMP
// #include "Rndr/Core/Application.h"

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/rotate_vector.hpp>
// #include <glm/gtx/vector_angle.hpp>

// #include <glm/gtc/quaternion.hpp>
// #include <glm/gtx/quaternion.hpp>

// namespace Rndr
// {
// 	PerspectiveCameraController::PerspectiveCameraController(float aspectRatio, float fov, float nearClip, float farClip)
// 		: m_AspectRatio(aspectRatio), m_FOV(fov), m_NearClip(nearClip), m_FarClip(farClip),
// 		m_Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
// 	{

// 		m_Projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
// 		m_InverseProjection = glm::inverse(m_Projection);

// 		m_View = glm::lookAt(m_CameraPosition, m_CameraPosition + m_Front, m_Up);
// 		m_InverseView = glm::inverse(m_View);
// 	}

// 	void PerspectiveCameraController::OnUpdate(Timestep ts)
// 	{
// 		bool updated = false;
// 		if (Input::IsKeyPressed(RNDR_KEY_W))
// 		{
// 			m_CameraPosition += m_Front * m_CameraTranslationSpeed * (float)ts;
// 			updated = true;
// 		}
// 		if (Input::IsKeyPressed(RNDR_KEY_S))
// 		{
// 			m_CameraPosition -= m_Front * m_CameraTranslationSpeed * (float)ts;
// 			updated = true;
// 		}
// 		if (Input::IsKeyPressed(RNDR_KEY_A))
// 		{
// 			m_CameraPosition -= m_Right * m_CameraTranslationSpeed * (float)ts;
// 			updated = true;
// 		}
// 		if (Input::IsKeyPressed(RNDR_KEY_D))
// 		{
// 			m_CameraPosition += m_Right * m_CameraTranslationSpeed * (float)ts;
// 			updated = true;
// 		}


// 		if (Input::IsMouseButtonPressed(RNDR_MOUSE_BUTTON_RIGHT) == RNDR_PRESS)
// 		{
// 			Input::SetCursorMode(RNDR_CURSOR_HIDDEN);
// 			auto [xpos, ypos] = Input::GetMousePosition();
// 			// if (!m_FirstMouseClick)
// 			// {
// 			// 	m_CursorPos = { (float)xpos, (float)ypos };
// 			// }
// 			// else
// 			// {
// 			// 	Application::Get().GetWindow().SetCursorPos((double)m_CursorPos.x, (double)m_CursorPos.y);
// 			// }
// 			if (m_FirstMouseClick)
// 			{
// 				Application::Get().GetWindow().SetCursorPos((double)m_CursorPos.x, (double)m_CursorPos.y);
// 			}

// 			float pan = m_CameraRotationSpeed * (float)(xpos - m_CursorPos.x);
// 			float tilt = m_CameraRotationSpeed * (float)(ypos - m_CursorPos.y);

// 			if (pan != 0.0f || tilt != 0.0f)
// 			{
// 				// m_Rotation.y += pan;
// 				// m_Rotation.x += tilt;
// 				m_Rotation.x += -tilt;
// 				m_Rotation.y += pan;
// 				updated = true;


// 				m_Front = CalculateDirectionFromEulerAngles(-m_Rotation);
// 				m_Right = glm::normalize(glm::cross(m_Front, m_Up));

// 				Input::SetMousePosition(m_CursorPos.x, m_CursorPos.y);
// 			}

// 		}
// 		else if (Input::IsMouseButtonPressed(RNDR_MOUSE_BUTTON_RIGHT) == RNDR_RELEASE)
// 		{
// 			Application::Get().GetWindow().SetCursorMode(RNDR_CURSOR_NORMAL);
// 			m_FirstMouseClick = true;
// 		}
// 	}

// 	void PerspectiveCameraController::OnEvent(Event& e)
// 	{
// 		EventDispatcher dispatcher(e);
// 		dispatcher.Dispatch<MouseScrolledEvent>(RNDR_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
// 		dispatcher.Dispatch<WindowResizeEvent>(RNDR_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
// 	}

// 	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
// 	{
// 		// m_CameraPosition += m_Front * e.GetYOffset() * 0.1f;
// 		return false;
// 	}

// 	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
// 	{
// 		// m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
// 		// m_Camera.SetProjection(m_AspectRatio, m_FOV, m_NearClip, m_FarClip);
// 		return false;
// 	}




// 	glm::vec3 PerspectiveCameraController::CalculateDirectionFromEulerAngles(const glm::vec3& rotation)
// 	{
// 		// Convert Euler angles from degrees to radians
// 		// rotation.y = -rotation.y;
// 		glm::vec3 rotationRadians = glm::radians(rotation);

// 		// Initialize a forward direction vector
// 		glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f); // Default forward direction

// 		// Apply rotations around X, Y, and Z axes in order
// 		// direction = glm::rotateX(direction, -rotationRadians.x);
// 		// direction = glm::rotateY(direction, -rotationRadians.y);
// 		// direction = glm::rotateZ(direction, rotationRadians.z);
// 		direction = glm::rotateX(direction, rotationRadians.x);
// 		direction = glm::rotateY(direction, rotationRadians.y);
// 		direction = glm::rotateZ(direction, rotationRadians.z);

// 		// Return the resulting normalized direction vector
// 		return glm::normalize(direction);
// 	}

// 	void PerspectiveCameraController::RecalculateCameraView()
// 	{
// 		m_View = glm::lookAt(m_CameraPosition, m_CameraPosition + m_Front, m_Up);
// 		m_InverseView = glm::inverse(m_View);
// 	}
// }