// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RAW_TEXTURE_GUARD
#define SAPPHIRE_RENDERING_RAW_TEXTURE_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Framework/Primitives/Texture/TextureType.hpp>
#include <Rendering/Framework/Primitives/Texture/TextureChannel.hpp>

namespace Sa
{
	struct RawTexture
	{
		uint32 width = 0u;
		uint32 height = 0u;

		uint32 mipLevels = 1u;

		char* data = nullptr;

		TextureType type = TextureType::Simple;
		TextureChannel channel = TextureChannel::RGBA;
		
		// Compute original image size in format unit.
		uint32 GetMainSize(bool _bApplyType = true) const noexcept;

		// Compute total image size (including mipmaps) in format unit.
		uint32 GetTotalSize(bool _bApplyType = true) const noexcept;
	};
}

#endif // GUARD
