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
	void Scene::drawBVH(bvhNode node, int depth, int visableDepth)
	{
		if (depth > visableDepth)
			return;

		LineRenderer::DrawAABB(node.Min, node.Max, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

		if (node.ChildIndex == 0)
			return;
		drawBVH(m_BVH[node.ChildIndex], depth + 1, visableDepth);
		drawBVH(m_BVH[node.ChildIndex + 1], depth + 1, visableDepth);
	}

	// void Scene::drawBVHnew(Mesh::bvhNode node, int depth, int visableDepth)
	// {
	// 	if (depth > visableDepth)
	// 		return;

	// 	LineRenderer::DrawAABB(node.Min, node.Max, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

	// 	if (node.ChildIndex == 0)
	// 		return;
	// 	drawBVHnew(node[])
	// }
	void Scene::drawBVHnew(std::vector<Mesh::bvhNode> bvh, int index, int depth, int visableDepth)
	{
		if (depth > visableDepth)
			return;

		// LineRenderer::DrawAABB(bvh[index].Min, bvh[index].Max, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		LineRenderer::DrawAABB(bvh[index].Min, bvh[index].Max, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

		if (bvh[index].ChildIndex == 0)
			return;

		drawBVHnew(bvh, bvh[index].ChildIndex, depth + 1, visableDepth);
		drawBVHnew(bvh, bvh[index].ChildIndex + 1, depth + 1, visableDepth);

		// if (bvh[depth] == 0)
			// return;
		// drawBVHnew(node[])
	}


	const int maxDepth = 10;
	

	void Scene::generateAABB(int parentIndex)
	{
		// m_BVH[parentIndex];
		int startIndex = m_BVH[parentIndex].TriangleIndex;
		int TriangleCount = m_BVH[parentIndex].TriangleCount;


		glm::vec3 min = glm::vec3(1.0f) * INFINITY;
		glm::vec3 max = glm::vec3(1.0f) * -INFINITY;
		for (int i = startIndex; i < startIndex + TriangleCount; i++)
		{
			// min = glm::min(min, m_Mesh.m_Triangles[i].V1.Position);
			// min = glm::min(min, m_Mesh.m_Triangles[i].V2.Position);
			// min = glm::min(min, m_Mesh.m_Triangles[i].V3.Position);

			// max = glm::max(max, m_Mesh.m_Triangles[i].V1.Position);
			// max = glm::max(max, m_Mesh.m_Triangles[i].V2.Position);
			// max = glm::max(max, m_Mesh.m_Triangles[i].V3.Position);


			// Mesh::Triangle tri = m_Mesh.m_TriangleCache[0][1];
			min = glm::min(min, m_Mesh.m_Triangles[i].V1);
			min = glm::min(min, m_Mesh.m_Triangles[i].V2);
			min = glm::min(min, m_Mesh.m_Triangles[i].V3);

			max = glm::max(max, m_Mesh.m_Triangles[i].V1);
			max = glm::max(max, m_Mesh.m_Triangles[i].V2);
			max = glm::max(max, m_Mesh.m_Triangles[i].V3);
		}
		m_BVH[parentIndex].Min = min;
		m_BVH[parentIndex].Max = max;
	}

	void Scene::generateAABB(int parentIndex, int meshIndex)
	{
		// m_BVH[parentIndex];
		int startIndex = m_BVH[parentIndex].TriangleIndex;
		int TriangleCount = m_BVH[parentIndex].TriangleCount;


		glm::vec3 min = glm::vec3(1.0f) * INFINITY;
		glm::vec3 max = glm::vec3(1.0f) * -INFINITY;
		for (int i = startIndex; i < startIndex + TriangleCount; i++)
		{
			// min = glm::min(min, m_Mesh.m_Triangles[i].V1.Position);
			// min = glm::min(min, m_Mesh.m_Triangles[i].V2.Position);
			// min = glm::min(min, m_Mesh.m_Triangles[i].V3.Position);

			// max = glm::max(max, m_Mesh.m_Triangles[i].V1.Position);
			// max = glm::max(max, m_Mesh.m_Triangles[i].V2.Position);
			// max = glm::max(max, m_Mesh.m_Triangles[i].V3.Position);


			Mesh::Triangle tri = m_Mesh.m_TriangleCache[meshIndex][i];
		}
		m_BVH[parentIndex].Min = min;
		m_BVH[parentIndex].Max = max;
	}

	static int bvhnew = 0;
	void Scene::splitBVH(int parentIndex, int depth)
	{
		const int maxDepth = 32;
		if (depth == maxDepth)
			return;
		// if (depth == maxDepth || m_BVH[parentIndex].TriangleCount <= 1)
		// 	return;
		bvhnew++;
		bvhNode parent = m_BVH[parentIndex];

		m_BVH[parentIndex].ChildIndex = m_BVH.size();
		glm::vec3 size = parent.Max - parent.Min;
		int splitAxis = size.x > std::max(size.y, size.z) ? 0 : size.y > size.z ? 1 : 2;
		float splitPos = (parent.Min[splitAxis] + parent.Max[splitAxis]) / 2.0f;

		bvhNode childA, childB;
		childA.TriangleIndex = parent.TriangleIndex;
		childA.TriangleCount = 0;
		childB.TriangleIndex = parent.TriangleIndex;
		childB.TriangleCount = 0;

		for (int i = parent.TriangleIndex; i < parent.TriangleIndex + parent.TriangleCount; i++)
		{
			// glm::vec3 V1 = m_Mesh.m_Triangles[i].V1.Position;
			// glm::vec3 V2 = m_Mesh.m_Triangles[i].V2.Position;
			// glm::vec3 V3 = m_Mesh.m_Triangles[i].V3.Position;

			glm::vec3 V1 = m_Mesh.m_Triangles[i].V1;
			glm::vec3 V2 = m_Mesh.m_Triangles[i].V2;
			glm::vec3 V3 = m_Mesh.m_Triangles[i].V3;

			glm::vec3 center = (V1 + V2 + V3) / 3.0f;

			bool isSideA = center[splitAxis] < splitPos;

			bvhNode& child = isSideA ? childA : childB;
			child.TriangleCount++;

			if (isSideA)
			{
				int swap = child.TriangleIndex + child.TriangleCount - 1;
				std::swap(m_Mesh.m_Triangles[i], m_Mesh.m_Triangles[swap]);
				childB.TriangleIndex++;
			}
		}

		m_BVH.push_back(childA);
		m_BVH.push_back(childB);

		generateAABB(m_BVH[parentIndex].ChildIndex);
		generateAABB(m_BVH[parentIndex].ChildIndex + 1);

		//TODO: compress the bvh stuct for memory and cache efficiency
		// m_BVH[parentIndex].TriangleCount = 0;

		if (childA.TriangleCount > 0 && childB.TriangleCount > 0)
		{
			splitBVH(m_BVH[parentIndex].ChildIndex, depth + 1);
			splitBVH(m_BVH[parentIndex].ChildIndex + 1, depth + 1);
		}
		// splitBVH(m_BVH[parentIndex].ChildIndex, depth + 1);
		// splitBVH(m_BVH[parentIndex].ChildIndex + 1, depth + 1);
		
	}

	void Scene::splitBVH(int parentIndex, int depth, int meshIndex)
	{
		const int maxDepth = 32;
		if (depth == maxDepth)
			return;
		// if (depth == maxDepth || m_BVH[parentIndex].TriangleCount <= 1)
		// 	return;
		bvhnew++;
		bvhNode parent = m_BVH[parentIndex];

		m_BVH[parentIndex].ChildIndex = m_BVH.size();
		glm::vec3 size = parent.Max - parent.Min;
		int splitAxis = size.x > std::max(size.y, size.z) ? 0 : size.y > size.z ? 1 : 2;
		float splitPos = (parent.Min[splitAxis] + parent.Max[splitAxis]) / 2.0f;

		bvhNode childA, childB;
		childA.TriangleIndex = parent.TriangleIndex;
		childA.TriangleCount = 0;
		childB.TriangleIndex = parent.TriangleIndex;
		childB.TriangleCount = 0;

		for (int i = parent.TriangleIndex; i < parent.TriangleIndex + parent.TriangleCount; i++)
		{
			// glm::vec3 V1 = m_Mesh.m_Triangles[i].V1.Position;
			// glm::vec3 V2 = m_Mesh.m_Triangles[i].V2.Position;
			// glm::vec3 V3 = m_Mesh.m_Triangles[i].V3.Position;

			glm::vec3 V1 = m_Mesh.m_Triangles[i].V1;
			glm::vec3 V2 = m_Mesh.m_Triangles[i].V2;
			glm::vec3 V3 = m_Mesh.m_Triangles[i].V3;

			glm::vec3 center = (V1 + V2 + V3) / 3.0f;

			bool isSideA = center[splitAxis] < splitPos;

			bvhNode& child = isSideA ? childA : childB;
			child.TriangleCount++;

			if (isSideA)
			{
				int swap = child.TriangleIndex + child.TriangleCount - 1;
				std::swap(m_Mesh.m_Triangles[i], m_Mesh.m_Triangles[swap]);
				childB.TriangleIndex++;
			}
		}

		m_BVH.push_back(childA);
		m_BVH.push_back(childB);

		generateAABB(m_BVH[parentIndex].ChildIndex, meshIndex);
		generateAABB(m_BVH[parentIndex].ChildIndex + 1, meshIndex);

		//TODO: compress the bvh stuct for memory and cache efficiency
		// m_BVH[parentIndex].TriangleCount = 0;

		if (childA.TriangleCount > 0 && childB.TriangleCount > 0)
		{
			splitBVH(m_BVH[parentIndex].ChildIndex, depth + 1);
			splitBVH(m_BVH[parentIndex].ChildIndex + 1, depth + 1);
		}
		// splitBVH(m_BVH[parentIndex].ChildIndex, depth + 1);
		// splitBVH(m_BVH[parentIndex].ChildIndex + 1, depth + 1);
		
	}

	void Scene::generateBVH()
	{
		// bvhNode root;
		// root.TriangleIndex = 0;
		// root.TriangleCount = m_Mesh.m_Triangles.size();
		// RNDR_CORE_INFO("Triangle Count: {0}", root.TriangleCount);
		// int parentIndex = m_BVH.size();
		// m_BVH.push_back(root);
		// generateAABB(parentIndex);
		// splitBVH(parentIndex, 0);

		for (int i = 1; i < m_Mesh.m_SubMeshes.size(); i++)
		{
			bvhNode root;
			root.TriangleIndex = m_Mesh.m_SubMeshes[i].BaseIndex;
			root.TriangleCount = m_Mesh.m_SubMeshes[i].IndexCount / 3;
			int parentIndex = m_BVH.size();
			m_BVH.push_back(root);
			generateAABB(parentIndex, i);
			splitBVH(parentIndex, 0, i);
		}
	}


	Scene::Scene()
	{
		// m_Mesh = Mesh("Editor/assets/meshes/cube.obj");
		// m_Mesh = Mesh("Editor/assets/meshes/dragon.obj");
		// m_Mesh = Mesh("Editor/assets/meshes/bunny.obj");

		// m_Mesh = Mesh("Editor/assets/meshes/sphere.obj");
		// m_Mesh = Mesh("Editor/assets/meshes/dragon.obj");
		// m_Mesh = Mesh("Editor/assets/meshes/dragon_with_plane.obj");
		m_Mesh = Mesh("Editor/assets/meshes/3cubes.fbx");
		// m_Mesh = Mesh("Editor/assets/meshes/4_cubes.obj");
		// m_Mesh = Mesh("Editor/assets/meshes/dragon_80k.obj");
		// m_Mesh = Mesh("Editor/assets/meshes/dragon.fbx");
		m_MeshMaterial = CreateRef<Material>();
		m_MeshMaterial->SetShader(Shader::Create("Editor/assets/shaders/MeshShader.glsl"));

		m_NullObject = Texture2D::Create(std::string("Editor/assets/icons/null_icon.png"));
		m_MeshIcon = Texture2D::Create(std::string("Editor/assets/icons/mesh_icon.png"));
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
		// m_BVHTree.AABB = {min, max};
		// m_BVHTree.Triangles = m_Mesh.m_Triangles;
		// splitNodes(m_BVHTree, 0);


		// BVHV2Generate();


		//TODO: fix the aab generation
		// generateBVH();

		RNDR_CORE_WARN("BVH New: {0}", bvhnew);


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

		entity.AddComponent<ChildComponent>();
		entity.AddComponent<ParentComponent>();
		return entity;
	}


	Entity Scene::GetEntityWithID(UUID uuid)
	{
		auto view = m_Registry.view<IDComponent>();
		for (auto entity : view)
		{
			auto& id = view.get<IDComponent>(entity);
			if (id.ID == uuid)
				return { entity, this };
		}
		return {};
	}

	// Entity Scene::Get
	
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	// void Scene::RenderBLAS()
	// {
	// 	// int BLASCount = m_BLASes.size();
	// 	// RNDR_CORE_INFO("BLAS Count: {0}", BLASCount);
	// 	// LineRenderer::BeginScene(camera, m_LineMaterial, 1.0f);
	// 	for (int i = 0; i < m_BLASes.size(); i++)
	// 	{
	// 		glm::vec3 min = m_BLASes[i].m_Transform * glm::vec4(m_BVHBuffer[m_BLASes[i].m_IndexOffset].Min, 1.0f);
	// 		glm::vec3 max = m_BLASes[i].m_Transform * glm::vec4(m_BVHBuffer[m_BLASes[i].m_IndexOffset].Max, 1.0f);
	// 		glm::vec3 originalMin = m_BVHBuffer[m_BLASes[i].m_IndexOffset].Min;
	// 		glm::vec3 originalMax = m_BVHBuffer[m_BLASes[i].m_IndexOffset].Max;
	// 		min = glm::min(min, originalMin);
	// 		max = glm::max(max, originalMax);

	// 		// Mesh::bvhNode node = m_BVHBuffer[m_BLASes[i].m_IndexOffset];
	// 		// glm::vec3 min = node.Min 
	// 		// glm::vec3 max = node.Max;
	// 		LineRenderer::DrawAABB(min, max, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

	// 	}
	// 	// LineRenderer::EndScene();
	// }
	void Scene::RenderBLAS()
	{
		for (int i = 0; i < m_BLASes.size(); i++)
		{
			const auto& node = m_BVHBuffer[m_BLASes[i].m_IndexOffset];

			// Get original min and max points
			glm::vec3 min = node.Min;
			glm::vec3 max = node.Max;

			// Define the 8 corners of the AABB
			std::vector<glm::vec3> corners = {
				{min.x, min.y, min.z},
				{max.x, min.y, min.z},
				{min.x, max.y, min.z},
				{max.x, max.y, min.z},
				{min.x, min.y, max.z},
				{max.x, min.y, max.z},
				{min.x, max.y, max.z},
				{max.x, max.y, max.z},
			};

			// Initialize new min and max for transformed AABB
			glm::vec3 transformedMin = glm::vec3(std::numeric_limits<float>::infinity());
			glm::vec3 transformedMax = glm::vec3(-std::numeric_limits<float>::infinity());

			// Transform each corner and update the new min and max
			for (auto& corner : corners)
			{
				//? With a rotated object, the AABB will be larger than the object itself,
				//? if you want a perfect with need to loop over all triangles with the current transform
				glm::vec3 transformedCorner = glm::vec3(m_BLASes[i].m_Transform * glm::vec4(corner, 1.0f));
				transformedMin = glm::min(transformedMin, transformedCorner);
				transformedMax = glm::max(transformedMax, transformedCorner);
			}

			// Draw the updated AABB
			LineRenderer::DrawAABB(transformedMin, transformedMax, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		}
	}

	void Scene::RenderTLAS()
	{
		glm::vec3 min = m_TLAS[0].Min;
		glm::vec3 max = m_TLAS[0].Max;

		LineRenderer::DrawAABB(min, max, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	}

	void Scene::GenerateTLAS()
	{
		m_TLAS.clear();

		int BLASCount = m_BLASes.size();
		// auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
		glm::vec3 min = glm::vec3(std::numeric_limits<float>::infinity());
		glm::vec3 max = glm::vec3(-std::numeric_limits<float>::infinity());
		for (int i = 0; i < m_BLASes.size(); i++)
		{
			glm::vec4 tempMin = m_BLASes[i].m_Transform * glm::vec4(m_BVHBuffer[m_BLASes[i].m_IndexOffset].Min, 1.0f);
			glm::vec4 tempMax = m_BLASes[i].m_Transform * glm::vec4(m_BVHBuffer[m_BLASes[i].m_IndexOffset].Max, 1.0f);
			glm::vec3 blasMin = glm::vec3(tempMin);
			glm::vec3 blasMax = glm::vec3(tempMax);
			// TLAS tlas;
			// tlas.m_Transform = m_BLASes[i].m_Transform;
			// min = glm::min(min, m_BLASes[i].m_Transform * glm::vec4(m_BVHBuffer[m_BLASes[i].m_IndexOffset].Min, 1.0f));
			// max = glm::max(max, m_BLASes[i].m_Transform * glm::vec4(m_BVHBuffer[m_BLASes[i].m_IndexOffset].Max, 1.0f));
			min = glm::min(min, blasMin);
			max = glm::max(max, blasMax);
		}
		TLAS tlas;
		tlas.Min = min;
		tlas.Max = max;
		m_TLAS.push_back(tlas);
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		// populate the bvhbuffer //TODO dont do this every frame
		{
			m_BVHBuffer.clear();
			m_BLASes.clear();
			m_Triangles.clear();
			auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
				// auto node = mesh.Mesh->m_BVH[0];
				// Mesh::bvhNode node = mesh.Mesh->m_BVH[0];

				auto nodes = mesh.Mesh->m_BVH;
				BLAS blas;
				blas.m_IndexOffset = m_BVHBuffer.size();
				blas.TriangleOffset = m_Triangles.size();
				blas.TriangleCount = mesh.Mesh->m_Triangles.size();
				blas.m_Transform = transform.GetWorldTransform();
				m_BLASes.push_back(blas);

				m_Triangles.reserve(m_Triangles.size() + mesh.Mesh->m_Triangles.size());
				m_Triangles.insert(m_Triangles.end(), mesh.Mesh->m_Triangles.begin(), mesh.Mesh->m_Triangles.end());
				// for (int i = 0; i < mesh.Mesh->m_Triangles.size(); i++)
				// {
					// m_Triangles.push_back(mesh.Mesh->m_Triangles[i]);
				// }
				m_BVHBuffer.reserve(m_BVHBuffer.size() + nodes.size());
				m_BVHBuffer.insert(m_BVHBuffer.end(), nodes.begin(), nodes.end());
				// for (int i = 0; i < nodes.size(); i++)
				// {
				// 	m_BVHBuffer.push_back(nodes[i]);
				// }
			}
		}

		// generate TLAS
		GenerateTLAS();




		LineRenderer::BeginScene(camera, m_LineMaterial, 1.0f);

		RenderBLAS();
		// RenderTLAS();

		// drawBVH( m_BVHTree.ChildA.get());
		// drawBVH( m_BVHTree.ChildB.get());

		// LineRenderer::DrawAABB(m_BVH[0].Min, m_BVH[0].Max, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
		if (m_EnableBVH)
		{
			// drawBVH(m_BVH[0], 0, m_BVHDepth);


			// drawBVH(&m_BVHTree, 0, m_BVHDepth);
			// drawBVHV2(m_BVHNodes[0], 0, m_BVHDepth);
			// drawBVHV2temp(m_TempBVH[0], 0, m_BVHDepth);






			// auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
			// for (auto entity : group)
			// {
			// 	auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
			// 	// auto node = mesh.Mesh->m_BVH[0];
			// 	// Mesh::bvhNode node = mesh.Mesh->m_BVH[0];
			// 	auto nodes = mesh.Mesh->m_BVH;
			// 	drawBVHnew(nodes, 0, 0, m_BVHDepth);
			// 	// exit(0);
			// }
		}
		// drawBVHV2(m_BVHNodes[m_BVHNodes[0].ChildIndex], m_BVHDepth);
		// drawBVHV2(m_BVHNodes[m_BVHNodes[0].ChildIndex + 1], m_BVHDepth);
		
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
					// glm::mat4 transform = ent.GetComponent<TransformComponent>().GetTransform();
					glm::mat4 transform = ent.GetComponent<TransformComponent>().GetWorldTransform();
					CubeComponent cube = ent.GetComponent<CubeComponent>();
					DefaultMaterialComponent material = ent.GetComponent<DefaultMaterialComponent>();
					Renderer2D::DrawCube(transform, cube, material, (int)entity);
				}
			}
			Renderer2D::EndScene();
		}

		// //? Mesh
		// {
		// 	auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
		// 	for (auto entity : group)
		// 	{
		// 		auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		// 		if (mesh.Material)
		// 		{
		// 			mesh.Material->Bind();
		// 			mesh.Material->GetShader()->SetMat4("u_ViewProjection", camera.GetViewProjection());
		// 			mesh.Material->GetShader()->SetMat4("u_Transform", transform.GetTransform());
		// 			RenderCommand::DrawIndexed(mesh.Mesh->m_VertexArray, mesh.Mesh->m_IndexBuffer->GetCount());
		// 		}
		// 	}
		// }


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



		#if 0
		//? Draw Mesh
		m_MeshMaterial->Bind();
		m_MeshMaterial->GetShader()->SetMat4("u_ViewProjection", camera.GetViewProjection());
		constexpr glm::mat4 transform  = glm::mat4(1.0f);
		m_MeshMaterial->GetShader()->SetMat4("u_Transform", transform);
		// RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.VertexIndexCount);
		// m_Mesh.m_SubMeshes[0].BaseIndex;

		//TODO: this is temp fix this
		// RenderCommand::DrawIndexed(m_Mesh.m_VertexArray, m_Mesh.m_SubMeshes[0].IndexCount, m_Mesh.m_SubMeshes[0].BaseIndex);

		for (int i = 0; i < m_Mesh.m_SubMeshes.size(); i++)
		{
			RenderCommand::DrawIndexed(
				m_Mesh.m_VertexArray, 
				m_Mesh.m_SubMeshes[i].IndexCount, 
				m_Mesh.m_SubMeshes[i].BaseIndex,
				m_Mesh.m_SubMeshes[i].BaseVertex);
		}
		#endif

		//? Mesh
		{
			auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
				if (mesh.Material)
				{
					mesh.Material->Bind();
					mesh.Material->GetShader()->SetMat4("u_ViewProjection", camera.GetViewProjection());
					mesh.Material->GetShader()->SetMat4("u_Transform", transform.GetWorldTransform());
					// RenderCommand::DrawIndexed(mesh.Mesh->m_VertexArray, mesh.Mesh->m_IndexBuffer->GetCount());
					for (int i = 0; i < mesh.Mesh->m_SubMeshes.size(); i++)
					{
						RenderCommand::DrawIndexed(
							mesh.Mesh->m_VertexArray, 
							mesh.Mesh->m_SubMeshes[i].IndexCount, 
							mesh.Mesh->m_SubMeshes[i].BaseIndex,
							mesh.Mesh->m_SubMeshes[i].BaseVertex);
					}
				}
			}
		}





		
		// RenderCommand::DrawIndexed(
		// 	m_Mesh.m_VertexArray, 
		// 	m_Mesh.m_SubMeshes[1].IndexCount, 
		// 	m_Mesh.m_SubMeshes[1].BaseIndex,
		// 	m_Mesh.m_SubMeshes[1].BaseVertex);

		// m_Mesh.m_VertexArray->Bind();
		// glDrawElementsBaseVertex(GL_TRIANGLES, 
			// m_Mesh.m_SubMeshes[1].IndexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * m_Mesh.m_SubMeshes[1].BaseIndex), m_Mesh.m_SubMeshes[1].BaseVertex);
		
		
		// glDrawElements(GL_TRIANGLES, m_Mesh.m_SubMeshes[0].IndexCount, GL_UNSIGNED_INT, nullptr);



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
	template void Scene::OnComponentAdded<NullComponent>(Entity entity, NullComponent& component);

	template void Scene::OnComponentAdded<ChildComponent>(Entity entity, ChildComponent& component);
	template void Scene::OnComponentAdded<ParentComponent>(Entity entity, ParentComponent& component);







}







