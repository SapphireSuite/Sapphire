// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Material/MaterialTextures.hpp>

#include <Core/Algorithms/SizeOf.hpp>

namespace Sa
{
	uint32 MaterialTextures::GetTextureNum() const noexcept
	{
		uint32 result = 0u;

		for(uint32 i = 0u; i < MaterialTextures::size; ++i)
		{
			if (data[i])
				++result;
		}

		return result;
	}
}
