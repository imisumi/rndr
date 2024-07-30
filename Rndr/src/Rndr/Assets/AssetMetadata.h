#pragma once

#include "Asset.h"

#include <filesystem>

namespace Rndr {

	struct AssetMetadata
	{
		AssetType Type = AssetType::Unknown;
		std::filesystem::path FilePath;

		operator bool() const { return Type != AssetType::Unknown; }
	};

}
