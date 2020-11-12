// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_MIPMAP_GUARD
#define SAPPHIRE_RENDERING_MIPMAP_GUARD

#include <Maths/Space/Vector2.hpp>

namespace Sa
{
	class Mipmap
	{
	public:
		//static const uint32 minLevel;

		static uint32 ComputeLevels(const Vec2ui& _extent, uint32 _minLevel = 0u) noexcept;
		static uint64 ComputeTotalSize(Vec2ui _extent, uint32 _mipmapLevels = 0u, Vec2ui* extents = nullptr) noexcept;
	};
}

#endif // GUARD
