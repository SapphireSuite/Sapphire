// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDERPASS_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_RENDERPASS_CREATE_INFOS_GUARD

#include <vector>

#include <Maths/Space/Vector2.hpp>

#include <Rendering/Framework/Misc/RenderFormat.hpp>
#include <Rendering/Framework/Misc/Color.hpp>
#include <Rendering/Framework/System/RenderPass/SampleBits.hpp>

namespace Sa
{
	struct SubPassAttachmentInfos
	{
		RenderFormat format;
	};

	struct SubPassCreateInfos
	{
		std::vector<SubPassAttachmentInfos> attachmentInfos;
	};

	struct RenderPassCreateInfos
	{
		SampleBits sampling = SampleBits::Sample8Bits;

		bool bPresent = true;

		bool bDepthBuffer = true;
		bool bStencilBuffer = false;

		bool bClear = true;
		Color clearColor = Color{ 0.0f, 0.0f, 0.05f, 1.0f };

		std::vector<SubPassCreateInfos> subPassInfos;

		SA_ENGINE_API static const RenderPassCreateInfos defaultForward;
		SA_ENGINE_API static const RenderPassCreateInfos defaultPBRDeferred;
	};
}

#endif // GUARD