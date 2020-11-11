// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDERPASS_DESCRIPTOR_GUARD
#define SAPPHIRE_RENDERING_RENDERPASS_DESCRIPTOR_GUARD

#include <vector>

#include <Maths/Space/Vector2.hpp>

#include <Rendering/Framework/Misc/RenderFormat.hpp>
#include <Rendering/Framework/Misc/Color.hpp>
#include <Rendering/Framework/System/RenderPass/SampleBits.hpp>

namespace Sa
{
	struct SubPassAttachmentDescriptor
	{
		RenderFormat format;
	};

	struct SubPassDescriptor
	{
		std::vector<SubPassAttachmentDescriptor> attachmentDescriptors;
	};

	struct RenderPassDescriptor
	{
		SampleBits sampling = SampleBits::Sample8Bits;

		bool bPresent = true;

		bool bDepthBuffer = true;
		bool bStencilBuffer = false;

		bool bClear = true;
		Color clearColor = Color{ 0.0f, 0.0f, 0.05f, 1.0f };

		std::vector<SubPassDescriptor> subPassDescriptors;

		SA_ENGINE_API static const RenderPassDescriptor defaultForward;
		SA_ENGINE_API static const RenderPassDescriptor defaultPBRDeferred;
	};
}

#endif // GUARD