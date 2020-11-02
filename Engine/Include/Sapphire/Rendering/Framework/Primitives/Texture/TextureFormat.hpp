// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_TEXTURE_FORMAT_GUARD
#define SAPPHIRE_RENDERING_TEXTURE_FORMAT_GUARD

#include <Rendering/Config.hpp>

#include <Core/Types/Int.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

#endif

namespace Sa
{
	enum class TextureFormat : uint8
	{
		RGB,

		sRGB
	};

#if SA_RENDERING_API == SA_VULKAN

	VkFormat API_GetFormat(TextureFormat _format, uint32 _channelNum = 4u);

#endif
}

#endif // GUARD
