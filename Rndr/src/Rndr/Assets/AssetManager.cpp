


#include "AssetManager.h"


#include "Rndr/Core/Log.h"

#include "AssetImporter.h"


namespace Rndr
{
	AssetManager::AssetManager()
	{
		//todo: load asset registry from file if it exists
	}

	bool AssetManager::IsAssetHandleValid(AssetHandle handle) const
	{
		return handle != 0 && m_AssetRegistry.find(handle) != m_AssetRegistry.end();
	}

	bool AssetManager::IsAssetLoaded(AssetHandle handle) const
	{
		return m_LoadedAssets.find(handle) != m_LoadedAssets.end();
	}

	AssetType AssetManager::GetAssetType(AssetHandle handle) const
	{
		if (!IsAssetHandleValid(handle))
			return AssetType::Unknown;

		return m_AssetRegistry.at(handle).Type;
	}

	const AssetMetadata& AssetManager::GetAssetMetadata(AssetHandle handle) const
	{
		static AssetMetadata s_NullMetadata;
		auto it = m_AssetRegistry.find(handle);
		if (it == m_AssetRegistry.end())
			return s_NullMetadata;

		return it->second;
	}

	Ref<Asset> AssetManager::GetAsset(AssetHandle handle)
	{
		if (!IsAssetHandleValid(handle))
			return nullptr;

		Ref<Asset> asset;
		if (IsAssetLoaded(handle))
		{
			asset = m_LoadedAssets.at(handle);
		}
		else
		{
			const AssetMetadata& metadata = GetAssetMetadata(handle);
			asset = AssetImporter::ImportAsset(handle, metadata);
			if (!asset)
			{
				RNDR_CORE_ERROR("Failed to load asset '{0}'", metadata.FilePath.string().c_str());
				return nullptr;
			}
			m_LoadedAssets[handle] = asset;
		}
		return asset;
	}


	const AssetMetadata& AssetManager::ViewAsset(AssetHandle handle)
	{
		return GetAssetMetadata(handle);
	}


	

	static std::map<std::filesystem::path, AssetType> s_AssetExtensionMap = {
		{".png", AssetType::Texture},
		{".jpg", AssetType::Texture},
		{".jpeg", AssetType::Texture},


		{".glsl", AssetType::Shader},
	};

	static AssetType GetAssetTypeFromFile(const std::filesystem::path& extension)
	{

		if (s_AssetExtensionMap.find(extension) == s_AssetExtensionMap.end())
		{
			RNDR_CORE_WARN("Unsuported file extension '{0}'", extension.string().c_str());
			return AssetType::Unknown;
		}

		return s_AssetExtensionMap.at(extension);
	}

	AssetHandle AssetManager::ImportAsset(const std::filesystem::path& filepath)
	{
		AssetHandle handle; // uuid
		AssetMetadata metadata;
		metadata.FilePath = filepath;
		metadata.Type = GetAssetTypeFromFile(filepath.extension());
		if (metadata.Type == AssetType::Unknown)
		{
			RNDR_CORE_WARN("Unknown asset type for file '{0}'", filepath.string().c_str());
			return 0;
		}
		// Ref<Asset> asset;
		// asset->handle = handle;
		m_AssetRegistry[handle] = metadata;
		// m_LoadedAssets[handle] = asset;
		//TODO: save asset registry to file
		// exit(0);
		return handle;

	}
} // namespace Rndr