// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_CUBEMAP_IMPORT_INFOS_GUARD
#define SAPPHIRE_SDK_CUBEMAP_IMPORT_INFOS_GUARD

#include <SDK/Assets/IAssetImportInfos.hpp>

#include <Rendering/Framework/Primitives/Texture/TextureFormat.hpp>

namespace Sa
{
	class CubemapImportInfos : public IAssetImportInfos
	{
	public:
		uint32 channelNum = 4; // RGBA.
		TextureFormat format = TextureFormat::sRGB;
	};
}

#endif // GUARD
