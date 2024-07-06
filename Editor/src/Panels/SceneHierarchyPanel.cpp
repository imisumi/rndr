// #include "SceneHierarchyPanel.h"

// #include <vector>

// #include "Rndr/Scene/Components.h"

// #include <glm/gtc/type_ptr.hpp>


// #include "imgui/imgui_internal.h"

// #include "Rndr/Core/Input.h"

// // #include <cmath>
// // #include <cstddef>
// // #include <functional>
// // #include <iterator>
// // #include <limits>
// // #include <memory>
// // #include <tuple>
// // #include <type_traits>
// // #include <utility>
// // #include <vector>

// namespace Rndr
// {
// 	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
// 	{
// 		SetContext(scene);
// 	}

// 	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
// 	{
// 		m_Context = scene;
// 		m_SelectionContext = { entt::null, nullptr };
// 	}

// 	void SceneHierarchyPanel::OnImGuiRender()
// 	{

// 		ImGui::Begin("Objects");

// 		auto view = m_Context->m_Registry.view<TagComponent>();
// 		for (auto entity : view)
// 		{
// 			Entity ent = { entity, m_Context.get() };
// 			DrawEntityNode(ent);
// 		}

// 		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
// 		{
// 			m_SelectionContext = { entt::null, nullptr };
// 		}


// 		// Right-click on the scene hierarchy panel
// 		ImGuiPopupFlags popup_flags = ImGuiPopupFlags_MouseButtonRight | 
// 			ImGuiPopupFlags_NoOpenOverExistingPopup | ImGuiPopupFlags_NoOpenOverItems;
// 		if (ImGui::BeginPopupContextWindow(0, popup_flags))
// 		{
// 			if (ImGui::MenuItem("Create Empty Entity"))
// 			{
// 				m_Context->CreateEntity("Empty Entity");
// 			}

// 			ImGui::EndPopup();
// 		}


// 		ImGui::End();

// 		ImGui::Begin("Attributes");
// 		if (m_SelectionContext)
// 		{
// 			DrawComponents(m_SelectionContext);

// 			// if (ImGui::Button("Add Component"))
// 			// {
// 			// 	ImGui::OpenPopup("AddComponent");
// 			// }

// 			// if (ImGui::BeginPopup("AddComponent"))
// 			// {
// 			// 	if (ImGui::MenuItem("Quad"))
// 			// 	{
// 			// 		m_SelectionContext.AddComponent<QuadComponent>();
// 			// 		ImGui::CloseCurrentPopup();
// 			// 	}

// 			// 	if (ImGui::MenuItem("Camera"))
// 			// 	{
// 			// 		m_SelectionContext.AddComponent<CameraComponent>();
// 			// 		ImGui::CloseCurrentPopup();
// 			// 	}

// 			// 	ImGui::EndPopup();
// 			// }
// 		}
// 		ImGui::End();
// 	}

// 	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
// 	{
// 		auto& tag = entity.GetComponent<TagComponent>().Tag;
// 		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | 
// 			ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
// 		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
// 		if (ImGui::IsItemClicked())
// 		{
// 			m_SelectionContext = entity;
// 		}

// 		bool entityDeleted = false;
// 		if (ImGui::BeginPopupContextItem())
// 		{
// 			if (ImGui::MenuItem("Delete Entity"))
// 			{
// 				entityDeleted = true;
// 			}

// 			ImGui::EndPopup();
// 		}

// 		if (opened)
// 		{
// 			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
// 			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
// 			if (opened)
// 			{
// 				ImGui::TreePop();
// 			}
// 			ImGui::TreePop();
// 		}

// 		if (entityDeleted)
// 		{
// 			m_Context->m_Registry.destroy(entity);
// 			if (m_SelectionContext == entity)
// 			{
// 				m_SelectionContext = { entt::null, nullptr };
// 			}
// 		}
// 	}

// 	static void DrawVec3Control(const std::string& label, glm::vec3& value, float resetValue = 0.0f, float columnWidth = 100.0f)
// 	{
// 		ImGuiIO& io = ImGui::GetIO();
// 		auto boldFont = io.Fonts->Fonts[0];

// 		ImGui::PushID(label.c_str());

// 		ImGui::Columns(2);
// 		ImGui::SetColumnWidth(0, columnWidth);
// 		ImGui::Text(label.c_str());

