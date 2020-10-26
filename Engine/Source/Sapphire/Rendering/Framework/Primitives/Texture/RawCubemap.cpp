// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/RawCubemap.hpp>

#include <Rendering/Framework/Primitives/Texture/Mipmap.hpp>

namespace Sa
{
	const uint32 RawCubemap::mipLevels = 5u;

	uint64 RawCubemap::GetMapSize() const noexcept
	{
		return width * height * static_cast<uint32>(channel) * 6u;
	}

	uint64 RawCubemap::GetTotalSize() const noexcept
	{
		return Mipmap::ComputeTotalSize(width, height, mipLevels) * static_cast<uint32>(channel) * 6u;
	}
}
