

#pragma once

#include "Rndr/Core/UUID.h"

namespace Rndr
{
	using AssetHandle = UUID;

	enum struct AssetType
	{
		Unknown = 0,
		Shader,
		Texture,
		Mesh,
		ComputeShader,
		Scene,
		Material,
		Font,
		Count
	};

	// std::string_view AssetTypeToString(AssetType type);
	// AssetType AssetTypeFromString(std::string_view assetType);

	class Asset
	{
	public:
		AssetHandle handle;

		virtual AssetType GetType() const = 0;
	};
} // namespace Rndr



