#pragma once

#include "Rndr/Core/Layer.h"

#include "Rndr/Events/ApplicationEvent.h"
#include "Rndr/Events/KeyEvent.h"
#include "Rndr/Events/MouseEvent.h"

namespace Rndr
{
	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		// void OnUpdate() override;
		// void OnImGuiRender() override;

		void Begin();
		void End();
		// void OnEvent(Event& e);


		void BlockEvents(bool block) { m_BlockEvents = block; }

		static void ImGuiBeginDockspace();
		static void ImGuiEndDockspace();
		void SetDarkThemeColors();
	
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}