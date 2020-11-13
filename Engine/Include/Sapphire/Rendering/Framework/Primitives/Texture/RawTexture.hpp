// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RAW_TEXTURE_GUARD
#define SAPPHIRE_RENDERING_RAW_TEXTURE_GUARD

#include <Maths/Space/Vector2.hpp>

#include <Rendering/Framework/Misc/Format.hpp>

namespace Sa
{
	struct RawTexture
	{
		Vec2ui extent;

		std::vector<char> data;

		uint32 mipLevels = 1u;

		Format format = Format::RGBA_32;

		// Compute original image size in format unit.
		uint64 GetMainSize() const noexcept;

		// Compute total image size (including mipmaps) in format unit.
		uint64 GetTotalSize() const noexcept;
	};
}

#endif // GUARD
