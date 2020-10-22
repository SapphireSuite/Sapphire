// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_MATERIAL_TEXTURES_GUARD
#define SAPPHIRE_RENDERING_MATERIAL_TEXTURES_GUARD

#include <Core/Types/Int.hpp>

namespace Sa
{
	class ITexture;

	struct MaterialTextures
	{
		static constexpr uint32 size = 4u;
		
		union
		{
			const ITexture* data[size]{};

			struct
			{
				const ITexture* albedo;
				const ITexture* normalMap;
				const ITexture* specularMap;
				const ITexture* roughMap;
			};
		};

		uint32 GetTextureNum() const noexcept;
	};
}

#endif // GUARD