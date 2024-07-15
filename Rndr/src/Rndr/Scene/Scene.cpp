#include "Scene.h"

#include "glm/glm.hpp"

#include "Rndr/Scene/Components.h"

#include "Entity.h"

#include "Rndr/Renderer/Renderer2D.h"
#include "Rndr/Renderer/Renderer3D.h"

#include "Rndr/Renderer/LineRenderer.h"

#include "Rndr/Core/Log.h"

#include "Rndr/Renderer/RenderCommand.h"


#include "Rndr/Renderer/Mesh.h"

#include <OpenImageIO/imageio.h>

namespace Rndr
{
	static void splitNodes(BVHNode& node, int depth)
	{
		if (node.Triangles.size() <= 1 || depth > 15)
			return;

		node.ChildA = CreateRef<BVHNode>();
		node.ChildB = CreateRef<BVHNode>();

		glm::vec3 min = glm::vec3(1.0f) * INFINITY;
		glm::vec3 max = glm::vec3(1.0f) * -INFINITY;
		for (Mesh::Triangle tri : node.Triangles)
		{
			min = glm::min(min, tri.V1.Position);
			min = glm::min(min, tri.V2.Position);
			min = glm::min(min, tri.V3.Position);

			max = glm::max(max, tri.V1.Position);
			max = glm::max(max, tri.V2.Position);
			max = glm::max(max, tri.V3.Position);
		}
		node.AABB.Min = min;
		node.AABB.Max = max;

		glm::vec3 size = max - min;
		int axis = 0;
		if (size.y > size.x)
			axis = 1;
		if (size.z > size.y && size.z > size.x)
			axis = 2;

		float split = (min[axis] + max[axis]) / 2.0f;
		std::vector<Mesh::Triangle> left, right;
		for (Mesh::Triangle tri : node.Triangles)
		{
			glm::vec3 center = (tri.V1.Position + tri.V2.Position + tri.V3.Position) / 3.0f;
			if (center[axis] < split)
				left.push_back(tri);
			else
				right.push_back(tri);
		}

		node.ChildA->Triangles = left;
		node.ChildB->Triangles = right;

		splitNodes(*node.ChildA, depth + 1);
		splitNodes(*node.ChildB, depth + 1);
	}

	static void drawBVH(BVHNode* node, int depth, int visableDepth)
	{
		// const int visableDepth = 10;

		if (depth > visableDepth || !node)
			return;

		LineRenderer::DrawAABB(node->AABB.Min, node->AABB.Max, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

		drawBVH(node->ChildA.get(), depth + 1, visableDepth);
		drawBVH(node->ChildB.get(), depth + 1, visableDepth);
	}

	Scene::Scene()
	{
		// m_Mesh = Mesh("Editor/assets/meshes/cube.obj");
		// m_Mesh = Mesh("Editor/assets/meshes/dragon.obj");
		// m_Mesh = Mesh("Editor/assets/meshes/bunny.obj");
		m_Mesh = Mesh("Editor/assets/meshes/dragon_80k.obj");
		// m_Mesh = Mesh("Editor/assets/meshes/dragon.fbx");
		m_MeshMaterial = CreateRef<Material>();
		m_MeshMaterial->SetShader(Shader::Create("Editor/assets/shaders/MeshShader.glsl"));



		glm::vec3 min = glm::vec3(1.0f) * INFINITY;
		glm::vec3 max = glm::vec3(1.0f) * -INFINITY;
		for (Mesh::Triangle tri : m_Mesh.m_Triangles)
		{
			min = glm::min(min, tri.V1.Position);
			min = glm::min(min, tri.V2.Position);
			min = glm::min(min, tri.V3.Position);

			max = glm::max(max, tri.V1.Position);
			max = glm::max(max, tri.V2.Position);
			max = glm::max(max, tri.V3.Position);
		}
		m_BVHTree.AABB = {min, max};
		m_BVHTree.Triangles = m_Mesh.m_Triangles;
		splitNodes(m_BVHTree, 0);
		// LineRenderer::BeginScene(camera, m_LineMaterial, 1.0f);
		// LineRenderer::DrawAABB(min, max, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		// LineRenderer::EndScene();



	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		// Entity entity = { m_Registry.create(), this };
		// entity.AddComponent<IDComponent>();
		// entity.AddComponent<TransformComponent>();
		// auto& tag = entity.AddComponent<TagComponent>();
		// tag.Tag = name.empty() ? "Entity" : name;
		// return entity;
		return CreateEntityWithID(UUID(), name);
	}

	Entity Scene::CreateEntityWithID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}
	
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		LineRenderer::BeginScene(camera, m_LineMaterial, 1.0f);
		// drawBVH( m_BVHTree.ChildA.get());
		// drawBVH( m_BVHTree.ChildB.get());
		if (m_EnableBVH)
			drawBVH(&m_BVHTree, 0, m_BVHDepth);;
		
