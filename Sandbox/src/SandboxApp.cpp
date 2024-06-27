#include "Rndr.h"
#include <iostream>

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"

class ExampleLayer : public Rndr::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Rndr::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Rndr::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Rndr::VertexBuffer::Create(vertices, sizeof(vertices)));
		Rndr::BufferLayout layout = {
			{ Rndr::ShaderDataType::Float3, "a_Position" },
			{ Rndr::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Rndr::IndexBuffer> indexBuffer;
		indexBuffer.reset(Rndr::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Rndr::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Rndr::VertexBuffer> squareVB;
		squareVB.reset(Rndr::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Rndr::ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Rndr::IndexBuffer> squareIB;
		squareIB.reset(Rndr::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(Rndr::Shader::Create(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(Rndr::Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void OnMovement(Rndr::Timestep ts)
	{
		if (Rndr::Input::IsKeyPressed(RNDR_KEY_ESCAPE))
			exit(0);
		if (Rndr::Input::IsKeyPressed(RNDR_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Rndr::Input::IsKeyPressed(RNDR_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Rndr::Input::IsKeyPressed(RNDR_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;

		if (Rndr::Input::IsKeyPressed(RNDR_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;


		if (Rndr::Input::IsKeyPressed(RNDR_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		if (Rndr::Input::IsKeyPressed(RNDR_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * ts;
	}

	void OnUpdate(Rndr::Timestep ts) override
	{
		// RNDR_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		OnMovement(ts);





		Rndr::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Rndr::RenderCommand::Clear();

		m_Camera.SetPosition(glm::vec3(m_CameraPosition));
		m_Camera.SetRotation(m_CameraRotation);

		Rndr::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Rndr::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Rndr::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Rndr::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}


		Rndr::Renderer::Submit(m_FlatColorShader, m_SquareVA, scale);
		Rndr::Renderer::Submit(m_Shader, m_VertexArray, scale);

		Rndr::Renderer::EndScene();

		// Renderer::Flush();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Rndr::Event& event) override
	{

	}


private:
	Rndr::Ref<Rndr::Shader> m_Shader;
	Rndr::Ref<Rndr::VertexArray> m_VertexArray;
		
	Rndr::Ref<Rndr::Shader> m_FlatColorShader;
	Rndr::Ref<Rndr::VertexArray> m_SquareVA;


	Rndr::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 45.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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