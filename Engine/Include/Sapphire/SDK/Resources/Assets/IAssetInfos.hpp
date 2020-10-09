// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_IASSET_INFOS_GUARD
#define SAPPHIRE_SDK_IASSET_INFOS_GUARD

#include <Core/Types/IInterface.hpp>

namespace Sa
{
	struct IAssetImportInfos : public IInterface
	{
		bool bKeepLoaded = true;
	};

	struct IAssetCreateInfos : public IInterface
	{
		std::string outFilePaths;

		bool bErase = false;
		bool bKeepLoaded = true;
	};
}

#endif // GUARD
