#pragma once

#include "Rndr/Layer.h"

#include "Rndr/Events/ApplicationEvent.h"
#include "Rndr/Events/KeyEvent.h"
#include "Rndr/Events/MouseEvent.h"

namespace Rndr
{
	class RNDR_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		// void OnUpdate() override;
		void OnImGuiRender() override;

		void Begin();
		void End();
		// void OnEvent(Event& e);


	// private:
	// 	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
	// 	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
	// 	bool OnMouseMovedEvent(MouseMovedEvent& e);
	// 	bool OnMouseScrolledEvent(MouseScrolledEvent& e);
	// 	bool OnKeyPressedEvent(KeyPressedEvent& e);
	// 	bool OnKeyReleasedEvent(KeyReleasedEvent& e);
	// 	bool OnKeyTypedEvent(KeyTypedEvent& e);
	// 	bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		float m_Time = 0.0f;
	};

}