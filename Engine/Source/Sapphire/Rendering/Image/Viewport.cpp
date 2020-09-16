// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Image/Viewport.hpp>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	VkViewport Viewport::GetVkViewport() const
	{
		return VkViewport
		{
			static_cast<float>(offset.x),									// x.
			static_cast<float>(offset.y),									// y.
			static_cast<float>(extent.width),								// width.
			static_cast<float>(extent.height),								// height.
			0.0f,															// minDepth.
			1.0f,															// maxDepth.
		};
	}

	VkRect2D Viewport::GetVkScissor() const
	{
		return VkRect2D
		{
			VkOffset2D{ static_cast<int32>(scissor.offset.x), static_cast<int32>(scissor.offset.y) },				// offset.
			scissor.extent, 																						// extent.
		};
	}

#endif

}
