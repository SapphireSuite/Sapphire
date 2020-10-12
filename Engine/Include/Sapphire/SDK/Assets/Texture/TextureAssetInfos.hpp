// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_TEXTURE_ASSET_INFOS_GUARD
#define SAPPHIRE_SDK_TEXTURE_ASSET_INFOS_GUARD

#include <Core/Types/Int.hpp>

#include <SDK/Assets/Texture/TextureChannel.hpp>

namespace Sa
{
	struct TextureRawData
	{
		uint32 width = 0;
		uint32 height = 0;

		char* data = nullptr;

		TextureChannel channel = TextureChannel::RGBA;
	};
}

#endif // GUARD
