#pragma once

#include "AssetMetadata.h"

#include "Rndr/Core/Base.h"

#include "Rndr/Renderer/Texture.h"

namespace Rndr {

	class AssetImporter
	{
	public:
		static Ref<Asset> ImportAsset(AssetHandle handle, const AssetMetadata& metadata);
	};


	class TextureImporter
	{
	public:
		// static Ref<Asset> Import(AssetHandle handle, const AssetMetadata& metadata);
		static Ref<Texture2D> ImportTexture2D(AssetHandle handle, const AssetMetadata& metadata);
		static Ref<Texture2D> LoadTexture2D(const std::filesystem::path& path);
	};

}
