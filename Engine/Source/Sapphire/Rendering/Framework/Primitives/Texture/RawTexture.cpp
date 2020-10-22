// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/RawTexture.hpp>

#include <Rendering/Framework/Primitives/Texture/Mipmap.hpp>

namespace Sa
{
	uint64 RawTexture::GetMainSize() const noexcept
	{
		return width * height * static_cast<uint32>(channel);
	}

	uint64 RawTexture::GetTotalSize() const noexcept
	{
		return Mipmap::ComputeTotalSize(width, height, mipLevels) * static_cast<uint32>(channel);
	}
}