// 		ImGui::NextColumn();
// 		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
// 		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

// 		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
// 		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

// 		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
// 		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
// 		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
// 		ImGui::PushFont(boldFont);
// 		if (ImGui::Button("X", buttonSize) && Input::IsKeyPressed(RNDR_KEY_LEFT_SHIFT))
// 			value.x = resetValue;
// 		ImGui::PopStyleColor(3);
// 		ImGui::PopFont();

// 		ImGui::SameLine();
// 		ImGui::DragFloat("##X", &value.x, 0.1f);
// 		ImGui::PopItemWidth();
// 		ImGui::SameLine();

// 		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
// 		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.35f, 1.0f));
// 		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.3f, 1.0f));
// 		ImGui::PushFont(boldFont);
// 		if (ImGui::Button("Y", buttonSize))
// 			value.y = resetValue;
// 		ImGui::PopStyleColor(3);
// 		ImGui::PopFont();

// 		ImGui::SameLine();
// 		ImGui::DragFloat("##Y", &value.y, 0.1f);
// 		ImGui::PopItemWidth();
// 		ImGui::SameLine();

// 		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.2f, 0.8f, 1.0f));
// 		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.3f, 0.9f, 1.0f));
// 		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.2f, 0.8f, 1.0f));
// 		ImGui::PushFont(boldFont);
// 		if (ImGui::Button("Z", buttonSize))
// 			value.z = resetValue;
// 		ImGui::PopStyleColor(3);
// 		ImGui::PopFont();

// 		ImGui::SameLine();
// 		ImGui::DragFloat("##Z", &value.z, 0.1f);
// 		ImGui::PopItemWidth();




// 		// if (ImGui::Button("X", buttonSize))
// 		// 	value.x = resetValue;

// 		// ImGui::SameLine();
// 		// ImGui::DragFloat("##X", &value.x, 0.1f);
// 		// ImGui::PopItemWidth();
// 		// ImGui::SameLine();

// 		// if (ImGui::Button("Y", buttonSize))
// 		// 	value.y = resetValue;

// 		// ImGui::DragFloat("##Y", &value.y, 0.1f);
// 		// ImGui::PopItemWidth();
// 		// ImGui::SameLine();

// 		// if (ImGui::Button("Z", buttonSize))
// 		// 	value.z = resetValue;
		
// 		// ImGui::DragFloat("##Z", &value.y, 0.1f);
// 		// ImGui::PopItemWidth();



// 		ImGui::PopStyleVar();

// 		ImGui::Columns(1);

// 		ImGui::PopID();
// 	}

// 	template<typename T, typename UIFunction>
// 	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
// 	{
// 		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | 
// 			ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth | 
// 			ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;
// 		if (entity.HasComponent<T>())
// 		{
// 			auto& component = entity.GetComponent<T>();

// 			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

// 			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
// 			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
// 			ImGui::Separator();
// 			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
// 			ImGui::PopStyleVar();
// 			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
// 			// ImGui::SameLine(ImGui::GetWindowWidth() - lineHeight * 0.5f);
// 			if (ImGui::Button("+", ImVec2(lineHeight, lineHeight)))
// 			{
// 				ImGui::OpenPopup("ComponentSettings");
// 			}

// 			bool removeComponent = false;
// 			if (ImGui::BeginPopup("ComponentSettings"))
// 			{
// 				if (ImGui::MenuItem("Remove Component"))
// 					removeComponent = true;
// 				ImGui::EndPopup();
// 			}

// 			if (open)
// 			{
// 				uiFunction(component);
// 				ImGui::TreePop();
// 			}

// 			if (removeComponent)
// 			{
// 				entity.RemoveComponent<T>();
// 			}
// 		}
// 	}

// 	void SceneHierarchyPanel::DrawComponents(Entity entity)
// 	{
// 		if (entity.HasComponent<TagComponent>())
// 		{
// 			auto& tag = entity.GetComponent<TagComponent>().Tag;

// 			char buffer[128] = { 0 };
// 			strcpy_s(buffer, sizeof(buffer), tag.c_str());

// 			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
// 			{
// 				tag = std::string(buffer);
// 			}
// 		}

// 		ImGui::SameLine();
// 		ImGui::PushItemWidth(-1);

// 		if (ImGui::Button("Add Component"))
// 			ImGui::OpenPopup("AddComponent");

