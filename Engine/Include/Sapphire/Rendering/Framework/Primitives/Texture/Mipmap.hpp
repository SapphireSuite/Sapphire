// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_MIPMAP_GUARD
#define SAPPHIRE_RENDERING_MIPMAP_GUARD

#include <Core/Types/Int.hpp>

namespace Sa
{
	struct MipMapInfos
	{
		uint32 width = 0u;
		uint32 height = 0u;
	};

	class Mipmap
	{
	public:
		//static const uint32 minLevel;

		static uint32 ComputeLevels(uint32 _width, uint32 _height, uint32 _minLevel = 0u) noexcept;
		static uint32 ComputeTotalSize(uint32 _width, uint32 _height, uint32 _mipmapLevels = 0u, MipMapInfos* _infos = nullptr) noexcept;
	};
}

#endif // GUARD
