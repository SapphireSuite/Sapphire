// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_TEXTURE_CHANNEL_GUARD
#define SAPPHIRE_RENDERING_TEXTURE_CHANNEL_GUARD

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

#endif

namespace Sa
{
	enum class TextureChannel
	{
		Grey = 1,

		GreyA,

		RGB,

		RGBA
	};

#if SA_RENDERING_API == SA_VULKAN

	VkFormat API_GetFormat(TextureChannel _channel);

#endif
}

#endif // GUARD
