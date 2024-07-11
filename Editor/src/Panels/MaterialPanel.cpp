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
		if (m_SelectedMaterial)
			RNDR_CORE_INFO("MaterialPanel::OnImGuiRender: {0}", m_SelectedMaterial->GetName());

		// ImGui::Begin("Material Properties");
		// ImGui::Text("testing");
		// ImGui::End();
	}
}