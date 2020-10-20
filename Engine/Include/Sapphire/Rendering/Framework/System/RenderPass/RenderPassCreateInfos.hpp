// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_PASS_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_RENDER_PASS_CREATE_INFOS_GUARD

#include <vector>

#include <Core/Types/Color.hpp>

#include <Rendering/Framework/System/RenderPass/SampleBits.hpp>
#include <Rendering/Framework/Primitives/Material/UniformBuffers.hpp>

#include <Rendering/Image/ImageExtent.hpp>

namespace Sa
{
	class IRenderSurface;

	struct SubPassCreateInfos
	{
	};

	struct RenderPassCreateInfos
	{
		ImageExtent extent;
		// TODO Aurel: Move it somewhere appriopriate or create Sa::Format
		VkFormat	format;

		Color clearColor = Color::defaultClearColor;

		SampleBits sampling = SampleBits::Sample1Bit;

		//bool bDepthBuffer = true;

		std::vector<SubPassCreateInfos> subPassCreateInfos;
	};
}

#endif // GUARD
