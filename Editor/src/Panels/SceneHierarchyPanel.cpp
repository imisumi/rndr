

#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rndr/Scene/Components.h"
#include <cstring>


#include "Rndr/Renderer/Texture.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */

namespace Rndr {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}



	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Objects");

		auto view = m_Context->m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			Entity ent = { entity, m_Context.get() };
			if (DrawEntityNode(ent) == false)
				break;
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = { entt::null, nullptr };

		// Right-click on blank space
		ImGuiPopupFlags popup_flags = ImGuiPopupFlags_MouseButtonRight | 
			ImGuiPopupFlags_NoOpenOverExistingPopup | ImGuiPopupFlags_NoOpenOverItems;
		if (ImGui::BeginPopupContextWindow(0, popup_flags))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context->CreateEntity("Empty Entity");
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Attributes");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();


		ImGui::Begin("object bvh info");

		if (m_SelectionContext && m_SelectionContext.HasComponent<MeshComponent>())
		{
			auto& mesh = m_SelectionContext.GetComponent<MeshComponent>();
			ImGui::Text("BVH info");
			int meshCount = mesh.Mesh->m_SubMeshes.size();
			ImGui::Text("Mesh count: %d", meshCount);
			int min  = 0;
			int max = meshCount - 1;
			static int selectedMesh = 0;
			ImGui::SliderInt("Mesh", &selectedMesh, min, max);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::BeginDragDropSource(UUID id, const std::string& tag)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("ENTITY_DRAG_DROP", &id, sizeof(UUID));
			ImGui::Text("%s", tag.c_str());
			ImGui::EndDragDropSource();
		}
	}
	bool SceneHierarchyPanel::BeginDragDropTarget(Entity entity)
	{
		if (ImGui::BeginDragDropTarget())
		{
			UUID id = entity.GetComponent<IDComponent>().ID;
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG_DROP"))
			{
				UUID droppedID = *(const UUID*)payload->Data;
				if (droppedID != id)
				{
					Entity droppedEntity = m_Context->GetEntityWithID(droppedID);
					UUID parentID = droppedEntity.GetComponent<ParentComponent>().Parent;
					if (parentID != 0)
					{
						Entity parent = m_Context->GetEntityWithID(parentID);
						parent.GetComponent<ChildComponent>().RemoveChild(droppedID);
					}
					entity.GetComponent<ChildComponent>().AddChild(droppedID);
					droppedEntity.GetComponent<ParentComponent>().Parent = id;
					return true;
				}
			}
			ImGui::EndDragDropTarget();
		}
		return false;
	}

	void SceneHierarchyPanel::RenderEntityIcon(Entity entity)
	{
		ImVec2 uv0 = ImVec2(0.0f, 1.0f);
		ImVec2 uv1 = ImVec2(1.0f, 0.0f);
		if (entity.HasComponent<NullComponent>())
		{
			uint64_t img = m_Context->m_NullObject->GetTextureID();
			ImVec2 cursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPosY(cursorPos.y + 1.0f);
			ImGui::Image((void*)img, ImVec2(16, 16), uv0, uv1);
			ImGui::SetCursorPosY(cursorPos.y);

		}
		else if (entity.HasComponent<MeshComponent>())
		{
			uint64_t img = m_Context->m_MeshIcon->GetTextureID();
			ImVec2 cursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPosY(cursorPos.y + 1.0f);
			ImGui::Image((void*)img, ImVec2(16, 16), uv0, uv1);
			ImGui::SetCursorPosY(cursorPos.y);
		}
		else
		{
			uint64_t img = m_Context->m_MeshIcon->GetTextureID();
			ImVec2 cursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPosY(cursorPos.y + 1.0f);
			ImGui::Image((void*)img, ImVec2(16, 16), uv0, uv1);
			ImGui::SetCursorPosY(cursorPos.y);
		}
	}

	void SceneHierarchyPanel::DrawChildEntityNode(Entity entity)
	{
		int childCount = entity.GetComponent<ChildComponent>().Children.size();
		bool exitLoop = false;
		if (childCount > 0)
		{
			// RNDR_CORE_INFO("Child count: {0}", childCount);
			for (int i = 0; i < childCount; i++)
			{
				UUID childID = entity.GetComponent<ChildComponent>().Children[i];
				Entity child = m_Context->GetEntityWithID(childID);
				auto& childTag = child.GetComponent<TagComponent>().Tag;

				ImGuiTreeNodeFlags childFlags = ((m_SelectionContext == child) ? ImGuiTreeNodeFlags_Selected : 0) | 
					ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
				uint64_t img = m_Context->m_NullObject->GetTextureID();
				// ImGui::Image((void*)img, ImVec2(16, 16));
				// bool opened = ImGui::TreeNodeEx((void*)(uint64_t)childID, childFlags, "%s", childTag.c_str());
				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)childID, childFlags, "");
				// if (opened)
				// {
				// 	ImGui::SameLine();
				// 	RenderEntityIcon(child);
				// }

				if (ImGui::IsItemClicked())
					m_SelectionContext = child;
				bool entityDeleted = false;
				// std::string id = std::to_string(childID);
				// const char* idc = id.c_str();
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete Entity"))
						entityDeleted = true;
					ImGui::EndPopup();
				}

				BeginDragDropSource(childID, childTag);
				if (BeginDragDropTarget(child))
					exitLoop = true;
				
				ImGui::SameLine();
				RenderEntityIcon(child);
				ImGui::SameLine();
				ImGui::Text("%s", childTag.c_str());
				// ImGui::Text("Hello");
				




				if (opened)
				{
					DrawChildEntityNode(child);
					ImGui::TreePop();
				}

				if (entityDeleted)
				{
					DeleteEntityNode(child);
					break ;
				}
				if (exitLoop)
					break;
			}
		}
	}

	bool SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		UUID parentID = entity.GetComponent<ParentComponent>().Parent;

		if (parentID != 0) // only render root entities
			return true;

		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | 
			ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		UUID entID = entity.GetComponent<IDComponent>().ID;
		// bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entID, flags, "%s", tag.c_str());
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entID, flags, "");
		// ImGui::SameLine();
		// ImGui::Text("Hello");

		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		bool entityDeleted = false;
		// const char* idc = std::to_string(entID).c_str();
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
			ImGui::EndPopup();
		}


		BeginDragDropSource(entID, tag);
		BeginDragDropTarget(entity);


		ImGui::SameLine();
		RenderEntityIcon(entity);
		ImGui::SameLine();
		// ImGui::Text("Hello");
		ImGui::Text("%s", tag.c_str());



		if (opened)
		{
			DrawChildEntityNode(entity);
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			DeleteEntityNode(entity);
			return false;
		}
		return true;
	}

	void SceneHierarchyPanel::ReccursiveDeleteEntityNode(Entity entity)
	{
		int childCount = entity.GetComponent<ChildComponent>().Children.size();
		if (childCount > 0)
		{
			for (int i = 0; i < childCount; i++)
			{
				UUID childID = entity.GetComponent<ChildComponent>().Children[i];
				Entity child = m_Context->GetEntityWithID(childID);
				ReccursiveDeleteEntityNode(child);
				m_Context->DestroyEntity(child);
			}
		}
		// m_Context->DestroyEntity(entity);
	}

	void SceneHierarchyPanel::DeleteEntityNode(Entity entity)
	{
		UUID parentID = entity.GetComponent<ParentComponent>().Parent;
		if (parentID != 0)
		{
			Entity parent = m_Context->GetEntityWithID(parentID);
			parent.GetComponent<ChildComponent>().RemoveChild(entity.GetComponent<IDComponent>().ID);
		}
		ReccursiveDeleteEntityNode(entity);
		m_Context->DestroyEntity(entity);
		// if (m_SelectionContext == entity)
			m_SelectionContext = { entt::null, nullptr };
	}

	static void DrawFloatControll(const std::string& label, float& value, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("%s",label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			value = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		// ImGui::SameLine();

	

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		bool valueChanged = false;
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
			valueChanged = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f"))
			valueChanged = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
			valueChanged = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f"))
			valueChanged = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
			valueChanged = true;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f"))
			valueChanged = true;
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		// ImGui::Separator();
		return valueChanged;
	}
	
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			// ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
			ImGui::PopStyleVar(
			);
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::ReccursiveUpdateWorldTransform(Entity entity)
	{
		int childCount = entity.GetComponent<ChildComponent>().Children.size();
		UUID parentID = entity.GetComponent<ParentComponent>().Parent;
		Entity parent = m_Context->GetEntityWithID(parentID);
		if (childCount > 0)
		{
			for (int i = 0; i < childCount; i++)
			{
				auto& parentTC = parent.GetComponent<TransformComponent>();
				auto& tc = entity.GetComponent<TransformComponent>();
				tc.WorldTranslation = parentTC.WorldTranslation + tc.Translation;
				tc.WorldRotation = parentTC.WorldRotation + tc.Rotation;
				tc.WorldScale = parentTC.WorldScale * tc.Scale;

				UUID childID = entity.GetComponent<ChildComponent>().Children[i];
				Entity child = m_Context->GetEntityWithID(childID);
				ReccursiveUpdateWorldTransform(child);
			}
		}
		// else
		// {
			
		// }
	}


	//TODO: fix propper scale/rotaion around parents axis
	void SceneHierarchyPanel::UpdateWorldTransform(Entity entity)
	{
		auto& tc = entity.GetComponent<TransformComponent>();
		UUID parentID = entity.GetComponent<ParentComponent>().Parent;
		if (parentID == 0)
		{
			tc.WorldTranslation = tc.Translation;
			tc.WorldRotation = tc.Rotation;
			tc.WorldScale = tc.Scale;
		}
		else
		{
			Entity parent = m_Context->GetEntityWithID(parentID);
			auto& parentTC = parent.GetComponent<TransformComponent>();
			// tc.WorldTranslation = parentTC.WorldTranslation + tc.Translation;
			// tc.WorldRotation = parentTC.WorldRotation + tc.Rotation;
			// tc.WorldScale = parentTC.WorldScale * tc.Scale;

			glm::mat4 worldTransform = parentTC.GetWorldTransform();
			// scale
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), tc.Scale);
			// rotation
			glm::mat4 rotation = glm::toMat4(glm::quat(tc.Rotation));
			// translation
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), tc.Translation);

			worldTransform = worldTransform * translation * rotation * scale;

			tc.WorldTranslation = glm::vec3(worldTransform[3]);
			tc.WorldRotation = glm::eulerAngles(glm::quat(worldTransform));
			tc.WorldScale = glm::vec3(glm::length(glm::vec3(worldTransform[0])), glm::length(glm::vec3(worldTransform[1])), glm::length(glm::vec3(worldTransform[2])));
			

			// scale * rotation * translation
			// first scale around the parent origin
			// glm::mat4 parentTransform = parentTC.GetTransform();
			// glm::mat4 localTransform = tc.GetTransform();

			// glm::mat4 worldTransform = parentTransform * localTransform;


			// rotate tc.WorldRotation around the parent origin
			// glm::vec3 worldPosition = parentTC.WorldTranslation;
			


			// then rotate around the parent origin
			// then translate to the parent origin


			// glm::rota
		}
		int childCount = entity.GetComponent<ChildComponent>().Children.size();
		if (childCount > 0)
		{
			for (int i = 0; i < childCount; i++)
			{
				UUID childID = entity.GetComponent<ChildComponent>().Children[i];
				Entity child = m_Context->GetEntityWithID(childID);
				// ReccursiveUpdateWorldTransform(child);
				UpdateWorldTransform(child);
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		// return ;
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
				if (ImGui::MenuItem("Camera"))
				{
					if (!m_SelectionContext.HasComponent<CameraComponent>())
						m_SelectionContext.AddComponent<CameraComponent>();
					else
						RNDR_CORE_WARN("This entity already has the Camera Component!");
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Quad"))
				{
					if (!m_SelectionContext.HasComponent<QuadComponent>())
						m_SelectionContext.AddComponent<QuadComponent>();
					else
						RNDR_CORE_WARN("This entity already has the Quad Component!");
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Cube"))
				{
					if (!m_SelectionContext.HasComponent<CubeComponent>())
						m_SelectionContext.AddComponent<CubeComponent>();
					else
						RNDR_CORE_WARN("This entity already has the Cube Component!");
					ImGui::CloseCurrentPopup();
				}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		if (entity.HasComponent<IDComponent>())
		{
			auto& uuid = entity.GetComponent<IDComponent>().ID;
			// convert uint64_t to char array
			// std::string str = std::to_string(uuid);
			int childCount = entity.GetComponent<ChildComponent>().Children.size();
			uint64_t parentID = entity.GetComponent<ParentComponent>().Parent;
			ImGui::Text("UUID: %llu", uuid);
			ImGui::Text("Parent ID: %llu", parentID);
			ImGui::Text("Child count: %d", childCount);
		}

		// DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		// {
		// 	DrawVec3Control("Translation", component.Translation);
		// 	glm::vec3 rotation = glm::degrees(component.Rotation);
		// 	DrawVec3Control("Rotation", rotation);
		// 	component.Rotation = glm::radians(rotation);
		// 	DrawVec3Control("Scale", component.Scale, 1.0f);
		// });

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | 
				ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth | 
				ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform"))
			{
				auto& tc = entity.GetComponent<TransformComponent>();

				bool transformChanged = false;
				if (DrawVec3Control("Translation", tc.Translation))
					transformChanged = true;
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				if (DrawVec3Control("Rotation", rotation))
				{
					tc.Rotation = glm::radians(rotation);
					transformChanged = true;
				}
				if (DrawVec3Control("Scale", tc.Scale, 1.0f))
					transformChanged = true;

				if (transformChanged)
				{
				}
				//TODO: also update when moved true gizmo
				UpdateWorldTransform(entity);

				ImGui::Dummy(ImVec2(0.0f, 0.0f));

				// ImGui::DragFloat3("Position", glm::value_ptr(tc.Translation), 0.1f);
				ImGui::TreePop();
			}


			//TODO: remove, this is for testing
			if (ImGui::TreeNodeEx((void*)(typeid(TransformComponent).hash_code() + 1), treeNodeFlags, "WorldTransform"))
			{
				auto& tc = entity.GetComponent<TransformComponent>();

				glm::vec3 originalTranslation = tc.WorldTranslation;
				DrawVec3Control("Translation", tc.WorldTranslation);
				tc.WorldTranslation = originalTranslation;

				glm::vec3 originalRotation = tc.WorldRotation;
				glm::vec3 rotation = glm::degrees(tc.WorldRotation);
				DrawVec3Control("Rotation", rotation);
				tc.WorldRotation = originalRotation;

				glm::vec3 originalScale = tc.WorldScale;
				DrawVec3Control("Scale", tc.WorldScale, 1.0f);
				tc.WorldScale = originalScale;


				ImGui::Dummy(ImVec2(0.0f, 0.0f));

				// ImGui::DragFloat3("Position", glm::value_ptr(tc.Translation), 0.1f);
				ImGui::TreePop();
			}
		}

		// if (entity.HasComponent<CubeComponent>())
		// {
		// 	if (ImGui::TreeNodeEx((void*)typeid(CubeComponent).hash_code(), treeNodeFlags, "Object Properties"))
		// 	{
		// 		auto& cc = entity.GetComponent<CubeComponent>();
		// 		DrawVec3Control("Size", cc.Size, 1.0f);
		// 		ImGui::Dummy(ImVec2(0.0f, 0.0f));
		// 		ImGui::TreePop();
		// 	}
		// }

		if (entity.HasComponent<DefaultMaterialComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(DefaultMaterialComponent).hash_code(), treeNodeFlags, "Material"))
			{
				auto& cc = entity.GetComponent<DefaultMaterialComponent>();

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				std::strncpy(buffer, cc.Material->GetName().c_str(), sizeof(buffer));
				if (ImGui::InputText("##Material", buffer, sizeof(buffer)))
				{
					cc.Material->SetName(std::string(buffer));
					RNDR_CORE_INFO("Material name: {0}", cc.Material->GetName());
				}

				glm::vec4 color = cc.Material->GetColor();
				// if (ImGui::ColorEdit4("MyColor##3", glm::value_ptr(color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
				// 	cc.Material->SetColor(color);
				if (ImGui::ColorEdit4("MyColor##3", glm::value_ptr(color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
				{
					cc.Material->SetColor(color);
				}

				// ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
				// ImGui::Image((ImTextureID)cc.Material->GetTextureID(TextureType::Diffuse), ImVec2(100.0f, 100.0f));
				// ImGui::ImageButton((ImTextureID)cc.Material->GetTextureID(TextureType::Diffuse), ImVec2(100.0f, 100.0f));
				// image is upside down

				// ImGui::Image((ImTextureID)cc.Material->GetTextureID(TextureType::Diffuse), ImVec2(100.0f, 100.0f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 })
				if (ImGui::ImageButton((ImTextureID)(uint64_t)cc.Material->GetTextureID(TextureType::Diffuse), ImVec2(100.0f, 100.0f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
					RNDR_CORE_INFO("Texture clicked");
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;


						std::filesystem::path texturePath(path);
						RNDR_CORE_INFO("Texture path: {0}", texturePath.string());
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture)
							cc.Material->SetTexture(texture, TextureType::Diffuse);
					}

					ImGui::EndDragDropTarget();
				}
				//? Diffuse texture
				// ImGui::Image((ImTextureID)cc.Material->GetTextureID(TextureType::Diffuse), ImVec2(100.0f, 100.0f));

				//
				// ImGui::Image((void*)0, ImVec2(100.0f, 100.0f));


				// DrawVec3Control("Size", cc.Size, 1.0f);
				ImGui::Dummy(ImVec2(0.0f, 0.0f));
				ImGui::TreePop();
			}
		}

		DrawComponent<QuadComponent>("Quad", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});

		// DrawComponent<CubeComponent>("Cube", entity, [](auto& component)
		// {
		// 	ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		// });


		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
					camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);

				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}
		});

	}

}
