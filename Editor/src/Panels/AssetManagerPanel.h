#pragma once

#include <filesystem>


#include "Rndr/Scene/Scene.h"


#include "Rndr/Assets/AssetManager.h"

namespace Rndr
{
	class AssetManagerPanel
	{
	public:
		AssetManagerPanel();
		~AssetManagerPanel();

		void OnImGuiRender();

		void SetAssetManager(const Ref<AssetManager>& assetManager) { m_AssetManager = assetManager; }

	private:
		// Ref<Scene> m_Scene;
		Ref<AssetManager> m_AssetManager;

		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}