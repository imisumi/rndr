

#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Rndr/Scene/Components.h"
#include <cstring>

#include "Rndr/Renderer/Texture.h"

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
			DrawEntityNode(ent);
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
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		// auto& tag = entity.GetComponent<TagComponent>().Tag;
		
		// ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		// flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		// bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		// if (ImGui::IsItemClicked())
		// {
		// 	m_SelectionContext = entity;
		// }

		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | 
			ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());
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
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, "%s", tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = { entt::null, nullptr };
		}
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

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
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

		// ImGui::Separator();
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

				DrawVec3Control("Translation", tc.Translation);
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", tc.Scale, 1.0f);

				ImGui::Dummy(ImVec2(0.0f, 0.0f));

				// ImGui::DragFloat3("Position", glm::value_ptr(tc.Translation), 0.1f);
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CubeComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CubeComponent).hash_code(), treeNodeFlags, "Object Properties"))
			{
				auto& cc = entity.GetComponent<CubeComponent>();
				DrawVec3Control("Size", cc.Size, 1.0f);
				ImGui::Dummy(ImVec2(0.0f, 0.0f));
				ImGui::TreePop();
			}
		}

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
