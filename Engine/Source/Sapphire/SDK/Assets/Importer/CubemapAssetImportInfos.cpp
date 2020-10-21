// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Importer/CubemapAssetImportInfos.hpp>

namespace Sa
{
	CubemapAssetImportInfos::CubemapAssetImportInfos(const std::string& _right, const std::string& _left,
		const std::string& _top, const std::string& _bottom,
		const std::string& _front, const std::string& _back) noexcept :
		right{ _right },
		left{ _left },
		top{ _top },
		bottom{ _bottom },
		front{ _front },
		back{ _back }
	{
	}

	CubemapAssetImportInfos::~CubemapAssetImportInfos()
	{
	}
}
