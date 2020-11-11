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
	class IRenderSurface;

	struct SubPassAttachmentDescriptor
	{
		RenderFormat format;
	};

	struct SubPassDescriptor
	{
		std::vector<SubPassAttachmentDescriptor> attachmentDescriptors;
	};

	class SA_ENGINE_API RenderPassDescriptor
	{
	public:
		SampleBits sampling = SampleBits::Sample8Bits;

		bool bPresent = true;

		bool bDepthBuffer = true;
		RenderFormat depthFormat = RenderFormat::Depth_16;

		bool bClear = true;
		Color clearColor = Color{ 0.0f, 0.0f, 0.05f, 1.0f };

		std::vector<SubPassDescriptor> subPassDescriptors;

		/**
		*	\brief Create a default forward rendering renderpass descriptor.
		*/
		static RenderPassDescriptor CreateDefaultForward(const IRenderSurface* _surface = nullptr);

		/**
		*	\brief Create a default PBR deferred rendering renderpass descriptor.
		*/
		static RenderPassDescriptor CreateDefaultPBRDeferred(const IRenderSurface* _surface = nullptr);
	};
}

#endif // GUARD