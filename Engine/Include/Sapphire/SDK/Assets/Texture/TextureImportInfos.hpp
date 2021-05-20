// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TEXTURE_IMPORT_INFOS_GUARD
#define SAPPHIRE_SDK_TEXTURE_IMPORT_INFOS_GUARD

#include <SDK/Assets/IAssetImportInfos.hpp>

#include <Rendering/Framework/Misc/Format.hpp>

namespace Sa
{
	class TextureImportInfos : public IAssetImportInfos
	{
	public:
		Format format = Format::RGBA_32;

		uint32 mipLevels = ~uint32();
	};
}

#endif // GUARD
