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
		uint32 width = 100u;
		uint32 height = 100u;

		ImageExtent() = default;

		SA_ENGINE_API ImageExtent(uint32 _width, uint32 _height);

#if SA_RENDERING_API == SA_VULKAN

		ImageExtent(const VkExtent2D& _extent);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API operator VkExtent2D() const;

		operator VkExtent3D() const;

#endif
	};
}

#endif // GUARD
