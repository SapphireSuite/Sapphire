// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_FORMAT_GUARD
#define SAPPHIRE_RENDERING_RENDER_FORMAT_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/APIConfig.hpp>

namespace Sa
{
	enum RenderFormatType : uint8
	{
		RGB,

		sRGB,

		Depth,

		Stencil,

		DepthStencil,
	};

	struct RenderFormat
	{
		uint32 channelNum = 4u;
		RenderFormatType type = RenderFormatType::RGB;
	};

#if SA_RENDERING_API == SA_VULKAN

	VkFormat API_GetRenderFormat(RenderFormat _format);

#endif
}

#endif // GUARD
