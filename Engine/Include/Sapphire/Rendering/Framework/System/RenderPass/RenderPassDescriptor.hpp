// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDERPASS_DESCRIPTOR_GUARD
#define SAPPHIRE_RENDERING_RENDERPASS_DESCRIPTOR_GUARD

#include <vector>

#include <Maths/Space/Vector2.hpp>

#include <Rendering/Framework/Misc/Format.hpp>
#include <Rendering/Framework/Misc/Color.hpp>
#include <Rendering/Framework/System/RenderPass/SampleBits.hpp>

namespace Sa
{
	class IRenderSurface;

	struct SubPassAttachmentDescriptor
	{
		Format format;

		// Used as input attachment in next subpass.
		bool bInputNext = false;
	};

	struct SubPassDescriptor
	{
		// All color attachment must have the same sample count.
		SampleBits sampling = SampleBits::Sample1Bit;

		std::vector<SubPassAttachmentDescriptor> attachmentDescs;
	};

	class SA_ENGINE_API RenderPassDescriptor
	{
	public:
		bool bClear = true;
		Color clearColor = Color{ 0.0f, 0.0f, 0.05f, 0.0f };

		std::vector<SubPassDescriptor> subPassDescs;

		/**
		*	\brief Create a default single-pass forward rendering renderpass descriptor.
		*/
		static RenderPassDescriptor DefaultSingle(const IRenderSurface* _surface = nullptr);

		/**
		*	\brief Create a default multi-pass PBR deferred rendering renderpass descriptor.
		*/
		static RenderPassDescriptor DefaultPBR(const IRenderSurface* _surface = nullptr);
	};
}

#endif // GUARD