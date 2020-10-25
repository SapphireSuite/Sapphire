// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RAW_CUBEMAP_GUARD
#define SAPPHIRE_RENDERING_RAW_CUBEMAP_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Framework/Primitives/Texture/TextureChannel.hpp>

namespace Sa
{
	struct RawCubemap
	{
		uint32 width = 0u;
		uint32 height = 0u;

		char* cubemapData = nullptr;

		//char* irradiancemapData = nullptr;
	
		TextureChannel channel = TextureChannel::RGBA;

		static const uint32 mipLevels;
		//static const float irradianceFactor;

		//uint32 GetIrradianceWidth() const noexcept;
		//uint32 GetIrradianceHeight() const noexcept;

		// Compute original map size in format unit.
		uint64 GetMapSize() const noexcept;

		uint64 GetTotalSize() const noexcept;

		// Compute irradiance map size in format unit.
		//uint64 GetIrradianceMapSize() const noexcept;
	};
}

#endif // GUARD
