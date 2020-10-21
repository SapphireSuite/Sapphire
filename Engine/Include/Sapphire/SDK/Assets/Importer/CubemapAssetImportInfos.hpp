// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_CUBEMAP_ASSET_IMPORT_INFOS_GUARD
#define SAPPHIRE_SDK_CUBEMAP_ASSET_IMPORT_INFOS_GUARD

#include <string>

#include <SDK/Config.hpp>

namespace Sa
{
	struct CubemapAssetImportInfos
	{
		union
		{
			std::string pathes[6]{};

			struct
			{
				std::string right;
				std::string left;
				std::string top;
				std::string bottom;
				std::string front;
				std::string back;
			};
		};

		CubemapAssetImportInfos() = default;
		
		// Remove SA_ENGINE_API.
		SA_ENGINE_API CubemapAssetImportInfos(const std::string& _right, const std::string& _left,
			const std::string& _top, const std::string& _bottom,
			const std::string& _front, const std::string& _back) noexcept;

		SA_ENGINE_API ~CubemapAssetImportInfos();
	};
}

#endif // GUARD
