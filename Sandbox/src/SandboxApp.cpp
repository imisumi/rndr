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
		Rndr::FrameBufferSpecification frameBufferSpec;
		frameBufferSpec.Width = 1280;
		frameBufferSpec.Height = 720;
		m_FrameBuffer = Rndr::FrameBuffer::Create(frameBufferSpec);




		m_VertexArray.reset(Rndr::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Rndr::VertexBuffer> vertexBuffer;
		// vertexBuffer.reset(Rndr::VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer = Rndr::VertexBuffer::Create(vertices, sizeof(vertices));
		Rndr::BufferLayout layout = {
			{ Rndr::ShaderDataType::Float3, "a_Position" },
			{ Rndr::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Rndr::IndexBuffer> indexBuffer;
		// indexBuffer.reset(Rndr::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		indexBuffer = Rndr::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Rndr::VertexArray::Create());

		float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<Rndr::VertexBuffer> squareVB;
		// squareVB.reset(Rndr::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB = Rndr::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Rndr::ShaderDataType::Float3, "a_Position" },
			{ Rndr::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Rndr::IndexBuffer> squareIB;
		// squareIB.reset(Rndr::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		squareIB = Rndr::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
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

		// m_Shader.reset(Rndr::Shader::Create("m_Shader", vertexSrc, fragmentSrc));
		m_Shader = Rndr::Shader::Create("VertexColorTriangle", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
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

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		// m_FlatColorShader.reset(Rndr::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
		m_FlatColorShader = Rndr::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);




		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		// m_TextureShader.reset(Rndr::Shader::Create("m_TextureShader", textureShaderVertexSrc, textureShaderFragmentSrc));
		// m_TextureShader = Rndr::Shader::Create("m_TextureShader", textureShaderVertexSrc, textureShaderFragmentSrc);
		auto textureShader = m_ShaderLibrary.Load("Sandbox/assets/shaders/Texture.glsl");
		// m_TextureShader = Rndr::Shader::Create("Sandbox/assets/shaders/Texture.glsl");

		m_Texture = Rndr::Texture2D::Create("Sandbox/assets/textures/uv.png");
		m_TextureAlpha = Rndr::Texture2D::Create("Sandbox/assets/textures/penguin.png");

		std::dynamic_pointer_cast<Rndr::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Rndr::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
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

		RNDR_PROFILE_FUNCTION();

		if (m_Resize)
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_Camera = Rndr::OrthographicCamera(-m_ViewPortSize.x / m_ViewPortSize.y, m_ViewPortSize.x / m_ViewPortSize.y, -1.0f, 1.0f);
			m_Resize = false;
		}

		OnMovement(ts);


		m_FrameBuffer->Bind();


		Rndr::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		Rndr::RenderCommand::Clear();

		m_Camera.SetPosition(glm::vec3(m_CameraPosition));
		m_Camera.SetRotation(m_CameraRotation);

		Rndr::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.09f));

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


		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Rndr::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		// m_TextureAlpha->Bind();
		// Rndr::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_FrameBuffer->Unbind();



		// Rndr::Renderer::Submit(m_FlatColorShader, m_SquareVA, scale);
		//? triangle
		// Rndr::Renderer::Submit(m_Shader, m_VertexArray, scale);

		Rndr::Renderer::EndScene();

		// Renderer::Flush();
	}

	void OnImGuiRender() override
	{
		// ImGui::Begin("Settings");
		// ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		// ImGui::End();


		static bool dockSpaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) Rndr::Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");
		ImVec2 viewPortSize =  ImGui::GetContentRegionAvail();
		if (m_ViewPortSize != *(glm::vec2*)&viewPortSize)
		{
			m_ViewPortSize =  { viewPortSize.x, viewPortSize.y };
			m_Resize = true;
			RNDR_WARN("Viewport size: {0}, {1}", viewPortSize.x, viewPortSize.y);
		}
		ImGui::Image((void*)m_FrameBuffer->GetColorAttachmentRendererID(), 
			ImVec2{ (float)m_FrameBuffer->GetWidth(), (float)m_FrameBuffer->GetHeight() }, 
			ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();

		ImGui::PopStyleVar();

		ImGui::Begin("Settings");
		ImGui::End();

		ImGui::End();
	}

	void OnEvent(Rndr::Event& event) override
	{

	}


private:
	Rndr::ShaderLibrary m_ShaderLibrary;
	// Rndr::Ref<Rndr::Shader> m_TextureShader;
	Rndr::Ref<Rndr::Shader> m_Shader;
	Rndr::Ref<Rndr::VertexArray> m_VertexArray;
		
	Rndr::Ref<Rndr::Shader> m_FlatColorShader;
	Rndr::Ref<Rndr::VertexArray> m_SquareVA;

	Rndr::Ref<Rndr::Texture2D> m_Texture, m_TextureAlpha;


	Rndr::Ref<Rndr::FrameBuffer> m_FrameBuffer;


	glm::vec2 m_ViewPortSize;
	bool m_Resize = false;


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