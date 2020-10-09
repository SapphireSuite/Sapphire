// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_PASS_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_RENDER_PASS_CREATE_INFOS_GUARD

#include <vector>

#include <Rendering/Framework/System/RenderPass/SamplingBits.hpp>
#include <Rendering/Framework/Primitives/Material/UniformBuffers.hpp>

#include <Rendering/Image/ImageExtent.hpp>

namespace Sa
{
	class IRenderSurface;

	struct SubPassCreateInfos
	{
		SamplingBits sampling = SamplingBits::Sample1Bit;

		//bool bDepthBuffer = true;
	};

	struct RenderPassCreateInfos
	{
		ImageExtent extent;

		SubPassCreateInfos subPassCreateInfos;

		uint32 staticUniformBufferSize = sizeof(StaticUniformBuffer);
	};
}

#endif // GUARD
