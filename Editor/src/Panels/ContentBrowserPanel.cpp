#include "ContentBrowserPanel.h"

#include <imgui.h>
#include <imgui_internal.h>


#include "Rndr/Core/Log.h"

namespace Rndr
{
	// constexpr const char* s_AssetPath = "assets";
	// constexpr const char* s_AssetPath = "Editor/assets";
	const std::filesystem::path s_AssetPath = "Editor/assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create(std::string("Editor/assets/icons/folder.png"));
		m_FileIcon = Texture2D::Create(std::string("Editor/assets/icons/file.png"));
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
	}


	// void CombinedImageButton

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (!std::filesystem::exists(m_CurrentDirectory))
			std::filesystem::create_directory(m_CurrentDirectory);

		if (m_CurrentDirectory != s_AssetPath)
		{
			if (ImGui::Button("<-"))
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}

		static float s_Padding = 16.0f;
		static float s_IconSize = 128.0f;
		float cellSize = s_IconSize + s_Padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		// ImTextureID
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		int itemIndex = 0;
		for (const auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;

			// RNDR_CORE_INFO("ContentBrowserPanel::OnImGuiRender: {0}", path.string());

			ImGui::BeginGroup();
			if (ImGui::ImageButton(path.string().c_str(), (ImTextureID)icon->GetTextureID(), { s_IconSize, s_IconSize }, { 0, 1 }, { 1, 0 }))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
			}
			// ImGui::Text("%s", relativePath.filename().string().c_str());
	

			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* pathStr = path.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", pathStr, (wcslen(pathStr) + 1) * sizeof(wchar_t));
				// ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", relativePath.string().c_str(), relativePath.string().size() + 1);
				ImGui::EndDragDropSource();
			}

			ImGui::TextWrapped("%s", relativePath.filename().string().c_str());

			ImGui::EndGroup();

			itemIndex++;
			// Add same line if the current item is not the last in the row
			if (itemIndex % columnCount != 0)
				ImGui::SameLine();
		}
		ImGui::PopStyleColor();

		ImGui::End();
		// exit(1);
	}
}