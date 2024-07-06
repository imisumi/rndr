#pragma once

#include "Rndr/Core/Layer.h"

#include "Rndr/Events/ApplicationEvent.h"
#include "Rndr/Events/KeyEvent.h"
#include "Rndr/Events/MouseEvent.h"

namespace Rndr {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}
