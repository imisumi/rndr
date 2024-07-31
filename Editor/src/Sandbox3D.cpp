#include "Sandbox3D.h"


#include <iostream>

// #include "Platform/OpenGL/OpenGLShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"

#include "Rndr/Core/Input.h"

#include "Rndr/Scene/SceneSerializer.h"

#include "Rndr/Utils/PlatformUtils.h"


// #include "ImGuizmo.h"
#include "Rndr/ImGui/ImGuizmo.h"



#include "Rndr/Math/Math.h"

#include <OpenImageIO/imageio.h>


#include <limits>
#include <OpenColorIO/OpenColorIO.h>
namespace OCIO = OCIO_NAMESPACE;



namespace Rndr
{
	Sandbox3D* Sandbox3D::s_Instance = nullptr;

	Sandbox3D::Sandbox3D()
		: Layer("Sandbox3D")
	{
		s_Instance = this;
	}

	// struct alignas(16) tempBVH
	// {
	// 	glm::vec3 Min = glm::vec3(INFINITY);
	// 	glm::vec3 Max = glm::vec3(-INFINITY);
	// 	int childIndex = 0;
	// };

	// Scene* s_SceneInstance = nullptr;

	void Sandbox3D::OnAttach()
	{
		// s_Instance = this;
		RNDR_CORE_TRACE("Sandbox3D::OnAttach");
		FrameBufferSpecification frameBufferSpec;
		frameBufferSpec.Attachments = { FrameBufferTextureFormat::RGBA8, 
			FrameBufferTextureFormat::RED_INTEGER, 
			FrameBufferTextureFormat::Depth };
		frameBufferSpec.Width = 1280;
		frameBufferSpec.Height = 720;
		m_FrameBufferLibrary.Add("EditorFrameBuffer", frameBufferSpec);
		// m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);

		frameBufferSpec.Attachments = { FrameBufferTextureFormat::RGBA32F };
		m_ComputeFrameBuffer = FrameBuffer::Create(frameBufferSpec);


		Ref<Material> material = CreateRef<Material>();
		material->SetShader(Shader::Create("Editor/assets/shaders/CubeShader.glsl"));
		material->SetName("CubeMaterial");

		Ref<Material> mat2 = CreateRef<Material>();
		mat2->SetShader(Shader::Create("Editor/assets/shaders/CubeShader.glsl"));
		mat2->SetName("tempMaterial");

		m_MaterialLibrary = CreateRef<MaterialLibrary>();
		m_MaterialLibrary->Add("CubeMaterial", material);
		m_MaterialLibrary->Add("tempMaterial", mat2);
		// m_MaterialPanel(m_MaterialLibrary)
		m_MaterialPanel.SetMaterialLibrary(m_MaterialLibrary);

		m_ActiveScene = CreateRef<Scene>();


		auto cube = m_ActiveScene->CreateEntity("Cube Entity");
		cube.AddComponent<CubeComponent>();
		cube.AddComponent<DefaultMaterialComponent>(material);
		auto name = cube.GetComponent<DefaultMaterialComponent>().Material->GetName();
		RNDR_CORE_INFO("Material Name: {0}", name);


		m_LineMaterial = LineMaterial::Create(Shader::Create("Editor/assets/shaders/LineShader.glsl"));
		m_ActiveScene->SetLineMaterial(m_LineMaterial);


		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_AssetManagerPanel.SetAssetManager(m_AssetManager);
		{
			AssetHandle handle = m_AssetManager->ImportAsset("Editor/assets/icons/null_icon.png");
			Ref<Texture2D> asset = m_AssetManager->GetAssetAs<Texture2D>(handle);
			m_NullIcon = asset;
		}
		{
			AssetHandle handle = m_AssetManager->ImportAsset("Editor/assets/textures/cube_icon.png");
			Ref<Texture2D> asset = m_AssetManager->GetAssetAs<Texture2D>(handle);
			m_CubeIcon = asset;
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		// m_EditorCamera.SetPosition({ 0.0f, 0.0f, 2.0f });
		m_ComputeShader = ComputeShader::Create("Editor/assets/shaders/raytracing/ComputeShader.glsl");

		// m_TempComputeTextureID;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_TempComputeTextureID);
		glTextureStorage2D(m_TempComputeTextureID, 1, GL_RGBA32F, 32, 32);
		glTextureParameteri(m_TempComputeTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TempComputeTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_TempComputeTextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_TempComputeTextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// m_Mesh = Mesh("Editor/assets/meshes/cube.obj");



		// disable vsync
		Application::Get().GetWindow().SetVSync(false);


		const char* filename = "Editor/assets/hdri/autumn_field_puresky_4k.exr";
		// const char* filename = "Editor/assets/hdri/empty_workshop_2k.exr";
		std::unique_ptr<OIIO::ImageInput> inp = OIIO::ImageInput::open(filename);
		if (!inp) {
			RNDR_CORE_ASSERT(inp, "Could not open image");
		}
		const OIIO::ImageSpec &spec = inp->spec();
		int width = spec.width;
		int height = spec.height;
		int channels = spec.nchannels;

		m_SkyTextureWidth = width;
		m_SkyTextureHeight = height;
		bool convertToRgba = false;
		if (channels == 3)
		{
			convertToRgba = true;
			channels = 4;
		}
		m_SkyPixels.resize(width * height * channels);
		inp->read_image(OIIO::TypeDesc::FLOAT, &m_SkyPixels[0]);
		inp->close();

		if (convertToRgba)
		{
			std::vector<float> tempPixels(width * height * 4);
			for (int i = 0; i < width * height; i++)
			{
				tempPixels[i * 4] = m_SkyPixels[i * 3];
				tempPixels[i * 4 + 1] = m_SkyPixels[i * 3 + 1];
				tempPixels[i * 4 + 2] = m_SkyPixels[i * 3 + 2];
				tempPixels[i * 4 + 3] = 1.0f;
			}
			m_SkyPixels = tempPixels;
		}

		// set m_SkyPixel to white 1.0f
		// for (int i = 0; i < m_SkyPixels.size(); i++)
		// 	m_SkyPixels[i] = 1.0f;
		RNDR_CORE_INFO("Number of channels: {0}", channels);
		RNDR_CORE_INFO("Image width: {0}, height: {1}", width, height);
		RNDR_CORE_INFO("Image loaded successfully");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_SkyTextureID);
		glTextureStorage2D(m_SkyTextureID, 1, GL_RGBA32F, width, height);
		glTextureSubImage2D(m_SkyTextureID, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, &m_SkyPixels[0]);
		glTextureParameteri(m_SkyTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_SkyTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_SkyTextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_SkyTextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		// float verticces[9] = {
		// 	-20.0f, 0.0f, -10.0f,
		// 	0.0f, 0.0f, 10.0f,
		// 	10.0f, 0.0f, -10.0f
		// };

		// std::vector<glm::vec3> vertices;
		// vertices.push_back({ -10.0f, 0.0f, -10.0f });
		// vertices.push_back({ 0.0f, 0.0f, 10.0f });
		// vertices.push_back({ 10.0f, 0.0f, -10.0f });
		std::vector<glm::vec4> vertices;
		vertices.push_back({ -10.0f, 0.0f, -10.0f, 1.0f });
		vertices.push_back({ 0.0f, 0.0f, 10.0f, 1.0f });
		vertices.push_back({ 10.0f, 0.0f, -10.0f, 1.0f });

		// glCreateBuffers(1, &m_PositionsSSBO);
		// // glNamedBufferData(m_PositionsSSBO, vertices.size() * sizeof(glm::vec4), vertices.data(), GL_STATIC_DRAW);
		// std::vector<glm::vec4> meshVertices = m_ActiveScene->m_Mesh.m_VerticesPositions;

		// glNamedBufferData(m_PositionsSSBO, meshVertices.size() * sizeof(glm::vec4), meshVertices.data(), GL_STATIC_DRAW);


		// glCreateBuffers(1, &m_IndexSSBO);
		// std::vector<uint32_t> index = m_ActiveScene->m_Mesh.m_VerticesIndex;
		// glNamedBufferData(m_IndexSSBO, index.size() * sizeof(uint32_t), index.data(), GL_STATIC_DRAW);


		// glCreateBuffers(1, &m_bvhSSBO);
		// glNamedBufferData(m_bvhSSBO, m_ActiveScene->m_TempBVH.size() * sizeof(tempBVH), m_ActiveScene->m_TempBVH.data(), GL_STATIC_DRAW);

		// glm::vec3 min = m_ActiveScene->m_TempBVH[1].Min;
		// glm::vec3 max = m_ActiveScene->m_TempBVH[1].Max;

		// RNDR_CORE_WARN("Min: {0}, {1}, {2}", min.x, min.y, min.z);
		// RNDR_CORE_WARN("Max: {0}, {1}, {2}", max.x, max.y, max.z);
		// RNDR_CORE_WARN("Size of tempBVH: {0}", sizeof(tempBVH));



		// m_ActiveScene->m_Mesh.m_Triangles[1].V1 = glm::vec3(-10.0f, 0.0f, -10.0f);
		// m_ActiveScene->m_Mesh.m_Triangles[1].V2 = glm::vec3(0.0f, 0.0f, 10.0f);
		// m_ActiveScene->m_Mesh.m_Triangles[1].V3 = glm::vec3(10.0f, 0.0f, -10.0f);

		glCreateBuffers(1, &m_TriangleBuffer);
		glNamedBufferData(m_TriangleBuffer, 
			m_ActiveScene->m_Mesh.m_Triangles.size() * sizeof(Mesh::Triangle), 
			m_ActiveScene->m_Mesh.m_Triangles.data(), GL_STATIC_DRAW);

		int size = m_ActiveScene->m_Mesh.m_Triangles.size() * sizeof(Mesh::Triangle);
		int sizeMB = size / 1024 / 1024;
		RNDR_CORE_INFO("Size of Triangle Buffer: {0}MB", sizeMB);

		glm::vec3 v1 = m_ActiveScene->m_Mesh.m_Triangles[0].V1;
		glm::vec3 v2 = m_ActiveScene->m_Mesh.m_Triangles[0].V2;
		glm::vec3 v3 = m_ActiveScene->m_Mesh.m_Triangles[0].V3;

		RNDR_CORE_WARN("V1: {0}, {1}, {2}", v1.x, v1.y, v1.z);
		RNDR_CORE_WARN("V2: {0}, {1}, {2}", v2.x, v2.y, v2.z);
		RNDR_CORE_WARN("V3: {0}, {1}, {2}", v3.x, v3.y, v3.z);


		glCreateBuffers(1, &m_BVHBuffer);
		glNamedBufferData(m_BVHBuffer, 
			m_ActiveScene->m_BVH.size() * sizeof(bvhNode), 
			m_ActiveScene->m_BVH.data(), GL_STATIC_DRAW);
		

		// int count = m_ActiveScene->m_BVH.size();
		// int triangleCount = m_ActiveScene->m_BVH[0].TriangleCount;
		int index = 2;
		// int count = m_ActiveScene->m_BVH[index].TriangleCount;
		// int triIndex = m_ActiveScene->m_BVH[index].TriangleIndex;/
		RNDR_CORE_INFO("BVH Index: {0}", index);
		// RNDR_CORE_INFO("BVH Count: {0}", count);
		// RNDR_CORE_INFO("Triangle Index: {0}", triIndex);
		// RNDR_CORE_INFO("BVH Count: {0}", count);
		// RNDR_CORE_INFO("Triangle Count: {0}", triangleCount);



		// m_ActiveScene->SetSkyTexture(m_SkyTextureID);




		// frameBufferSpec.Attachments = { FrameBufferTextureFormat::RGBA8 };





		// const char* ocioVert = R"(
		// 	#version 460 core
		// 	out vec2 TexCoords;

		// 	void main()
		// 	{
		// 		const vec2 pos[4] = vec2[4](
		// 			vec2(-1.0, -1.0),
		// 			vec2( 1.0, -1.0),
		// 			vec2(-1.0,  1.0),
		// 			vec2( 1.0,  1.0)
		// 		);
				
		// 		TexCoords = (pos[gl_VertexID] + 1.0) / 2.0;
		// 		gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);
		// 		// gl_Position = vec4(1.0, 1.0, 0.0, 1.0);
		// 	}
		// )";

		// const char* ocioFrag = R"(
		// 	#version 460 core
		// 	in vec2 TexCoords;
		// 	out vec4 FragColor;

		// 	uniform sampler2D u_Texture;

		// 	uniform uint u_FrameCount;

		// 	void main()
		// 	{
		// 		vec4 accumulatedBuffer = texture(u_Texture, TexCoords);
		// 		vec4 averagedColor = accumulatedBuffer / float(u_FrameCount);


		// 		vec3 color = averagedColor.rgb;

		// 		FragColor = vec4(color, 1.0);
		// 	}
		// )";






		// OCIO::ConstConfigRcPtr config = OCIO::Config::CreateFromFile("config.ocio");
		std::string ocioAces;
		{
			OCIO::ConstConfigRcPtr config = OCIO::Config::CreateFromFile("ocio://default");
			const char* display = config->getDefaultDisplay();
			// const char* view = config->getDefaultView(display);
			const char* view = "ACES 1.0 - SDR Video";
			const char* look = config->getDisplayViewLooks(display, view);

			OCIO::DisplayViewTransformRcPtr transform = OCIO::DisplayViewTransform::Create();
			transform->setSrc(OCIO::ROLE_SCENE_LINEAR);
			transform->setDisplay(display);
			transform->setView(view);

			auto vpt = OCIO::LegacyViewingPipeline::Create();
			vpt->setDisplayViewTransform(transform);
			vpt->setLooksOverrideEnabled(true);
			vpt->setLooksOverride(look);

			OCIO::ConstProcessorRcPtr processor = vpt->getProcessor(config, config->getCurrentContext());
			OCIO::ConstGPUProcessorRcPtr gpu = processor->getDefaultGPUProcessor();

			OCIO::GpuShaderDescRcPtr shaderDesc = OCIO::GpuShaderDesc::CreateShaderDesc();
			shaderDesc->setLanguage(OCIO::GPU_LANGUAGE_GLSL_4_0);
			shaderDesc->setFunctionName("OCIODisplay");
			shaderDesc->setResourcePrefix("ocio_");

			gpu->extractGpuShaderInfo(shaderDesc);
			ocioAces = shaderDesc->getShaderText();
		}


		std::string ocioLinearToAces;
		{
			// OCIO::ConstConfigRcPtr config = OCIO::Config::CreateFromFile("ocio://default");
			// OCIO::ConstConfigRcPtr config = OCIO::Config::CreateFromFile("config.ocio");
			// auto processor = config->getProcessor("ACES2065-1", "Linear Rec.709 (sRGB)");
			// auto gpu = processor->getDefaultGPUProcessor();
			// auto desc = OCIO::GpuShaderDesc::CreateShaderDesc();
			// desc->setLanguage(OCIO::GPU_LANGUAGE_GLSL_4_0);
			// // desc->setFunctionName("OCIODisplay");
			// desc->setFunctionName("ocioLinearToAces");
			// desc->setResourcePrefix("ocioLinearToAces_");
			// gpu->extractGpuShaderInfo(desc);

			// ocioLinearToAces = desc->getShaderText();



			// OCIO::ConstConfigRcPtr config = OCIO::Config::CreateFromFile("ocio://default");
			// const char* display = config->getDefaultDisplay();
			// const char* view = config->getDefaultView(display);
			// const char* look = config->getDisplayViewLooks(display, view);

			// OCIO::DisplayViewTransformRcPtr transform = OCIO::DisplayViewTransform::Create();
			// transform->setSrc("ACES 1.0 - SDR Video");
			// transform->setDisplay(display);
			// transform->setView(view);

			// auto vpt = OCIO::LegacyViewingPipeline::Create();
			// vpt->setDisplayViewTransform(transform);
			// vpt->setLooksOverrideEnabled(true);
			// vpt->setLooksOverride(look);

			// OCIO::ConstProcessorRcPtr processor = vpt->getProcessor(config, config->getCurrentContext());
			// OCIO::ConstGPUProcessorRcPtr gpu = processor->getDefaultGPUProcessor();

			// OCIO::GpuShaderDescRcPtr shaderDesc = OCIO::GpuShaderDesc::CreateShaderDesc();
			// shaderDesc->setLanguage(OCIO::GPU_LANGUAGE_GLSL_4_0);
			// shaderDesc->setFunctionName("OCIODisplay");
			// shaderDesc->setResourcePrefix("ocio_");

			// gpu->extractGpuShaderInfo(shaderDesc);
		}

		


		std::string ocioVert = R"(
			#version 460 core
			out vec2 TexCoords;

			void main()
			{
				const vec2 pos[4] = vec2[4](
					vec2(-1.0, -1.0),
					vec2( 1.0, -1.0),
					vec2(-1.0,  1.0),
					vec2( 1.0,  1.0)
				);
				
				TexCoords = (pos[gl_VertexID] + 1.0) / 2.0;
				gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);
			}
		)";

		std::string ocioFrag = R"(
			#version 460 core
			in vec2 TexCoords;
			out vec4 FragColor;

			uniform sampler2D u_Texture;

			uniform uint u_FrameCount;

			vec4 OCIODisplay(vec4 inPixel);

			void main()
			{
				vec4 accumulatedBuffer = texture(u_Texture, TexCoords);
				vec4 averagedColor = accumulatedBuffer / float(u_FrameCount);


				averagedColor *= 0.8;
				averagedColor = OCIODisplay(averagedColor);
				vec3 color = averagedColor.rgb;

				FragColor = vec4(color, 1.0);
			}
		)";