		// LineRenderer::DrawAABB(min, max, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		LineRenderer::EndScene();


		int defaultMaterialCount = 0;
		auto group = m_Registry.group<TransformComponent>(entt::get<DefaultMaterialComponent>);
		if (group.size())
		{
			auto defaultMaterial = group.get<DefaultMaterialComponent>(group.front());
			Renderer2D::BeginScene(camera, defaultMaterial.Material);
			for (auto entity : group)
			{
				Entity ent = { entity, this };
				if (ent.HasComponent<CubeComponent>())
				{
					glm::mat4 transform = ent.GetComponent<TransformComponent>().GetTransform();
					CubeComponent cube = ent.GetComponent<CubeComponent>();
					DefaultMaterialComponent material = ent.GetComponent<DefaultMaterialComponent>();
					Renderer2D::DrawCube(transform, cube, material, (int)entity);
				}
			}
			Renderer2D::EndScene();
		}

		// //? Mesh
		{
			auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
				if (mesh.Material)
				{
					mesh.Material->Bind();
					mesh.Material->GetShader()->SetMat4("u_ViewProjection", camera.GetViewProjection());
					mesh.Material->GetShader()->SetMat4("u_Transform", transform.GetTransform());
					RenderCommand::DrawIndexed(mesh.Mesh->m_VertexArray, mesh.Mesh->m_IndexBuffer->GetCount());
				}
			}
		}


		LineRenderer::BeginScene(camera, m_LineMaterial, 1.0f);
		float camY = camera.GetPosition().y;
		glm::vec4 gridColor = {0.28f, 0.28f, 0.28f, .0f};
		float gridSize = 25.0f;
		float gridSpacing = 1.0f;
		// float gridSpacing = 1.0f + std::floor(camY / 5.0f);
		for (int x = -gridSize; x <= gridSize; x++)
		{
			for (int z = -gridSize; z <= gridSize; z++)
			{
				// LineRenderer::DrawLine({(float)x, 0.0f, (float)z}, {(float)x + 1.0f, 0.0f, (float)z}, gridColor);
				// LineRenderer::DrawLine({(float)x, 0.0f, (float)z}, {(float)x, 0.0f, (float)z + 1.0f}, gridColor);
				LineRenderer::DrawLine({(float)x, 0.0f, (float)z}, {(float)x + gridSpacing, 0.0f, (float)z}, gridColor);
				LineRenderer::DrawLine({(float)x, 0.0f, (float)z}, {(float)x, 0.0f, (float)z + gridSpacing}, gridColor);
			}
		}
		LineRenderer::EndScene();


		LineRenderer::BeginScene(camera, m_LineMaterial, 2.0f);
		//? x
		LineRenderer::DrawLine(glm::vec3(0.0), glm::vec3(2.0f, 0.0f, 0.0f), glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));
		//? Y
		LineRenderer::DrawLine(glm::vec3(0.0f), glm::vec3(0.0f, 2.0f, 0.0f), glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
		//? Z
		LineRenderer::DrawLine(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 2.0f), glm::vec4(0.2f, 0.2f, 0.8f, 1.0f));
		LineRenderer::EndScene();


		//? Draw Mesh
		m_MeshMaterial->Bind();
		m_MeshMaterial->GetShader()->SetMat4("u_ViewProjection", camera.GetViewProjection());
		constexpr glm::mat4 transform  = glm::mat4(1.0f);
		m_MeshMaterial->GetShader()->SetMat4("u_Transform", transform);
		// RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.VertexIndexCount);
		RenderCommand::DrawIndexed(m_Mesh.m_VertexArray, m_Mesh.m_IndexBuffer->GetCount());





		// glm::vec3 min = glm::vec3(-5.0f, -5.0f, -5.0f);
		// glm::vec3 max = glm::vec3(5.0f, 5.0f, 5.0f);
		// LineRenderer::BeginScene(camera, m_LineMaterial, 1.0f);
		// LineRenderer::DrawAABB(min, max, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		// LineRenderer::EndScene();



		// glm::vec3 min = glm::vec3(1.0f) * INFINITY;
		// glm::vec3 max = glm::vec3(1.0f) * -INFINITY;
		// for (Mesh::Triangle tri : m_Mesh.m_Triangles)
		// {
		// 	min = glm::min(min, tri.V1.Position);
		// 	min = glm::min(min, tri.V2.Position);
		// 	min = glm::min(min, tri.V3.Position);

		// 	max = glm::max(max, tri.V1.Position);
		// 	max = glm::max(max, tri.V2.Position);
		// 	max = glm::max(max, tri.V3.Position);
		// }
		// LineRenderer::BeginScene(camera, m_LineMaterial, 1.0f);
		// LineRenderer::DrawAABB(min, max, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		// LineRenderer::EndScene();



		// glm::vec3 min = glm::vec3(1.0f) * INFINITY;
		// glm::vec3 max = glm::vec3(1.0f) * -INFINITY;
		// //? Mesh
		// {
		// 	auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
		// 	for (auto entity : group)
		// 	{
		// 		auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		// 		if (mesh.Material)
		// 		{
		// 			glm::mat4& t = transform.GetTransform();
		// 			std::vector<glm::vec3> transformedVertices;
		// 			transformedVertices.reserve(mesh.Mesh->m_Triangles.size() * 3);  // Reserve memory

		// 			for (const Mesh::Triangle& tri : mesh.Mesh->m_Triangles)
		// 			{
		// 				transformedVertices.push_back(glm::vec3(t * glm::vec4(tri.V1.Position, 1.0f)));
		// 				transformedVertices.push_back(glm::vec3(t * glm::vec4(tri.V2.Position, 1.0f)));
		// 				transformedVertices.push_back(glm::vec3(t * glm::vec4(tri.V3.Position, 1.0f)));
		// 			}

		// 			for (const glm::vec3& vert : transformedVertices)
		// 			{
		// 				min = glm::min(min, vert);
		// 				max = glm::max(max, vert);
		// 			}
		// 			mesh.Material->Bind();
		// 			mesh.Material->GetShader()->SetMat4("u_ViewProjection", camera.GetViewProjection());
		// 			mesh.Material->GetShader()->SetMat4("u_Transform", transform.GetTransform());
		// 			RenderCommand::DrawIndexed(mesh.Mesh->m_VertexArray, mesh.Mesh->m_IndexBuffer->GetCount());
		// 		}
		// 	}
		// }
		// LineRenderer::BeginScene(camera, m_LineMaterial, 1.0f);
		// LineRenderer::DrawAABB(min, max, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		// LineRenderer::EndScene();
	}


	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		//? Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}


	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{entity, this};
		}
		return {};
	}












	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		if constexpr (std::is_same<T, CameraComponent>::value)
		{
			RNDR_CORE_INFO("Camera Component Added");
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
		}
		// Add more specializations as needed
	}

	// Explicit template instantiations
	template void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component);
	template void Scene::OnComponentAdded<QuadComponent>(Entity entity, QuadComponent& component);
	template void Scene::OnComponentAdded<CubeComponent>(Entity entity, CubeComponent& component);
	template void Scene::OnComponentAdded<DefaultMaterialComponent>(Entity entity, DefaultMaterialComponent& component);
	template void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component);







}