// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IMAGE_EXTENT_GUARD
#define SAPPHIRE_RENDERING_IMAGE_EXTENT_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

#endif

namespace Sa
{
	class ImageExtent
	{
	public:
		uint32 width = 0u;
		uint32 height = 0u;

		ImageExtent() = default;

#if SA_RENDERING_API == SA_VULKAN

		ImageExtent(const VkExtent2D& _extent);

		operator const VkExtent2D&() const;

#endif
	};
}

#endif // GUARD
