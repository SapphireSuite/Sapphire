// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Texture/Mipmap.hpp>

#include <Maths/Misc/Maths.hpp>

namespace Sa
{
	//const uint32 Mipmap::minLevel = ComputeLevels(16u, 16u, 1);

	uint32 Mipmap::ComputeLevels(uint32 _width, uint32 _height, uint32 _minLevel) noexcept
	{
		// Source https://vulkan-tutorial.com/Generating_Mipmaps.

		return static_cast<uint32_t>(std::floor(std::log2(Maths::Max(_width, _height)))) + 1 - _minLevel;
	}

	uint64 Mipmap::ComputeTotalSize(uint32 _width, uint32 _height, uint32 _mipmapLevels, MipMapInfos* _infos) noexcept
	{
		if (_mipmapLevels == 0u)
			_mipmapLevels = ComputeLevels(_width, _height);


		uint64 totalSize = 0u;

		for (uint32 i = 0u; i < _mipmapLevels; ++i)
		{
			if (_infos)
			{
				_infos[i].width = _width;
				_infos[i].height = _height;
			}

			totalSize += _width * _height;

			if (_width > 1)
				_width >>= 1;

			if (_height > 1)
				_height >>= 1;
		}

		return totalSize;
	}
}
