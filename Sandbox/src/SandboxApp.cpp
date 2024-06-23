#include "Rndr.h"
#include <iostream>

#include <glm/glm.hpp>

#include "imgui.h"

class ExampleLayer : public Rndr::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		// RNDR_INFO("ExampleLayer::Update");






		// if (Rndr::Input::IsKeyPressed(RNDR_KEY_TAB))
			// RNDR_TRACE("Tab key is pressed!");






		// glm::vec2 mouse(Rndr::Input::GetMouseX(), Rndr::Input::GetMouseY());
		// RNDR_TRACE("Mouse position: {0}, {1}", mouse.x, mouse.y);
	}

	void OnImGuiRender() override
	{
		// RNDR_INFO("ExampleLayer::ImGuiRender");
		ImGui::Begin("Test");
		ImGui::Text("Hello, world!");
		ImGui::End();
	}

	void OnEvent(Rndr::Event& event) override
	{
		// RNDR_TRACE("{0}", event);
	}
};

class Sandbox : public Rndr::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		// PushLayer(new Rndr::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Rndr::Application* Rndr::CreateApplication()
{
	return new Sandbox();
}