// 		if (ImGui::BeginPopup("AddComponent"))
// 		{
// 			if (ImGui::MenuItem("Quad"))
// 			{
// 				m_SelectionContext.AddComponent<QuadComponent>();
// 				ImGui::CloseCurrentPopup();
// 			}

// 			if (ImGui::MenuItem("Camera"))
// 			{
// 				m_SelectionContext.AddComponent<CameraComponent>();
// 				ImGui::CloseCurrentPopup();
// 			}

// 			if (ImGui::MenuItem("Cube"))
// 			{
// 				m_SelectionContext.AddComponent<CubeComponent>();
// 				ImGui::CloseCurrentPopup();
// 			}

// 			ImGui::EndPopup();
// 		}

// 		ImGui::PopItemWidth();



// 		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | 
// 			ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth | 
// 			ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

// 		if (entity.HasComponent<TransformComponent>())
// 		{
// 			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform"))
// 			{
// 				auto& tc = entity.GetComponent<TransformComponent>();

// 				DrawVec3Control("Translation", tc.Translation);
// 				glm::vec3 rotation = glm::degrees(tc.Rotation);
// 				DrawVec3Control("Rotation", rotation);
// 				tc.Rotation = glm::radians(rotation);
// 				DrawVec3Control("Scale", tc.Scale, 1.0f);

// 				// ImGui::DragFloat3("Position", glm::value_ptr(tc.Translation), 0.1f);
// 				ImGui::TreePop();
// 			}
// 		}


// 		DrawComponent<CubeComponent>("Cube", entity, [](auto& component)
// 		{
// 			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
// 		});

// 		DrawComponent<QuadComponent>("Quad", entity, [](auto& component)
// 		{
// 			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
// 		});

// 		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
// 		{
// 			// auto& cameraComponent = component;
// 			auto& camera = component.Camera;

// 			ImGui::Checkbox("Primary", &component.Primary);

// 			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
// 			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
// 			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
// 			{
// 				for (int i = 0; i < 2; i++)
// 				{
// 					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
// 					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
// 					{
// 						currentProjectionTypeString = projectionTypeStrings[i];
// 						camera.SetProjectionType((SceneCamera::ProjectionType)i);
// 					}

// 					if (isSelected)
// 						ImGui::SetItemDefaultFocus();
// 				}
// 				ImGui::EndCombo();
// 			}

// 			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
// 			{
// 				float perspectiveFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
// 				if (ImGui::DragFloat("FOV", &perspectiveFOV))
// 					camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveFOV));

// 				float perspectiveNear = camera.GetPerspectiveNearClip();
// 				if (ImGui::DragFloat("Near Clip", &perspectiveNear))
// 					camera.SetPerspectiveNearClip(perspectiveNear);

// 				float perspectiveFar = camera.GetPerspectiveFarClip();
// 				if (ImGui::DragFloat("Far Clip", &perspectiveFar))
// 					camera.SetPerspectiveFarClip(perspectiveFar);
// 			}
// 			else if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
// 			{
// 				float orthoSize = camera.GetOrthographicSize();
// 				if (ImGui::DragFloat("Size", &orthoSize))
// 					camera.SetOrthographicSize(orthoSize);

// 				float orthoNear = camera.GetOrthographicNearClip();
// 				if (ImGui::DragFloat("Near Clip", &orthoNear))
// 					camera.SetOrthographicNearClip(orthoNear);

// 				float orthoFar = camera.GetOrthographicFarClip();
// 				if (ImGui::DragFloat("Far Clip", &orthoFar))
// 					camera.SetOrthographicFarClip(orthoFar);
// 			}
// 		});

// 	}
// }













#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Rndr/Scene/Components.h"
#include <cstring>

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
		ImGui::Begin("Scene Hierarchy");

		auto view = m_Context->m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			Entity ent = { entity, m_Context.get() };
			DrawEntityNode(ent);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
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
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
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
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
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

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
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

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					if (!m_SelectionContext.HasComponent<QuadComponent>())
						m_SelectionContext.AddComponent<QuadComponent>();
					else
						RNDR_CORE_WARN("This entity already has the Sprite Renderer Component!");
					ImGui::CloseCurrentPopup();
				}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

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

		DrawComponent<QuadComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});

	}

}
