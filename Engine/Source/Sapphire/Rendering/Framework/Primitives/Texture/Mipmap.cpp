// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/Mipmap.hpp>

#include <Maths/Misc/Maths.hpp>

namespace Sa
{
	//const uint32 Mipmap::minLevel = ComputeLevels(16u, 16u, 1);

	uint32 Mipmap::ComputeLevels(const Vec2ui& _extent, uint32 _minLevel) noexcept
	{
		// Source https://vulkan-tutorial.com/Generating_Mipmaps.

		return static_cast<uint32_t>(std::floor(std::log2(Maths::Max(_extent.x, _extent.y)))) + 1 - _minLevel;
	}

	uint64 Mipmap::ComputeTotalSize(Vec2ui _extent, uint32 _mipmapLevels, Vec2ui* _extents) noexcept
	{
		if (_mipmapLevels == 0u)
			_mipmapLevels = ComputeLevels(_extent);


		uint64 totalSize = 0u;

		for (uint32 i = 0u; i < _mipmapLevels; ++i)
		{
			if (_extents)
				_extents[i] = _extent;

			totalSize += _extent.x * _extent.y;

			if (_extent.x > 1)
				_extent.x >>= 1;

			if (_extent.y > 1)
				_extent.y >>= 1;
		}

		return totalSize;
	}
}
