#include "AssetImporter.h"

#include "Rndr/Core/Log.h"

#include <map>

namespace Rndr
{
	// using AssetImportFunction = std::function<Ref<Asset>(AssetHandle, const AssetMetadata&)>;

	// static std::map<AssetType, AssetImportFunction> s_AssetImportFunctions = {
	// 	{ AssetType::Texture, TextureImporter::ImportTexture2D },

	// };

	Ref<Asset> AssetImporter::ImportAsset(AssetHandle handle, const AssetMetadata& metadata)
	{
		Ref<Asset> asset;
		switch (metadata.Type)
		{
			case AssetType::Texture:     asset = std::static_pointer_cast<Asset>(TextureImporter::ImportTexture2D(handle, metadata)); break;

			// case AssetType::Texture:

			// default: RNDR_CORE_ERROR("Unknown asset type '{0}'", metadata.Type); return nullptr;
			default: RNDR_CORE_ERROR("Unknown asset type"); return nullptr;
		}
		return asset;


		// if (s_AssetImportFunctions.find(metadata.Type) == s_AssetImportFunctions.end())
		// {
		// 	RNDR_CORE_ERROR("No importer available for asset type: {}", (uint32_t)metadata.Type);
		// 	return nullptr;
		// }

		// return s_AssetImportFunctions.at(metadata.Type)(handle, metadata);
	}








	// Ref<Asset> TextureImporter::Import(AssetHandle handle, const AssetMetadata& metadata)
	// {
	// 	return nullptr;
	// };

	Ref<Texture2D> TextureImporter::ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata)
	{
		return LoadTexture2D(metadata.FilePath);
	}

	Ref<Texture2D> TextureImporter::LoadTexture2D(const std::filesystem::path& path)
	{
		Ref<Texture2D> texture = Texture2D::Create(path.string());
		return texture;
	}
}