// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TEXTURE_IMPORT_INFOS_GUARD
#define SAPPHIRE_SDK_TEXTURE_IMPORT_INFOS_GUARD

#include <SDK/Assets/IAssetImportInfos.hpp>

#include <Rendering/Framework/Primitives/Texture/TextureFormat.hpp>

namespace Sa
{
	class TextureImportInfos : public IAssetImportInfos
	{
	public:
		TextureFormat format = TextureFormat::RGB;
		uint32 channelNum = 4; // RGBA.

		uint32 mipLevels = uint32(-1u);
	};
}

#endif // GUARD
