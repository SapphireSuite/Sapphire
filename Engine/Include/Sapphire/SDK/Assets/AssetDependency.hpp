// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_ASSET_DEPENDENCY_GUARD
#define SAPPHIRE_SDK_ASSET_DEPENDENCY_GUARD

#include <string>

#include <SDK/Assets/AssetType.hpp>

namespace Sa
{
	struct AssetPathDependency
	{
		std::string path;
		AssetType assetType = AssetType::Unknown;
	};

	struct AssetDependency
	{
		class IRenderPrimitive* primitive;
		AssetType assetType = AssetType::Unknown;
	};
}

#endif // GUARD
