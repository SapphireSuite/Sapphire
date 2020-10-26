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
			std::string pathes[12]{};

			struct
			{
				std::string right;
				std::string left;
				std::string top;
				std::string bottom;
				std::string front;
				std::string back;

				std::string rightIrr;
				std::string leftIrr;
				std::string topIrr;
				std::string bottomIrr;
				std::string frontIrr;
				std::string backIrr;
			};
		};

		CubemapAssetImportInfos() = default;
		
		// Remove SA_ENGINE_API.
		SA_ENGINE_API CubemapAssetImportInfos(const std::string& _right, const std::string& _left,
			const std::string& _top, const std::string& _bottom,
			const std::string& _front, const std::string& _back,
			const std::string& _rightIrr, const std::string& _leftIrr,
			const std::string& _topIrr, const std::string& _bottomIrr,
			const std::string& _frontIrr, const std::string& _backIrr) noexcept;

		SA_ENGINE_API ~CubemapAssetImportInfos();
	};
}

#endif // GUARD