		ocioFrag += ocioAces;
		// ocioFrag += ocioLinearToAces;

		// RNDR_CORE_INFO("OCIO Fragment Shader: {0}", ocioFrag);

		m_AccumulateShader = Shader::Create("Editor/assets/shaders/accumulation/accumulate.glsl");
		// m_AccumulateShader = Shader::Create("ocioShader", ocioVert.c_str(), ocioFrag.c_str());
		// frameBufferSpec.Attachments = { FrameBufferTextureFormat::RGBA32F };
		// FrameBufferTextureFormat::RGBA8
		frameBufferSpec.Attachments = { FrameBufferTextureFormat::RGBA8 };
		m_AccumulateFB = FrameBuffer::Create(frameBufferSpec);










		// {
		// 	OCIO::ConstConfigRcPtr config = OCIO::Config::CreateFromFile("ocio://default");

		// 	// List all color spaces
		// 	int numColorSpaces = config->getNumColorSpaces();
		// 	for (int i = 0; i < numColorSpaces; ++i)
		// 	{
		// 		const char* colorSpaceName = config->getColorSpaceNameByIndex(i);
		// 		// std::cout << colorSpaceName << std::endl;
		// 		RNDR_CORE_INFO("Color Space: {0}", colorSpaceName);
		// 	}
		// 	exit(0);
		// }





