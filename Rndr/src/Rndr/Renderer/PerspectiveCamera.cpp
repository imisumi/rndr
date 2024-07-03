#include "PerspectiveCamera.h"

#include "Rndr/Core/Input.h"

namespace Rndr
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		m_Projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
		m_InverseProjection = glm::inverse(m_Projection);

		RecalculateCameraView();
	}


	void PerspectiveCamera::OnUpdate(Timestep ts)
	{
		bool updated = false;
		if (Input::IsKeyPressed(RNDR_KEY_W))
		{
			m_Position += m_Front * m_CameraTranslationSpeed * (float)ts;
			updated = true;
		}
		if (Input::IsKeyPressed(RNDR_KEY_S))
		{
			m_Position -= m_Front * m_CameraTranslationSpeed * (float)ts;
			updated = true;
		}
		if (Input::IsKeyPressed(RNDR_KEY_A))
		{
			m_Position -= m_Right * m_CameraTranslationSpeed * (float)ts;
			updated = true;
		}
		if (Input::IsKeyPressed(RNDR_KEY_D))
		{
			m_Position += m_Right * m_CameraTranslationSpeed * (float)ts;
			updated = true;
		}
		if (Input::IsKeyPressed(RNDR_KEY_SPACE))
		{
			m_Position += m_Up * m_CameraTranslationSpeed * (float)ts;
			updated = true;
		}
		if (Input::IsKeyPressed(RNDR_KEY_LEFT_SHIFT))
		{
			m_Position -= m_Up * m_CameraTranslationSpeed * (float)ts;
			updated = true;
		}


		if (Input::IsMouseButtonPressed(RNDR_MOUSE_BUTTON_RIGHT) == RNDR_PRESS)
		{
			Input::SetCursorMode(RNDR_CURSOR_HIDDEN);
			auto [xpos, ypos] = Input::GetMousePosition();
			// if (!m_FirstMouseClick)
			// {
			// 	m_CursorPos = { (float)xpos, (float)ypos };
			// }
			// else
			// {
			// 	Application::Get().GetWindow().SetCursorPos((double)m_CursorPos.x, (double)m_CursorPos.y);
			// }
			if (m_FirstMouseClick)
			{
				Input::SetMousePosition(m_CursorPos.x, m_CursorPos.y);
			}

			float pan = m_CameraRotationSpeed * (float)(xpos - m_CursorPos.x);
			float tilt = m_CameraRotationSpeed * (float)(ypos - m_CursorPos.y);

			if (pan != 0.0f || tilt != 0.0f)
			{
				// m_Rotation.y += pan;
				// m_Rotation.x += tilt;
				m_Rotation.x += -tilt;
				m_Rotation.y += pan;
				updated = true;


				m_Front = CalculateDirectionFromEulerAngles(-m_Rotation);
				m_Right = glm::normalize(glm::cross(m_Front, m_Up));

				Input::SetMousePosition(m_CursorPos.x, m_CursorPos.y);
			}

		}
		else if (Input::IsMouseButtonPressed(RNDR_MOUSE_BUTTON_RIGHT) == RNDR_RELEASE)
		{
			Input::SetCursorMode(RNDR_CURSOR_NORMAL);
			m_FirstMouseClick = true;
		}
	}

	void PerspectiveCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RNDR_BIND_EVENT_FN(PerspectiveCamera::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RNDR_BIND_EVENT_FN(PerspectiveCamera::OnWindowResized));
	}

	bool PerspectiveCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		// m_Position += m_Front * e.GetYOffset() * 0.1f;
		return false;
	}

	bool PerspectiveCamera::OnWindowResized(WindowResizeEvent& e)
	{
		// m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		// m_Camera.SetProjection(m_AspectRatio, m_FOV, m_NearClip, m_FarClip);
		return false;
	}

	glm::vec3 PerspectiveCamera::CalculateDirectionFromEulerAngles(const glm::vec3& rotation)
	{
		// Convert Euler angles from degrees to radians
		// rotation.y = -rotation.y;
		glm::vec3 rotationRadians = glm::radians(rotation);

		// Initialize a forward direction vector
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f); // Default forward direction

		// Apply rotations around X, Y, and Z axes in order
		// direction = glm::rotateX(direction, -rotationRadians.x);
		// direction = glm::rotateY(direction, -rotationRadians.y);
		// direction = glm::rotateZ(direction, rotationRadians.z);
		direction = glm::rotateX(direction, rotationRadians.x);
		direction = glm::rotateY(direction, rotationRadians.y);
		direction = glm::rotateZ(direction, rotationRadians.z);

		// Return the resulting normalized direction vector
		return glm::normalize(direction);
	}

	void PerspectiveCamera::RecalculateCameraView()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_InverseView = glm::inverse(m_View);
	}
}