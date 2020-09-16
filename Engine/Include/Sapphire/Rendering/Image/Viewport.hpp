// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VIEWPORT_GUARD
#define SAPPHIRE_RENDERING_VIEWPORT_GUARD

#include <Rendering/Image/ImageViewExtent.hpp>

namespace Sa
{
	class Viewport : public ImageViewExtent
	{
	public:
		ImageViewExtent scissor;

#if SA_RENDERING_API == SA_VULKAN

		VkViewport GetVkViewport() const;

		VkRect2D GetVkScissor() const;

#endif
	};
}

#endif // GUARD
