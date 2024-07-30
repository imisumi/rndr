
#include "Rndr/Core/Base.h"
#include "Rndr/Core/UUID.h"

#include "Asset.h"

#include <map>
#include "AssetMetadata.h"

// #include "Sandbox3D.h"

namespace Rndr
{
	// using AssetRegistry = std::unordered_map<AssetHandle, Ref<Asset>>;
	// using AssetMap = std::unordered_map<AssetType, AssetRegistry>;

	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;
	using AssetMap = std::map<AssetHandle, Ref<Asset>>;

	class AssetManager
	{
	public:
		AssetManager();

		bool AssetManager::IsAssetHandleValid(AssetHandle handle) const;
		bool AssetManager::IsAssetLoaded(AssetHandle handle) const;
		AssetType AssetManager::GetAssetType(AssetHandle handle) const;
		const AssetMetadata& AssetManager::GetAssetMetadata(AssetHandle handle) const;


		// Ref<Asset> GetAsset(AssetHandle handle);
		Ref<Asset> GetAsset(AssetHandle handle);

		const AssetMetadata& ViewAsset(AssetHandle handle);


		// itterate over all assets
		const AssetRegistry& GetAssetRegistry() const { return m_AssetRegistry; }



		AssetHandle ImportAsset(const std::filesystem::path& filepath);


		template<typename T>
		Ref<T> GetAssetAs(AssetHandle handle)
		{
			Ref<Asset> asset = GetAsset(handle);
			return std::dynamic_pointer_cast<T>(asset);
		}

		// template<typename T>
		// static Ref<T> GetAsset(AssetHandle handle)
		// {
		// 	Ref<Asset> asset = Project::GetActive()->GetAssetManager()->GetAsset(handle);
		// 	return std::static_pointer_cast<T>(asset);
		// }

	private:
		// AssetRegistry m_Assets;

		AssetRegistry m_AssetRegistry;
		AssetMap m_LoadedAssets;
	};

}