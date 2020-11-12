// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RAW_CUBEMAP_GUARD
#define SAPPHIRE_RENDERING_RAW_CUBEMAP_GUARD

#include <Rendering/Framework/Primitives/Texture/RawTexture.hpp>

namespace Sa
{
	struct RawCubemap : public RawTexture
	{
		std::vector<char> irradiancemapData;

		// Maximum rough level used for IBL. Generate as much mipmaps.
		static const uint32 maxRoughLevel;

		// Compute original map size in format unit.
		uint64 GetMapSize() const noexcept;

		// Compute total map size (including mipmaps) in format unit.
		uint64 GetTotalMapSize() const noexcept;
	};
}

#endif // GUARD
