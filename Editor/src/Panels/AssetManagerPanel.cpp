#include "AssetManagerPanel.h"

#include <imgui/imgui.h>
// #include <imgui/imgui_internal.h>


namespace Rndr
{
	const std::filesystem::path s_AssetPath = "Editor/assets";

	// const char* allowedDirectories[] = {
	// 	"dir1",
	// 	"dir2",
	// 	"dir3"
	// };
	std::unordered_set<std::string> allowedDirectories = {
		"textures",
		"meshes",
	};

	AssetManagerPanel::AssetManagerPanel()
		: m_CurrentDirectory(s_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create(std::string("Editor/assets/icons/folder.png"));
		m_FileIcon = Texture2D::Create(std::string("Editor/assets/icons/file.png"));
	}

	AssetManagerPanel::~AssetManagerPanel()
	{
	}

	void AssetManagerPanel::OnImGuiRender()
	{
		RNDR_CORE_ASSERT(m_AssetManager != nullptr, "Asset Manager is null!");
		// if (m_AssetManager == nullptr)
			// RNDR_CORE_ASSERT(false, "Asset Manager is null!");

		// RNDR_CORE_ASSERT(false, "Asset Manager Panel not implemented yet!");

		ImGui::Begin("Asset Manager");

		const AssetRegistry& assetRegistry = m_AssetManager->GetAssetRegistry();

		// handle == UUID
		for (const auto& [handle, metadata] : assetRegistry)
		{
			// RNDR_CORE_INFO("AssetManagerPanel::OnImGuiRender: {0}", metadata.FilePath.string().c_str());
			std::string assetName = metadata.FilePath.filename().string();
			ImGui::Text("%s", assetName.c_str());
		}



		// // TODO: maybe not do this every frame
		// if (!std::filesystem::exists(m_CurrentDirectory))
		// 	std::filesystem::create_directory(m_CurrentDirectory);

		// if (m_CurrentDirectory != s_AssetPath)
		// {
		// 	if (ImGui::Button("<-"))
		// 		m_CurrentDirectory = m_CurrentDirectory.parent_path();
		// }

		// static float s_Padding = 16.0f;
		// static float s_IconSize = 128.0f;
		// float cellSize = s_IconSize + s_Padding;

		// float panelWidth = ImGui::GetContentRegionAvail().x;
		// int columnCount = (int)(panelWidth / cellSize);
		// if (columnCount < 1)
		// 	columnCount = 1;

		// // ImTextureID
		// ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		// int itemIndex = 0;
		// for (const auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		// {
		// 	const auto& path = directoryEntry.path();
		// 	auto relativePath = std::filesystem::relative(path, s_AssetPath);

		// 	// Check if it's a directory and if it's allowed
		// 	if (directoryEntry.is_directory() && 
		// 		allowedDirectories.find(path.filename().string()) == allowedDirectories.end())
		// 	{
		// 		continue; // Skip directories not in the allowed list
		// 	}



		// 	Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;

		// 	// RNDR_CORE_INFO("ContentBrowserPanel::OnImGuiRender: {0}", path.string());

		// 	ImGui::BeginGroup();
		// 	if (ImGui::ImageButton(path.string().c_str(), (ImTextureID)(uint64_t)icon->GetTextureID(), { s_IconSize, s_IconSize }, { 0, 1 }, { 1, 0 }))
		// 	{
		// 		if (directoryEntry.is_directory())
		// 			m_CurrentDirectory /= path.filename();
		// 	}
		// 	// ImGui::Text("%s", relativePath.filename().string().c_str());
	

		// 	if (ImGui::BeginDragDropSource())
		// 	{
		// 		// const wchar_t* pathStr = path.c_str();
		// 		std::wstring wpath = path.wstring();
		// 		const wchar_t* pathStr = wpath.c_str();
		// 		ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", pathStr, (wcslen(pathStr) + 1) * sizeof(wchar_t));
		// 		ImGui::EndDragDropSource();
		// 	}

		// 	ImGui::TextWrapped("%s", relativePath.filename().string().c_str());

		// 	ImGui::EndGroup();

		// 	itemIndex++;
		// 	// Add same line if the current item is not the last in the row
		// 	if (itemIndex % columnCount != 0)
		// 		ImGui::SameLine();
		// }
		// ImGui::PopStyleColor();

		ImGui::End();
	}
}