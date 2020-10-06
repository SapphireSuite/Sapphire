// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_IASSET_IMPORT_INFOS_GUARD
#define SAPPHIRE_SDK_IASSET_IMPORT_INFOS_GUARD

#include <Core/Types/IInterface.hpp>

namespace Sa
{
	struct IAssetImportInfos : public IInterface
	{
		bool bKeepLoaded = true;

		SA_ENGINE_API IAssetImportInfos() = default;
		SA_ENGINE_API IAssetImportInfos(bool _bKeepLoaded) noexcept;
	};

	struct IAssetCreateInfos : public IInterface
	{
		std::string outFilePaths;

		bool bErase = false;
		bool bKeepLoaded = true;
	};
}

#endif // GUARD