		//? OCIO - OpenColorIO


	}

	void Sandbox3D::OnDetach()
	{

	}

	void Sandbox3D::OnUpdate(Timestep ts)
	{
		// m_FrameCount = 1;

		Ref<FrameBuffer> fb = m_FrameBufferLibrary.Get("EditorFrameBuffer");
		// FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
		FrameBufferSpecification spec = fb->GetSpecification();
		if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameCount = 1;
			FrameBufferSpecification spec;
			spec.Width = m_ViewportSize.x;
			spec.Height = m_ViewportSize.y;
			// m_FrameBuffer = FrameBuffer::Create(spec);
			// m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			fb->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_AccumulateFB->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			// m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);


			glDeleteTextures(1, &m_TempComputeTextureID);
			glCreateTextures(GL_TEXTURE_2D, 1, &m_TempComputeTextureID);
			glTextureStorage2D(m_TempComputeTextureID, 1, GL_RGBA32F, spec.Width, spec.Height);
			glTextureParameteri(m_TempComputeTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_TempComputeTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(m_TempComputeTextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_TempComputeTextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		}

		if (m_ViewportFocused)
		{
			// m_CameraController.OnUpdate(ts);
			if (m_EditorCamera.OnUpdate(ts))
				m_FrameCount = 1;
		}

		Renderer2D::ResetStats();
		// m_FrameBuffer->Bind();
		fb->Bind();
		RenderCommand::SetClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1));
		RenderCommand::Clear();

		// m_FrameBuffer->ClearAttachment(1, -1);
		fb->ClearAttachment(1, -1);

		// m_ActiveScene->OnUpdateRuntime(ts);
		LineRenderer::ResetStats();
		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		// glm::vec3 min = glm::vec3(1.0f) * INFINITY;
		// glm::vec3 max = glm::vec3(1.0f) * -INFINITY;

		// min = glm::min(min, m_Mesh.m_Triangles.);


		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		my = m_ViewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;


		if (mouseX >= 0 && mouseY >= 0 && mouseX < m_ViewportSize.x && mouseY < m_ViewportSize.y)
		{
			// int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			int pixelData = fb->ReadPixel(1, mouseX, mouseY);
			// RNDR_CORE_INFO("Pixel Data: {0}", pixelData);
			// RNDR_CORE_WARN("MouseX: {0}, MouseY: {1}", mouseX, mouseY);

			if (pixelData > -1)
				m_HoveredEntity = { (entt::entity)pixelData, m_ActiveScene.get()} ;
			else
				m_HoveredEntity = { entt::null, nullptr };
		}
		// m_FrameBuffer->Unbind();
		fb->Unbind();

		// Compute
		// m_ComputeFrameBuffer->Bind();


		// calculate aabb
		
		
		// if (ImGui::Checkbox("Enable Compute", &m_EnableCompute))
		// m_EnableCompute = false;
		if (m_EnableCompute)
		{
			RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1));
			RenderCommand::Clear();
			m_ComputeShader->Bind();
			m_ComputeShader->SetInt("u_ScreenWidth", m_ViewportSize.x);
			m_ComputeShader->SetInt("u_ScreenHeight", m_ViewportSize.y);
			m_ComputeShader->SetInt("u_SkyTextureWidth", m_SkyTextureWidth);
			m_ComputeShader->SetInt("u_SkyTextureHeight", m_SkyTextureHeight);
			m_ComputeShader->SetMat4("u_InvView", glm::inverse(m_EditorCamera.GetViewMatrix()));
			m_ComputeShader->SetMat4("u_InvProj", glm::inverse(m_EditorCamera.GetProjection()));
			m_ComputeShader->SetFloat3("u_CameraPos", m_EditorCamera.GetPosition());


			m_ComputeShader->SetUnsignedInt("u_FrameCount", m_FrameCount);
			// m_ComputeShader->SetUnsignedInt("u_MaxIndices", m_ActiveScene->m_Mesh.m_VerticesIndex.size());
			// m_ComputeShader->SetUnsignedInt("u_ShowDepth", m_ActiveScene->GetBVHDepth());
			glBindImageTexture(0, m_TempComputeTextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
			glBindImageTexture(1, m_SkyTextureID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

			m_ComputeShader->SetInt("u_TriangleCount", m_ActiveScene->m_Mesh.m_Triangles.size());
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_TriangleBuffer);

			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_BVHBuffer);

			// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_PositionsSSBO);
			// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_IndexSSBO);
			// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_bvhSSBO);
			m_ComputeShader->Dispatch((m_ViewportSize.x + 7) / 8, (m_ViewportSize.y + 7) / 8, 1);
			// glGetError();
		}


		{
			m_AccumulateFB->Bind();
			RenderCommand::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1));
			RenderCommand::Clear();
			m_AccumulateShader->Bind();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_TempComputeTextureID);

			m_AccumulateShader->SetInt("u_Texture", 0);
			m_AccumulateShader->SetUnsignedInt("u_FrameCount", m_FrameCount);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			m_AccumulateShader->Unbind();
			m_AccumulateFB->Unbind();
		}
		// m_TempComputeTextureID
		// glBindTexture(GL_TEXTURE_2D, m_TempComputeTextureID);

		// m_ComputeFrameBuffer->Unbind();














		// if (m_FrameCount == 10000)
		// {
		// 	std::vector<float> data;

		// 	data.resize(m_ViewportSize.x * m_ViewportSize.y * 4);
		// 	glGetTextureImage(m_TempComputeTextureID, 0, GL_RGBA, GL_FLOAT, data.size() * sizeof(float), data.data());



		// 	using namespace OIIO;
		// 	auto out = ImageOutput::create("output.exr");

		// 	ImageSpec spec(m_ViewportSize.x, m_ViewportSize.y, 4, TypeDesc::FLOAT);
		// 	out->open("output.exr", spec);
		// 	out->write_image(TypeDesc::FLOAT, data.data());
		// 	out->close();

		// 	RNDR_CORE_INFO("Image saved successfully");
		// 	exit(0);
		// }



		if (Input::IsKeyPressed(RNDR_KEY_LEFT_CONTROL) && Input::IsKeyPressed(RNDR_KEY_P))
		{
			// std::vector<float> data;

			// data.resize(m_ViewportSize.x * m_ViewportSize.y * 4);
			// glGetTextureImage(m_AccumulateFB->GetColorAttachmentRendererID(), 0, GL_RGBA, GL_FLOAT, data.size() * sizeof(float), data.data());

			// using namespace OIIO;
			// auto out = ImageOutput::create("output.exr");

			// ImageSpec spec(m_ViewportSize.x, m_ViewportSize.y, 4, TypeDesc::FLOAT);
			// out->open("output.exr", spec);
			// out->write_image(TypeDesc::FLOAT, data.data());
			// out->close();

		}






		m_FrameCount++;
	}


	void Sandbox3D::OnImGuiRender()
	{
		{
			ImGuiLayer::ImGuiBeginDockspace();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		{
			m_SceneHierarchyPanel.OnImGuiRender();
			m_ContentBrowserPanel.OnImGuiRender();
			m_MaterialPanel.OnImGuiRender();

			m_AssetManagerPanel.OnImGuiRender();
		}
		{
			ImGui::Begin("Stats");

			ImGui::ColorEdit4("Test Color", glm::value_ptr(M_TestColor));
			// ImGui::ColorEdit4("MyColor##3", glm::value_ptr(color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			std::string name = "None";
			if (m_HoveredEntity)
				name = m_HoveredEntity.GetComponent<TagComponent>().Tag;

			ImGui::Text("Hovered Entity: %s", name.c_str());

			ImGui::Text("Viewport Size: %.0fx%.0f", m_ViewportSize.x, m_ViewportSize.y);

			// ImGui::Text("Framebuffer Size: %dx%d", m_FrameBuffer->GetWidth(), m_FrameBuffer->GetHeight());
			Ref<FrameBuffer> fb = m_FrameBufferLibrary.Get("EditorFrameBuffer");
			ImGui::Text("Framebuffer Size: %dx%d", fb->GetWidth(), fb->GetHeight());

			ImGui::Separator();

			glm::vec3 cameraPos = m_EditorCamera.GetPosition();
			ImGui::Text("Camera Position: %.3f, %.3f, %.3f", cameraPos.x, cameraPos.y, cameraPos.z);
			// auto stats = Renderer2D::GetStats();
			// ImGui::Text("Renderer2D Stats:");
			// ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			// ImGui::Text("Quads: %d", stats.QuadCount);
			// ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			// ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::Separator();
			ImGui::Text("Materials");
			for (auto& material : *m_MaterialLibrary)
			{
				auto [map_name, mat] = material;
				std::string mat_name = mat->GetName();
				// ImGui::Text("%s", map_name.c_str());
				ImGui::Text("%s", mat_name.c_str());
			}

			ImGui::Checkbox("Enable Compute", &m_EnableCompute);
			bool enableBVH = m_ActiveScene->GetBVHEnabled();
			if (ImGui::Checkbox("Enable BVH", &enableBVH))
				m_ActiveScene->EnableBVH(enableBVH);
			uint32_t bvhDepth = m_ActiveScene->GetBVHDepth();
			if (ImGui::InputInt("BVH Depth", (int*)&bvhDepth))
				m_ActiveScene->SetBVHDepth(bvhDepth);



			// LineRenderer::ResetStats();
			LineRenderer::Statistics stats = LineRenderer::GetStats();
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Line Count: %d", stats.LineCount);


			if (ImGui::Button("Refresh render view"))
				m_FrameCount = 1;

			ImGui::Text("Framecount %d", m_FrameCount);



			ImGui::End();



			ImGui::Begin("Entity list");
			const auto& reg = m_ActiveScene->GetRegistry();
			auto view = reg.view<TagComponent>();
			// list all entit tags
			for (auto entity : view)
			{
				Entity entt = { entity, m_ActiveScene.get() };
				auto& tag = entt.GetComponent<TagComponent>().Tag;
				ImGui::Text("%s", tag.c_str());
			}

			ImGui::End();
		}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		{
			ImGui::Begin("Viewport");
			auto viewportOffset = ImGui::GetCursorPos(); // Includes tab bar

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			// uint64_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
			uint64_t textureID = m_FrameBufferLibrary.Get("EditorFrameBuffer")->GetColorAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			auto windowSize = ImGui::GetWindowSize();
			auto minBound = ImGui::GetWindowPos();
			minBound.x += viewportOffset.x;
			minBound.y += viewportOffset.y;

			ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
			m_ViewportBounds[0] = { minBound.x, minBound.y };
			m_ViewportBounds[1] = { maxBound.x, maxBound.y };

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					std::filesystem::path path((const wchar_t*)payload->Data);
					RNDR_CORE_INFO("Dropped file: {0}", path.string());
					// print file extension
					std::string extension = path.extension().string();
					RNDR_CORE_INFO("Extension: {0}", extension);
					// OpenScene(path);
					if (extension == ".fbx" || extension == ".obj")
					{
						// LoadMeshEntity(path);
						Mesh::ImportMesh(path, m_ActiveScene);
					}
				}
				ImGui::EndDragDropTarget();
			}

			

			// Gizmos
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity && m_GizmoType != -1)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				float windowWidth = (float)ImGui::GetWindowWidth();
				float windowHeight = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				// Editor camera
				const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

				// Entity transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 localTransform = tc.GetTransform();
				glm::mat4 worldTransform = tc.GetWorldTransform();

				// Snapping
				bool snap = Input::IsKeyPressed(RNDR_KEY_LEFT_CONTROL);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };


				glm::mat4 worldTransformCopy = worldTransform;
				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(worldTransformCopy),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					// glm::vec3 translation, rotation, scale;
					// Math::DecomposeTransform(worldTransform, translation, rotation, scale);

					// glm::vec3 deltaRotation = rotation - tc.Rotation;
					// tc.Translation = translation;
					// tc.Rotation += deltaRotation;
					// tc.Scale = scale;
					// m_SceneHierarchyPanel.UpdateWorldTransform(selectedEntity);


					glm::vec3 worldTranslation, worldRotation, worldScale;
					Math::DecomposeTransform(worldTransform, worldTranslation, worldRotation, worldScale);
					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(worldTransformCopy, translation, rotation, scale);

					tc.Translation = translation - worldTranslation + tc.Translation;
					tc.Rotation = rotation - worldRotation + tc.Rotation;
					tc.Scale = scale - worldScale + tc.Scale;

					m_SceneHierarchyPanel.UpdateWorldTransform(selectedEntity);
				}
			}
			ImGui::End();


			{
				ImGui::Begin("Render view");
				uint64_t textureID = m_TempComputeTextureID;
				textureID = m_AccumulateFB->GetColorAttachmentRendererID();
				// uint64_t textureID = m_SkyTextureID;
				ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::End();
			}
		}
		ImGui::PopStyleVar();


		


		{
			ImGui::Begin("##New-Entity");
			{
				ImVec2 size = ImVec2(32.0f, 32.0f);                         // Size of the image we want to make visible
				// ImVec2 uv1 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
				// ImVec2 uv0 = ImVec2(1.0f, 1.0f);                            // UV coordinates for upper-right
				ImVec2 uv1 = ImVec2(1.0f, 0.0f);                            // UV coordinates for lower-left
				ImVec2 uv0 = ImVec2(0.0f, 1.0f);                            // UV coordinates for upper-right
				ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);             // Black background
				ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
				// if (ImGui::ImageButton("##Cube", reinterpret_cast<void*>(m_CubeIcon->GetTextureID()), size, uv0, uv1, bg_col, tint_col))
				if (ImGui::ImageButton("##Cube", (ImTextureID)(uint64_t)m_CubeIcon->GetTextureID(), size, uv0, uv1, bg_col, tint_col))
				{
					static int count = 0;
					auto entity = m_ActiveScene->CreateEntity("Cube " + std::to_string(count++));
					entity.AddComponent<CubeComponent>(glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
					static int i = 0;
					Ref<Material> newMaterial = CreateRef<Material>();
					newMaterial->SetShader(Shader::Create("Editor/assets/shaders/CubeShader.glsl"));
					newMaterial->SetName("tempMaterial" + std::to_string(i++));
					m_MaterialLibrary->Add(newMaterial->GetName(), newMaterial);
					entity.AddComponent<DefaultMaterialComponent>(newMaterial);
					m_SceneHierarchyPanel.SetSelectedEntity(entity);
				}
				if (ImGui::ImageButton("##NullEntity", (ImTextureID)(uint64_t)m_NullIcon->GetTextureID(), size, uv0, uv1, bg_col, tint_col))
				{
					static int count = 0;
					auto entity = m_ActiveScene->CreateEntity("Null " + std::to_string(count++));
					entity.AddComponent<NullComponent>();
					m_SceneHierarchyPanel.SetSelectedEntity(entity);
				}
			}
			ImGui::End();
		}
		// ImGui::ShowDemoWindow();

		// ImGui::End();
		ImGuiLayer::ImGuiEndDockspace();

	}




	void Sandbox3D::OnEvent(Event& e)
	{
		m_FrameCount = 1;
		m_EditorCamera.OnEvent(e);
		//TODO: camera controller
	
		// RNDR_CORE_INFO("{0}", e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(RNDR_BIND_EVENT_FN(Sandbox3D::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(RNDR_BIND_EVENT_FN(Sandbox3D::OnMouseButtonPressed));
	}

	bool Sandbox3D::OnKeyPressed(KeyPressedEvent& e)
	{
		// RNDR_CORE_INFO("Key Pressed: {0}", e.GetKeyCode());
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(RNDR_KEY_LEFT_CONTROL) || Input::IsKeyPressed(RNDR_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(RNDR_KEY_LEFT_SHIFT) || Input::IsKeyPressed(RNDR_KEY_RIGHT_SHIFT);
		switch (e.GetKeyCode())
		{
			case RNDR_KEY_S:
			{
				if (control && shift)
					SaveSceneAs();
				break;
			}
			case RNDR_KEY_O:
			{
				if (control)
					OpenScene();
				break;
			}
			case RNDR_KEY_N:
			{
				if (control)
					NewScene();
				break;
			}

			// Gizmos
			case RNDR_KEY_Q:
				m_GizmoType = -1;
				break;
			case RNDR_KEY_E:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case RNDR_KEY_R:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case RNDR_KEY_T:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
		return false;
	}

	bool Sandbox3D::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		//? Mouse picking
		if (e.GetMouseButton() == RNDR_MOUSE_BUTTON_1 && m_ViewportHovered)
		{
			if (!ImGuizmo::IsOver() && !Input::IsKeyPressed(RNDR_KEY_LEFT_ALT))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void Sandbox3D::NewScene()
	{
		RNDR_CORE_INFO("New Scene");
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	// void Sandbox3D::OpenScene()
	// {
	// 	RNDR_CORE_INFO("Open Scene");
	// 	std::string filepath = FileDialogs::OpenFile("Rndr Scene (*.yaml)\0*.yaml\0");
	// 	if (!filepath.empty())
	// 	{
	// 		OpenScene(filepath);
	// 		// m_ActiveScene = CreateRef<Scene>();
	// 		// m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	// 		// m_SceneHierarchyPanel.SetContext(m_ActiveScene);

	// 		// SceneSerializer serializer(m_ActiveScene);
	// 		// serializer.Deserialize(filepath);
	// 	}
	// }

	// void Sandbox3D::OpenScene(const std::filesystem::path& path)
	// {
	// 	//TODO: check for balid file type
	// 	m_ActiveScene = CreateRef<Scene>();
	// 	m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	// 	m_SceneHierarchyPanel.SetContext(m_ActiveScene);

	// 	SceneSerializer serializer(m_ActiveScene);
	// 	serializer.Deserialize(path.string());
	// }


	void Sandbox3D::OpenScene()
	{
		// std::string filepath = FileDialogs::OpenFile("Hazel Scene (*.hazel)\0*.hazel\0");
		std::string filepath = FileDialogs::OpenFile("Rndr Scene (*.yaml)\0*.yaml\0");
		if (!filepath.empty())
			OpenScene(filepath);
	}

	void Sandbox3D::OpenScene(const std::filesystem::path& path)
	{
		RNDR_CORE_INFO("Open Scene: {0}", path.string());
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize(path.string());
	}

	void Sandbox3D::SaveSceneAs()
	{
		RNDR_CORE_INFO("Save Scene As");
		std::string filepath = FileDialogs::SaveFile("Rndr Scene (*.yaml)\0*.yaml\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}









	void Sandbox3D::LoadMeshEntity(const std::filesystem::path& path)
	{
		auto entity = m_ActiveScene->CreateEntity("Mesh Entity");
		auto& mc = entity.AddComponent<MeshComponent>();
		mc.Mesh = CreateRef<Mesh>(path.string());
		mc.Material->SetShader(Shader::Create("Editor/assets/shaders/MeshShader.glsl"));
		// entity.AddComponent<DefaultMaterialComponent>(m_MaterialLibrary->Get("tempMaterial"));
		m_SceneHierarchyPanel.SetSelectedEntity(entity);
	}
}