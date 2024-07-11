#include "MaterialPanel.h"

#include <imgui.h>

#include "Rndr/Core/Log.h"

namespace Rndr
{
	MaterialPanel::~MaterialPanel()
	{
	}

	void MaterialPanel::OnImGuiRender()
	{
		ImGui::Begin("Materials");

		// ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
		for (auto& material : *m_MaterialLibrary)
		{
			auto [map_name, mat] = material;
			std::string mat_name = mat->GetName();
			// ImGui::Text("%s", mat_name.c_str());
			// ImGui::Begin("Material Properties");
			// ImGui::Text("Name: %s", m_SelectedMaterial->GetName().c_str());
			// ImGui::End();
			if (ImGui::Button(mat_name.c_str()))
			{
				m_SelectedMaterial = mat;
			}
		}
		ImGui::End();

		ImGui::Begin("Material Properties");
		if (m_SelectedMaterial)
		{
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strcpy(buffer, m_SelectedMaterial->GetName().c_str());
			if (ImGui::InputText("Name", buffer, sizeof(buffer)))
				m_SelectedMaterial->SetName(std::string(buffer));

			// glm::vec4 color = m_SelectedMaterial->GetColor();
			// ImGui::SetNextItemWidth(64.0f);
			// if (ImGui::ColorEdit4("MyColor##3", glm::value_ptr(color), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
			// 	m_SelectedMaterial->SetColor(color);

			ImVec4 color = ImVec4(m_SelectedMaterial->GetColor().r, m_SelectedMaterial->GetColor().g, 
				m_SelectedMaterial->GetColor().b, m_SelectedMaterial->GetColor().a);
			if (ImGui::ColorButton("Albedo Color", color, ImGuiColorEditFlags_None, ImVec2{ 64.0f, 16.0f }))
				ImGui::OpenPopup("color-picker");
			if (ImGui::BeginPopup("color-picker"))
			{
				ImGui::ColorPicker4("##color-picker", &color.x, ImGuiColorEditFlags_None, NULL);
				ImGui::EndPopup();
				m_SelectedMaterial->SetColor(glm::vec4(color.x, color.y, color.z, color.w));
			}

			if (ImGui::ImageButton((ImTextureID)(uint64_t)m_SelectedMaterial->GetTextureID(TextureType::Diffuse), 
				ImVec2(64.0f, 64.0f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
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
						m_SelectedMaterial->SetTexture(texture, TextureType::Diffuse);
				}

				ImGui::EndDragDropTarget();
			}



			// RNDR_CORE_INFO("MaterialPanel::OnImGuiRender: {0}", m_SelectedMaterial->GetName());
		}

		ImGui::End();
		// ImGui::Begin("Material Properties");
		// ImGui::Text("testing");
		// ImGui::End();
	}
}