// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RAW_TEXTURE_GUARD
#define SAPPHIRE_RENDERING_RAW_TEXTURE_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Framework/Primitives/Texture/TextureFormat.hpp>

namespace Sa
{
	struct RawTexture
	{
		uint32 width = 0u;
		uint32 height = 0u;

		uint32 mipLevels = 1u;

		char* data = nullptr;

		uint32 channelNum = 4; // RGBA.
		TextureFormat format = TextureFormat::RGB;
		
		// Compute original image size in format unit.
		uint64 GetMainSize() const noexcept;

		// Compute total image size (including mipmaps) in format unit.
		uint64 GetTotalSize() const noexcept;
	};
}

#endif // GUARD
